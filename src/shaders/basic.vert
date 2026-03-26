#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uView * vec4(position, 1.0);
}
