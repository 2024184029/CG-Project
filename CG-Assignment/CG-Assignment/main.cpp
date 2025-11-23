#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits>

#include <gl/glew.h>				
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h> 

#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma warning(disable: 4711 4710 4100)

// -------------------- 함수 선언 --------------------
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
void initScene();
void askGridSize();
void update(int value);
void keyboard(unsigned char key, int x, int y);

// -------------------- 셰이더 전역 --------------------
GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;

// -------------------- VAO / VBO --------------------
GLuint gFloorVAO = 0;
GLuint gFloorVBO = 0;

GLuint gCubeVAO = 0;
GLuint gCubeVBO = 0;

// -------------------- MVP 유니폼 위치 --------------------
GLint locModel = -1;
GLint locView = -1;
GLint locProj = -1;

// -------------------- 카메라 / 프로젝션 --------------------
glm::mat4 gView(1.0f);
glm::mat4 gProj(1.0f);

// -------------------- 메인 카메라 / 투영 설정 --------------------
// o p z 명령어
glm::vec3 gEyeMain = glm::vec3(0.0f, 15.0f, 25.0f); // 카메라 위치
glm::vec3 gCenterMain = glm::vec3(0.0f, 0.0f, 0.0f); // 바라보는 지점
glm::vec3 gUpMain = glm::vec3(0.0f, 1.0f, 0.0f); // 위 방향

bool  gUsePerspective = true;   // true: 원근 투영, false: 직각 투영
float gFovY = 45.0f;  // 원근 투영일 때 시야각(도 단위)
float gCamZStep = 1.0f;   // z/Z 키 눌렀을 때 이동량


// -------------------- 창 크기 --------------------
int gWidth = 1280;
int gHeight = 960;

// -------------------- 바닥/그리드 설정 --------------------
const float FLOOR_HALF_SIZE = 10.0f;   // 바닥은 x,z 방향 [-10, 10]

// 입력으로 받는 칸 수 (5~25)
int gGridX = 10;   // 가로 (x 방향)
int gGridZ = 10;   // 세로 (z 방향)

// -------------------- 큐브 높이 / 애니메이션 --------------------
const int MAX_GRID = 25;

// 각 칸의 최대 높이 (min~max 안에서 랜덤, 여기서는 1.0~5.0)
float gMaxHeight[MAX_GRID][MAX_GRID];

// 각 칸의 속도 계수 (속도 다양하게 주기 위함, 여기서는 0.5~2.0배)
float gSpeedFactor[MAX_GRID][MAX_GRID];

// 전체 시간 (애니메이션용)
float gGlobalTime = 0.0f;

// 애니메이션 재생 여부 (true: 움직임, false: 정지)
bool  gAnimating = true;

// 지난 프레임 시간(ms)
int   gLastTimeMS = 0;

// 큐브 높이 범위 (원하면 숫자 마음대로 바꿔도 됨)
const float GLOBAL_MIN_H = 0.5f;  // 최소 높이
const float GLOBAL_MAX_H = 5.0f;  // 최대 높이



// -------------------- glsl 파일 불러오기 --------------------
char* filetobuf(const char* file)
{
    FILE* fptr;
    long length;
    char* buf;

    fptr = fopen(file, "rb");
    if (!fptr)
        return NULL;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;

    return buf;
}

// -------------------- 셰이더 유틸 --------------------
void make_vertexShaders()
{
    GLchar* vertexSource = filetobuf("vertex.glsl");
    if (!vertexSource) {
        std::cerr << "vertex.glsl 파일을 열 수 없습니다.\n";
        exit(-1);
    }

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
        free(vertexSource);
        exit(-1);
    }

    free(vertexSource);
}

void make_fragmentShaders()
{
    GLchar* fragmentSource = filetobuf("fragment.glsl");
    if (!fragmentSource) {
        std::cerr << "fragment.glsl 파일을 열 수 없습니다.\n";
        exit(-1);
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
        free(fragmentSource);
        exit(-1);
    }

    free(fragmentSource);
}

