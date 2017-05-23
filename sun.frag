#version 130
varying vec4 texC;
varying vec4 norC;
uniform sampler2D texture;
uniform sampler2D distortionTex;
uniform float time;
void main()
{
    vec4 distortionMapCoord = texC;
    distortionMapCoord.y -= time*2;
    vec4 distortionValue = texture2D(distortionTex, vec2(distortionMapCoord.x, distortionMapCoord.y));
    vec2 distortionOffset = distortionValue.xy;
    distortionOffset -= (0.5, 0.5);
    distortionOffset *= 2;
    distortionOffset *= 0.05;
    distortionOffset *= (1 - texC.y);
    vec4 distortedCoord = texC;
    distortedCoord.x += distortionOffset.x;
    distortedCoord.y += distortionOffset.y;
    vec4 color1 = texture2D(texture, vec2(texC.x, texC.y));
    vec4 color2 = texture2D(texture, vec2(distortedCoord.x, distortedCoord.y));
    gl_FragColor = color1 * color2;
}
