#version 330
#extension GL_ARB_separate_shader_objects : enable
// default330.vert: a simple vertex shader
//
// notes:
// GL_ARB_explicit_uniform_location is not needed for version >= 430
// GL_ARB_separate_shader_objects is not needed for version >= 410

uniform mat4 matrix;
uniform mat4 perspectiveMatrix;
layout(location = 0)in vec4 vert;
layout(location = 3)in vec4 texCoord;
layout(location = 1)out vec4 texC;



void main()
{
    texC = texCoord;
    gl_Position = (perspectiveMatrix*matrix) * vert;
}
