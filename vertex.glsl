#version 300 es

layout(location = 0) in vec3 position;

layout(location = 1) in vec2 a_texCoord;

layout(location = 3) in vec4 color;

out vec4 aColor;
out vec2 v_texCoord;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    v_texCoord = a_texCoord;
    aColor = color;
}

