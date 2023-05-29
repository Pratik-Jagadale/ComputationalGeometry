#version 460 core

in vec3 MCposition;
in float LightIntensity;

uniform sampler3D u_textureSampler;

uniform vec3 LightWoodColor;
uniform vec3 DarkWoodColor;
uniform float RingFreq;
uniform float LightGrains;
uniform float DarkGrains;
uniform float GrainThreshold;
uniform vec3 NoiseScale;
uniform float Noisiness;
uniform float GrainScale;

out vec4 FragColor;

void main(void)
{
    
    vec4 noisevec = texture(u_textureSampler,MCposition*NoiseScale) * Noisiness;
    //vec4 noisevec=(texture(u_textureSampler,MCposition*NoiseScale));
    
    vec3 location=MCposition+vec3(noisevec);
    
    float dist=sqrt(location.x*location.x+location.z*location.z);
    dist *= RingFreq;
    
    float r=fract( dist+noisevec[0]+noisevec[1]+noisevec[2])*2.0;
    
    if(r>1.0)
    r=2.0-r;
    
    vec3 color=mix(LightWoodColor,DarkWoodColor,r);
    
    r=fract((MCposition.x+MCposition.z)*GrainScale+0.5);
    noisevec[2]*=r;
    
    if(r<GrainThreshold)
    color+=LightWoodColor*LightGrains*noisevec[2];
    else
    color-=LightWoodColor*LightGrains*noisevec[2];
    
    color*=LightIntensity;
    
    FragColor=vec4(color,1.0);
}