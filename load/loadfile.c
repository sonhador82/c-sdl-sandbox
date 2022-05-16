#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "loadfile.h"


const int maxVtx = 50*4;


void load_file(const char *f_name, VertexData *vtxData) {
    puts("load file");

    FILE *fp;
    fp = fopen(f_name, "rt");
    if (fp == NULL){
        printf(strerror(errno));
        exit(EXIT_FAILURE);
    }

    
    char *line = malloc(sizeof(char)*80);
    vtxData->vtx_data = (float *) malloc(sizeof(float)*maxVtx);

    int indx = 0;
    int arrpos = 0;

    while(fgets(line, 80, fp))
    {
        if (strncmp(line, "v ", 2) == 0) {
            printf("Line: %s", line);   
            char *token = malloc(sizeof(char)*80);
            strtok(line, " "); // pass v
            float x, y, z;
            x = strtof(strtok(NULL, " "), NULL);
            y = strtof(strtok(NULL, " "), NULL);
            z = strtof(strtok(NULL, " "), NULL);

            // printf("x, y, z: %f, %f, %f\n", x, y, z);

            vtxData->vtx_data[arrpos] = x;
            vtxData->vtx_data[arrpos+1] = y;
            vtxData->vtx_data[arrpos+2] = z;
            arrpos = arrpos + 3;
            indx++;
            free(token);
            printf("end of line\n");
        }
    }
    fclose(fp);


    free(line);

    vtxData->vtx_count = indx*3;
}
