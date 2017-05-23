#version 330
#extension GL_ARB_separate_shader_objects : enable
// default330.vert: a simple vertex shader
//
// notes:
// GL_ARB_explicit_uniform_location is not needed for version >= 430
// GL_ARB_separate_shader_objects is not needed for version >= 410

uniform mat4 matrix;
uniform mat4 perspectiveMatrix;
uniform mat4 viewMatrix;
layout(location = 0)in vec4 vert;
layout(location = 3)in vec4 texCoord;
layout(location = 4)in vec4 normCoord;
layout(location = 1)out vec4 texC;
layout(location = 2)out vec4 norC;



void main()
{
    texC = texCoord;
    norC = normCoord;
    gl_Position = perspectiveMatrix*viewMatrix*matrix * vert;
}
