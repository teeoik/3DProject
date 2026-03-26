#version 330 core
in vec2 vPos;
out vec4 FragColor;

uniform vec3 uColor;
uniform vec2 uResolution;

void main()
{
    vec2 screenPos = (vPos + 1.0) * 0.5;
    vec2 pixelCoord = screenPos * uResolution;
    float tileSize = 40.0;
    vec2 tile = floor(pixelCoord / tileSize);
    
    float pattern = mod(tile.x + tile.y, 2.0);
    vec3 color1 = uColor;
    vec3 color2 = uColor * 1.3;
    
    FragColor = vec4(mix(color1, color2, pattern), 1.0);
}
