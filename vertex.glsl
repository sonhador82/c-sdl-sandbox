#version 100

attribute vec4 aPosition;
attribute vec3 aColor;

varying vec3 color;

void main() {

    gl_Position = aPosition;
    gl_PointSize = 2.0;
    color = aColor;
}