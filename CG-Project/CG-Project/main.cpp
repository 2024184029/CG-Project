#define _CRT_SECURE_NO_WARNINGS 
#include "title.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>
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

void drawWoodPlanks();    // 레일 위 목재 침목 그리기
void drawColoredCube(glm::mat4 modelMatrix, glm::vec3 color);

// ------------ 전역변수 -------------
GLuint vaoCube[6];
GLuint vaoLaneLine;   // 바닥 레인 만들기 위한 VAO
GLuint vaoCoin;     // 코인 VAO
const int COIN_SEGMENTS = 32;
// 앞/뒤 원판 + 옆면(삼각형들) 개수
const int COIN_FRONT_COUNT = COIN_SEGMENTS + 2;      // 앞면 팬
const int COIN_BACK_COUNT = COIN_SEGMENTS + 2;      // 뒷면 팬
const int COIN_SIDE_COUNT = COIN_SEGMENTS * 6;      // 옆면(세그먼트당 삼각형 2개 = 정점 6개)

// 전체 정점 수
const int COIN_VERT_COUNT = COIN_FRONT_COUNT + COIN_BACK_COUNT + COIN_SIDE_COUNT;

float coinRotateAngle = 0.0f;   // 코인 회전 각도

float aspect = 1.0f; // 종횡비

// [전역 변수]
GLuint trainSideTexID;
GLuint trainFrontTexID;
GLuint trainTopTexID;

GLuint floorTexID;   // 선로 바닥(tile.bmp)용 텍스처
GLuint coinTexID;    // 코인(coin_f.bmp) 텍스처
GLuint grassTexID; // 잔디 텍스처 ID

//=======================================
//건물 관련 변수
GLuint buildingTexIDs[3]; // 건물 텍스처 ID 3개를 담을 배열
// 건물 데이터 배열
#define MAX_SEGMENTS 1000
float leftBuildingHeight[MAX_SEGMENTS];
float rightBuildingHeight[MAX_SEGMENTS];
int leftBuildingColor[MAX_SEGMENTS];
int rightBuildingColor[MAX_SEGMENTS];
int leftBuildingTexIndex[MAX_SEGMENTS];  // 왼쪽 건물 텍스처 번호
int rightBuildingTexIndex[MAX_SEGMENTS]; // 오른쪽 건물 텍스처 번호
//===============================================================

// +++ 플레이어 위치 변수 추가 +++
float playerX = 0.0f;
float playerZ = -3.0f; // 초기 Z 위치 (기존 값)
float moveSpeed = 0.2f; // 한 번에 이동할 거리
//터널 z방향 이동량 (트레드밀 오프셋)
float tunnelOffsetZ = 0.0f;

//자동 이동되는 변수 추가
bool isAutoMove = false;
float autoMoveSpeedPerFrame = 0.05f;

// [로봇 애니메이션용 전역 변수 추가]
float limbAngle = 0.0f;   // 팔다리 각도
float limbDir = 1.0f;     // 팔다리 움직임 방향

//점프 관련 변수
bool isJumping = false;
float jumpY = 0.0f;
float jumpVelocity = 0.0f;
const float GRAVITY = 0.0009f;    // 중력을 절반 정도로 낮춤 (천천히 떨어짐)
const float JUMP_POWER = 0.045f;

// 전역 변수 선언 부분에 추가
bool isGameClear = false;       // 게임이 끝났는지 확인하는 변수
//float finishDistance = 400.0f;  // 목표 지점 (터널 길이와 맞춰주세요)
float finishDistance = 300;


//좌우 부드러운 이동을 위한 변수
bool isLeftDown = false;  // 왼쪽 키가 눌려있는가?
bool isRightDown = false; // 오른쪽 키가 눌려있는가?
float playerMoveSpeed = 0.05f; // 좌우 이동 속도 (이 값을 키우면 더 빠르게 움직임)
const float PLAYER_LIMIT = 1.8f;

int currentLane = 0;        // 현재 레인 번호 (-1: 왼쪽, 0: 가운데, 1: 오른쪽)
float targetPlayerX = 0.0f; // 플레이어가 이동해야 할 목표 X 좌표
const float LANE_WIDTH = 0.85f; // 레인 간격 (선이 0.25니까 0.5 간격이면 딱 맞습니다)
float laneSwitchSpeed = 0.04f;


//캐릭터 관련 변수
float snotAngle = 0.0f;     // 기본 회전 각도
float snotLag = 0.0f;       // 스프링 지연
float snotRadiusX = 0.35f;  // 좌우로 뻗는 길이 (뒤에서도 보이게 크게)
float snotRadiusZ = 0.25f;  // 앞뒤로 흔들림
float snotLength = 0.45f;  // 콧물 실제 길이


//---------------------------------------------------
//기차 관련 구조체 및 변수
struct Train {
    int lane;
    float zPos;
    int colorType;
    bool isMoving;
    float speed;
    int type;
};

// ======== 코인 추가 ========
struct Coin {
    int lane;       // -1, 0, 1 레인
    float zPos;     // 기차와 같은 방식으로 z 위치
    bool collected; // 먹었는지 여부
};

std::vector<Coin> coins; // 코인 목록
int coinCount = 0;       // 먹은 코인 개수

int gWidth = 800, gHeight = 800; // 현재 창 크기 (텍스트용)

// ------------ 자석(Magnet) 아이템 ------------

struct Magnet {
    int lane;       // -1, 0, 1
    float zPos;     // 트랙상의 위치
    bool collected; // 먹었는지 여부
};

std::vector<Magnet> magnets;

// 자석 활성화 상태
bool  isMagnetActive = false;
float magnetTimer = 0.0f;   // 남은 시간(초)
const float MAGNET_DURATION = 5.0f;  // 5초 동안 유지

int   prevTime = 0;                 // 이전 프레임 시간(ms)
float deltaTime = 0.0f;             // 지난 프레임에서 지난 시간(초)



//---------------------------------------------------

std::vector<Train> trains; // 기차들을 관리할 리스트
//----------------------------------------------------------
bool isGameStarted = false; //게임 시작 여부
//----------------------------------------
//슬라이딩 관련 변수
bool isSliding = false;
//int slideTimer = 0;//지속시간
const int SLIDE_DURATION = 45; //슬라이딩 유지 시간

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

// 전역 변수 부분의 색상 배열 수정
GLfloat cubeFaceColors[6][3] = {
   {0.5f, 0.5f, 0.5f},        // 0: 회색 (전봇대/기둥 색으로 사용!)
   {0.0f, 0.0f, 0.0f},        // 1: 검정
   {0.0f, 0.0f, 1.0f},        // 2: 파랑 (바닥)
   {0.5f, 0.0f, 1.0f},        // 3: 보라 (천장)
   {0.2f, 0.4f, 1.0f},        // 4: 벽 1
   {0.2f, 0.4f, 1.0f},        // 5: 벽 2
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


//==========================================================

// [1] BMP 파일을 직접 파싱하는 함수 (외부 라이브러리 X)
unsigned char* loadBMP_Manual(const char* imagepath, unsigned int* width, unsigned int* height) {
    FILE* file;
    fopen_s(&file, imagepath, "rb"); // Windows 안전 버전 fopen
    if (!file) {
        printf("이미지 파일을 열 수 없습니다: %s\n", imagepath);
        return NULL;
    }

    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54) { // 헤더 54바이트 읽기
        printf("BMP 헤더 오류\n");
        fclose(file);
        return NULL;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        printf("BMP 파일이 아닙니다 (BM 매직넘버 없음)\n");
        fclose(file);
        return NULL;
    }

    // 헤더에서 정보 추출
    unsigned int dataPos = *(int*)&(header[0x0A]);
    unsigned int imageSize = *(int*)&(header[0x22]);
    *width = *(int*)&(header[0x12]);
    *height = *(int*)&(header[0x16]);

    // BMP 정보 보정
    if (imageSize == 0)    imageSize = (*width) * (*height) * 3;
    if (dataPos == 0)      dataPos = 54;

    // 실제 데이터 위치로 이동
    unsigned char* data = new unsigned char[imageSize];

    // 데이터 위치로 점프해서 읽기 (이게 안 되면 지지직거림)
    fseek(file, dataPos, SEEK_SET);
    fread(data, 1, imageSize, file);
    fclose(file);

    return data;
}

