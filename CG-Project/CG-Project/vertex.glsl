#version 330 core

layout (location = 0) in vec3 vPos;       // 위치 (location 0)
layout (location = 1) in vec3 vColor;     // 색상 (location 1)
layout (location = 2) in vec2 vTexCoord;  // 텍스처 좌표 (location 2)

out vec3 outColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    outColor = vColor;
    TexCoord = vTexCoord;
}