#version 330 core

out vec4 FragColor;

uniform vec3 uMeshColor;

void main()
{
    FragColor = vec4(uMeshColor, 1.0);
}
