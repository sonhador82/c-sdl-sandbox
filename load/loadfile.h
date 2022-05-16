#ifndef LOADFILE_H
#define LOADFILE_H

typedef struct VertexData
{
    int vtx_count;
    float *vtx_data;
} VertexData;


void load_file(const char *f_name, VertexData *vtxData);

#endif
