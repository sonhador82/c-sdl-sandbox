#version 100

varying mediump vec3  color;
varying mediump vec2 tex;

uniform sampler2D sampler;

void main() {
    // gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
    gl_FragColor = texture2D(sampler, tex);
}