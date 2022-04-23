#version 100

attribute vec4 aPosition;
attribute vec3 aColor;
attribute vec2 aTex;

varying vec3 color;
varying vec2 tex;

uniform mat4 mMatrix;

void main() {

    gl_Position = aPosition * mMatrix;
    gl_PointSize = 2.0;
    color = aColor;
    tex = aTex;
}