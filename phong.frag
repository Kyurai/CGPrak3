#version 130
varying vec4 texC;
varying vec4 norC;
varying vec3 normal;
varying vec3 position;
uniform sampler2D texture;


uniform vec4 lightPosition; // light position in world coordinates
uniform vec3 lightIntensity; // light 'color '
uniform vec3 kd; // diffuse
uniform vec3 ka; // ambient
uniform vec3 ks; // specular
uniform float shininess; // controls specular effect

float controlSpecular = 0.0;

void main()
{


    vec3 n = normalize(normal);
    vec3 s = normalize(vec3(lightPosition) - position);
    vec3 v = normalize(vec3(-position));
    vec3 r = reflect (-s, n);
    vec3 outputColor = lightIntensity * (
                        ka +
                        kd * max(dot(s, n), 0.0) +
                        (ks*controlSpecular) * pow(max(dot(r, v), 0.0), shininess));

    gl_FragColor = vec4(outputColor, 1.0f) * texture2D(texture, vec2(texC.x, texC.y));

}