// [2] 텍스처 생성 함수
void makeTexture(const char* filename, GLuint* targetID) {
    unsigned int width, height;
    unsigned char* data = loadBMP_Manual(filename, &width, &height);

    if (data != NULL) {
        glGenTextures(1, targetID);
        glBindTexture(GL_TEXTURE_2D, *targetID);

        // ★ 텍스처가 깨지는 것을 방지하는 핵심 설정
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // 텍스처 반복 및 필터 설정
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // ★ BMP는 보통 BGR 순서로 되어 있습니다. GL_BGR 사용!
        // 만약 색이 파랗게 나오면 GL_RGB로 바꾸세요.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        delete[] data; // 메모리 해제
        std::cout << "텍스처 로드 성공: " << filename << " (" << width << "x" << height << ")" << std::endl;
    }
    else {
        std::cout << "텍스처 로드 실패 (경로 확인 필수): " << filename << std::endl;
    }
}
//================================================================

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
        GLfloat data[4 * 8];

        float maxU = 1.0f;

        // [기본 텍스처 좌표] (0:좌하, 1:우하, 2:우상, 3:좌상)
        // 기본적으로 이미지를 꽉 채워서 보여줍니다.
        GLfloat cubeTexCoords[4][2] = {
            {0.0f, 0.0f},
            {maxU, 0.0f},
            {maxU, 1.0f},
            {0.0f, 1.0f}
        };


        // ========================================================
        // ★ 1. 옆면 (4, 5번): 텍스처를 90도 회전 (기차 길이에 맞춤)
        // ========================================================
        if (f == 4 || f == 5) {
            cubeTexCoords[0][0] = 0.0f; cubeTexCoords[0][1] = 0.0f;
            cubeTexCoords[1][0] = 0.0f; cubeTexCoords[1][1] = 1.0f;
            cubeTexCoords[2][0] = maxU; cubeTexCoords[2][1] = 1.0f;
            cubeTexCoords[3][0] = maxU; cubeTexCoords[3][1] = 0.0f;
        }

        // ========================================================
        // ★ 2. 앞면(0)과 뒷면(1): 수정됨!
        // ========================================================
        // 보내주신 사진(subway2.bmp)이 이미 기차 비율(5:8)에 딱 맞으므로
        // 아까 넣었던 cropX(잘라내기) 코드를 싹 지우고 기본 좌표를 씁니다.
        // 이렇게 해야 사진 전체가 기차 앞면에 딱 맞게 들어갑니다.

        // (여기에 아무 코드도 안 넣으면 위에서 설정한 기본 좌표가 적용됩니다)


        for (int i = 0; i < 4; i++) {
            int vi = cubeFacesIndices[f][i];

            // 위치
            for (int j = 0; j < 3; j++) data[i * 8 + j] = cubeVertices[vi][j];
            // 색상
            for (int j = 0; j < 3; j++) data[i * 8 + 3 + j] = cubeFaceColors[f][j];
            // 텍스처
            data[i * 8 + 6] = cubeTexCoords[i][0];
            data[i * 8 + 7] = cubeTexCoords[i][1];
        }

        GLuint vbo;
        glGenVertexArrays(1, &vaoCube[f]);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vaoCube[f]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    // ==========================================
    // 3. 바닥 라인 (기존 코드 유지)
    // ==========================================
    float lineLength = 20.0f;
    GLfloat lineColor[3] = { 1.0f, 1.0f, 1.0f };

    GLfloat lineVertices[] = {
        -0.01f, -0.49f,  0.0f,        lineColor[0], lineColor[1], lineColor[2],  0.0f, 0.0f,
         0.01f, -0.49f,  0.0f,        lineColor[0], lineColor[1], lineColor[2],  0.0f, 0.0f,
        -0.01f, -0.49f, -lineLength,  lineColor[0], lineColor[1], lineColor[2],  0.0f, 0.0f,
         0.01f, -0.49f, -lineLength,  lineColor[0], lineColor[1], lineColor[2],  0.0f, 0.0f,
    };

    GLuint vboLaneLine;
    glGenVertexArrays(1, &vaoLaneLine);
    glGenBuffers(1, &vboLaneLine);

    glBindVertexArray(vaoLaneLine);
    glBindBuffer(GL_ARRAY_BUFFER, vboLaneLine);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}
// ------------------------------------------------------
void setupCoinVAO()
{
    // [x, y, z, r, g, b, u, v] * (COIN_VERT_COUNT)
    GLfloat data[COIN_VERT_COUNT * 8];
    int idx = 0;

    // 기본 두께: 앞면 z = +0.5, 뒷면 z = -0.5  (나중에 coinThickness로 스케일)
    const float frontZ = 0.5f;
    const float backZ = -0.5f;

    // 텍스처 파라미터 (너가 맞춰둔 값 그대로)
    const float texRadius = 0.38f;
    const float texCenterU = 0.5f;
    const float texCenterV = 0.52f;

    // =======================
    // 1) 앞면 원판 (텍스처 사용)
    // =======================

    // 중심
    data[idx++] = 0.0f;          // x
    data[idx++] = 0.0f;          // y
    data[idx++] = frontZ;        // z
    data[idx++] = 1.0f;          // r
    data[idx++] = 1.0f;          // g
    data[idx++] = 1.0f;          // b
    data[idx++] = texCenterU;    // u
    data[idx++] = texCenterV;    // v

    // 둘레
    for (int i = 0; i <= COIN_SEGMENTS; i++) {
        float theta = (2.0f * 3.141592f * i) / COIN_SEGMENTS;
        float x = cosf(theta);
        float y = sinf(theta);

        data[idx++] = x;                // x
        data[idx++] = y;                // y
        data[idx++] = frontZ;           // z
        data[idx++] = 1.0f;             // r
        data[idx++] = 1.0f;             // g
        data[idx++] = 1.0f;             // b

        float u = texCenterU + texRadius * x;
        float v = texCenterV + texRadius * y;
        data[idx++] = u;                // u
        data[idx++] = v;                // v
    }
    // 여기까지가 COIN_FRONT_COUNT 개

  // =======================
// 2) 뒷면 원판 (앞면과 똑같이 텍스처 사용)
// =======================
    glm::vec3 backColor(1.0f, 1.0f, 1.0f);   // 색은 사실 안 써도 됨

    // 중심
    data[idx++] = 0.0f;
    data[idx++] = 0.0f;
    data[idx++] = backZ;
    data[idx++] = 1.0f;          // r
    data[idx++] = 1.0f;          // g
    data[idx++] = 1.0f;          // b
    data[idx++] = texCenterU;    // u
    data[idx++] = texCenterV;    // v

    // 둘레 : 앞면과 같은 텍스처 매핑 + 정점 순서만 반대로(컬링용)
    for (int i = COIN_SEGMENTS; i >= 0; --i) {
        float theta = (2.0f * 3.141592f * i) / COIN_SEGMENTS;
        float x = cosf(theta);
        float y = sinf(theta);

        data[idx++] = x;
        data[idx++] = y;
        data[idx++] = backZ;
        data[idx++] = 1.0f;
        data[idx++] = 1.0f;
        data[idx++] = 1.0f;

        float u = texCenterU + texRadius * x;
        float v = texCenterV + texRadius * y;
        data[idx++] = u;
        data[idx++] = v;
    }
    // 여기까지가 COIN_BACK_COUNT 개 추가


    // =======================
    // 3) 옆면(원통 측면) – 삼각형 2개씩
    // =======================
    glm::vec3 sideColor(1.0f, 0.85f, 0.0f);

    for (int i = 0; i < COIN_SEGMENTS; i++) {
        float theta0 = (2.0f * 3.141592f * i) / COIN_SEGMENTS;
        float theta1 = (2.0f * 3.141592f * (i + 1)) / COIN_SEGMENTS;

        float x0 = cosf(theta0);
        float y0 = sinf(theta0);
        float x1 = cosf(theta1);
        float y1 = sinf(theta1);

        // 네 꼭짓점
        // v0: 앞면 테두리
        // v1: 앞면 다음 테두리
        // v2: 뒷면 다음 테두리
        // v3: 뒷면 테두리

        // 삼각형 1: v0, v1, v2
        data[idx++] = x0; data[idx++] = y0; data[idx++] = frontZ;
        data[idx++] = sideColor.r; data[idx++] = sideColor.g; data[idx++] = sideColor.b;
        data[idx++] = 0.0f; data[idx++] = 0.0f;

        data[idx++] = x1; data[idx++] = y1; data[idx++] = frontZ;
        data[idx++] = sideColor.r; data[idx++] = sideColor.g; data[idx++] = sideColor.b;
        data[idx++] = 0.0f; data[idx++] = 0.0f;

        data[idx++] = x1; data[idx++] = y1; data[idx++] = backZ;
        data[idx++] = sideColor.r; data[idx++] = sideColor.g; data[idx++] = sideColor.b;
        data[idx++] = 0.0f; data[idx++] = 0.0f;

        // 삼각형 2: v0, v2, v3
        data[idx++] = x0; data[idx++] = y0; data[idx++] = frontZ;
        data[idx++] = sideColor.r; data[idx++] = sideColor.g; data[idx++] = sideColor.b;
        data[idx++] = 0.0f; data[idx++] = 0.0f;

        data[idx++] = x1; data[idx++] = y1; data[idx++] = backZ;
        data[idx++] = sideColor.r; data[idx++] = sideColor.g; data[idx++] = sideColor.b;
        data[idx++] = 0.0f; data[idx++] = 0.0f;

        data[idx++] = x0; data[idx++] = y0; data[idx++] = backZ;
        data[idx++] = sideColor.r; data[idx++] = sideColor.g; data[idx++] = sideColor.b;
        data[idx++] = 0.0f; data[idx++] = 0.0f;
    }
    // 여기까지가 COIN_SIDE_COUNT 개 추가

    // ========= VAO / VBO 세팅 =========
    GLuint vbo;
    glGenVertexArrays(1, &vaoCoin);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vaoCoin);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    // 위치 (x,y,z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 색 (r,g,b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 텍스처 좌표 (u,v)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
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

