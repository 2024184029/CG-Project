#version 330 core
in vec3 vertexColor;          
out vec4 FragColor;          

uniform float uBrightness;   // <- Ãß°¡

void main()
{
FragColor = vec4(vertexColor * uBrightness, 1.0); 
}