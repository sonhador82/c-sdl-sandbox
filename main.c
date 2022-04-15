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

    GLint link_result;
    glGetProgramiv(shader_prog, GL_LINK_STATUS, &link_result);
    if (link_result != GL_TRUE)
    {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "error linking shader program");
    }


    printf("vendor: %s\n version: %s\n shading vers: %s\n, renderer: %s\n", 
        glGetString(GL_VENDOR), 
        glGetString(GL_VERSION),
        glGetString(GL_SHADING_LANGUAGE_VERSION),
        glGetString(GL_RENDERER));
        
    glClearColor(0.15, 0.15, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat v_arr[] = {
        0.0, 0.0, 0.0,
        0.3, 0.3, 0.0,
        0.3, -0.3, 0.0
    };
    
    GLuint index_in_shader = 0;
    GLint num_comp_per_vertex_attr = 3; 

    GLuint buffers[2];
    glGenBuffers(1, buffers);
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("status glGenBuffers: %i\n", err);
    }

    printf("gen buffer name: %d\n", buffers[0]);
  
     glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    err = glGetError();

    if (err != GL_NO_ERROR) {
        printf("status glBindBuffer: %i\n", err);
    }
 


    printf("buf size: %d\n", sizeof(GLfloat)*9);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat)*9,
        &v_arr,
        GL_STATIC_DRAW
    );
    err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("status glBufferData: %x\n", err);
        if(err == GL_INVALID_OPERATION) {
            printf("invalid operation\n");
        }
    }
      

    glUseProgram(shader_prog);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("status glUseProgram: %i\n", err);
    }

    //
    GLfloat uv_ar[] = {
        0.0, 0.0,
        0.5, 0.5,
        1.0, 1.0
    };
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        &uv_ar
    );

    // color
    GLfloat color[] = {
        1.0, 1.0, 1.0, 1.0,
        0.5, 1.0, 1.0, 1.0,
        0.0, 1.0, 0.0, 1.0
    };
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        4,
        GL_FLOAT,
        GL_FALSE,
        0,
        &color
    );

    // TEXTURE PREPARE
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLuint tex;
    glGenTextures(1, &tex);
    GLuint rgb_txt[] = {
        255,255,255, 255,255,0, 128,255,255, 0,0,0,
        0,0,0, 128,128,128,  0,0,255, 255,255,255,
        128,128,255, 128,128,0, 255,128,255, 128,128,128,
        50, 0, 50, 90,0,0, 90,90,30, 25,255,10
    };

    // get
    GLuint tex_loc = glGetUniformLocation(shader_prog, "s_texture");
    glActiveTexture(GL_TEXTURE0);

    glUniform1i(tex_loc, tex);


    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        4, // width
        4, // height
        0, // not used border
        GL_RGB,
        GL_UNSIGNED_BYTE,
        &rgb_txt
    );
    err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("status glTexImage2D: %i\n", err);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    GLuint uni_loc;
    GLint result = glGetUniformLocation(&uni_loc, "aColor");
    printf("res: %i, uni_loc: %i\n", result, uni_loc);
    glUniform4f(uni_loc, 0.0, 0.0, 1.0, 1.0);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        0
    );

    glDrawArrays(GL_TRIANGLES, 0, 9);

    glDeleteTextures(tex, rgb_txt);
    SDL_GL_SwapWindow(wnd);

    SDL_Delay(2000);

    //free(vrtxs);
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
