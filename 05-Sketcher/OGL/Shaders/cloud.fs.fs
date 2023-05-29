#version 460 core

in vec3  MCposition;
in float LightIntensity;

uniform sampler3D u_textureSampler;
uniform vec3 SkyColor;
uniform vec3 CloudColor;   
uniform float Scale;

out vec4 FragColor;
void main(void)
{

vec4  noisevec  = texture(u_textureSampler, MCposition * Scale);

float intensity = (noisevec[0] + noisevec[1] + 
                       noisevec[2] + noisevec[3] + 0.03125) * 1.5;

vec3 color   = mix(SkyColor, CloudColor, intensity) * LightIntensity;
//vec3 color   = mix(SkyColor, CloudColor, intensity) ;
FragColor = vec4(color, 1.0);
}