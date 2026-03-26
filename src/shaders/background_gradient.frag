#version 330 core
in vec2 vPos;
out vec4 FragColor;

uniform vec3 uColor;

void main()
{
    float t = (vPos.y + 1.0) * 0.5;
    vec3 topColor = uColor * 0.5;
    vec3 bottomColor = uColor;
    FragColor = vec4(mix(bottomColor, topColor, t), 1.0);
}
