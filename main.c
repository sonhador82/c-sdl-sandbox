#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>

#include <math.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#define MAX_SHADER_LINES 100
#define MAX_SHADER_LINE_LEN 82

typedef struct shaderCode
{
  char *code[100];
  int size;
  GLenum type; // GL_VERTEX_SHADER || GL_FRAGMENT_SHADER
} shaderCode;

GLuint
compileShader (shaderCode *shaderCode)
{

  GLuint shader = glCreateShader (shaderCode->type);
  printf ("Shader: %i\n", shader);

  glShaderSource (shader, shaderCode->size, (const char **)shaderCode->code,
                  NULL);
  glCompileShader (shader);

  GLint params;
  glGetShaderiv (shader, GL_COMPILE_STATUS, &params);
  printf ("shader res: %i\n", params);

  if (params != GL_TRUE)
    {
      printf ("compil failed\n");
      GLchar *buf = malloc (1024);
      GLsizei err_size;

      glGetShaderInfoLog (shader, 1024, &err_size, buf);
      printf ("err: %s\n", buf);
      free (buf);
    }
  return shader;
}

void
render (GLuint *shader_prog)
{
  // bind shader location
  glBindAttribLocation (shader_prog, 0, "aPosition");
  int err = glGetError ();
  if (err != GL_NO_ERROR)
    {
      printf ("status glBindAttribLocation: %x\n", err);
    }

  // vertex 12 points
  GLfloat v_arr[] = {
    -0.9, -0.9, 0.0, 1.0, -0.9, 0.9, 0.0, 1.0, 0.9, 0.9, 0.0, 1.0,
  };

  // создаем буффер на GPU
  GLuint buffer;
  glGenBuffers (1, &buffer);
  glBindBuffer (GL_ARRAY_BUFFER, buffer);
  glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * 12, &v_arr,
                GL_STATIC_DRAW);
  glVertexAttribPointer (0, 4, GL_FLOAT, GL_FALSE, sizeof (GLfloat) * 4, 0);

  glEnableVertexAttribArray (0);

  glDrawArrays (GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray (0);
}

void
initSDL (shaderCode *vertexCode, shaderCode *fragmentCode)
{
  int res = SDL_Init (SDL_INIT_EVERYTHING);
  SDL_Window *wnd;
  wnd = SDL_CreateWindow ("hello", 0, 0, 600, 400, SDL_WINDOW_OPENGL);

  SDL_GLContext ctx;
  ctx = SDL_GL_CreateContext (wnd);
  if (ctx == NULL)
    {
      const char *err = SDL_GetError ();
      printf ("err: %s\n", err);
      exit (EXIT_FAILURE);
    }

  GLuint vertex_shader = compileShader (vertexCode);
  GLuint fragment_shader = compileShader (fragmentCode);

  GLuint shader_prog = glCreateProgram ();
  if (shader_prog != GL_TRUE)
    {
      SDL_LogError (SDL_LOG_CATEGORY_VIDEO, "error creating shader prog");
    }
  glAttachShader (shader_prog, vertex_shader);
  glAttachShader (shader_prog, fragment_shader);

  // linking
  glLinkProgram (shader_prog);

  GLint link_result;
  glGetProgramiv (shader_prog, GL_LINK_STATUS, &link_result);
  if (link_result != GL_TRUE)
    {
      SDL_LogError (SDL_LOG_CATEGORY_VIDEO, "error linking shader program");
    }

  printf ("vendor: %s\n version: %s\n shading vers: %s\n, renderer: %s\n",
          glGetString (GL_VENDOR), glGetString (GL_VERSION),
          glGetString (GL_SHADING_LANGUAGE_VERSION),
          glGetString (GL_RENDERER));

  glUseProgram (shader_prog);
  int err = glGetError ();
  if (err != GL_NO_ERROR)
    {
      printf ("status glUseProgram: %i\n", err);
    }

  int width, height;
  SDL_GL_GetDrawableSize (wnd, &width, &height);

  glViewport (0, 0, width, height);

  glClearColor (0.15, 0.15, 0.1, 1.0);
  glClear (GL_COLOR_BUFFER_BIT);

  render (shader_prog);

  SDL_GL_SwapWindow (wnd);

  SDL_Delay (2000);

  // free(vrtxs);
  SDL_GL_DeleteContext (ctx);
  SDL_Quit ();
}

void
loadCode (char *file_path, shaderCode *shader)
{

  FILE *fp = fopen (file_path, "r");
  if (fp == NULL)
    {
      perror ("error");
      exit (EXIT_FAILURE);
    }

  for (int i = 0; i < MAX_SHADER_LINES; i++)
    {
      shader->code[i] = malloc (MAX_SHADER_LINE_LEN);
    }

  int idx = 0;
  while (1)
    {
      char *linebuf = NULL;
      size_t n = 0;
      int num_bytes = 0;
      num_bytes = getline (&linebuf, &n, fp);
      if (num_bytes == -1)
        {
          free (linebuf);
          break;
        }
      strcpy (shader->code[idx], linebuf);
      idx++;
    };
  shader->size = idx;
  fclose (fp);
}

void
free_shader_mem (shaderCode *shader)
{
  for (int i = 0; i < MAX_SHADER_LINES; i++)
    {
      free (shader->code[i]);
    }
}

int
main (int argc, char **argv)
{

  const char *path = "vertex.glsl";
  shaderCode vertex;
  vertex.type = GL_VERTEX_SHADER;
  loadCode (path, &vertex);

  const char *fragm = "fragment.glsl";
  shaderCode fragment;
  fragment.type = GL_FRAGMENT_SHADER;
  loadCode (fragm, &fragment);

  // init SDL
  initSDL (&vertex, &fragment);

  free_shader_mem (&vertex);
  free_shader_mem (&fragment);

  exit (EXIT_SUCCESS);
}
