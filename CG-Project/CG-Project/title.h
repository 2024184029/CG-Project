#pragma once
#include <GL/glew.h>

// 외부에서 접근할 함수들
void Title_Init();
void Title_Render();
bool Title_IsInsideButton(int x, int y);

extern bool gIsGameStarted;
