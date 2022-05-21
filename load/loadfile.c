#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "loadfile.h"


#define MAX_LINE_WIDTH 80


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

void parse_face_line(const char* line, short* indx){
    // in f 5/11/4 6/12/4 2/13/4 
    char* tokens_line = (char*)malloc(sizeof(char)*MAX_LINE_WIDTH);
    strcpy(tokens_line, line);
    char *token = (char*)malloc(sizeof(char)*10);
    strtok(line, " "); // ingore first f        
    for(int i=0; i<3; i++){
        token = strtok(NULL, " ");
        indx[i] = atoi(token)-1;
    }
}

void load_obj_file(const char *f_name, TinyObject *object) {
    FILE *fp;
    fp = fopen(f_name, "rt");
    if (fp == NULL){
        exit(EXIT_FAILURE);
    }
    
    char *line = malloc(sizeof(char)*MAX_LINE_WIDTH);
    object->indices = (short*)malloc(sizeof(short)*1000);
    object->ind_num = 0;
    object->vtx_num = 0;
    object->vertices = (float*)malloc(sizeof(float)*1000);

    while (fgets(line, MAX_LINE_WIDTH, fp)) {
        // load faces || indices
        unsigned char face_buf[5]; // не больше 5 точек в фейсе
        if(line[0] == 'f') {
            short buf[3];
            parse_face_line(line, buf);
            for(int i=0; i<3; i++){
                printf("t: %i\n", buf[i]);
                object->indices[object->ind_num] = buf[i];
                object->ind_num++;
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

    fclose(fp);
}
