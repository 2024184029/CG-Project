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
    glDisable(GL_DEPTH_TEST);

    // 전역 shaderProgramID 를 그대로 사용
    GLint locView = glGetUniformLocation(shaderProgramID, "view");
    GLint locProj = glGetUniformLocation(shaderProgramID, "projection");
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");
    GLint locUseTexture = glGetUniformLocation(shaderProgramID, "uUseTexture");
    GLint locUseObjectColor = glGetUniformLocation(shaderProgramID, "uUseObjectColor");
    GLint locBrightness = glGetUniformLocation(shaderProgramID, "uBrightness");

    // 단위행렬
    float I[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    glUniformMatrix4fv(locView, 1, GL_FALSE, I);
    glUniformMatrix4fv(locProj, 1, GL_FALSE, I);
    glUniformMatrix4fv(locModel, 1, GL_FALSE, I);

    glUniform1i(locUseTexture, 1);
    glUniform1i(locUseObjectColor, 0);
    glUniform1f(locBrightness, 1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, titleTexID);

    glBindVertexArray(vaoTitle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

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
