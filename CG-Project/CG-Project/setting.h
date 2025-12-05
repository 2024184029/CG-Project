#pragma once
#include <GL/glew.h>

// 세팅 화면 초기화 (VAO + setting.bmp 텍스처)
void Setting_Init();

// 세팅 화면 렌더링
void Setting_Render();

// 세팅 화면에서 "뒤로가기" 버튼 클릭 판정
bool Setting_IsInsideBackButton(int x, int y);

// 현재 세팅 화면이 켜져 있는지 여부 (true면 setting.bmp 모드)
extern bool gIsSettingMode;
