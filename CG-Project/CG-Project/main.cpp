#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <gl/glew.h>            
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h> 
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma warning(disable: 4711 4710 4100)

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void make_vertexShaders();
void make_fragmentShaders();
GLuint createShaderProgram();

GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;

// ------------ 전역변수 -------------
GLuint vaoCube[6];
GLuint vaoLaneLine;   // 바닥 레인 만들기 위한 VAO

float aspect = 1.0f; // 종횡비

// +++ 플레이어 위치 변수 추가 +++
float playerX = 0.0f;
float playerZ = -3.0f; // 초기 Z 위치 (기존 값)

float moveSpeed = 0.2f; // 한 번에 이동할 거리
//터널 z방향 이동량 (트레드밀 오프셋)
float tunnelOffsetZ = 0.0f;

//자동 이동되는 변수 추가
bool isAutoMove = false;
float autoMoveSpeedPerFrame = 0.08f;
//------------------------------------------------------------

float playerScale = 0.3f;  // 플레이어 큐브 크기 (기존값)
float obstacleSize = 0.25f; // 장애물 큐브 크기   (spawnRandomObstacle에서 설정)

// ================== ★★★ 장애물 관련 전역 변수 추가 ★★★ ==================
struct Obstacle {
    float x;       // 레인 위치 (왼/가운데/오른쪽)
    float z;       // 터널 기준 z 파라미터 (tunnelOffsetZ와 함께 사용)
    float size;    // 장애물 크기
    bool  active;  // 활성화 여부
	int lane;    // 장애물 어느 레인인지 (0: 왼쪽, 1: 가운데, 2: 오른쪽)
};

const int MAX_OBSTACLES = 50;
Obstacle obstacles[MAX_OBSTACLES];

const float OBSTACLE_Z_STRETCH = 3.f;  // 장애물을 z축으로 늘려서 길쭉하게 보이게 하는 용도

// ★★★ 레인 위치 & 간격 상수 ★★★
//좌우 부드러운 이동을 위한 변수
bool isLeftDown = false;  // 왼쪽 키가 눌려있는가?
bool isRightDown = false; // 오른쪽 키가 눌려있는가?
float playerMoveSpeed = 0.05f; // 좌우 이동 속도 (이 값을 키우면 더 빠르게 움직임)
const float PLAYER_LIMIT = 1.8f;

int currentLane = 0;        // 현재 레인 번호 (-1: 왼쪽, 0: 가운데, 1: 오른쪽)
float targetPlayerX = 0.0f; // 플레이어가 이동해야 할 목표 X 좌표
const float LANE_WIDTH = 0.6f; // 레인 간격
float laneSwitchSpeed = 0.04f;
//const float OBSTACLE_SHIFT = 0.5f;   // 플레이어 레인에서 얼마나 더 바깥/안쪽으로 밀지

// 레인 중심 (플레이어 + 장애물 둘 다 공통으로 사용) 
const float LANE_POSITIONS[3] = { -LANE_WIDTH, 0.0f, LANE_WIDTH};
// 장애물 레인 위치도 플레이어와 동일한 레인 중심 사용
// 장애물도 레인 중심 그대로 사용
const float OBSTACLE_LANE_POS[3] = {
    LANE_POSITIONS[0] - 0.5,
    LANE_POSITIONS[1],
    LANE_POSITIONS[2]+ 0.5
};
// 숫자 키우면 더 넓게, 줄이면 더 촘촘하게
const float OBSTACLE_Z_GAP = 10.0f;   // 10 정도면 점프+좌우 이동 충분히 여유
const float OBSTACLE_SPAWN_Z = -40.0f;  // 생성될 때 화면 멀~리 앞쪽에서 등장

// 얼마나 전진했을 때 새 장애물을 하나 더 만들지 체크용
float lastSpawnOffsetZ = 0.0f;

// [로봇 애니메이션용 전역 변수 추가]
float limbAngle = 0.0f;   // 팔다리 각도
float limbDir = 1.0f;     // 팔다리 움직임 방향

//점프 관련 변수
bool isJumping = false;
float jumpY = 0.0f;
float jumpVelocity = 0.0f;
const float GRAVITY = 8.f;
const float JUMP_POWER = 4.f;


// ------- 육면체 정점 -------------------------------------------

