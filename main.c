#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>

#include <math.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include <cglm/call.h>

#include <stdio.h>

#define MAX_SHADER_LINES 100
#define MAX_SHADER_LINE_LEN 82

#include "load/loadfile.h"

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

  TinyObject *obj = (TinyObject*)malloc(sizeof(TinyObject));
  load_obj_file("sphere.obj", obj);

  printf("obj_v: %i\n", obj->vtx_num);
  printf("obj_f: %i\n", obj->ind_num);
  for(int i=0; i<obj->vtx_num; i++){
    printf("%0.2f,", obj->vertices[i]);
  }
  putchar('\n');
  for(int i=0; i<obj->ind_num; i++){
    printf("%i,", obj->indices[i]);
  }
  putchar('\n');

  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (GLfloat) * 3, obj->vertices);  
  glEnableVertexAttribArray (0);

  GLuint uniLoc = glGetUniformLocation (shader_prog, "aMVMatrix");
  SDL_Log ("matrix loc: %i", uniLoc);
  if (uniLoc == -1)
    {
      SDL_Log ("cant find unitofm");
    }

  // создаем камеру, двигаем на -3 по Z
  mat4 model;
  glmc_mat4_identity(model);
  // glmc_perspective(60.0f, 1000.0f, -1000.0, 1.0, mat);
  //glmc_scale_uni(mat, 0.3);
  //glmc_rotate_z(mat, glm_rad(180.0), mat);
  //glmc_translate_x(model, 0.3f);
  // glmc_translate_y(mat, -0.3f);


  mat4 view; 
  vec3 eye={-0.0, 0.0, -1.0};
  vec3 dir={0.0, 0.0, 0.0};
  vec3 up={0.0, 1.0, 0.0};
  glm_lookat(eye, dir, up, view);
  glmc_mat4_print(view, stdout);


  mat4 projection;
  glmc_perspective(glm_deg(45.0), 1.0, -1.0, 1.0, projection);
  //glmc_perspective(glm_deg(30.0), 1.0, -1.0, 1.0, mat);
  //glmc_mul(mat, p, mat);

  // glmc_mat4_print(model, stdout);
  // glmc_mat4_print(view, stdout);
  // glmc_mat4_print(projection, stdout);
    // GLfloat mvMatrix[] = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
    //                       0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 };
    // glUniformMatrix4fv (uniLoc, 1, GL_FALSE, &mvMatrix);

 //glm::mat4 MVP = Projection * View * Model; // Помните, что умножение матрицы производиться в обратном порядке
 glmc_mul(projection, view, view);
 glmc_mul(view, model, model);
  glUniformMatrix4fv (uniLoc, 1, GL_FALSE, (float *) model);

  glDrawElements(
    GL_TRIANGLES,
    obj->ind_num,
    GL_UNSIGNED_SHORT,
    obj->indices
  );
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
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
