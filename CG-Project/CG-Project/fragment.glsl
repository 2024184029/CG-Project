#version 330 core

in vec3 Color;      // 버텍스 쉐이더에서 받은 원래 색상 (터널용)
out vec4 FragColor; // 최종 출력 색상

uniform float uBrightness;      // 밝기 조절 (터널 등)
uniform vec3 uObjectColor;      // 물체 단색 (로봇용)
uniform bool uUseObjectColor;   // true면 단색 사용, false면 원래 색 사용

void main()
{
    vec3 resultColor;

    if(uUseObjectColor) {
        // 로봇: C++에서 보내준 특정 색상(살색, 빨강 등) 사용
        resultColor = uObjectColor; 
    } else {
        // 터널/배경: 원래 정점 색상에 밝기 적용
        resultColor = Color * uBrightness;
    }

    FragColor = vec4(resultColor, 1.0);
}