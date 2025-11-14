//*** 헤더파일과 라이브러리 포함시키기
// 헤더파일 디렉토리 추가하기: 프로젝트 메뉴 -> 맨 아래에 있는 프로젝트 속성 -> VC++ 디렉토리 -> 일반의 포함 디렉토리 -> 편집으로 가서 현재 디렉토리의 include 디렉토리 추가하기
// 라이브러리 디렉토리 추가하기: 프로젝트 메뉴 -> 맨 아래에 있는 프로젝트 속성 -> VC++ 디렉토리 -> 일반의 라이브러리 디렉토리 -> 편집으로 가서 현재 디렉토리의 lib 디렉토리 추가하기


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
float playerZ = -5.0f; // 초기 Z 위치 (기존 값)
float moveSpeed = 0.2f; // 한 번에 이동할 거리

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

// 이동 행렬 생성
Mat4 translate(float tx, float ty, float tz) {
    Mat4 mat = identity();
    mat.m[12] = tx;
    mat.m[13] = ty;
    mat.m[14] = tz;
    return mat;
}

// 크기 조절 행렬 생성
Mat4 scale(float sx, float sy, float sz) {
    Mat4 mat = identity();
    mat.m[0] = sx;
    mat.m[5] = sy;
    mat.m[10] = sz;
    return mat;
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
// -----------------렌더링---------------------

// -----------------렌더링---------------------

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramID);

    // --- 카메라(View)와 원근(Projection) 설정 ---

    // View 행렬: 카메라를 원점(0,0,0)에 둠 (1인칭 시점)
    Mat4 view = identity();

    // Projection 행렬: 45도 시야각으로 원근감 적용
    Mat4 projection = perspective(45.0f * 3.14159f / 180.0f, aspect, 0.1f, 100.0f);

    // 셰이더에 View, Projection 행렬 전송 (이 행렬들은 한 번만 전송하면 됨)
    GLint locView = glGetUniformLocation(shaderProgramID, "view");
    GLint locProj = glGetUniformLocation(shaderProgramID, "projection");
    glUniformMatrix4fv(locView, 1, GL_FALSE, view.m);
    glUniformMatrix4fv(locProj, 1, GL_FALSE, projection.m);

    // 셰이더에서 Model 행렬의 위치 가져오기
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");


    // --- 터널 그리기 (큐브 벽면 반복) ---
    int tunnelSegments = 20; // 터널 길이 (20개 조각)

    for (int i = 0; i < tunnelSegments; i++)
    {
        // 1. Model 행렬 계산:
        //    매번 새로운 단위 행렬로 시작
        Mat4 model = identity();

        //    Z축으로 큐브를 차례대로 배치 (z=0, z=-1, z=-2, ...)
        //    (i * 1.0f) -> 큐브 크기가 1.0이므로 1.0 간격으로 배치
        model.m[14] = -(float)i * 1.0f;

        // 2. 셰이더에 이 큐브 조각의 Model 행렬 전송
        glUniformMatrix4fv(locModel, 1, GL_FALSE, model.m);

        // 3. 큐브의 4개 벽면만 그리기 (앞/뒤 뚜껑은 그리지 않음)
        //    (cubeFacesIndices 배열 순서에 따름)
        drawCubeFace(2); // 아랫면 (0,1,5,4)
        drawCubeFace(3); // 윗면 (3,2,6,7)
        drawCubeFace(4); // 왼쪽 면 (0,3,7,4)
        drawCubeFace(5); // 오른쪽 면 (1,2,6,5)

       
    }

    // --- 추가 --- 3레인 스트립 그리기 ---
       // 바닥을 3등분하는 세로 라인 2개 그리기
    {
        // 기준 model: 단위행렬 (라인 자체가 이미 z로 길게 만들어져 있음)
        Mat4 model = identity();

        // 1) 왼쪽 경계선
        Mat4 leftLine = model;
        leftLine.m[12] = -0.25f;          // x 이동 => 값이 작을수록 가운데 좁아짐
        glUniformMatrix4fv(locModel, 1, GL_FALSE, leftLine.m);
        drawLaneLine();

        // 2) 오른쪽 경계선
        Mat4 rightLine = model;
        rightLine.m[12] = 0.25f;
        glUniformMatrix4fv(locModel, 1, GL_FALSE, rightLine.m);
        drawLaneLine();
    }


    glutSwapBuffers();
}


int main(int argc, char** argv)
{

    srand((unsigned)time(NULL));

    //--- 윈도우 생성하기
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 깊이 버퍼 추가
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 800);
    glutCreateWindow("2025 Coding Test-Computer Graphics");

    //--- GLEW 초기화하기
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";


    //--- 세이더 읽어와서 세이더 프로그램 만들기 ---
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
    glutMainLoop();

    return 0;
}

GLvoid drawScene()             //--- 콜백 함수: 출력 콜백 함수
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    glPointSize(5.0f);
    glDrawArrays(GL_POINTS, 0, 1);

    glutSwapBuffers();

}

GLvoid Reshape(int w, int h)         //--- 콜백 함수: 다시 그리기 콜백 함수
{
    glViewport(0, 0, w, h);
    aspect = (float)w / (float)h;
}
