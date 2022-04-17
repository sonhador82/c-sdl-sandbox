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
