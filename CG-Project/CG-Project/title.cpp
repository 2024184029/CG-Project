#include "title.h"
#include <GL/freeglut.h>

GLuint vaoTitle;
GLuint titleTexID;
extern GLuint shaderProgramID;

extern int gWidth;
extern int gHeight;

// 전역 플래그
bool gIsGameStarted = false;

// -------------------------------
// 초기화: VAO + 텍스처 로드
// -------------------------------
void Title_Init()
{
    // 1) 정점 구성
    float vertices[] = {
        // x, y, z,     r,g,b,    u,v
        -1, -1, 0,      1,1,1,    0,0,
         1, -1, 0,      1,1,1,    1,0,
        -1,  1, 0,      1,1,1,    0,1,
         1,  1, 0,      1,1,1,    1,1
    };

    GLuint vbo;
    glGenVertexArrays(1, &vaoTitle);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vaoTitle);
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

    // 2) 텍스처 로드
    extern void makeTexture(const char*, GLuint*);
    makeTexture("title.bmp", &titleTexID);
}

// -------------------------------
// 렌더링 (단위행렬로 NDC에 바로 뿌림)
// -------------------------------

void Title_Render()
{
    // 1. 깊이 테스트 끄기 (2D 이미지가 맨 앞에 보여야 함)
    glDisable(GL_DEPTH_TEST);

    // ★ [핵심 1] 셰이더 프로그램 사용 시작!
    glUseProgram(shaderProgramID);

    // 유니폼 위치 가져오기
    GLint locView = glGetUniformLocation(shaderProgramID, "view");
    GLint locProj = glGetUniformLocation(shaderProgramID, "projection");
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");
    GLint locUseTexture = glGetUniformLocation(shaderProgramID, "uUseTexture");
    GLint locUseObjectColor = glGetUniformLocation(shaderProgramID, "uUseObjectColor");
    GLint locBrightness = glGetUniformLocation(shaderProgramID, "uBrightness");

    // ★ [핵심 2] 텍스처 슬롯(Sampler) 위치 가져오기
    // 쉐이더에서 sampler2D 변수명이 "outTexture"라고 가정 (이전 코드 기반)
    GLint locTextureSlot = glGetUniformLocation(shaderProgramID, "outTexture");

    // 단위행렬 (Identity Matrix) 생성
    // 인트로 화면은 카메라 변환 없이 화면 좌표(-1 ~ 1)에 그대로 그립니다.
    float I[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    GLint locTexScale = glGetUniformLocation(shaderProgramID, "uTexScale");
    glUniform2f(locTexScale, 1.0f, 1.0f);

    // 행렬 전송
    glUniformMatrix4fv(locView, 1, GL_FALSE, I);
    glUniformMatrix4fv(locProj, 1, GL_FALSE, I);
    glUniformMatrix4fv(locModel, 1, GL_FALSE, I);

    // 쉐이더 설정
    glUniform1i(locUseTexture, 1);       // 텍스처 사용 ON
    glUniform1i(locUseObjectColor, 0);   // 객체 색상 OFF
    glUniform1f(locBrightness, 1.0f);    // 밝기 100%

    // ★ [핵심 3] 텍스처 유닛 연결
    glActiveTexture(GL_TEXTURE0);        // 0번 슬롯 활성화
    glBindTexture(GL_TEXTURE_2D, titleTexID); // 타이틀 이미지 바인딩
    glUniform1i(locTextureSlot, 0);      // 쉐이더에게 0번 슬롯을 쓰라고 알림

    // 그리기
    glBindVertexArray(vaoTitle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    // 상태 복구 (게임 화면을 위해 다시 켜기)
    glEnable(GL_DEPTH_TEST);
}

// -------------------------------
// 버튼 클릭 판정
// -------------------------------
bool Title_IsInsideButton(int x, int y)
{
    int yBottom = gHeight - y;

    int left = gWidth * 0.25;
    int right = gWidth * 0.75;
    int bottom = gHeight * 0.15;
    int top = gHeight * 0.30;

    return (x >= left && x <= right &&
        yBottom >= bottom && yBottom <= top);
}

// -------------------------------
// 세팅 버튼 클릭 판정 (좌측 하단 톱니 아이콘)
// -------------------------------
bool Title_IsInsideSettingButton(int x, int y)
{
    // GLUT에서 y는 위가 0이라서, 아래 기준으로 바꾸기
    int yBottom = gHeight - y;

    // 버튼 크기: 화면 높이의 8% 정도 정사각형 (800x600 기준 대략 48픽셀)
    int size = (int)(gHeight * 0.08f);

    // 좌측 하단에서 약간 안쪽으로 들어온 위치에 버튼 배치
    //  - left : 화면 왼쪽에서 5% 지점
    //  - bottom : 화면 아래에서 5% 지점
    int left = (int)(gWidth * 0.05f);
    int bottom = (int)(gHeight * 0.05f);

    int right = left + size;
    int top = bottom + size;

    return (x >= left && x <= right &&
        yBottom >= bottom && yBottom <= top);
}
