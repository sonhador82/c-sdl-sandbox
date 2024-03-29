#version 300 es

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 aColor;


void main() {

    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    aColor = color;
}