// ------------------------------------------------------
//   원판 코인 그리기 헬퍼
// ------------------------------------------------------
void drawCoinMesh(glm::mat4 modelMatrix, glm::vec3 color) {
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");
    GLint locObjectColor = glGetUniformLocation(shaderProgramID, "uObjectColor");
    GLint locUseObjectColor = glGetUniformLocation(shaderProgramID, "uUseObjectColor");
    GLint locUseTexture = glGetUniformLocation(shaderProgramID, "uUseTexture");

    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glBindVertexArray(vaoCoin);

    // 1) 앞면 : 텍스처 사용
    glUniform1i(locUseTexture, 1);
    glUniform1i(locUseObjectColor, 0); // 텍스처만 사용
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, coinTexID);
    glDrawArrays(GL_TRIANGLE_FAN, 0, COIN_FRONT_COUNT);

    // 2) 뒷면 : 텍스처 그대로 (정점에 u,v 넣었으니까)
    glDrawArrays(GL_TRIANGLE_FAN, COIN_FRONT_COUNT, COIN_BACK_COUNT);

    // 3) 옆면(두께) : 단색 노랑
    glUniform1i(locUseTexture, 0);
    glUniform1i(locUseObjectColor, 1);
    glUniform3f(locObjectColor, color.r, color.g, color.b);
    glDrawArrays(GL_TRIANGLES,
        COIN_FRONT_COUNT + COIN_BACK_COUNT, COIN_SIDE_COUNT);

    glBindVertexArray(0);
    glUniform1i(locUseObjectColor, 0); // 상태 원복
}

// ------------------------------------------------------
// ------------------------------------------------------
// U자 + 곡선 자석 메쉬 그리기
// baseMatrix : 자석 전체 위치/크기
// ------------------------------------------------------
void drawMagnetMesh(glm::mat4 baseMatrix)
{
    glm::vec3 red(1.0f, 0.0f, 0.0f); // 자석 본체 색
    glm::vec3 yellow(1.0f, 0.7f, 0.0f); // 자석 노란팁 색

    // 파라미터 (자석 기본 모양)
    const float ARM_HEIGHT = 0.8f;   // 세로 기둥 높이
    const float ARM_WIDTH = 0.25f;  // 세로 기둥 두께
    const float ARM_DEPTH = 0.25f;
    const float RADIUS = 0.55f;  // U 곡선 반지름
    const float ARC_THICKNESS = 0.22f;  // 곡선 두께
    const float TIP_SIZE = 0.30f;  // 노란 팁 크기

    // 1) 왼쪽 세로 기둥
    {
        glm::mat4 m = baseMatrix;
        m = glm::translate(m, glm::vec3(-RADIUS, 0.0f + ARM_HEIGHT * 0.5f, 0.0f));
        m = glm::scale(m, glm::vec3(ARM_WIDTH, ARM_HEIGHT, ARM_DEPTH));
        drawColoredCube(m, red);
    }

    // 2) 오른쪽 세로 기둥
    {
        glm::mat4 m = baseMatrix;
        m = glm::translate(m, glm::vec3(RADIUS, 0.0f + ARM_HEIGHT * 0.5f, 0.0f));
        m = glm::scale(m, glm::vec3(ARM_WIDTH, ARM_HEIGHT, ARM_DEPTH));
        drawColoredCube(m, red);
    }

    // 3) 아래 곡선 부분 (여러 개의 작은 큐브로 반원 그리기)
    {
        const int SEG = 18;   // 세그먼트 많을수록 더 부드러움

        for (int i = 0; i <= SEG; i++) {
            // 0 ~ π : 왼쪽 기둥 밑 → 오른쪽 기둥 밑으로 가는 반원
            float t = glm::pi<float>() * i / SEG;

            // U 자 아래쪽 곡선 (웃는 입 모양)
            float x = cosf(t) * RADIUS;       // -RADIUS ~ +RADIUS
            float y = -sinf(t) * RADIUS;      // 끝점 y = 0, 가운데 y = -RADIUS

            glm::mat4 m = baseMatrix;
            m = glm::translate(m, glm::vec3(x, y, 0.0f));
            m = glm::scale(m, glm::vec3(ARC_THICKNESS, ARC_THICKNESS, ARM_DEPTH));
            drawColoredCube(m, red);
        }
    }


    // 4) 위쪽 노란 팁 (양쪽 끝만)
    {
        // 왼쪽 팁
        glm::mat4 m = baseMatrix;
        m = glm::translate(m, glm::vec3(-RADIUS, ARM_HEIGHT + TIP_SIZE * 0.3f, 0.0f));
        m = glm::scale(m, glm::vec3(TIP_SIZE, TIP_SIZE, TIP_SIZE));
        drawColoredCube(m, yellow);

        // 오른쪽 팁
        m = baseMatrix;
        m = glm::translate(m, glm::vec3(RADIUS, ARM_HEIGHT + TIP_SIZE * 0.3f, 0.0f));
        m = glm::scale(m, glm::vec3(TIP_SIZE, TIP_SIZE, TIP_SIZE));
        drawColoredCube(m, yellow);
    }
}





//=========================================================================
void initTrains() {
    trains.clear();

    float startZ = -30.0f;
    float gapZ = 25.0f;
    int numberOfSets = 100;

    int lastStationaryIdx[3] = { -999, -999, -999 };

    for (int i = 0; i < numberOfSets; i++) {
        float currentZPos = startZ - (i * gapZ);
        int trainCount = (rand() % 2) + 1;

        std::vector<int> lanes = { -1, 0, 1 };
        std::random_shuffle(lanes.begin(), lanes.end());

        // 달려오는 기차 줄 확률: 40%
        bool isMovingRow = (rand() % 100 < 40);

        for (int k = 0; k < trainCount; k++) {
            Train t;
            int currentLane = lanes[k];
            int laneArrayIdx = currentLane + 1;

            t.lane = (float)currentLane;
            t.zPos = currentZPos;
            t.colorType = rand() % 5;

            // ======================================================
            // [A] 달려오는 기차 시도 (40%)
            // ======================================================
            if (isMovingRow) {
                // ★ [수정] 안전거리를 12칸 -> 6칸으로 대폭 완화!
                // 이제 앞에 멈춘 기차가 있어도 좀 더 과감하게 생성됩니다.
                if (i - lastStationaryIdx[laneArrayIdx] < 6) {
                    // [대체] 너무 가까우면 '멈춘 장애물'로 변경
                    t.isMoving = false;
                    t.speed = 0.0f;

                    if (rand() % 3 == 0) t.type = 1; // 울타리
                    else t.type = 0; // 멈춘 기차

                    lastStationaryIdx[laneArrayIdx] = i;
                }
                else {
                    // 안전하면 '달려오는 기차' 생성
                    t.type = 0;
                    t.isMoving = true;
                    t.speed = 0.5f;
                }
            }
            // ======================================================
            // [B] 멈춰있는 장애물 줄 (60%)
            // ======================================================
            else {
                t.isMoving = false;
                t.speed = 0.0f;

                if (rand() % 3 == 0) {
                    t.type = 1; // 울타리 (20%)
                }
                else {
                    t.type = 0; // 멈춘 기차 (40%)
                }

                lastStationaryIdx[laneArrayIdx] = i;
            }

            trains.push_back(t);
        }
    }
}
// ======== 여기부터 추가: 코인 초기화 함수 ========
void initCoins() {
    coins.clear();

    // 플레이어 기준 앞쪽에서 시작
    float startZ = -20.0f;
    float groupGap = 10.0f;    // 코인 그룹 간 Z 간격
    int   groups = 60;       // 얼마나 멀리까지 만들지

    for (int g = 0; g < groups; g++) {
        // 이 그룹에서 사용할 레인 하나 선택 (-1, 0, 1)
        int lane = (rand() % 3) - 1;

        // 한 그룹 안에 몇 개의 코인을 줄지 랜덤 (3~5개)
        int coinsInGroup = 3 + rand() % 3;

        for (int i = 0; i < coinsInGroup; i++) {
            Coin c;
            c.lane = lane;
            // 그룹마다 Z는 멀어지고, 그룹 안에서는 조금씩 앞쪽으로
            c.zPos = startZ - g * groupGap - i * 1.5f;
            c.collected = false;
            coins.push_back(c);
        }
    }
}

// ------------------------------------------------------
// 자석 아이템 초기화
// ------------------------------------------------------
void initMagnets() {
    magnets.clear();

    float startZ = -60.0f;   // 플레이어 기준 좀 더 앞쪽에서 시작
    float gapZ = 80.0f;    // 자석 간 간격 (자주 나오면 줄이고, 드물게면 늘려도 됨)
    int   count = 10;       // 전체 자석 개수

    for (int i = 0; i < count; i++) {
        Magnet m;
        m.lane = (rand() % 3) - 1;   // -1, 0, 1
        m.zPos = startZ - i * gapZ;
        m.collected = false;
        magnets.push_back(m);
    }
}

//===============================================
//건물 초기화