GLfloat cubeVertices[8][3] = {
   {-0.7f, -0.5f, -0.5f},        // {밑면 바닥, , }
   {0.7f, -0.5f, -0.5f},
   {0.7f, 0.5f, -0.5f},
   {-0.7f, 0.5f, -0.5f},
   {-0.7f,-0.5f, 0.5f},
   {0.7f, -0.5f, 0.5f},
   {0.7f, 0.5f,0.5f},
   {-0.7f, 0.5f, 0.5f}
};

int cubeFacesIndices[6][4] = {
   {0,1,2,3},
   {4,5,6,7},
   {0,1,5,4},
   {3,2,6,7},
   {0,3,7,4},
   {1,2,6,5}
};

GLfloat cubeFaceColors[6][3] = {
   {0.0f, 0.0f, 0.0f},        // 0: 앞   (안 보임)
   {0.0f, 0.0f, 0.0f},        // 1: 뒤   (안 보임)

   {0.0f, 0.0f, 1.0f},        // 2: 아래  (바닥)

   {0.5f, 0.0f, 1.0f},        // 3: 위   (천장)

   {0.2f, 0.4f, 1.0f},        // 4: 왼쪽 벽
   {0.2f, 0.4f, 1.0f},        // 5: 오른쪽 벽
};



// ------- 행렬 계산 ------
struct Mat4 {
    float m[16];
};

// 단위행렬
Mat4 identity() {
    Mat4 mat = { {
          1,0,0,0,
          0,1,0,0,
          0,0,1,0,
          0,0,0,1
    } };
    return mat;
}

Mat4 multifly(const Mat4& a, const Mat4& b) {
    Mat4 r = {};
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                r.m[i * 4 + j] += a.m[i * 4 + k] * b.m[k * 4 + j];
    return r;
}

Mat4 rotateX(float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    Mat4 mat = identity();

    mat.m[5] = c;
    mat.m[6] = s;
    mat.m[9] = -s;
    mat.m[10] = c;

    return mat;
}

Mat4 rotateY(float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    Mat4 mat = identity();

    mat.m[0] = c;
    mat.m[2] = -s;
    mat.m[8] = s;
    mat.m[10] = c;

    return mat;
}

Mat4 perspective(float fov, float aspect, float nearZ, float farZ) {
    float tanHalfFov = tanf(fov / 2.0f);
    Mat4 mat = {  };
    mat.m[0] = 1.0f / (aspect * tanHalfFov);
    mat.m[5] = 1.0f / tanHalfFov;
    mat.m[10] = -(farZ + nearZ) / (farZ - nearZ);
    mat.m[11] = -1.0f;
    mat.m[14] = -(2.0f * farZ * nearZ) / (farZ - nearZ);
    return mat;
}

// --- 추가: 크기 변환 행렬 ---
Mat4 scale(float sx, float sy, float sz)
{
    Mat4 m = { 0 };      // 모두 0으로 초기화
    m.m[0] = sx;       // (0,0)
    m.m[5] = sy;       // (1,1)
    m.m[10] = sz;       // (2,2)
    m.m[15] = 1.0f;     // (3,3) 동차좌표용 1
    return m;
}

// --- 추가: 이동 행렬 ---
Mat4 translate(float tx, float ty, float tz)
{
    Mat4 m = identity(); // 기본은 단위행렬
    m.m[12] = tx;        // 4x4 행렬에서 (3,0) = x 이동
    m.m[13] = ty;        // (3,1) = y 이동
    m.m[14] = tz;        // (3,2) = z 이동
    return m;
}


// ------ 행렬 함수 ---------------------------------

// --------------- 파일 불러오기 -----------------------------------------
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


void make_vertexShaders()
{
    GLchar* vertexSource;

    vertexSource = filetobuf("vertex.glsl");
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
        return;
    }
}

void make_fragmentShaders()
{
    GLchar* fragmentSource;

    fragmentSource = filetobuf("fragment.glsl");
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
        return;
    }
}

GLuint createShaderProgram()
{
    GLint result;
    GLchar* errorLog = NULL;
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
        return false;
    }

    glUseProgram(shaderID);

    return shaderID;
}



// --------- 도형 초기화 -----------------------------

