#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexCoord;

out vec3 outColor;
out vec2 TexCoord; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// ★ C++에서 보내준 (buildingLen * density, h * density) 값이 여기로 들어옴
uniform vec2 uTexScale; 

void main()
{
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    outColor = vColor;

    // ★★★ [여기가 문제의 원인일 확률 99%] ★★★
    // 반드시 vTexCoord에 uTexScale을 곱해야 합니다.
    // 안 곱하면 C++에서 density를 아무리 높여도 소용이 없습니다.
    TexCoord = vTexCoord * uTexScale; 
}