GLuint make_shaderProgram()
{
    GLint result;
    GLchar errorLog[512];
    GLuint shaderID;
    shaderID = glCreateProgram();

    glAttachShader(shaderID, vertexShader);
    glAttachShader(shaderID, fragmentShader);

    glLinkProgram(shaderID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
        std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
        return 0;
    }

    glUseProgram(shaderID);

    return shaderID;
}

// -------------------- 콘솔에서 그리드 크기 입력 --------------------
void askGridSize()
{
    while (true)
    {
        std::cout << "가로 길이와 세로 길이를 입력해주세요. (5 ~ 25 범위 내의 숫자를 입력)\n";
        std::cout << "입력 : ";

        int gx, gz;
        if (!(std::cin >> gx >> gz)) {
            // 숫자 입력 실패 (글자 등)
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "숫자로 다시 입력해주세요.\n\n";
            continue;
        }

        if (gx < 5 || gx > 25 || gz < 5 || gz > 25) {
            std::cout << "5 이상 25 이하의 정수를 입력해야 합니다.\n\n";
            continue;
        }

        gGridX = gx;
        gGridZ = gz;
        std::cout << "가로 = " << gGridX << ", 세로 = " << gGridZ << " 로 설정되었습니다.\n\n";
        break;
    }
}

// 메인 화면용 view / projection 행렬을 현재 상태에 맞게 갱신
void updateMainViewProj()
{
    // 창 비율
    float aspect = (float)gWidth / (float)gHeight;
    if (aspect <= 0.0f) aspect = 1.0f;

    // view 행렬 : 현재 카메라 위치/방향 기준
    gView = glm::lookAt(gEyeMain, gCenterMain, gUpMain);

    // projection 행렬 : 직각 / 원근 토글
    if (gUsePerspective)
    {
        // 원근 투영
        gProj = glm::perspective(glm::radians(gFovY), aspect, 0.1f, 100.0f);
    }
    else
    {
        // 직각 투영 (바닥 전체가 보이도록 약간 여유 있게)
        float size = FLOOR_HALF_SIZE * 1.5f; // 필요하면 숫자 조절
        gProj = glm::ortho(
            -size * aspect, size * aspect,  // left, right
            -size, size,           // bottom, top
            -50.0f, 50.0f           // near, far
        );
    }
}