void initBuildings() {
    for (int i = 0; i < MAX_SEGMENTS; i++) {
        // 높이 랜덤
        leftBuildingHeight[i] = 1.0f + (rand() % 15) * 0.1f;
        rightBuildingHeight[i] = 1.0f + (rand() % 15) * 0.1f;

        // 색상 랜덤
        leftBuildingColor[i] = 2 + (rand() % 6);
        rightBuildingColor[i] = 2 + (rand() % 6);

        // 텍스처 번호 랜덤 설정 (이게 없으면 오른쪽 건물이 안 보임) ★
        leftBuildingTexIndex[i] = rand() % 3;
        rightBuildingTexIndex[i] = rand() % 3;
    }
}
//==============================================

//플레이어 발 밑의 높이 계산 함수
float getGroundHeight() {
    float trainHeight = 0.8f;   // 기차 높이
    float trainWidth = 0.3f;    // 기차 폭 (충돌 판정용)
    float trainLength = 15.0f;  // 기차 길이

    for (int i = 0; i < trains.size(); i++) {
        // 1. 기차의 현재 Z 위치 계산 (플레이어 쪽으로 다가오는 것 반영)
        float currentTrainZ = trains[i].zPos + tunnelOffsetZ;

        // 2. 기차의 X 위치 계산
        float trainX = trains[i].lane * LANE_WIDTH;

        // 3. 충돌 검사 (플레이어가 기차 영역 안에 있는가?)
        // 플레이어 Z는 -3.0f로 고정되어 있다고 가정 (전역변수 playerZ)

        // Z축 판정: 기차 앞뒤 길이 안에 플레이어가 있는가?
        bool inZ = (playerZ >= currentTrainZ - trainLength / 2.0f) &&
            (playerZ <= currentTrainZ + trainLength / 2.0f);

        // X축 판정: 플레이어가 기차 레인(좌우 폭) 안에 있는가?
        // 플레이어 큐브 크기가 있으니 약간 여유를 둡니다 (+0.1f)
        bool inX = (playerX >= trainX - trainWidth / 2.0f - 0.1f) &&
            (playerX <= trainX + trainWidth / 2.0f + 0.1f);

        // 플레이어가 기차 위에 있고, 플레이어 높이가 기차보다 높거나 같을 때 (뚫고 올라오지 않게)
        if (inZ && inX && jumpY >= trainHeight - 0.1f) {
            return trainHeight; // 여기는 바닥이 0.8층이다!
        }
    }

    return 0.0f;
}
//=============================================================


bool checkCollision() {
    float tHeight = 0.8f;      // 기차 높이
    float tLength = 15.0f;     // 기차 길이
    float collisionWidth = 0.4f; // 충돌 너비

    for (int i = 0; i < trains.size(); i++) {
        // 1. 공통 위치 계산
        float currentTrainZ = trains[i].zPos + tunnelOffsetZ;
        float trainX = trains[i].lane * LANE_WIDTH;

        // 2. [X축 검사] 레인이 다르면 충돌할 일이 없으니 바로 패스 (최적화)
        bool inLane = abs(playerX - trainX) < collisionWidth;
        if (!inLane) continue;


        // ---------------------------------------------------------
        // ★ 타입별 충돌 로직 분기 (기차 vs 장애물) ★
        // ---------------------------------------------------------

        if (trains[i].type == 0) {
            // [TYPE 0: 기차] - 기존 로직 유지

            float trainFront = currentTrainZ + (tLength / 2.0f);
            float trainBack = currentTrainZ - (tLength / 2.0f);

            // 진동 방지를 위해 앞쪽 판정을 넉넉하게 잡았던(+0.8f) 기존 코드 유지
            bool inZ = (playerZ < trainFront + 0.8f) && (playerZ > trainBack + 0.2f);

            // 점프해서 기차 위에 올라가지 못한 경우(낮을 때)만 충돌
            bool isLow = jumpY < (tHeight - 0.1f);

            if (inZ && isLow) {
                return true; // 쾅!
            }
        }
        else {
            // [TYPE 1: 장애물(울타리)] - 새로 추가된 로직

            // 장애물은 기차처럼 길지 않고 얇음 (앞뒤 폭 0.4f 정도 여유)
            // 울타리 위치(currentTrainZ)를 기준으로 플레이어가 지나가는 중인가?
            bool hitBarrierZ = (playerZ < currentTrainZ + 0.4f) && (playerZ > currentTrainZ - 0.4f);

            if (hitBarrierZ) {
                // 1) 슬라이딩 중이라면? -> 무사 통과 (충돌 아님)
                if (isSliding) continue;

                // 2) 점프를 아주 높게 뛰었다면? (울타리 높이 0.6 이상) -> 무사 통과
                if (jumpY > 0.6f) continue;

                // 슬라이딩도 아니고, 점프도 낮으면 -> 충돌!
                return true;
            }
        }
    }
    return false; // 충돌 없음
}



// ======== 여기부터 추가: 코인 업데이트 & 먹기 ========
void updateCoins() {
    float coinWidth = 0.25f;   // 기본 X 충돌 폭
    float coinLength = 0.5f;    // 기본 Z 충돌 폭

    // 기차와 똑같이 트랙 길이를 돌려주기 위해 사용
    float loopDistance = 50.0f * 40.0f; // (train에서 numberOfSets * gapZ)

    // 자석이 활성화되면 약간만 범위를 늘려준다.
    // (옆 레인까지는 살짝, 너무 멀리 있는 건 먹지 않게)
    float extraX = 0.0f;
    float extraZ = 0.0f;
    if (isMagnetActive) { // 자석 범위
        extraX = 1.5f;   // 좌/우
        extraZ = 1.5f;   // 앞/뒤
    }

    for (int i = 0; i < coins.size(); i++) {
        float currentZ = coins[i].zPos + tunnelOffsetZ;

        // 화면 뒤로 넘어간 코인은 트랙 뒤로 보내면서 "새 코인"으로 재사용
        if (currentZ > 5.0f) {
            coins[i].zPos -= loopDistance;
            coins[i].collected = false;
            continue;
        }

        // 이미 먹은 코인은 더 이상 체크하지 않는다.
        if (coins[i].collected)
            continue;

        float coinX = coins[i].lane * LANE_WIDTH;

        bool inX = fabs(playerX - coinX) < (coinWidth + extraX);
        bool inZ = fabs(playerZ - currentZ) < (coinLength + extraZ);

        if (inX && inZ) {
            // 여기서 단 한 번만 collected -> true 로 바꾸고,
            // coinCount를 +1 해준다.
            coins[i].collected = true;
            coinCount++;
        }
    }
}
// ------------------------------------------------------
// 자석 위치 업데이트 + 플레이어가 먹었는지 체크
// ------------------------------------------------------
void updateMagnets() {
    float collisionWidth = 0.4f;  // X 충돌 허용
    float collisionLength = 0.7f;  // Z 충돌 허용
    float loopDistance = 50.0f * 40.0f; // 트랙 길이 (기차랑 동일)

    for (int i = 0; i < magnets.size(); i++) {
        float currentZ = magnets[i].zPos + tunnelOffsetZ;

        // 화면 뒤로 나가면 뒤로 보내기
        if (currentZ > 5.0f) {
            magnets[i].zPos -= loopDistance;
            magnets[i].collected = false;
            continue;
        }

        if (magnets[i].collected) continue;

        float mX = magnets[i].lane * LANE_WIDTH;

        bool inX = fabs(playerX - mX) < collisionWidth;
        bool inZ = fabs(playerZ - currentZ) < collisionLength;

        if (inX && inZ) {
            // 자석 먹음!
            magnets[i].collected = true;
            isMagnetActive = true;
            magnetTimer = MAGNET_DURATION;
        }
    }
}

