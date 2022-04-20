#version 100

attribute vec4 aPosition;
attribute vec3 aColor;

varying vec3 color;

uniform mat4 mMatrix;

void main() {

    gl_Position = aPosition * mMatrix;
    gl_PointSize = 2.0;
    color = aColor;
}