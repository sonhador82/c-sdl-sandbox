#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>

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

GLuint compileShader(shaderCode *shaderCode)
{

    GLuint shader = glCreateShader(shaderCode->type);
    printf("Shader: %i\n", shader);

    glShaderSource(shader, shaderCode->size, (const char **)shaderCode->code, NULL);
    glCompileShader(shader);

    GLint params;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
    printf("shader res: %i\n", params);

    if (params != GL_TRUE)
    {
        printf("compil failed\n");
        GLchar *buf = malloc(1024);
        GLsizei err_size;

        glGetShaderInfoLog(shader, 1024, &err_size, buf);
        printf("err: %s\n", buf);
        free(buf);
    }
    return shader;
}

void initSDL(shaderCode *vertexCode, shaderCode *fragmentCode)
{
    int res = SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *wnd;
    wnd = SDL_CreateWindow("hello", 0, 0, 600, 400, SDL_WINDOW_OPENGL);

    SDL_GLContext ctx;
    ctx = SDL_GL_CreateContext(wnd);
    if (ctx == NULL)
    {
        const char *err = SDL_GetError();
        printf("err: %s\n", err);
        exit(EXIT_FAILURE);
    }

    GLuint vertex_shader = compileShader(vertexCode);
    GLuint fragment_shader = compileShader(fragmentCode);

    GLuint shader_prog = glCreateProgram();
    if (shader_prog != GL_TRUE)
    {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "error creating shader prog");
    }
    glAttachShader(shader_prog, vertex_shader);
    glAttachShader(shader_prog, fragment_shader);

    // linking
    glLinkProgram(shader_prog);

    GLint *link_result;
    glGetProgramiv(shader_prog, GL_LINK_STATUS, &link_result);
    if (link_result != GL_TRUE)
    {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "error linking shader program");
    }

    // glGetVertexAttribPointerv()
    // glBindAttribLocation
    //  glPointSize
    // glDrawArrays
    glUseProgram(shader_prog);
    // udpate
    // use program
    // glDraw Arrays

    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(wnd);

    SDL_Delay(1000);

    SDL_GL_DeleteContext(ctx);
    SDL_Quit();
}

void loadCode(char *file_path, shaderCode *shader)
{

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL)
    {
        perror("error");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAX_SHADER_LINES; i++)
    {
        shader->code[i] = malloc(MAX_SHADER_LINE_LEN);
    }

    int idx = 0;
    while (1)
    {
        char *linebuf = NULL;
        size_t n = 0;
        int num_bytes = 0;
        num_bytes = getline(&linebuf, &n, fp);
        if (num_bytes == -1)
        {
            free(linebuf);
            break;
        }
        strcpy(shader->code[idx], linebuf);
        idx++;
    };
    shader->size = idx;
    fclose(fp);
}

void free_shader_mem(shaderCode *shader)
{
    for (int i = 0; i < MAX_SHADER_LINES; i++)
    {
        free(shader->code[i]);
    }
}

int main(int argc, char **argv)
{

    const char *path = "vertex.glsl";
    shaderCode vertex;
    vertex.type = GL_VERTEX_SHADER;
    loadCode(path, &vertex);

    const char *fragm = "fragment.glsl";
    shaderCode fragment;
    fragment.type = GL_FRAGMENT_SHADER;
    loadCode(fragm, &fragment);

    // init SDL
    initSDL(&vertex, &fragment);

    free_shader_mem(&vertex);

    exit(EXIT_SUCCESS);
}