void drawCubeFace(int f) {
    glBindVertexArray(vaoCube[f]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

//================================================================
//기차 그리기 함수
void drawTrains() {
    float tWidth = 0.5f;
    float tHeight = 0.8f;
    float tLength = 15.0f;
    float loopDistance = 100 * 25.0f;

    GLint locUseTexture = glGetUniformLocation(shaderProgramID, "uUseTexture");
    GLint locObjectColor = glGetUniformLocation(shaderProgramID, "uObjectColor");
    GLint locUseObjectColor = glGetUniformLocation(shaderProgramID, "uUseObjectColor");
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");



    float gateZ = tunnelOffsetZ - finishDistance;

    for (int i = 0; i < trains.size(); i++) {
        float currentZ = trains[i].zPos + tunnelOffsetZ;

        if (currentZ < gateZ) continue;

        // ★ [수정] 기차가 화면 뒤로 지나가서 재활용(Respawn) 될 때
        // =========================================================
        if (currentZ > 5.0f) {
            trains[i].zPos -= loopDistance; // 뒤로 보냄
            trains[i].colorType = rand() % 5;

            // ★ [추가] 30% 확률로 "달려오는 기차"로 변신!
            // 장애물(type 1)은 움직이면 안 되므로 기차(type 0)일 때만 적용
            if (trains[i].type == 0 && (rand() % 10) < 3) {
                trains[i].isMoving = true;
                trains[i].speed = 0.3f; // 기차 속도 (조절 가능)
            }
            else {
                trains[i].isMoving = false;
                trains[i].speed = 0.0f;
            }

            continue;
        }


        float x = trains[i].lane * LANE_WIDTH;

        if (trains[i].type == 0) {
            // [기차 그리기]
            float y = -1.0f + (tHeight / 2.0f) + 0.05f;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(x, y, currentZ));
            model = glm::scale(model, glm::vec3(tWidth, tHeight, tLength));

            glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));

            // 텍스처 모드 켜기
            glUniform1i(locUseTexture, 1);
            glUniform1i(locUseObjectColor, 0);

            glActiveTexture(GL_TEXTURE0);

            // -------------------------------------------------
            // 1) 옆면 그리기 (왼쪽:4, 오른쪽:5) -> trainSideTexID
            // -------------------------------------------------
            glBindTexture(GL_TEXTURE_2D, trainSideTexID);
            drawCubeFace(4);
            drawCubeFace(5);

            // -------------------------------------------------
            // 2) 앞/뒷면 그리기 (앞:0, 뒤:1) -> trainFrontTexID
            // -------------------------------------------------
            glBindTexture(GL_TEXTURE_2D, trainFrontTexID);
            drawCubeFace(0);
            drawCubeFace(1);

            // -------------------------------------------------
            // 3) ★★★ 윗면 그리기 (천장:3) -> trainTopTexID ★★★
            // 이 부분이 있어야 윗면에 지붕 사진이 나옵니다!
            // -------------------------------------------------
            glBindTexture(GL_TEXTURE_2D, trainTopTexID);
            drawCubeFace(3);

            // 4) 바닥면 (2) -> 바닥은 안 보이니까 옆면 텍스처로 대충 채움
            glBindTexture(GL_TEXTURE_2D, trainSideTexID);
            drawCubeFace(2);

            // 텍스처 끄기
            glUniform1i(locUseTexture, 0);
        }
        else {
            // [장애물 그리기]
            float barY = -1.0f + 0.7f;
            glm::mat4 barModel = glm::mat4(1.0f);
            barModel = glm::translate(barModel, glm::vec3(x, barY, currentZ));
            barModel = glm::scale(barModel, glm::vec3(0.7f, 0.2f, 0.1f));
            drawColoredCube(barModel, glm::vec3(1.0f, 0.8f, 0.0f));

            glm::mat4 lLeg = glm::mat4(1.0f);
            lLeg = glm::translate(lLeg, glm::vec3(x - 0.3f, -1.0f + 0.35f, currentZ));
            lLeg = glm::scale(lLeg, glm::vec3(0.1f, 0.7f, 0.1f));
            drawColoredCube(lLeg, glm::vec3(0.3f, 0.3f, 0.3f));

            glm::mat4 rLeg = glm::mat4(1.0f);
            rLeg = glm::translate(rLeg, glm::vec3(x + 0.3f, -1.0f + 0.35f, currentZ));
            rLeg = glm::scale(rLeg, glm::vec3(0.1f, 0.7f, 0.1f));
            drawColoredCube(rLeg, glm::vec3(0.3f, 0.3f, 0.3f));
        }
    }
}
//=============================================================================

// ======== 여기부터: 코인 그리기 (원판 + 회전) ==========
void drawCoins() {
    float coinRadius = 0.2f;   // 동그라미 반지름
    float coinThickness = 0.1f;   // 두께 (얇은 원판)
    float coinY = -1.0f + 0.5f;  // 바닥 위로 띄우는 높이

    float loopDistance = 100 * 25.0f;

    float gateZ = tunnelOffsetZ - finishDistance;

    for (int i = 0; i < coins.size(); i++) {
        float currentZ = coins[i].zPos + tunnelOffsetZ;

        if (currentZ < gateZ) continue;

        // 화면 뒤로 넘어가면 재배치
        if (currentZ > 5.0f) {
            coins[i].zPos -= loopDistance;
            coins[i].collected = false;
            continue;
        }

        if (coins[i].collected)
            continue;

        float x = coins[i].lane * LANE_WIDTH;

        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, glm::vec3(x, coinY, currentZ));

        // 코인이 세로로 서 있고, Y축 기준으로 빙글빙글
        m = glm::rotate(m, glm::radians(coinRotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));

        // XY 평면에 만든 원판을 실제 크기로 스케일
        m = glm::scale(m, glm::vec3(coinRadius, coinRadius, coinThickness));

        // 노란 코인
        drawCoinMesh(m, glm::vec3(1.0f, 1.0f, 0.0f));
    }
}
// ------------------------------------------------------
// 자석 그리기
// ------------------------------------------------------
void drawMagnets() {
    float magnetY = -1.0f + 0.5f;  // 코인과 비슷한 높이
    float baseScale = 0.4f;

    float loopDistance = 100 * 25.0f;

    float gateZ = tunnelOffsetZ - finishDistance;

    for (int i = 0; i < magnets.size(); i++) {
        float currentZ = magnets[i].zPos + tunnelOffsetZ;

        if (currentZ < gateZ) continue;
        if (currentZ > 5.0f) {
            magnets[i].zPos -= loopDistance;
            magnets[i].collected = false;
            continue;
        }

        if (magnets[i].collected) continue;

        float x = magnets[i].lane * LANE_WIDTH;

        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, glm::vec3(x, magnetY, currentZ));
        m = glm::scale(m, glm::vec3(baseScale));

        // 살짝 회전시킨 디자인
        //m = glm::rotate(m, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        drawMagnetMesh(m);
    }
}


// ------------------------------------------------------
// 레일 위 나무 침목(큐브) 그리기
// ------------------------------------------------------
void drawWoodPlanks()
{
    // 나무 색 (짙은 갈색)
    glm::vec3 plankColor(0.45f, 0.26f, 0.09f);

    // y 위치: 바닥이 대략 y = -1 근처니까 그 위로 살짝 띄워서
    float plankY = -1.0f + 0.03f;        // 바닥 바로 위
    float plankHeight = 0.06f;               // 두께(y)

    // x, z 스케일
    //  - x : 레인 안을 가로질러서 (침목이 레인 가로로 놓이는 느낌)
    //  - z : 진행 방향으로는 짧게
    float plankWidth = LANE_WIDTH * 0.6f;   // x 방향 길이 (레인 거의 꽉 차게)
    float plankDepth = 0.25f;               // z 방향 길이 (앞뒤 길이)

    // Z 방향 간격 (얼마나 띄엄띄엄 놓을지)
    float plankGapZ = 1.52f;                // 값 줄이면 촘촘, 키우면 듬성듬성

    // 어디서부터 어디까지 깔지 (플레이어 기준)
    float startZ = -1.0f;   // 플레이어보다 약간 앞부터
    float endZ = -300.0f; // 터널 끝 쪽까지

    float gateZ = tunnelOffsetZ - finishDistance;

    for (float localZ = startZ; localZ > endZ; localZ -= plankGapZ)
    {
        // tunnelOffsetZ 를 더해서 실제 화면상의 위치로
        float z = localZ + tunnelOffsetZ;

        if (z < gateZ) continue;

        // 너무 뒤/앞은 안 그리기 (성능 + 필요 없는 부분 제거)
        if (z > 5.0f)   continue;
        if (z < -250.0f) continue;

        // 3개 레인(-1, 0, 1)에 각각 침목 하나씩
        for (int lane = -1; lane <= 1; lane++)
        {
            float x = lane * LANE_WIDTH;

            glm::mat4 m = glm::mat4(1.0f);
            m = glm::translate(m, glm::vec3(x, plankY, z));
            m = glm::scale(m, glm::vec3(plankWidth, plankHeight, plankDepth));

            // 실제로는 얇게 납작한 “큐브”를 그리는 것
            drawColoredCube(m, plankColor);
        }
    }
}


// --- 추가 --- 레인 경계선 하나 그리기
void drawLaneLine() {
    glBindVertexArray(vaoLaneLine);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}



//=========================================================
//idle 함수

