#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "loadfile.h"


#define MAX_LINE_WIDTH 80

typedef struct LineData
{   
    unsigned int* face_ind;
    int count;
} LineData;


void parse_vtx_line(const char *line, float *vtx) {
    // in v -1.000000 -1.000000 1.000000
    char *tokens_line = malloc(sizeof(char)*MAX_LINE_WIDTH);
    strcpy(tokens_line, line);
    char *token = (char*)malloc(sizeof(char)*10);
    strtok(tokens_line, " "); // skip first v
    int indx = 0;
    for(int i=0; i<3; i++){
        token = strtok(NULL, " ");
        vtx[i] = atof(token);
    }
}

void parse_face_line(char* line, LineData* LineData){
    // in f 5/11/4 6/12/4 2/13/4 1/14/4    
    LineData->face_ind = (int*)malloc(sizeof(int)*5);
    char* token = (char*)malloc(sizeof(char)*10);
    strtok(line, " "); // ingore first token f        
    int indx=0;
    while (token != NULL)
    {
        token = strtok(NULL, " ");
        if(token != NULL) {
            *(LineData->face_ind+indx) = (int)token[0]-48;
        }
        indx++;
    }
    LineData->count = indx;
}

void load_obj_file(const char *f_name, TinyObject *object) {
    FILE *fp;
    fp = fopen(f_name, "rt");
    if (fp == NULL){
        exit(EXIT_FAILURE);
    }
    
    char *line = malloc(sizeof(char)*MAX_LINE_WIDTH);
    object->indices = (int*)malloc(sizeof(int)*100);
    object->ind_num = 0;
    object->vtx_num = 0;
    object->vertices = (float*)malloc(sizeof(float)*100);

    while (fgets(line, MAX_LINE_WIDTH, fp)) {
        // load faces || indices
        unsigned char face_buf[5]; // не больше 5 точек в фейсе
        if(line[0] == 'f') {
           // printf("line: %s\n", line);
            LineData* lineData = malloc(sizeof(LineData));
            parse_face_line(line, lineData);
            for(int i=0; i<lineData->count; i++){
                if(lineData->face_ind[i] != 0){
                    object->indices[object->ind_num] = lineData->face_ind[i];
                    object->ind_num++;
                }
            }
        }else if(line[0] == 'v' && line[1] == ' ') {
            float buf[3];
            // printf("v line: %s\n", line);
            parse_vtx_line(line, buf);
            for(int i=0; i<3; i++){
                object->vertices[object->vtx_num] = buf[i];
                object->vtx_num++;
            }
        }
    }

    // printf("Num of indices: %i\n", object->ind_num);
    // for(int i=0; i<object->ind_num; i++){
    //     printf("%i,", object->indices[i]);
    // }
    // printf("\n");

    // printf("Num of vtx: %i/3\n", object->vtx_num);
    // for(int i=0; i<object->vtx_num; i++){
    //     printf("%.2f, ", object->vertices[i]);
    // }
    // printf("\n");

    fclose(fp);
}
