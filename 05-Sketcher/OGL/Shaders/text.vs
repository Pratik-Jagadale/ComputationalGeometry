#version 330 core

in vec4 a_position; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

void main()
{
    gl_Position = u_projectionMatrix * vec4(a_position.xy, 0.0, 1.0);
    TexCoords = a_position.zw;
}