void idle() {
    bool needRedisplay = false;

    // 지난 프레임에서 얼마나 시간이 지났는지(초) 계산
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if (prevTime == 0) prevTime = currentTime;
    int elapsed = currentTime - prevTime;
    prevTime = currentTime;
    deltaTime = elapsed / 1000.0f;

    // =========================================================
    // ★ [수정 1] 게임이 아직 안 끝났을 때만 게임 로직 실행
    // =========================================================
    if (!isGameClear)
    {

        // ★ [추가] 달려오는 기차 이동 로직
        // =========================================================
        for (int i = 0; i < trains.size(); i++) {
            // 게임이 진행 중이고, 이 기차가 '움직이는 기차'라면?
            if (isGameStarted && trains[i].isMoving) {
                // 플레이어가 다가가는 속도(tunnelOffsetZ 증가)에 더해서
                // 기차도 플레이어 쪽으로 다가오게 함 (zPos 증가)
                // zPos는 음수에서 시작해서 0(플레이어) 쪽으로 옵니다.
                trains[i].zPos += trains[i].speed;
            }
        }
        // =========================================================
        // 
        // 
        // ★ 충돌 체크 로직 ★
        if (isGameStarted) {
            if (checkCollision()) {
                // 충돌 상태
                isAutoMove = false; // 멈춤
                tunnelOffsetZ -= 0.2f;
                autoMoveSpeedPerFrame = 0.02f;
                needRedisplay = true;
            }
            else {
                // 충돌 상태가 아니라면 다시 달리기
                isAutoMove = true;
            }
        }

        // 1. 자동 이동 로직
        if (isAutoMove) {
            tunnelOffsetZ += autoMoveSpeedPerFrame;

            // =========================================================
            // ★ [수정 2] 도착 지점 체크 로직 추가
            // =========================================================
            if (tunnelOffsetZ >= finishDistance) {
                tunnelOffsetZ = finishDistance; // 위치 고정 (더 이상 못 감)
                isGameClear = true;             // 게임 종료 플래그 켜기!
                isAutoMove = false;             // 달리기 멈춤
            }
            // =========================================================

            // 로봇 팔다리 애니메이션 (게임 중일 때만)
            if (!isGameClear) {
                limbAngle += 2.0f * limbDir;
                if (limbAngle > 45.0f || limbAngle < -45.0f) {
                    limbDir *= -1.0f;
                }
            }
            needRedisplay = true;
        }
        else {
            // 멈춰있을 때는 차렷
            if (abs(limbAngle) > 0.1f) {
                limbAngle *= 0.9f;
                needRedisplay = true;
            }
        }

        // 2. 점프 및 착지 물리 로직
        float currentGround = getGroundHeight();

        if (jumpY > currentGround || isJumping) {
            jumpY += jumpVelocity;
            jumpVelocity -= GRAVITY;

            if (jumpY <= currentGround) {
                jumpY = currentGround;
                isJumping = false;
                jumpVelocity = 0.0f;
            }
            else {
                isJumping = true;
            }
            needRedisplay = true;
        }
        else if (jumpY > currentGround) {
            isJumping = true;
        }

        // 3. 레인 변경 애니메이션 로직
        if (abs(playerX - targetPlayerX) > 0.001f) {
            if (playerX < targetPlayerX) {
                playerX += laneSwitchSpeed;
                if (playerX > targetPlayerX) playerX = targetPlayerX;
            }
            else {
                playerX -= laneSwitchSpeed;
                if (playerX < targetPlayerX) playerX = targetPlayerX;
            }
            needRedisplay = true;
        }

        // 4. 코인 및 자석 업데이트
        updateCoins();
        updateMagnets();

        // 자석 타이머 감소
        if (isMagnetActive) {
            magnetTimer -= deltaTime;
            if (magnetTimer <= 0.0f) {
                magnetTimer = 0.0f;
                isMagnetActive = false;
            }
        }
    } // if (!isGameClear) 끝


    // --- 코인 회전은 게임 끝나도 계속 돌면 예쁘니까 if 밖으로 뺌 (선택사항) ---
    coinRotateAngle += 1.0f;
    if (coinRotateAngle > 360.0f)
        coinRotateAngle -= 360.0f;

    // 게임이 끝났어도 화면 갱신은 해야 UI가 뜹니다.
    // (게임 중에는 needRedisplay가 true일 때만, 끝났을 땐 항상 갱신해서 UI 그리기)
    if (needRedisplay || isGameClear) {
        glutPostRedisplay();
    }
}

// ----------------- 일반 키보드 입력  ---------------------
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 32:
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
        if (!isGameStarted) {
            isGameStarted = true;
            isAutoMove = true;
        }
        break;
    case GLUT_KEY_DOWN: // 'DOWN' 화살표 키
        if (isGameStarted) {
            isSliding = true;
        }
        break;
    case GLUT_KEY_LEFT:
        // 왼쪽 끝(-1)이 아닐 때만 왼쪽으로 한 칸 이동 명령
        if (currentLane > -1) {
            currentLane--; // -1 감소
            targetPlayerX = currentLane * LANE_WIDTH; // 목표 위치 재설정 (-0.5 or 0.0)
        }
        break;

    case GLUT_KEY_RIGHT:
        // 오른쪽 끝(1)이 아닐 때만 오른쪽으로 한 칸 이동 명령
        if (currentLane < 1) {
            currentLane++; // +1 증가
            targetPlayerX = currentLane * LANE_WIDTH; // 목표 위치 재설정 (0.0 or 0.5)
        }
        break;

    }


    // 화면을 갱신하도록 요청
    glutPostRedisplay();
}


void specialKeyboardUp(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_DOWN:
        // 키를 떼면 슬라이딩 끝! (일어남)
        isSliding = false;
        break;
    }
}

void mouse(int button, int state, int x, int y)
{
    if (!gIsGameStarted && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (Title_IsInsideButton(x, y))
            gIsGameStarted = true;
    }
}


//짱구 ver
 //로봇 그리기 함수
//void drawRobot(float x, float y, float z) {
//    // 로봇의 기본 위치 설정
//    glm::mat4 model = glm::mat4(1.0f);
//    model = glm::translate(model, glm::vec3(x, y, z));
//
//    // ★ 슬라이딩 처리 ★
//    if (isSliding) {
//        // 1. 뒤로 눕기 (-90도 X축 회전)
//        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//        // 2. 누우면 중심축이 바뀌므로 위치를 살짝 아래/뒤로 조정
//        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.3f));
//    }
//    else {
//
//        // 로봇이 뒤(Z-)를 보고 있으므로 180도 회전 (플레이어 시점)
//        // 필요에 따라 각도 조절: glm::radians(180.0f)
//        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    }
//    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
//
//    // 1. 몸통
//    glm::mat4 bodyM = glm::scale(model, glm::vec3(0.4f, 0.5f, 0.2f));
//    drawColoredCube(bodyM, glm::vec3(1.0f, 0.1f, 0.1f)
//); // 파랑
//
//    // 2. 머리
//    glm::mat4 headM = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.0f));
//    glm::mat4 headScaleM = glm::scale(headM, glm::vec3(0.3f, 0.3f, 0.3f));
//    drawColoredCube(headScaleM, glm::vec3(0.0f, 0.0f, 0.0f)); // 살색
//
//    // 3. 코 (빨강)
//    glm::mat4 noseM = glm::translate(headM, glm::vec3(0.0f, 0.0f, 0.18f)); // 약간 앞으로 뺌
//    noseM = glm::scale(noseM, glm::vec3(0.05f, 0.05f, 0.05f));
//    drawColoredCube(noseM, glm::vec3(1.0f, 0.0f, 0.0f));
//
//    // 팔다리 움직임 각도
//    float swing = glm::radians(limbAngle);
//
//    // 4. 왼팔 (노랑)
//    glm::mat4 lArmM = glm::translate(model, glm::vec3(-0.3f, 0.15f, 0.0f));
//    lArmM = glm::rotate(lArmM, swing, glm::vec3(1.0f, 0.0f, 0.0f)); // 회전
//    lArmM = glm::translate(lArmM, glm::vec3(0.0f, -0.2f, 0.0f)); // 회전축 아래로 내리기
//    drawColoredCube(glm::scale(lArmM, glm::vec3(0.1f, 0.4f, 0.1f)), glm::vec3(1.0f, 0.1f, 0.1f)
//    );
//
//    // 5. 오른팔 
//    glm::mat4 rArmM = glm::translate(model, glm::vec3(0.3f, 0.15f, 0.0f));
//    rArmM = glm::rotate(rArmM, -swing, glm::vec3(1.0f, 0.0f, 0.0f));
//    rArmM = glm::translate(rArmM, glm::vec3(0.0f, -0.2f, 0.0f));
//    drawColoredCube(glm::scale(rArmM, glm::vec3(0.1f, 0.4f, 0.1f)), glm::vec3(1.0f, 0.1f, 0.1f)
//    );
//
//    // 6. 왼다리 (회색 + 보라 살짝 섞인 색으로 바꾸셨네요)
//    glm::mat4 lLegM = glm::translate(model, glm::vec3(-0.12f, -0.25f, 0.0f)); // 1. 엉덩이 위치로 이동
//    lLegM = glm::rotate(lLegM, -swing, glm::vec3(1.0f, 0.0f, 0.0f));          // 2. 회전
//
//    lLegM = glm::translate(lLegM, glm::vec3(0.0f, -0.15f, 0.0f));
//    drawColoredCube(glm::scale(lLegM, glm::vec3(0.12f, 0.3f, 0.12f)), glm::vec3(1.0f, 1.0f, 0.0f)
//    );
//
//    // 7. 오른다리
//    glm::mat4 rLegM = glm::translate(model, glm::vec3(0.12f, -0.25f, 0.0f));
//    rLegM = glm::rotate(rLegM, swing, glm::vec3(1.0f, 0.0f, 0.0f));
//
//    rLegM = glm::translate(rLegM, glm::vec3(0.0f, -0.15f, 0.0f));
//    drawColoredCube(glm::scale(rLegM, glm::vec3(0.12f, 0.3f, 0.12f)), glm::vec3(1.0f, 1.0f, 0.0f)
//    );
//}

