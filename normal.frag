#version 130
// default130. frag : a simple fragment shader
//varying vec4 col;
varying vec4 texC; // #version 330: in
varying vec4 norC;
uniform sampler2D texture;
void main()
{
//gl_FragColor = col;
    gl_FragColor = norC;
}
