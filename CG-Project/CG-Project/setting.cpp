#include "setting.h"
#include <GL/freeglut.h>

extern GLuint shaderProgramID;
extern int gWidth;
extern int gHeight;

// 세팅 화면용 VAO / 텍스처
static GLuint vaoSetting = 0;
static GLuint settingTexID = 0;

// 세팅 화면 ON/OFF 플래그 (기본 false)
bool gIsSettingMode = false;

// -------------------------------
// 초기화: VAO + setting.bmp 로드
// -------------------------------
void Setting_Init()
{
    // 1) 정점 구성 (전체 화면 덮는 사각형)
    float vertices[] = {
        // x,  y,  z,   r,g,b,   u,v
        -1, -1, 0,   1,1,1,    0,0,
         1, -1, 0,   1,1,1,    1,0,
        -1,  1, 0,   1,1,1,    0,1,
         1,  1, 0,   1,1,1,    1,1
    };

    GLuint vbo;
    glGenVertexArrays(1, &vaoSetting);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vaoSetting);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // A: 위치 (0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // B: 색상 (1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // C: UV (2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // 2) setting.bmp 텍스처 로드
    extern void makeTexture(const char*, GLuint*);
    makeTexture("setting.bmp", &settingTexID);
}

// -------------------------------
// 세팅 화면 렌더링
// -------------------------------
void Setting_Render()
{
    // 1. 깊이 테스트 끄기 (2D 이미지 전면)
    glDisable(GL_DEPTH_TEST);

    glUseProgram(shaderProgramID);

    // 유니폼 위치
    GLint locView = glGetUniformLocation(shaderProgramID, "view");
    GLint locProj = glGetUniformLocation(shaderProgramID, "projection");
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");
    GLint locUseTexture = glGetUniformLocation(shaderProgramID, "uUseTexture");
    GLint locUseObjectColor = glGetUniformLocation(shaderProgramID, "uUseObjectColor");
    GLint locBrightness = glGetUniformLocation(shaderProgramID, "uBrightness");
    GLint locTextureSlot = glGetUniformLocation(shaderProgramID, "outTexture");
    GLint locTexScale = glGetUniformLocation(shaderProgramID, "uTexScale");

    // 단위행렬 (카메라 변환 없이 -1~1에 직접 그림)
    float I[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    glUniformMatrix4fv(locView, 1, GL_FALSE, I);
    glUniformMatrix4fv(locProj, 1, GL_FALSE, I);
    glUniformMatrix4fv(locModel, 1, GL_FALSE, I);

    glUniform2f(locTexScale, 1.0f, 1.0f);

    glUniform1i(locUseTexture, 1);
    glUniform1i(locUseObjectColor, 0);
    glUniform1f(locBrightness, 1.0f);

    // 텍스처 바인딩
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, settingTexID);
    glUniform1i(locTextureSlot, 0);

    // 그리기
    glBindVertexArray(vaoSetting);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    // 다시 깊이 테스트 ON (게임 화면 위해)
    glEnable(GL_DEPTH_TEST);
}

// -------------------------------
// 세팅 화면에서 "뒤로가기" 버튼 클릭 판정
// -------------------------------
// → 일단 타이틀 Start 버튼과 똑같이
//   화면 아래 중앙에 큰 버튼 하나 있다고 가정
//   (setting.bmp 만들 때 여기에 'Back' 같은 버튼을 두면 됨)
bool Setting_IsInsideBackButton(int x, int y)
{
    int yBottom = gHeight - y;

    int left = gWidth * 0.25;
    int right = gWidth * 0.75;
    int bottom = gHeight * 0.15;
    int top = gHeight * 0.30;

    return (x >= left && x <= right &&
        yBottom >= bottom && yBottom <= top);
}