//
//// 로봇 그리기 함수
void drawRobot(float x, float y, float z) {
    // 로봇의 기본 위치 설정
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, z));

    // ★ 슬라이딩 처리 ★
    if (isSliding) {
        // 1. 뒤로 눕기 (-90도 X축 회전)
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // 2. 누우면 중심축이 바뀌므로 위치를 살짝 아래/뒤로 조정
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, -0.3f));
    }
    else {

        // 로봇이 뒤(Z-)를 보고 있으므로 180도 회전 (플레이어 시점)
        // 필요에 따라 각도 조절: glm::radians(180.0f)
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

    // 1. 몸통
    glm::mat4 bodyM = glm::scale(model, glm::vec3(0.42f, 0.62f, 0.22f));

    drawColoredCube(bodyM, glm::vec3(0.9f, 0.8f, 0.2f)
    );


    // 2. 머리 (맹구 버전: 길고 둥근 머리)
    glm::mat4 headM = glm::translate(model, glm::vec3(0.0f, 0.48f, 0.0f));

    glm::mat4 headScaleM = glm::scale(headM, glm::vec3(0.32f, 0.48f, 0.32f));
    // 세로로 더 김

    drawColoredCube(headScaleM, glm::vec3(1.0f, 0.8f, 0.6f)
    ); // 검정 머리

    // =========================
// 🎐 맹구 콧물 (머리를 중심으로 회전)
// =========================

// 1) 머리 중심을 기준으로 회전
    glm::mat4 snotM = headM;

    // 회전 (snotLag는 스프링 지연 포함된 회전값)
    snotM = glm::rotate(snotM, snotLag, glm::vec3(0, 1, 0));

    // 2) 머리 중심에서 멀리 떨어진 궤도로 이동 (원 궤적 만들기)
    float orbitRadius = 0.45f;  // 반지름 — 클수록 크게 돎
    snotM = glm::translate(
        snotM,
        glm::vec3(orbitRadius, -0.10f, 0.0f)
    );

    // 3) 코 바로 아래로 이동한 것처럼 보이게 하기 위한 보정
    snotM = glm::rotate(snotM, glm::radians(90.0f), glm::vec3(0, 0, 1));

    // 4) 콧물 스케일 (길게!)
    snotM = glm::scale(snotM, glm::vec3(0.06f, 0.35f, 0.06f));

    // 5) 색상
    drawColoredCube(snotM, glm::vec3(0.6f, 0.85f, 1.0f));



    // ★ 맹구 머리카락 (얇은 층)
    glm::mat4 hairM = glm::translate(headM, glm::vec3(0.0f, 0.25f, 0.0f));
    hairM = glm::scale(hairM, glm::vec3(0.35f, 0.055f, 0.35f));

    drawColoredCube(hairM, glm::vec3(0.1f, 0.05f, 0.02f)); // 짙은 갈색 또는 검정


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
    drawColoredCube(glm::scale(lArmM, glm::vec3(0.1f, 0.4f, 0.1f)), glm::vec3(0.9f, 0.8f, 0.2f)

    );

    // 5. 오른팔 (초록)
    glm::mat4 rArmM = glm::translate(model, glm::vec3(0.3f, 0.15f, 0.0f));
    rArmM = glm::rotate(rArmM, -swing, glm::vec3(1.0f, 0.0f, 0.0f));
    rArmM = glm::translate(rArmM, glm::vec3(0.0f, -0.2f, 0.0f));
    drawColoredCube(glm::scale(rArmM, glm::vec3(0.1f, 0.4f, 0.1f)), glm::vec3(0.9f, 0.8f, 0.2f)

    );

    // 6. 왼다리 (회색 + 보라 살짝 섞인 색으로 바꾸셨네요)
    glm::mat4 lLegM = glm::translate(model, glm::vec3(-0.12f, -0.25f, 0.0f)); // 1. 엉덩이 위치로 이동
    lLegM = glm::rotate(lLegM, -swing, glm::vec3(1.0f, 0.0f, 0.0f));          // 2. 회전

    lLegM = glm::translate(lLegM, glm::vec3(0.0f, -0.15f, 0.0f));
    drawColoredCube(glm::scale(lLegM, glm::vec3(0.12f, 0.3f, 0.12f)), glm::vec3(0.0f, 0.4f, 0.0f)

    );

    // 7. 오른다리
    glm::mat4 rLegM = glm::translate(model, glm::vec3(0.12f, -0.25f, 0.0f));
    rLegM = glm::rotate(rLegM, swing, glm::vec3(1.0f, 0.0f, 0.0f));

    rLegM = glm::translate(rLegM, glm::vec3(0.0f, -0.15f, 0.0f));
    drawColoredCube(glm::scale(rLegM, glm::vec3(0.12f, 0.3f, 0.12f)), glm::vec3(0.0f, 0.4f, 0.0f)

    );
}
// -----------------렌더링---------------------

// --- 추가 --- 코인 개수 UI 그리기 (좌측 상단)
void drawCoinUI() {
    // 셰이더 잠깐 끄기
    glUseProgram(0);
    glDisable(GL_DEPTH_TEST);

    // 2D 오쏘그래픽 투영 설정
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, gWidth, 0, gHeight, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    std::string text = "Coins: " + std::to_string(coinCount);

    glColor3f(0.0f, 0.0f, 0.0f); // 검정색
    glRasterPos2i(10, gHeight - 30); // 좌측 상단

    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // 상태 복구
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(shaderProgramID);
}


//==========================================
// 색상 인덱스를 받아 RGB 값을 설정하는 헬퍼 함수
void applyBuildingColor(int colorIndex, GLint locColor) {
    float r, g, b;
    switch (colorIndex) {
        // 1. 베이지 / 크림색 (가장 흔한 건물 색)
    case 2:
        r = 0.92f; g = 0.85f; b = 0.70f;
        break;

        // 2. 짙은 갈색 (벽돌 건물 느낌)
    case 3:
        r = 0.45f; g = 0.25f; b = 0.10f;
        break;

        // 3. 겨자색 / 황토색 (사진 왼쪽의 노란 건물)
    case 4:
        r = 0.90f; g = 0.70f; b = 0.20f;
        break;

        // 4. 붉은 벽돌색 (약간 물빠진 빨강)
    case 5:
        r = 0.75f; g = 0.35f; b = 0.25f;
        break;

        // 5. 회색 / 시멘트색 (도심 건물)
    case 6:
        r = 0.60f; g = 0.60f; b = 0.65f;
        break;

        // 6. 짙은 청회색 (유리 건물 느낌)
    case 7:
        r = 0.30f; g = 0.40f; b = 0.50f;
        break;

        // 기본값: 연한 회색
    default:
        r = 0.80f; g = 0.80f; b = 0.80f;
        break;
    }
    glUniform3f(locColor, r, g, b);
}
//===============================================

