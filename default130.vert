#version 130
// default130.vert: a simple vertex shader

uniform mat4 matrix;
attribute vec4 vert;
attribute vec4 texCoord;
varying vec4 texC;

void main()
{
    texC = texCoord;
    gl_Position = matrix * vert;
}
