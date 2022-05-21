#include <stdlib.h>

#include "loadfile.h"


int main(){
    TinyObject *obj = (TinyObject*)malloc(sizeof(TinyObject));    
    load_obj_file("box_uvmap.obj", obj);
    return EXIT_SUCCESS; 
}