// -------------------- 바닥 & 큐브 초기화 --------------------
void initScene()
{
    // ---------- 바닥 ----------
    float floorVertices[] =
    {
        //  position                               // color
        -FLOOR_HALF_SIZE, 0.0f, -FLOOR_HALF_SIZE,   0.8f, 0.8f, 0.85f,
         FLOOR_HALF_SIZE, 0.0f,  FLOOR_HALF_SIZE,   0.8f, 0.8f, 0.85f,
         FLOOR_HALF_SIZE, 0.0f, -FLOOR_HALF_SIZE,   0.8f, 0.8f, 0.85f,

        -FLOOR_HALF_SIZE, 0.0f, -FLOOR_HALF_SIZE,   0.8f, 0.8f, 0.85f,
        -FLOOR_HALF_SIZE, 0.0f,  FLOOR_HALF_SIZE,   0.8f, 0.8f, 0.85f,
         FLOOR_HALF_SIZE, 0.0f,  FLOOR_HALF_SIZE,   0.8f, 0.8f, 0.85f
    };

    glGenVertexArrays(1, &gFloorVAO);
    glGenBuffers(1, &gFloorVBO);

    glBindVertexArray(gFloorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gFloorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // ---------- 큐브(단위 큐브, 밑면 y=0, 윗면 y=1) ----------
    float cubeVertices[] =
    {
        // 앞면
        -0.5f, 0.0f,  0.5f,   0.9f, 0.9f, 0.95f,
         0.5f, 0.0f,  0.5f,   0.9f, 0.9f, 0.95f,
         0.5f, 1.0f,  0.5f,   0.9f, 0.9f, 0.95f,

        -0.5f, 0.0f,  0.5f,   0.9f, 0.9f, 0.95f,
         0.5f, 1.0f,  0.5f,   0.9f, 0.9f, 0.95f,
        -0.5f, 1.0f,  0.5f,   0.9f, 0.9f, 0.95f,

        // 뒷면
        -0.5f, 0.0f, -0.5f,   0.9f, 0.9f, 0.95f,
         0.5f, 1.0f, -0.5f,   0.9f, 0.9f, 0.95f,
         0.5f, 0.0f, -0.5f,   0.9f, 0.9f, 0.95f,

        -0.5f, 0.0f, -0.5f,   0.9f, 0.9f, 0.95f,
        -0.5f, 1.0f, -0.5f,   0.9f, 0.9f, 0.95f,
         0.5f, 1.0f, -0.5f,   0.9f, 0.9f, 0.95f,

         // 왼쪽면
         -0.5f, 0.0f, -0.5f,   0.85f, 0.85f, 0.9f,
         -0.5f, 0.0f,  0.5f,   0.85f, 0.85f, 0.9f,
         -0.5f, 1.0f,  0.5f,   0.85f, 0.85f, 0.9f,

         -0.5f, 0.0f, -0.5f,   0.85f, 0.85f, 0.9f,
         -0.5f, 1.0f,  0.5f,   0.85f, 0.85f, 0.9f,
         -0.5f, 1.0f, -0.5f,   0.85f, 0.85f, 0.9f,

         // 오른쪽면
          0.5f, 0.0f, -0.5f,   0.85f, 0.85f, 0.9f,
          0.5f, 1.0f,  0.5f,   0.85f, 0.85f, 0.9f,
          0.5f, 0.0f,  0.5f,   0.85f, 0.85f, 0.9f,

          0.5f, 0.0f, -0.5f,   0.85f, 0.85f, 0.9f,
          0.5f, 1.0f, -0.5f,   0.85f, 0.85f, 0.9f,
          0.5f, 1.0f,  0.5f,   0.85f, 0.85f, 0.9f,

          // 윗면
          -0.5f, 1.0f, -0.5f,   0.92f, 0.92f, 0.96f,
          -0.5f, 1.0f,  0.5f,   0.92f, 0.92f, 0.96f,
           0.5f, 1.0f,  0.5f,   0.92f, 0.92f, 0.96f,

          -0.5f, 1.0f, -0.5f,   0.92f, 0.92f, 0.96f,
           0.5f, 1.0f,  0.5f,   0.92f, 0.92f, 0.96f,
           0.5f, 1.0f, -0.5f,   0.92f, 0.92f, 0.96f,

           // 아랫면 (사실 거의 안 보임)
           -0.5f, 0.0f, -0.5f,   0.8f, 0.8f, 0.85f,
            0.5f, 0.0f,  0.5f,   0.8f, 0.8f, 0.85f,
            0.5f, 0.0f, -0.5f,   0.8f, 0.8f, 0.85f,

           -0.5f, 0.0f, -0.5f,   0.8f, 0.8f, 0.85f,
           -0.5f, 0.0f,  0.5f,   0.8f, 0.8f, 0.85f,
            0.5f, 0.0f,  0.5f,   0.8f, 0.8f, 0.85f
    };

    glGenVertexArrays(1, &gCubeVAO);
    glGenBuffers(1, &gCubeVBO);

    glBindVertexArray(gCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // ---------- 카메라 / 투영 ----------
    //glm::vec3 eye = glm::vec3(0.0f, 15.0f, 25.0f);
    //glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    //glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    //gView = glm::lookAt(eye, center, up);

    //float aspect = (float)gWidth / (float)gHeight;
    //gProj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        // ---------- 카메라 / 투영 ----------
    updateMainViewProj();


    // ---------- 유니폼 위치 ----------
    locModel = glGetUniformLocation(shaderProgramID, "model");
    locView = glGetUniformLocation(shaderProgramID, "view");
    locProj = glGetUniformLocation(shaderProgramID, "projection");

    // ---------- 큐브 높이 / 속도 초기화 ----------
    // gMaxHeight: GLOBAL_MIN_H ~ GLOBAL_MAX_H
    // gSpeedFactor: 0.5 ~ 2.0 사이
    for (int z = 0; z < MAX_GRID; ++z)
    {
        for (int x = 0; x < MAX_GRID; ++x)
        {
            float r1 = rand() / (float)RAND_MAX;
            float r2 = rand() / (float)RAND_MAX;

            gMaxHeight[z][x] = GLOBAL_MIN_H + r1 * (GLOBAL_MAX_H - GLOBAL_MIN_H);
            gSpeedFactor[z][x] = 0.5f + r2 * 1.5f;   // 0.5 ~ 2.0배
        }
    }

    gGlobalTime = 0.0f;
    gLastTimeMS = glutGet(GLUT_ELAPSED_TIME);

}

// -------------------- 그리기 콜백 --------------------

// 화면에 바닥 + 큐브 전체를 그리는 공용 함수 => 화면 우측 상단에 탑뷰 미니맵을 그림
// view, proj 만 다르게 넣어서 메인화면/탑뷰 둘 다 사용
void drawWorld(const glm::mat4& view, const glm::mat4& proj)
{
    // 카메라 / 투영 행렬 유니폼 설정
    glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(locProj, 1, GL_FALSE, glm::value_ptr(proj));

    // ---------- 1) 바닥 ----------
    {
        glm::mat4 modelFloor(1.0f);
        glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(modelFloor));

        glBindVertexArray(gFloorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // ---------- 2) 큐브들 ----------
    float fullSizeX = FLOOR_HALF_SIZE * 2.0f;
    float fullSizeZ = FLOOR_HALF_SIZE * 2.0f;

    float cellSizeX = fullSizeX / (float)gGridX;
    float cellSizeZ = fullSizeZ / (float)gGridZ;

    float cubeSizeX = cellSizeX * 0.8f;
    float cubeSizeZ = cellSizeZ * 0.8f;

    glBindVertexArray(gCubeVAO);

    for (int z = 0; z < gGridZ; ++z)
    {
        for (int x = 0; x < gGridX; ++x)
        {
            float cx = -FLOOR_HALF_SIZE + (x + 0.5f) * cellSizeX;
            float cz = -FLOOR_HALF_SIZE + (z + 0.5f) * cellSizeZ;

            // 여기 h 계산 부분은 너 지금 쓰고 있는 수식 그대로 넣으면 됨
            //    (GLOBAL_MIN_H ~ gMaxHeight[z][x] 사이에서 sin 파형으로 왕복하는 그 부분)
            float maxH = gMaxHeight[z][x];
            float speed = gSpeedFactor[z][x];
            float t = gGlobalTime * speed;
            float hNorm = 0.5f * (sinf(t) + 1.0f);   // 0 ~ 1
            float h = GLOBAL_MIN_H + hNorm * (maxH - GLOBAL_MIN_H);

            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(cx, 0.0f, cz));
            model = glm::scale(model, glm::vec3(cubeSizeX, h, cubeSizeZ));

            glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    glBindVertexArray(0);
}


GLvoid drawScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    // ---------------- 1) 메인 화면 (전체 창) ----------------
    glViewport(0, 0, gWidth, gHeight);

    // 기존에 쓰던 메인 카메라 그대로 사용 (gView, gProj)
    drawWorld(gView, gProj);

    // ---------------- 2) 탑뷰 미니맵 (오른쪽 위 작은 창) ----------------
    // 뎁스 버퍼만 한 번 더 지워서 메인 화면 깊이값이 영향 안 주게
    glClear(GL_DEPTH_BUFFER_BIT);

    // 미니맵 크기/위치 (창의 1/4 크기, 오른쪽 위에서 10px 여백)
    int miniW = gWidth / 4;
    int miniH = gHeight / 4;
    int miniX = gWidth - miniW - 10;
    int miniY = gHeight - miniH - 10;

    glViewport(miniX, miniY, miniW, miniH);

    // 탑뷰 카메라 설정 (위에서 아래로 내려다보는 시점)
    //    eye: 위쪽 Y에서 아래를 봄, up 벡터는 -Z 방향으로 잡음
    glm::vec3 eyeTop = glm::vec3(0.0f, 40.0f, 0.0f);
    glm::vec3 centerTop = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upTop = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::mat4 viewTop = glm::lookAt(eyeTop, centerTop, upTop);

    // 탑뷰는 전체 바닥이 딱 들어오게 정사영(ortho)로 잡자
    float orthoSize = FLOOR_HALF_SIZE * 1.1f;  // 살짝 여유
    glm::mat4 projTop = glm::ortho(
        -orthoSize, orthoSize,
        -orthoSize, orthoSize,
        -50.0f, 50.0f
    );

    drawWorld(viewTop, projTop);

    glutSwapBuffers();
}


// -------------------- 리쉐이프 --------------------
GLvoid Reshape(int w, int h)
{
    if (h == 0) h = 1;
    gWidth = w;
    gHeight = h;

    glViewport(0, 0, w, h);

    // 창 크기가 바뀌면 비율도 바뀌니까, view/proj 다시 계산
    updateMainViewProj();

    glutPostRedisplay();
}

// -------------------- 애니메이션 업데이트 --------------------
void update(int value)
{
    int now = glutGet(GLUT_ELAPSED_TIME);
    float dt = (now - gLastTimeMS) / 1000.0f;   // 초 단위
    gLastTimeMS = now;

    if (gAnimating)          // 재생 중일 때만 time 진행
        gGlobalTime += dt;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}



// -------------------- 키보드 입력 --------------------
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'm':   // 소문자 m: 애니메이션 재생
        gAnimating = true;
        break;

    case 'M':   // 대문자 M: 애니메이션 정지
        gAnimating = false;
        break;

        // ----- o / p : 투영 방식 선택 -----
    case 'o':
    case 'O':
        // 직각 투영(orthographic)으로 전환
        gUsePerspective = false;
        updateMainViewProj();
        break;

    case 'p':
    case 'P':
        // 원근 투영(perspective)으로 전환
        gUsePerspective = true;
        updateMainViewProj();
        break;

        // ----- z / Z : 원근 투영 시 z축 이동 -----
    case 'z':   // 더 멀어지기 (z값 증가)
        if (gUsePerspective)
        {
            gEyeMain.z += gCamZStep;   // 카메라를 +z 방향으로 이동
            updateMainViewProj();
        }
        break;

    case 'Z':   // 더 가까워지기 (z값 감소)
        if (gUsePerspective)
        {
            gEyeMain.z -= gCamZStep;   // 카메라를 -z 방향으로 이동
            updateMainViewProj();
        }
        break;

        // ----- 종료 -----
    case 'q':
    case 'Q':
    case 27: // ESC
        exit(0);
        break;
    }
}


// -------------------- main --------------------
int main(int argc, char** argv)
{
    // 1) 콘솔에서 그리드 입력
    askGridSize();

    srand((unsigned)time(NULL));

    // 2) 윈도우 생성
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(gWidth, gHeight);
    glutCreateWindow("25숙제");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";

    // 셰이더
    make_vertexShaders();
    make_fragmentShaders();
    shaderProgramID = make_shaderProgram();

    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	glFrontFace(GL_CCW); // 반시계 방향이 앞면

    // 장면 초기화
    initScene();

    // 콜백 등록
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(keyboard);

    // 애니메이션 타이머 시작 (대략 60FPS)
    glutTimerFunc(16, update, 0);

    glutMainLoop();
}

