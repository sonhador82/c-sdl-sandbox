#version 300 es
precision mediump float;

//uniform vec4 aColor;

in vec4 aColor;

out vec4 fragColor; 

// in vec2 v_texCoord;
layout (location = 1) out vec4 outColor;
// uniform sampler2D s_texture;

void main()
{
    fragColor = aColor;
   // fragColor = vec4(0.0, 0.0, 1.0, 1.0);
    //outColor = texture (s_texture, v_texCoord);
}

