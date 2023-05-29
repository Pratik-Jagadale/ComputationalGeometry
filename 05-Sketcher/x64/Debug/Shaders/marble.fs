#version 460 core

in vec3  MCposition;
in float LightIntensity;

uniform sampler3D u_textureSampler;
uniform vec3 MarbleColor;
uniform vec3 VeinColor;   
uniform float Scale;

out vec4 FragColor;
void main(void)
{

vec4  noisevec  = texture(u_textureSampler, MCposition * Scale);

float intensity = abs(noisevec[0] - 0.25) +
                abs(noisevec[1] - 0.125) +
                abs(noisevec[2] - 0.0625) +
                abs(noisevec[3] - 0.03125);

float sineval = sin(MCposition.y * 6.0 + intensity * 12.0) * 0.5
 + 0.5;

vec3 color   = mix(VeinColor, MarbleColor, sineval) * LightIntensity;
FragColor = vec4(color, 1.0);
}