#version 130

attribute vec4 vert;
attribute vec4 texCoord;
attribute vec4 normCoord;

uniform mat4 modelMatrix;
uniform mat4 perspectiveMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

varying vec4 texC;
varying vec4 norC;

varying vec3 normal;
varying vec3 position;

void main()
{
    texC = texCoord;
    norC = normCoord;
    normal = normalize(normalMatrix * normCoord.xyz);
    position = vec3(modelMatrix * vert);
    gl_Position = perspectiveMatrix * viewMatrix * modelMatrix * vert;
}
