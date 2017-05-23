#version 130
// default130. vert : a simple vertex shader
attribute vec4 vert;
attribute vec4 texCoord;
attribute vec4 normCoord;
uniform mat4 modelMatrix;
uniform mat4 perspectiveMatrix;
uniform mat4 viewMatrix;

//attribute vec4 color;
//varying vec4 col;
 // #version 330: in, location=3
varying vec4 texC; // #version 330: out
varying vec4 norC;
void main()
{
    //col = vec4(1.0f,0.0f, 0.0f, 1.0f);
    texC = texCoord;
    norC = normCoord;
    gl_Position = perspectiveMatrix * viewMatrix * modelMatrix * vert;
}
