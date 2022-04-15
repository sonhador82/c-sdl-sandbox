#version 300 es
precision mediump float;

//uniform vec4 aColor;

in vec4 aColor;
out vec4 fragColor; 
in vec2 v_texCoord;
layout (location = 0) out vec4 outColor;
uniform sampler2D s_texture;

void main()
{
    outColor = aColor;
    //outColor = texture (s_texture, v_texCoord);
}

