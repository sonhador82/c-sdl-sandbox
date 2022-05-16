#include <stdlib.h>

#include "loadfile.h"


int main(){
    VertexData *data = (VertexData*)malloc(sizeof(VertexData));


    load_file("box_uvmap.obj", data);
    printf("num vtx: %i\n", data->vtx_count);
    return EXIT_SUCCESS; 
}