void setupCubeVAOs()
{
    for (int f = 0; f < 6; f++) {
        GLfloat data[4 * 6];
        for (int i = 0; i < 4; i++) {
            int vi = cubeFacesIndices[f][i];
            for (int j = 0; j < 3; j++) data[i * 6 + j] = cubeVertices[vi][j];
            for (int j = 0; j < 3; j++) data[i * 6 + 3 + j] = cubeFaceColors[f][j];

            glEnableVertexAttribArray(1); // 바닥 레인용 VAO

        }

        GLuint vbo;
        glGenVertexArrays(1, &vaoCube[f]);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vaoCube[f]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // ---- 2) 바닥 3등분용 세로 라인 VAO ----
        // tunnelSegments = 20, 한 세그먼트 길이 = 1.0f 이니까 전체 길이 20.0f
        float lineLength = 20.0f;

        // 바닥 y는 -0.5 이니까 살짝만 위로 (-0.49) 띄워서 z-fighting 방지
        GLfloat lineColor[3] = { 1.0f, 1.0f, 1.0f }; // 흰색

        GLfloat lineVertices[] = {
            // x,      y,       z,             r, g, b
            -0.01f, -0.49f,  0.0f,            lineColor[0], lineColor[1], lineColor[2],
             0.01f, -0.49f,  0.0f,            lineColor[0], lineColor[1], lineColor[2],
            -0.01f, -0.49f, -lineLength,      lineColor[0], lineColor[1], lineColor[2],
             0.01f, -0.49f, -lineLength,      lineColor[0], lineColor[1], lineColor[2],
        };

        GLuint vboLaneLine;
        glGenVertexArrays(1, &vaoLaneLine);
        glGenBuffers(1, &vboLaneLine);

        glBindVertexArray(vaoLaneLine);
        glBindBuffer(GL_ARRAY_BUFFER, vboLaneLine);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
}

void drawCubeFace(int f) {
    glBindVertexArray(vaoCube[f]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

// --- 추가 --- 레인 경계선 하나 그리기 (현재 model 행렬 기준)
void drawLaneLine() {
    glBindVertexArray(vaoLaneLine);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}




//====================================================================
// ================== ★★★ 장애물 함수 구현 ★★★ ==================
void initObstacles()
{
    for (int i = 0; i < MAX_OBSTACLES; ++i) {
        obstacles[i].active = false;
        obstacles[i].x = 0.0f;
        obstacles[i].z = 0.0f;
        obstacles[i].size = 0.25f; // 기본 크기
        obstacles[i].lane = 0;   // 기본은 가운데 레인으로 초기화
    }
}
// 장애물 하나를 랜덤 레인에 생성
void spawnRandomObstacle()
{
    // 비활성 슬롯 찾기
    int idx = -1;
    for (int i = 0; i < MAX_OBSTACLES; ++i) {
        if (!obstacles[i].active) {
            idx = i;
            break;
        }
    }
    if (idx == -1) return; // 빈자리 없으면 생성 안 함

    // 0,1,2 중 하나 (배열 인덱스)
    int laneIndex = rand() % 3;
    // 실제 논리 레인 번호 (-1,0,1)
    int logicalLane = laneIndex - 1;

    // 충돌 판정용 레인 번호 저장
    obstacles[idx].lane = logicalLane;

    // 그림용 x 위치 (벽 쪽으로 밀고 싶으면 OBSTACLE_SHIFT 로 조절)
    obstacles[idx].x = OBSTACLE_LANE_POS[laneIndex];

    obstacles[idx].size = obstacleSize;
    obstacles[idx].active = true;

    obstacles[idx].z = OBSTACLE_SPAWN_Z - tunnelOffsetZ;

}

void updateObstacles()
{
    // 1) 기존 장애물들 정리
    for (int i = 0; i < MAX_OBSTACLES; ++i) {
        if (!obstacles[i].active) continue;

        // 화면에서 보이는 실제 z
        float worldZ = obstacles[i].z + tunnelOffsetZ;

        // 플레이어(대략 z = -5) 뒤로 완전히 지나가면 비활성화
        if (worldZ > 5.0f) {
            obstacles[i].active = false;
        }
    }

    // 2) 자동 이동 중일 때만 새 장애물 생성
    if (!isAutoMove) return;

    // 터널이 전진한 거리 계산
    float distanceSinceLastSpawn = tunnelOffsetZ - lastSpawnOffsetZ;

    // ★★★ OBSTACLE_Z_GAP 만큼 전진할 때마다 한 개 생성 ★★★
    // → 간격이 항상 일정하게 유지됨 (점프+좌우 이동할 시간 충분히 확보)
    if (distanceSinceLastSpawn > OBSTACLE_Z_GAP) {
        spawnRandomObstacle();
        lastSpawnOffsetZ = tunnelOffsetZ;
    }
}

// ============================================================







//idle 함수 추가( 자동 이동)
//idle 함수 (자동 이동 및 점프 물리 계산)
//idle 함수 (자동 이동 및 점프, 레인 변경 애니메이션)
void idle() {
    // 0. 프레임 간 시간(dt) 계산 ---------------------------------
    static int prevTime = glutGet(GLUT_ELAPSED_TIME);  // 처음 호출 시 초기화

    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float dt = (currentTime - prevTime) / 1000.0f;     // 밀리초 → 초 단위
    if (dt < 0.0001f) dt = 0.0001f;                    // 너무 작은 값 방지
    if (dt > 0.05f)   dt = 0.05f;                      // 렉 걸려도 이상하게 튀지 않게
    prevTime = currentTime;

    bool needRedisplay = false;



    // 1. 자동 이동 로직 ------------------------------------------
    if (isAutoMove) {
        // 기존에는 frame당 autoMoveSpeedPerFrame 만큼 움직였으니까
        // 대략 60fps 기준 속도 유지하려고 dt * 60.0f를 곱해줌
        tunnelOffsetZ += autoMoveSpeedPerFrame * (dt * 60.0f);

        updateObstacles();

        // 로봇 걷기 애니메이션도 시간에 살짝 비례하게
        limbAngle += 120.0f * limbDir * dt;   // 대략 초당 120도 정도
        if (limbAngle > 45.0f || limbAngle < -45.0f) {
            limbDir *= -1.0f; // 방향 전환
        }

        needRedisplay = true;
    }
    else {
        // 멈춰있을 때는 차렷 자세로 복귀 (시간에 비례해서 천천히 0으로)
        if (fabs(limbAngle) > 0.1f) {
            limbAngle *= powf(0.5f, dt * 10.0f);  // dt에 따라 서서히 줄어듦
            needRedisplay = true;
        }
    }



    // 2. 점프 물리 로직 (시간 기반) -------------------------------
    if (isJumping) {
        // 중력 방향으로 속도 감소 (v = v - g * dt)
        jumpVelocity -= GRAVITY * dt;

        // 위치는 속도에 따라 변화 (y = y + v * dt)
        jumpY += jumpVelocity * dt;

        // 바닥에 닿았는지 확인
        if (jumpY <= 0.0f) {
            jumpY = 0.0f;
            isJumping = false;
            jumpVelocity = 0.0f;
        }

        needRedisplay = true;
    }



    // 3. 레인 변경 애니메이션 (좌우 이동) -------------------------
    if (fabs(playerX - targetPlayerX) > 0.001f) {
        // laneSwitchSpeed를 "초당 이동량"처럼 쓰기 위해 dt를 곱해줌
        float dir = (playerX < targetPlayerX) ? 1.0f : -1.0f;
        playerX += dir * laneSwitchSpeed * (dt * 60.0f);

        // 목표를 살짝 넘으면 딱 맞춰주기
        if ((dir > 0.0f && playerX > targetPlayerX) ||
            (dir < 0.0f && playerX < targetPlayerX)) {
            playerX = targetPlayerX;
        }

        needRedisplay = true;
    }



    // 4. 화면 갱신 요청 ------------------------------------------
    if (needRedisplay) {
        glutPostRedisplay();
    }
}


// ----------------- 일반 키보드 입력 (스페이스바 등) ---------------------
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 32: // Spacebar ASCII code
        if (!isJumping) // 이미 점프 중이 아닐 때만 점프 가능
        {
            isJumping = true;
            jumpVelocity = JUMP_POWER; // 위로 솟구치는 힘 부여
        }
        break;

        // 나중에 'q'를 눌러 종료하는 기능 등을 여기에 추가할 수 있습니다.
    case 'q':
    case 'Q':
    case 27: // ESC key
        exit(0);
        break;
    }
}

// ----------------- 키보드 입력 ---------------------
// +++ 키보드 콜백 함수 추가 +++
void specialKeyboard(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP: // 'Page Up' 키
            isAutoMove = !isAutoMove; // 자동 이동 플래그를 끄거나 켬 (토글)
            break;
        case GLUT_KEY_DOWN: // 'DOWN' 화살표 키
            //playerZ += moveSpeed; // Z축 양의 방향(뒤로)으로 이동
            //break;
            tunnelOffsetZ -= moveSpeed;   // 터널을 뒤로 밀어냄 -> 뒤로 가는 느낌
            break;
        case GLUT_KEY_LEFT:
        {
            // 왼쪽 끝(-1)이 아닐 때만 왼쪽으로 한 칸 이동 명령
            if (currentLane > -1) {
                currentLane--; // -1 감소
                targetPlayerX = currentLane * LANE_WIDTH; // 목표 위치 재설정 (-0.5 or 0.0)
            }
            break;
            //=======================================================
            float nextX = playerX - moveSpeed;

            bool blocked = false;

            for (int i = 0; i < MAX_OBSTACLES; ++i) {
                if (!obstacles[i].active) continue;

                float worldZ = obstacles[i].z + tunnelOffsetZ;

                // 플레이어 Z 근처에 있는 장애물만 검사
                if (fabs(worldZ - (-5.0f)) < (playerScale + obstacles[i].size)) {

                    // X 충돌 범위 검사 (좌우 겹치면 충돌)
                    if (fabs(nextX - obstacles[i].x) < (playerScale + obstacles[i].size)) {
                        blocked = true;
                        break;
                    }
                }
            }

            if (!blocked) {
                playerX = nextX;
            }
            break;
        //=========================================================

        }

    case GLUT_KEY_RIGHT:
    {
            // 오른쪽 끝(1)이 아닐 때만 오른쪽으로 한 칸 이동 명령
            if (currentLane < 1) {
                currentLane++; // +1 증가
                targetPlayerX = currentLane * LANE_WIDTH; // 목표 위치 재설정 (0.0 or 0.5)
            }
            break;
            //===============================================
            float nextX = playerX + moveSpeed;

            bool blocked = false;

            for (int i = 0; i < MAX_OBSTACLES; ++i) {
                if (!obstacles[i].active) continue;

                float worldZ = obstacles[i].z + tunnelOffsetZ;

                if (fabs(worldZ - (-5.0f)) < (playerScale + obstacles[i].size)) {

                    if (fabs(nextX - obstacles[i].x) < (playerScale + obstacles[i].size)) {
                        blocked = true;
                        break;
                    }
                }
            }

            if (!blocked) {
                playerX = nextX;
            }
            break;
            //==========================================================

        }
    }

    // 화면을 갱신하도록 요청
    glutPostRedisplay();
}

