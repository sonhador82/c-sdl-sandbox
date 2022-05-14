#include <stdio.h>
#include <stdlib.h>

#include <cglm/cglm.h>
//#include <cglm/call.h>

#define CGLM_DEFINE_PRINTS 1

void main()
{
    float *items = (float *)malloc(sizeof(float) * 3);

    *items = 1.0;
    *(items + 1) = 2.5;
    printf("sizeof float: %i\n", sizeof(float));
    printf("sizeof: %f\n", *items);
    printf("sizeof: %f\n", *items + 1);

    mat4 m = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

    mat4 model;
    // // = (mat4 *)malloc(sizeof(mat4));
    // // printf("sizeof mat4: %i\n", sizeof(mat4));
    glm_mat4_identity(model);
    // printf("model datas: %f\n", *model);
    glm_mat4_print(m, stdout);

    glm_mat4_print(model, stdout);
    // glm_mat4_print(m, stdout);
}