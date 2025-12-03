#version 330 core

in vec3 outColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 uObjectColor;
uniform int uUseObjectColor; // 1: 단색 모드, 0: 텍스처 모드
uniform float uBrightness;   // 밝기 조절

// 텍스처 관련
uniform sampler2D outTexture;
uniform int uUseTexture;     // 1: 텍스처 사용, 0: 미사용

void main()
{
    vec3 resultColor;

    if (uUseTexture == 1) {
        // 텍스처 사용 시: 이미지 색상 가져오기
        vec4 texColor = texture(outTexture, TexCoord);
        resultColor = texColor.rgb;
    }
    else if (uUseObjectColor == 1) {
        // 단색 모드 시: 지정된 색상 사용
        resultColor = uObjectColor;
    }
    else {
        // 그 외: 정점 색상 사용 (그라데이션 등)
        resultColor = outColor;
    }

//    // 터널 밝기(uBrightness) 적용 (0보다 클 때만)
//    if (uBrightness > 0.0)
//        resultColor = resultColor * uBrightness;

    FragColor = vec4(resultColor, 1.0);
}