// 로봇의 각 부위(큐브)를 그리는 헬퍼 함수
void drawColoredCube(glm::mat4 modelMatrix, glm::vec3 color) {
    // 1. 셰이더 유니폼 위치 가져오기
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");
    GLint locObjectColor = glGetUniformLocation(shaderProgramID, "uObjectColor");
    GLint locUseObjectColor = glGetUniformLocation(shaderProgramID, "uUseObjectColor");

    // 2. GLM 행렬을 셰이더로 전송 (glm::value_ptr 사용)
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // 3. 색상 전송 및 색상 모드 켜기
    glUniform3f(locObjectColor, color.r, color.g, color.b); // 원하는 색상
    glUniform1i(locUseObjectColor, 1); // true (로봇 색상 사용)

    // 4. 육면체 그리기 (기존 vaoCube 활용)
    // 색상은 셰이더에서 덮어쓰므로 아무 vao나 써도 되지만 6면을 다 그려야 함
    for (int i = 0; i < 6; i++) {
        glBindVertexArray(vaoCube[i]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    // 5. 색상 모드 끄기 (터널 그릴 때 영향을 주지 않도록)
    glUniform1i(locUseObjectColor, 0);
}

// 로봇 그리기 함수 (질문하신 코드 기반 수정)
void drawRobot(float x, float y, float z) {
    // 로봇의 기본 위치 설정
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, z));

    // 로봇이 뒤(Z-)를 보고 있으므로 180도 회전 (플레이어 시점)
    // 필요에 따라 각도 조절: glm::radians(180.0f)
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

    // 1. 몸통
    glm::mat4 bodyM = glm::scale(model, glm::vec3(0.4f, 0.5f, 0.2f));
    drawColoredCube(bodyM, glm::vec3(0.0f, 0.0f, 1.0f)); // 파랑

    // 2. 머리
    glm::mat4 headM = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.0f));
    glm::mat4 headScaleM = glm::scale(headM, glm::vec3(0.3f, 0.3f, 0.3f));
    drawColoredCube(headScaleM, glm::vec3(1.0f, 0.8f, 0.6f)); // 살색

    // 3. 코 (빨강)
    glm::mat4 noseM = glm::translate(headM, glm::vec3(0.0f, 0.0f, 0.18f)); // 약간 앞으로 뺌
    noseM = glm::scale(noseM, glm::vec3(0.05f, 0.05f, 0.05f));
    drawColoredCube(noseM, glm::vec3(1.0f, 0.0f, 0.0f));

    // 팔다리 움직임 각도
    float swing = glm::radians(limbAngle);

    // 4. 왼팔 (노랑)
    glm::mat4 lArmM = glm::translate(model, glm::vec3(-0.3f, 0.15f, 0.0f));
    lArmM = glm::rotate(lArmM, swing, glm::vec3(1.0f, 0.0f, 0.0f)); // 회전
    lArmM = glm::translate(lArmM, glm::vec3(0.0f, -0.2f, 0.0f)); // 회전축 아래로 내리기
    drawColoredCube(glm::scale(lArmM, glm::vec3(0.1f, 0.4f, 0.1f)), glm::vec3(1.0f, 1.0f, 0.0f));

    // 5. 오른팔 (초록)
    glm::mat4 rArmM = glm::translate(model, glm::vec3(0.3f, 0.15f, 0.0f));
    rArmM = glm::rotate(rArmM, -swing, glm::vec3(1.0f, 0.0f, 0.0f));
    rArmM = glm::translate(rArmM, glm::vec3(0.0f, -0.2f, 0.0f));
    drawColoredCube(glm::scale(rArmM, glm::vec3(0.1f, 0.4f, 0.1f)), glm::vec3(0.0f, 1.0f, 0.0f));

    // 6. 왼다리 (회색 + 보라 살짝 섞인 색으로 바꾸셨네요)
    glm::mat4 lLegM = glm::translate(model, glm::vec3(-0.12f, -0.25f, 0.0f)); // 1. 엉덩이 위치로 이동
    lLegM = glm::rotate(lLegM, -swing, glm::vec3(1.0f, 0.0f, 0.0f));          // 2. 회전

    lLegM = glm::translate(lLegM, glm::vec3(0.0f, -0.15f, 0.0f));
    drawColoredCube(glm::scale(lLegM, glm::vec3(0.12f, 0.3f, 0.12f)), glm::vec3(0.5f, 0.3f, 0.5f));

    // 7. 오른다리
    glm::mat4 rLegM = glm::translate(model, glm::vec3(0.12f, -0.25f, 0.0f));
    rLegM = glm::rotate(rLegM, swing, glm::vec3(1.0f, 0.0f, 0.0f));

    rLegM = glm::translate(rLegM, glm::vec3(0.0f, -0.15f, 0.0f));
    drawColoredCube(glm::scale(rLegM, glm::vec3(0.12f, 0.3f, 0.12f)), glm::vec3(0.3f, 0.3f, 0.3f));
}
void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramID);

    // --- 카메라(View)와 원근(Projection) 설정 ---
    Mat4 view = identity();
    Mat4 projection = perspective(45.0f * 3.14159f / 180.0f, aspect, 0.1f, 100.0f);
    GLint locView = glGetUniformLocation(shaderProgramID, "view");
    GLint locProj = glGetUniformLocation(shaderProgramID, "projection");
    glUniformMatrix4fv(locView, 1, GL_FALSE, view.m);
    glUniformMatrix4fv(locProj, 1, GL_FALSE, projection.m);
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");

    // 밝기 유니폼
    GLint locBrightness = glGetUniformLocation(shaderProgramID, "uBrightness");

    // --- 터널 그리기 (큐브 벽면 반복) ---
    int   tunnelSegments = 300;
    float tunnelScaleXY = 2.0f;                   // 터널 폭/높이 배율
    float playerBaseY = (-0.5f * tunnelScaleXY) + 0.5f; // 바닥 기준 플레이어 베이스 Y

    for (int i = 0; i < tunnelSegments; i++) {
        Mat4 modelTunnelScale = scale(tunnelScaleXY, tunnelScaleXY, 1.0f);
        Mat4 modelTunnelTranslate = translate(0.0f, 0.0f, -(float)i + tunnelOffsetZ);
        Mat4 model = multifly(modelTunnelTranslate, modelTunnelScale);

        glUniformMatrix4fv(locModel, 1, GL_FALSE, model.m);

        float brightness = (i % 2 == 0) ? 0.4f : 1.0f;
        glUniform1f(locBrightness, brightness);

        drawCubeFace(2); // 바닥
        drawCubeFace(4); // 왼쪽
        drawCubeFace(5); // 오른쪽
    }

    //// --- 바닥 3레인 경계선 그리기 ---
    {
        Mat4 model = identity();

        // 레인 경계선은 "레인 중심"들의 중간 지점에 위치시키기
        float leftLineX = (LANE_POSITIONS[0] + LANE_POSITIONS[1] + 0.1) * 0.5f; // 왼/중간 사이
        float rightLineX = (LANE_POSITIONS[1] + LANE_POSITIONS[2] - 0.1) * 0.5f; // 중간/오른쪽 사이

        Mat4 leftLine = model;
        leftLine.m[12] = leftLineX;
        glUniformMatrix4fv(locModel, 1, GL_FALSE, leftLine.m);
        drawLaneLine();

        Mat4 rightLine = model;
        rightLine.m[12] = rightLineX;
        glUniformMatrix4fv(locModel, 1, GL_FALSE, rightLine.m);
        drawLaneLine();
    }

    // ================== ★ 장애물 그리기 ★ ==================
    for (int i = 0; i < MAX_OBSTACLES; ++i) {
        if (!obstacles[i].active) continue;

        // 스폰 때 넣어 둔 z + 터널 이동량 = 장애물 기준 z
        float worldZ = obstacles[i].z + tunnelOffsetZ;
        float obsSize = obstacles[i].size;

        // Z 방향 길이 계산 (앞/뒤 대칭일 때의 반길이)
        float baseHalfDepth = 0.5f * obsSize;                    // 스트레치 전 반길이
        float stretchedHalfDepth = 0.5f * obsSize * OBSTACLE_Z_STRETCH; // 스트레치 후 반길이

        // "앞면 위치는 그대로 두고 뒤로만 늘리기" 위해
        // 센터를 뒤쪽으로 delta 만큼 옮겨줌
        float deltaCenterZ = stretchedHalfDepth - baseHalfDepth;      // 얼마나 뒤로 밀 것인지
        float centerZ = worldZ - deltaCenterZ;                   // 실제로 그릴 때 쓸 센터 z

        // 너무 멀리 있는 건 스킵 (센터 기준)
        if (centerZ > 5.0f || centerZ < -100.0f) continue;

        // Z만 길어진 직육면체
        Mat4 modelObsScale = scale(
            obsSize,                          // X 폭
            obsSize,                          // Y 높이
            obsSize * OBSTACLE_Z_STRETCH      // Z 길이
        );

        // 바닥 위로 반 높이만큼 올려서 딱 붙게
        float halfHeightY = 0.5f * obsSize;
        float obstacleCenterY = (-0.5f * tunnelScaleXY) + halfHeightY;

        // 방금 구한 centerZ 를 사용해서 배치
        Mat4 modelObsTranslate = translate(obstacles[i].x, obstacleCenterY, centerZ);
        Mat4 modelObs = multifly(modelObsTranslate, modelObsScale);

        glUniformMatrix4fv(locModel, 1, GL_FALSE, modelObs.m);
        glUniform1f(locBrightness, 0.8f);

        for (int f = 0; f < 6; ++f) {
            drawCubeFace(f);
        }
    }

    // ================== ★ 충돌 + 플랫폼 로직 ★ ==================
        // 플레이어 월드 Z (항상 -3.0 근처)
    float playerWorldZ = playerZ;

    // 플레이어 충돌 박스 (조금 작게 잡아서 너무 일찍 멈추지 않게)
    float playerHalfX = playerScale * 0.5f;
    float playerHalfZ = 0.15f;           // Z 반폭 줄임

    bool  onObstacle = false;         // 지금 장애물 위에 서 있는지
    float platformJumpY = 0.0f;          // 밟고 있는 장애물의 jumpY 높이

    for (int i = 0; i < MAX_OBSTACLES; ++i) {
        if (!obstacles[i].active) continue;

        // 장애물 월드 Z 위치 (큐브의 중심)
        float worldZ = obstacles[i].z + tunnelOffsetZ;
        float obsSize = obstacles[i].size;

        // 1) 레인이 다르면 X 방향으로는 절대 부딪히지 않음
        if (obstacles[i].lane != currentLane) continue;

        // 2) 장애물 실제 모델 기준 반폭 계산
        float halfWidthX = 0.7f * obsSize;                    // X 반폭
        float halfDepthZ = 0.5f * obsSize * OBSTACLE_Z_STRETCH; // Z 반폭 (z로 늘어난 만큼 반영)

        // 3) Z 방향(앞/뒤)으로만 겹치는지 확인
        bool overlapZ = fabs(playerWorldZ - worldZ) < (halfDepthZ + playerHalfZ);
        if (!overlapZ) continue;

        // 4) “jumpY 기준” 장애물 윗면 높이
        //    바닥(-1.0)에서 장애물 윗면까지 높이가 정확히 obsSize 만큼이라
        //    jumpY가 obsSize 가 되면 장애물 위에 올라선 상태가 됨.
        float thisPlatformJumpY = obsSize;

        if (isJumping) {
            // 점프해서 내려오는 중인데, 장애물 높이 근처에 닿으면 착지
            if (jumpVelocity <= 0.0f &&
                jumpY >= thisPlatformJumpY - 0.05f &&
                jumpY <= thisPlatformJumpY + 0.05f)
            {
                isJumping = false;
                jumpVelocity = 0.0f;
                jumpY = thisPlatformJumpY;
                onObstacle = true;
                platformJumpY = thisPlatformJumpY;
            }
        }
        else {
            // 점프 중이 아닌 상태에서 겹친 경우
            if (jumpY > thisPlatformJumpY * 0.8f) {
                // 이미 위쪽에 있으면 장애물 위에 서 있는 것으로 처리
                jumpY = thisPlatformJumpY;
                onObstacle = true;
                platformJumpY = thisPlatformJumpY;
            }
            else {
                // 거의 바닥 높이에서 정면으로 박으면 앞으로 진행을 멈춤
                isAutoMove = false;
            }
        }
    }

    // 5) 최종 jumpY 정리
    if (onObstacle) {
        // 장애물 위에 있을 땐 jumpY를 그 높이로 고정
        jumpY = platformJumpY;
    }
    else {
        // 어느 플랫폼 위도 아니고, 바닥보다 위에 떠 있는데
        // 점프 상태가 아니면 자연스럽게 떨어지도록 점프 모드로 전환
        if (!isJumping && jumpY > 0.0f) {
            isJumping = true;
            // 이미 위에서 떨어지는 중이라 초기 속도는 0으로 둠
            jumpVelocity = 0.0f;
        }
    }

    // 최종 플레이어 Y
    float finalPlayerY = playerBaseY + jumpY + 0.05f;

    // ================== ★ 플레이어 로봇 그리기 (항상 맨 위 레이어) ★
    glDisable(GL_DEPTH_TEST);
    drawRobot(playerX, finalPlayerY, playerZ);
    glEnable(GL_DEPTH_TEST);

    glutSwapBuffers();
   
}

int main(int argc, char** argv)
{

    srand((unsigned)time(NULL));

    //윈도우 생성하기
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 깊이 버퍼 추가
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG Project");

    //GLEW 초기화하기
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";


    //세이더 읽어와서 세이더 프로그램 만들기 
    make_vertexShaders();
    make_fragmentShaders();

    // ------ 추가 ----------
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.f, 1.f, 1.f, 1.f);
    shaderProgramID = createShaderProgram();

    setupCubeVAOs(); // 큐브 그림



    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutSpecialFunc(specialKeyboard);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}

GLvoid drawScene()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    glPointSize(5.0f);
    glDrawArrays(GL_POINTS, 0, 1);

    glutSwapBuffers();

}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    aspect = (float)w / (float)h;
}