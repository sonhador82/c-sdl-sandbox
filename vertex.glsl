#version 100

attribute vec4 aPosition;
uniform mat4 aMVMatrix;

void main() {
    gl_Position = aMVMatrix*vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
}
