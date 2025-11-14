#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

// ---------------------------------------------------------------------------
//  GLSL 파일 불러오기 (네가 보낸 그대로)
// ---------------------------------------------------------------------------
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

    buf[length] = 0; // 문자열 종료
    return buf;
}

GLuint vertexShader;
GLuint fragmentShader;

// ---------------------------------------------------------------------------
//  Vertex Shader 읽고 컴파일
// ---------------------------------------------------------------------------
void make_vertexShaders()
{
    GLchar* vertexSource = filetobuf("vertex.glsl");
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

    free(vertexSource);
}

// ---------------------------------------------------------------------------
//  Fragment Shader 읽고 컴파일
// ---------------------------------------------------------------------------
void make_fragmentShaders()
{
    GLchar* fragmentSource = filetobuf("fragment.glsl");
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: frag shader 컴파일 실패\n" << errorLog << std::endl;
        return;
    }

    free(fragmentSource);
}

// ---------------------------------------------------------------------------
//  Shader Program 링크
// ---------------------------------------------------------------------------
GLuint createShaderProgram()
{
    GLint result;
    GLchar errorLog[512];
    GLuint shaderID = glCreateProgram();

    glAttachShader(shaderID, vertexShader);
    glAttachShader(shaderID, fragmentShader);

    glLinkProgram(shaderID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
        std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
        return 0;
    }

    return shaderID;
}

// ======================= 여기까지 "네가 준 GLSL 로더" 완전 동일 =======================


// ---------------------------------------------------------
// 3D Runner 기본 변수
// ---------------------------------------------------------
GLuint shaderProgramID;
GLuint VAO_floor, VBO_floor;

glm::vec3 camPos = glm::vec3(0.0f, 3.5f, 8.0f); // 3D 카메라
glm::vec3 camLook = glm::vec3(0.0f, 1.0f, 0.0f); // 캐릭터 위치

float floorZ = 0.0f;
const float FLOOR_LENGTH = 20.0f;
const float SCROLL_SPEED = 10.0f;

// ---------------------------------------------------------
// 바닥(Plane) 생성
// ---------------------------------------------------------
void createFloor()
{
    float floorVertices[] = {
        // x, y, z
        -3.0f, 0.0f,  0.0f,
         3.0f, 0.0f,  0.0f,
         3.0f, 0.0f, -FLOOR_LENGTH,

        -3.0f, 0.0f,  0.0f,
         3.0f, 0.0f, -FLOOR_LENGTH,
        -3.0f, 0.0f, -FLOOR_LENGTH,
    };

    glGenVertexArrays(1, &VAO_floor);
    glGenBuffers(1, &VBO_floor);

    glBindVertexArray(VAO_floor);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_floor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

// ---------------------------------------------------------
// 바닥 그리기
// ---------------------------------------------------------
void drawFloor(glm::mat4 vp)
{
    glUseProgram(shaderProgramID);
    glBindVertexArray(VAO_floor);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, floorZ));

    GLuint loc = glGetUniformLocation(shaderProgramID, "uMVP");
    glm::mat4 mvp = vp * model;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mvp));

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

// ---------------------------------------------------------
// 화면 업데이트
// ---------------------------------------------------------
void update()
{
    float dt = 0.016f; // 60fps 가정

    floorZ += SCROLL_SPEED * dt;

    // 무한 반복 스크롤
    if (floorZ > FLOOR_LENGTH)
        floorZ -= FLOOR_LENGTH;

    glutPostRedisplay();
}

// ---------------------------------------------------------
// 렌더링
// ---------------------------------------------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    glm::mat4 view = glm::lookAt(camPos, camLook, glm::vec3(0, 1, 0));

    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f
    );

    glm::mat4 vp = projection * view;
    drawFloor(vp);

    glutSwapBuffers();
}

// ---------------------------------------------------------
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

// ---------------------------------------------------------
// main()
// ---------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Runner Base - Using File Shader Loader");

    glewInit();
    glEnable(GL_DEPTH_TEST);

    // 1) 네가 보낸 방식 그대로 사용
    make_vertexShaders();
    make_fragmentShaders();
    shaderProgramID = createShaderProgram();

    // 2) 바닥 생성
    createFloor();

    // 3) 콜백 함수 등록
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(update);

    glutMainLoop();
    return 0;
}
