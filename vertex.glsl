#version 100

attribute vec4 aPosition;
uniform mat4 aMVMatrix;

void main() {
    gl_Position = aPosition*aMVMatrix;
}
