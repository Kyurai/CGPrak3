#version 330
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable
// default330.vert: a simple vertex shader
//
// notes:
// GL_ARB_explicit_uniform_location is not needed for version >= 430
// GL_ARB_separate_shader_objects is not needed for version >= 410

layout(location = 0)uniform mat4 transMatrix;
layout(location = 1)uniform mat4 persMatrix;
layout(location = 0)in vec4 vert;
layout(location = 1)in vec4 color;
layout(location = 0)out vec4 col;

void main()
{
    col = vec4(1.0f,0.0f, 0.0f, 1.0f);
    gl_Position = transMatrix * vert;
}
