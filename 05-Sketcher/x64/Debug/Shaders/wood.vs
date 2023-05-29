#version 460 core

in vec4 a_position;
in vec3 a_normal;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform vec4 LightPos;

out float LightIntensity;
out vec3 MCposition;

void main(void)
{
    vec3 eyeCoordinate=vec3(u_viewMatrix*u_modelMatrix*a_position);
    MCposition=vec3(a_position);
    
    mat3 normalMatrix=mat3(transpose(inverse(u_viewMatrix*u_modelMatrix)));
    vec3 transformedNormals=normalize(normalMatrix*a_normal);
    
    LightIntensity=dot(normalize(vec3(LightPos)-eyeCoordinate),transformedNormals);
    LightIntensity*=1.5;
    
    gl_Position=u_projectionMatrix*u_viewMatrix*u_modelMatrix*a_position;
}