#ifndef LOADFILE_H
#define LOADFILE_H

typedef struct TinyObject
{
    unsigned int vtx_num;
    float *vertices;
    unsigned short *indices;
    unsigned int ind_num;
} TinyObject;


void load_obj_file(const char *f_name, TinyObject *object);

#endif
