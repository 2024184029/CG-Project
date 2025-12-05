#version 330 core

in vec3 outColor;
in vec2 TexCoord; // 버텍스 셰이더에서 uTexScale이 곱해져서 옴 (예: 0~3, 0~5 범위)

out vec4 FragColor;

uniform vec3 uObjectColor;
uniform int uUseObjectColor;
uniform float uBrightness;

uniform sampler2D outTexture;
uniform int uUseTexture;

// ★ [추가] 건물 창문 모드 (1: 켜기, 0: 끄기)
uniform int uIsBuilding; 

void main()
{
    vec3 resultColor;

    if (uUseTexture == 1) {
        vec4 texColor = texture(outTexture, TexCoord);
        resultColor = texColor.rgb;
    }
    // ★ [추가] 건물 창문 그리기 로직
    else if (uIsBuilding == 1) {
        // 1. 격자 만들기: 좌표의 소수점만 취함 (0.0 ~ 1.0 반복)
        vec2 grid = fract(TexCoord); 
        
        // 2. 창문 테두리 두께 설정 (0.0 ~ 0.5)
        // 값이 클수록 창문이 작아지고 벽이 두꺼워짐
        float border = 0.15; 
        // float border = 0.05; // 얇은 창틀
        
        // 3. 현재 픽셀이 테두리 안쪽(창문 영역)인지 확인
        if (grid.x > border && grid.x < (1.0 - border) &&
            grid.y > border && grid.y < (1.0 - border)) 
        {
            // --- 창문 색상 (약간 노란 빛) ---
            resultColor = vec3(0.9, 0.9, 0.7); 

            // (옵션) 랜덤하게 꺼진 창문 만들기 효과
            // sin 함수를 이용해 불규칙한 패턴 생성
            if (sin(floor(TexCoord.x) * 35.1 + floor(TexCoord.y) * 47.7) > 0.6) {
                 resultColor = vec3(0.3, 0.3, 0.3); // 꺼진 창문 (어두운 색)
            }
        } 
        else 
        {
            // --- 벽(테두리) 색상 ---
            // C++에서 넘겨준 건물 색상(uObjectColor)을 벽 색으로 사용
            resultColor = uObjectColor;
        }
    }
    else if (uUseObjectColor == 1) {
        resultColor = uObjectColor;
    }
    else {
        resultColor = outColor;
    }

    if (uBrightness > 0.0)
        resultColor = resultColor * uBrightness;

    FragColor = vec4(resultColor, 1.0);
}