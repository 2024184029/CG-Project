#version 330 core

layout (location = 0) in vec3 vPos;   // 위치 (0번 속성)
layout (location = 1) in vec3 vColor; // 색상 (1번 속성)

out vec3 Color; // 프래그먼트 쉐이더로 넘겨줄 색상

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    Color = vColor;
}