void Display() {


    if (!gIsGameStarted) {
        Title_Render();
        glutSwapBuffers();
        return;
    }
    glUseProgram(shaderProgramID);

    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ---------- Snot Physics Update (콧물 애니메이션 업데이트) ----------
   // ---- Snot Physics Update ----
    float targetAngle = snotAngle + 0.1f;
    snotLag += (targetAngle - snotLag) * 0.08f;
    snotAngle += 0.08f;          // 기본 회전





    // 유니폼 위치 가져오기
    GLint locIsBuilding = glGetUniformLocation(shaderProgramID, "uIsBuilding");
    GLint locTexScale = glGetUniformLocation(shaderProgramID, "uTexScale");

    // 기본값 초기화 (건물 아님, 스케일 1배)
    glUniform1i(locIsBuilding, 0);
    glUniform2f(locTexScale, 1.0f, 1.0f);

    // ==================================================
    // 1. 카메라(View) 및 투영(Projection) 설정
    // ==================================================
    Mat4 viewTranslate = translate(0.0f, -0.9f, -1.0f);
    Mat4 viewRotate = rotateX(20.0f * 3.14159f / 180.0f); // 20도 숙임
    Mat4 view = multifly(viewRotate, viewTranslate);
    Mat4 projection = perspective(45.0f * 3.14159f / 180.0f, aspect, 0.1f, 100.0f);

    GLint locView = glGetUniformLocation(shaderProgramID, "view");
    GLint locProj = glGetUniformLocation(shaderProgramID, "projection");
    glUniformMatrix4fv(locView, 1, GL_FALSE, view.m);
    glUniformMatrix4fv(locProj, 1, GL_FALSE, projection.m);

    // ==================================================
    // 2. 유니폼 위치 가져오기
    // ==================================================
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");
    GLint locUseTexture = glGetUniformLocation(shaderProgramID, "uUseTexture");
    GLint locUseObjectColor = glGetUniformLocation(shaderProgramID, "uUseObjectColor");
    GLint locObjectColor = glGetUniformLocation(shaderProgramID, "uObjectColor"); // RGB 색상용
    GLint locBrightness = glGetUniformLocation(shaderProgramID, "uBrightness");
    GLint locTextureSlot = glGetUniformLocation(shaderProgramID, "outTexture"); // 텍스처 슬롯

    // ==================================================
    // 3. 도로 및 건물 그리기 루프
    // ==================================================
    int tunnelSegments = 300;
    float tunnelScaleXY = 2.0f;
    int buildingSpacing = 2;        // 건물 간격 (2칸마다 배치)


    // ==================================================
    for (int i = 0; i < tunnelSegments; i++)
    {
        float zPos = -(float)i * 1.0f + tunnelOffsetZ;

        // ==================================================
        // 1.거대한 잔디 바닥 
        // ==================================================
        // 도로보다 먼저 그려서 아래에 깔리게 합니다.

        glUniform1i(locUseTexture, 1);       // 텍스처켜기
        glUniform1i(locUseObjectColor, 0);   // 색상 모드 켜기
        glUniform1i(locIsBuilding, 0);       // 건물 모드 끄기
        glUniform1f(locBrightness, 1.0f);    // 밝기 최대

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexID);
        glUniform2f(locTexScale, 2.0f, 1.0f);




        float grassWidth = 10.0f;
        Mat4 modelGrass = multifly(translate(0.0f, -0.01f, zPos), scale(grassWidth, tunnelScaleXY, 1.0f));
        glUniformMatrix4fv(locModel, 1, GL_FALSE, modelGrass.m);
        drawCubeFace(2); // 윗면 그리기


        // ==================================================
        // 2. [덮어 그리기] 도로 (기차 레일)
        // ==================================================
        // 잔디 위에 도로를 덮어씌웁니다.

        Mat4 modelTunnelScale = scale(tunnelScaleXY, tunnelScaleXY, 1.0f);
        Mat4 modelTunnelTranslate = translate(0.0f, 0.0f, zPos);
        Mat4 model = multifly(modelTunnelTranslate, modelTunnelScale);

        glUniformMatrix4fv(locModel, 1, GL_FALSE, model.m);

        // 바닥 밝기 설정 (체크무늬)
        float brightness = 1.0f;
        glUniform1f(locBrightness, brightness);

        // 바닥 텍스처 적용
        glUniform1i(locUseTexture, 1);      // 텍스처 ON
        glUniform1i(locUseObjectColor, 0);  // 색상 OFF
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexID);

        drawCubeFace(2); // 윗면(바닥) 그리기


        // ==================================================
        // 3. 건물 그리기 (기존 코드 유지)
        // ==================================================
        if (i % buildingSpacing == 0)
        {
            // 건물은 항상 밝게
            glUniform1f(locBrightness, 1.0f);
            //float buildingLen = 1.5f;
            float buildingLen = 5.0f;
            float density = 1.8f;

            // --- [왼쪽 건물] ---
            float h = leftBuildingHeight[i];

            // 건물 높이 계산 (도로 바닥 기준)
            // tunnelScaleXY가 2.0이므로 도로 바닥 표면은 Y=1.0일 수 있음.
            // 기존에 건물 배치가 잘 되었다면 이 공식 유지
            float yPos = -1.0f + (h / 2.0f);

            Mat4 modelLeft = multifly(translate(-2.5f, yPos, zPos), scale(1.0f, h, buildingLen));
            glUniformMatrix4fv(locModel, 1, GL_FALSE, modelLeft.m);

            // 1) 건물 벽
            glUniform1i(locUseObjectColor, 1);
            glUniform1i(locUseTexture, 0);
            glUniform1i(locIsBuilding, 0);
            applyBuildingColor(leftBuildingColor[i], locObjectColor);
            drawCubeFace(0); drawCubeFace(1); drawCubeFace(3); drawCubeFace(4);

            // 2) 건물 창문 (옆면)
            glUniform1i(locIsBuilding, 1);
            glUniform2f(locTexScale, buildingLen * density, h * density);
            drawCubeFace(5);


            // --- [오른쪽 건물] ---
            h = rightBuildingHeight[i];
            yPos = -1.0f + (h / 2.0f);

            Mat4 modelRight = multifly(translate(2.5f, yPos, zPos), scale(1.0f, h, buildingLen));
            glUniformMatrix4fv(locModel, 1, GL_FALSE, modelRight.m);

            // 1) 건물 벽
            glUniform1i(locIsBuilding, 0);
            glUniform2f(locTexScale, 1.0f, 1.0f);
            applyBuildingColor(rightBuildingColor[i], locObjectColor);
            drawCubeFace(0); drawCubeFace(1); drawCubeFace(3); drawCubeFace(5);

            // 2) 건물 창문 (옆면)
            glUniform1i(locIsBuilding, 1);
            glUniform2f(locTexScale, buildingLen * density, h * density);
            drawCubeFace(4);
        }

        // ==================================================
    // ★ [5] 도착 지점 (Finish Line) 그리기
    // ==================================================
    // for 루프가 끝난 후, 터널의 끝부분(z = -tunnelSegments)에 그립니다.

        float endZ = -(float)tunnelSegments * 1.0f + tunnelOffsetZ;

        // 1. 공통 설정 (텍스처 끄기, 색상 모드 켜기)
        glUniform1i(locUseTexture, 0);
        glUniform1i(locUseObjectColor, 1);
        glUniform1i(locIsBuilding, 0);
        glUniform1f(locBrightness, 1.0f); // 도착점은 밝게!
        glUniform2f(locTexScale, 1.0f, 1.0f);

        // --------------------------------------------------
        // A. 결승선 바닥 (빨간색 카펫 느낌)
        // --------------------------------------------------
        glUniform3f(locObjectColor, 0.8f, 0.1f, 0.1f); // 밝은 빨강

        // 바닥보다 살짝 위(-0.99)에 넓게 깝니다.
        Mat4 finishFloor = multifly(translate(0.0f, -0.99f, endZ), scale(6.0f, 1.0f, 4.0f));
        glUniformMatrix4fv(locModel, 1, GL_FALSE, finishFloor.m);
        drawCubeFace(2); // 윗면만 그리면 됨

        // --------------------------------------------------
        // B. 도착 게이트 기둥 (황금색)
        // --------------------------------------------------
        glUniform3f(locObjectColor, 1.0f, 0.84f, 0.0f); // 골드 색상

        float gateHeight = 7.0f;
        float gateWidth = 1.0f;
        float gateX = 3.5f; // 선로보다 넓게 배치

        // 왼쪽 기둥
        Mat4 gateLeft = multifly(translate(-gateX, 0.0f, endZ), scale(gateWidth, gateHeight, gateWidth));
        glUniformMatrix4fv(locModel, 1, GL_FALSE, gateLeft.m);
        // 육면체 전체 그리기
        for (int f = 0; f < 6; f++) drawCubeFace(f);

        // 오른쪽 기둥
        Mat4 gateRight = multifly(translate(gateX, 0.0f, endZ), scale(gateWidth, gateHeight, gateWidth));
        glUniformMatrix4fv(locModel, 1, GL_FALSE, gateRight.m);
        for (int f = 0; f < 6; f++) drawCubeFace(f);

        // --------------------------------------------------
        // C. 게이트 상단 간판 (파란색)
        // --------------------------------------------------
        glUniform3f(locObjectColor, 0.1f, 0.1f, 0.9f); // 진한 파랑

        // 기둥 위를 가로지르는 긴 막대
        float topY = (gateHeight / 2.0f) - 0.5f;
        Mat4 gateTop = multifly(translate(0.0f, topY, endZ), scale(gateX * 2.0f + 2.0f, 1.5f, 1.0f));
        glUniformMatrix4fv(locModel, 1, GL_FALSE, gateTop.m);
        for (int f = 0; f < 6; f++) drawCubeFace(f);


        // 상태 복구
        glUniform1i(locIsBuilding, 0);
        glUniform2f(locTexScale, 1.0f, 1.0f);
    }

    // ==================================================
    // 4. 나머지 오브젝트 그리기
    // ==================================================

    // 상태 초기화 (그리기 전 리셋)
    glUniform1i(locUseObjectColor, 0);
    glUniform1i(locUseTexture, 0);
    glUniform1f(locBrightness, 1.0f);

    drawWoodPlanks(); // 침목
    drawTrains();     // 기차
    drawCoins();      // 코인
    drawMagnets();    // 자석

    // 플레이어 그리기
    float basePlayerY = (-0.5f * tunnelScaleXY) + 0.25f;
    float finalPlayerY = basePlayerY + jumpY;
    drawRobot(playerX, finalPlayerY, playerZ);


    drawCoinUI(); // UI


    glutSwapBuffers();
}

//==========================================================
int main(int argc, char** argv)
{

    srand((unsigned)time(NULL));

    //윈도우 생성하기
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 깊이 버퍼 추가
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);
    glutCreateWindow("컴그 프로젝트");

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


    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(1.f, 1.f, 1.f, 1.f);
    shaderProgramID = createShaderProgram();

    setupCubeVAOs(); // 큐브 그림
    setupCoinVAO();  // 코인 원판 VAO 설정
    // 1. 옆면 (subway1.bmp) -> trainSideTexID에 저장
    makeTexture("subway1.bmp", &trainSideTexID);

    // 2. 앞면 (subway2.bmp) -> trainFrontTexID에 저장
    makeTexture("subway2.bmp", &trainFrontTexID);

    // 3. 윗면 (subway3.bmp) -> trainTopTexID에 저장
    makeTexture("subway3.bmp", &trainTopTexID);

    // 4. 선로 바닥 텍스처 (tile.bmp) -> floorTexID에 저장
    makeTexture("tile.bmp", &floorTexID);

    // 5. 코인 텍스처
    makeTexture("coin_f.bmp", &coinTexID);

    //6. 잔디 텍스쳐
    makeTexture("grass.bmp", &grassTexID);


    // 텍스처가 반복되게 설정 (건물이 아주 길어질 경우를 대비)
    for (int i = 0; i < 3; i++) {
        glBindTexture(GL_TEXTURE_2D, buildingTexIDs[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    Title_Init();
    initTrains();
    initCoins(); // 코인 초기화
    initBuildings();

    initMagnets();

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
//====================================================

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    aspect = (float)w / (float)h;

    gWidth = w;
    gHeight = h;
}

