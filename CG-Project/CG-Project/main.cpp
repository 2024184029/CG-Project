<<<<<<< HEAD
//*** «Ï¥ı∆ƒ¿œ∞˙ ∂Û¿Ã∫Í∑Ø∏Æ ∆˜«‘Ω√≈∞±‚
// «Ï¥ı∆ƒ¿œ µ∑∫≈‰∏Æ √ﬂ∞°«œ±‚: «¡∑Œ¡ß∆Æ ∏ﬁ¥∫ -> ∏« æ∆∑°ø° ¿÷¥¬ «¡∑Œ¡ß∆Æ º”º∫ -> VC++ µ∑∫≈‰∏Æ -> ¿œπ›¿« ∆˜«‘ µ∑∫≈‰∏Æ -> ∆Ì¡˝¿∏∑Œ ∞°º≠ «ˆ¿Á µ∑∫≈‰∏Æ¿« include µ∑∫≈‰∏Æ √ﬂ∞°«œ±‚
// ∂Û¿Ã∫Í∑Ø∏Æ µ∑∫≈‰∏Æ √ﬂ∞°«œ±‚: «¡∑Œ¡ß∆Æ ∏ﬁ¥∫ -> ∏« æ∆∑°ø° ¿÷¥¬ «¡∑Œ¡ß∆Æ º”º∫ -> VC++ µ∑∫≈‰∏Æ -> ¿œπ›¿« ∂Û¿Ã∫Í∑Ø∏Æ µ∑∫≈‰∏Æ -> ∆Ì¡˝¿∏∑Œ ∞°º≠ «ˆ¿Á µ∑∫≈‰∏Æ¿« lib µ∑∫≈‰∏Æ √ﬂ∞°«œ±‚
=======
Ôª?/*** ?§Îçî?åÏùºÍ≥??ºÏù¥Î∏åÎü¨Î¶??¨Ìï®?úÌÇ§Í∏?
// ?§Îçî?åÏùº ?îÎ†â?†Î¶¨ Ï∂îÍ??òÍ∏∞: ?ÑÎ°ú?ùÌä∏ Î©îÎâ¥ -> Îß??ÑÎûò???àÎäî ?ÑÎ°ú?ùÌä∏ ?çÏÑ± -> VC++ ?îÎ†â?†Î¶¨ -> ?ºÎ∞ò???¨Ìï® ?îÎ†â?†Î¶¨ -> ?∏Ïßë?ºÎ°ú Í∞Ä???ÑÏû¨ ?îÎ†â?†Î¶¨??include ?îÎ†â?†Î¶¨ Ï∂îÍ??òÍ∏∞
// ?ºÏù¥Î∏åÎü¨Î¶??îÎ†â?†Î¶¨ Ï∂îÍ??òÍ∏∞: ?ÑÎ°ú?ùÌä∏ Î©îÎâ¥ -> Îß??ÑÎûò???àÎäî ?ÑÎ°ú?ùÌä∏ ?çÏÑ± -> VC++ ?îÎ†â?†Î¶¨ -> ?ºÎ∞ò???ºÏù¥Î∏åÎü¨Î¶??îÎ†â?†Î¶¨ -> ?∏Ïßë?ºÎ°ú Í∞Ä???ÑÏû¨ ?îÎ†â?†Î¶¨??lib ?îÎ†â?†Î¶¨ Ï∂îÍ??òÍ∏∞
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553


#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <gl/glew.h>            
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h> 
<<<<<<< HEAD
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
=======
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553


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

<<<<<<< HEAD
// ------------ ¿¸ø™∫Øºˆ -------------
GLuint vaoCube[6];
GLuint vaoLaneLine;   // πŸ¥⁄ ∑π¿Œ ∏∏µÈ±‚ ¿ß«— VAO

float aspect = 1.0f; // ¡æ»æ∫Ò

// ------- ¿∞∏È√º ¡§¡° -------------------------------------------

GLfloat cubeVertices[8][3] = {
   {-0.7f, -0.5f, -0.5f},        // {πÿ∏È πŸ¥⁄, , }
   {0.7f, -0.5f, -0.5f},
   {0.7f, 0.5f, -0.5f},
   {-0.7f, 0.5f, -0.5f},
   {-0.7f,-0.5f, 0.5f},
   {0.7f, -0.5f, 0.5f},
   {0.7f, 0.5f,0.5f},
   {-0.7f, 0.5f, 0.5f}
=======
// ------------ ?ÑÏó≠Î≥Ä??-------------
GLuint vaoCube[6];

float aspect = 1.0f; // Ï¢ÖÌö°Îπ?
// +++ ?åÎ†à?¥Ïñ¥ ?ÑÏπò Î≥Ä??Ï∂îÍ? +++
float playerX = 0.0f;
float playerZ = -5.0f; // Ï¥àÍ∏∞ Z ?ÑÏπò (Í∏∞Ï°¥ Í∞?
float moveSpeed = 0.2f; // ??Î≤àÏóê ?¥Îèô??Í±∞Î¶¨

// ------- ?°Î©¥Ï≤??ïÏ†ê -------------------------------------------

GLfloat cubeVertices[8][3] = {
   {-0.5f, -0.5f, -0.5f},
   {0.5f, -0.5f, -0.5f},
   {0.5f, 0.5f, -0.5f},
   {-0.5f, 0.5f, -0.5f},
   {-0.5f,-0.5f, 0.5f},
   {0.5f, -0.5f, 0.5f},
   {0.5f,0.5f,0.5f},
   {-0.5f, 0.5f, 0.5f}
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
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
<<<<<<< HEAD
   {0.0f, 0.0f, 0.0f},        // 0: æ’   (æ» ∫∏¿”)
   {0.0f, 0.0f, 0.0f},        // 1: µ⁄   (æ» ∫∏¿”)

   {0.0f, 0.0f, 1.0f},        // 2: æ∆∑°  (πŸ¥⁄)

   {0.5f, 0.0f, 1.0f},        // 3: ¿ß   (√µ¿Â)

   {0.2f, 0.4f, 1.0f},        // 4: øﬁ¬  ∫Æ
   {0.2f, 0.4f, 1.0f},        // 5: ø¿∏•¬  ∫Æ
};



// ------- «‡∑ƒ ∞ËªÍ ------
=======
   {1.0f, 0.0f, 0.0f},
   {0.0f, 1.0f, 0.0f},
   {0.0f, 0.0f, 1.0f},
   {1.0f, 1.0f, 0.0f},
   {1.0f, 0.0f, 1.0f},
   {0.0f, 1.0f, 1.0f},
};


// ------- ?âÎ†¨ Í≥ÑÏÇ∞ ------
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
struct Mat4 {
    float m[16];
};

<<<<<<< HEAD
// ¥‹¿ß«‡∑ƒ
=======
// ?®ÏúÑ?âÎ†¨
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
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

<<<<<<< HEAD
// ------ «‡∑ƒ «‘ºˆ ---------------------------------

// --------------- ∆ƒ¿œ ∫“∑Øø¿±‚ -----------------------------------------
=======
// ?¥Îèô ?âÎ†¨ ?ùÏÑ±
Mat4 translate(float tx, float ty, float tz) {
    Mat4 mat = identity();
    mat.m[12] = tx;
    mat.m[13] = ty;
    mat.m[14] = tz;
    return mat;
}

// ?¨Í∏∞ Ï°∞Ï†à ?âÎ†¨ ?ùÏÑ±
Mat4 scale(float sx, float sy, float sz) {
    Mat4 mat = identity();
    mat.m[0] = sx;
    mat.m[5] = sy;
    mat.m[10] = sz;
    return mat;
}

// ------ ?âÎ†¨ ?®Ïàò ---------------------------------

// --------------- ?åÏùº Î∂àÎü¨?§Í∏∞ -----------------------------------------
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
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
<<<<<<< HEAD
        std::cerr << "ERROR: vertex shader ƒƒ∆ƒ¿œ Ω«∆–\n" << errorLog << std::endl;
=======
        std::cerr << "ERROR: vertex shader Ïª¥Ìåå???§Ìå®\n" << errorLog << std::endl;
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
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
<<<<<<< HEAD
        std::cerr << "ERROR: frag_shader ƒƒ∆ƒ¿œ Ω«∆–\n" << errorLog << std::endl;
=======
        std::cerr << "ERROR: frag_shader Ïª¥Ìåå???§Ìå®\n" << errorLog << std::endl;
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
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
<<<<<<< HEAD
        std::cerr << "ERROR: shader program ø¨∞· Ω«∆–\n" << errorLog << std::endl;
=======
        std::cerr << "ERROR: shader program ?∞Í≤∞ ?§Ìå®\n" << errorLog << std::endl;
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
        return false;
    }

    glUseProgram(shaderID);

    return shaderID;
}



<<<<<<< HEAD
// --------- µµ«¸ √ ±‚»≠ -----------------------------
=======
// --------- ?ÑÌòï Ï¥àÍ∏∞??-----------------------------
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553

void setupCubeVAOs()
{
    for (int f = 0; f < 6; f++) {
        GLfloat data[4 * 6];
        for (int i = 0; i < 4; i++) {
            int vi = cubeFacesIndices[f][i];
            for (int j = 0; j < 3; j++) data[i * 6 + j] = cubeVertices[vi][j];
            for (int j = 0; j < 3; j++) data[i * 6 + 3 + j] = cubeFaceColors[f][j];

<<<<<<< HEAD
            glEnableVertexAttribArray(1); // πŸ¥⁄ ∑π¿ŒøÎ VAO

=======
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
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
<<<<<<< HEAD

        // ---- 2) πŸ¥⁄ 3µÓ∫–øÎ ºº∑Œ ∂Û¿Œ VAO ----
        // tunnelSegments = 20, «— ºº±◊∏’∆Æ ±Ê¿Ã = 1.0f ¿Ã¥œ±Ó ¿¸√º ±Ê¿Ã 20.0f
        float lineLength = 20.0f;

        // πŸ¥⁄ y¥¬ -0.5 ¿Ã¥œ±Ó ªÏ¬¶∏∏ ¿ß∑Œ (-0.49) ∂Áøˆº≠ z-fighting πÊ¡ˆ
        GLfloat lineColor[3] = { 1.0f, 1.0f, 1.0f }; // »Úªˆ

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
=======
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
    }
}

void drawCubeFace(int f) {
    glBindVertexArray(vaoCube[f]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

<<<<<<< HEAD
// --- √ﬂ∞° --- ∑π¿Œ ∞Ê∞Ëº± «œ≥™ ±◊∏Æ±‚ («ˆ¿Á model «‡∑ƒ ±‚¡ÿ)
void drawLaneLine() {
    glBindVertexArray(vaoLaneLine);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
// -----------------∑ª¥ı∏µ---------------------

// -----------------∑ª¥ı∏µ---------------------
=======

// +++ ?§Î≥¥??ÏΩúÎ∞± ?®Ïàò Ï∂îÍ? +++
void specialKeyboard(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP: // 'UP' ?îÏÇ¥????
        playerZ -= moveSpeed; // ZÏ∂??åÏùò Î∞©Ìñ•(?ûÏúºÎ°??ºÎ°ú ?¥Îèô
        break;
    case GLUT_KEY_DOWN: // 'DOWN' ?îÏÇ¥????
        playerZ += moveSpeed; // ZÏ∂??ëÏùò Î∞©Ìñ•(?§Î°ú)?ºÎ°ú ?¥Îèô
        break;
    case GLUT_KEY_LEFT: // 'LEFT' ?îÏÇ¥????
        playerX -= moveSpeed; // XÏ∂??åÏùò Î∞©Ìñ•(?ºÏ™Ω)?ºÎ°ú ?¥Îèô
        break;
    case GLUT_KEY_RIGHT: // 'RIGHT' ?îÏÇ¥????
        playerX += moveSpeed; // XÏ∂??ëÏùò Î∞©Ìñ•(?§Î•∏Ï™??ºÎ°ú ?¥Îèô
        break;
    }

    // ?îÎ©¥??Í∞±Ïã†?òÎèÑÎ°??îÏ≤≠
    glutPostRedisplay();
}


// -----------------?åÎçîÎß?--------------------
// ... (Display ?®Ïàò) ...

// -----------------?åÎçîÎß?--------------------
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramID);

<<<<<<< HEAD
    // --- ƒ´∏ﬁ∂Û(View)øÕ ø¯±Ÿ(Projection) º≥¡§ ---

    // View «‡∑ƒ: ƒ´∏ﬁ∂Û∏¶ ø¯¡°(0,0,0)ø° µ“ (1¿Œƒ™ Ω√¡°)
    Mat4 view = identity();

    // Projection «‡∑ƒ: 45µµ Ω√æﬂ∞¢¿∏∑Œ ø¯±Ÿ∞® ¿˚øÎ
    Mat4 projection = perspective(45.0f * 3.14159f / 180.0f, aspect, 0.1f, 100.0f);

    // ºŒ¿Ã¥ıø° View, Projection «‡∑ƒ ¿¸º€ (¿Ã «‡∑ƒµÈ¿∫ «— π¯∏∏ ¿¸º€«œ∏È µ )
=======
    // --- Ïπ¥Î©î??View)?Ä ?êÍ∑º(Projection) ?§Ï†ï ---
    Mat4 view = identity();
    Mat4 projection = perspective(45.0f * 3.14159f / 180.0f, aspect, 0.1f, 100.0f);
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
    GLint locView = glGetUniformLocation(shaderProgramID, "view");
    GLint locProj = glGetUniformLocation(shaderProgramID, "projection");
    glUniformMatrix4fv(locView, 1, GL_FALSE, view.m);
    glUniformMatrix4fv(locProj, 1, GL_FALSE, projection.m);
<<<<<<< HEAD

    // ºŒ¿Ã¥ıø°º≠ Model «‡∑ƒ¿« ¿ßƒ° ∞°¡Æø¿±‚
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");


    // --- ≈Õ≥Œ ±◊∏Æ±‚ (≈•∫Í ∫Æ∏È π›∫π) ---
    int tunnelSegments = 20; // ≈Õ≥Œ ±Ê¿Ã (20∞≥ ¡∂∞¢)

    for (int i = 0; i < tunnelSegments; i++)
    {
        // 1. Model «‡∑ƒ ∞ËªÍ:
        //    ∏≈π¯ ªı∑ŒøÓ ¥‹¿ß «‡∑ƒ∑Œ Ω√¿€
        Mat4 model = identity();

        //    Z√‡¿∏∑Œ ≈•∫Í∏¶ ¬˜∑ ¥Î∑Œ πËƒ° (z=0, z=-1, z=-2, ...)
        //    (i * 1.0f) -> ≈•∫Í ≈©±‚∞° 1.0¿Ãπ«∑Œ 1.0 ∞£∞›¿∏∑Œ πËƒ°
        model.m[14] = -(float)i * 1.0f;

        // 2. ºŒ¿Ã¥ıø° ¿Ã ≈•∫Í ¡∂∞¢¿« Model «‡∑ƒ ¿¸º€
        glUniformMatrix4fv(locModel, 1, GL_FALSE, model.m);

        // 3. ≈•∫Í¿« 4∞≥ ∫Æ∏È∏∏ ±◊∏Æ±‚ (æ’/µ⁄ ∂—≤±¿∫ ±◊∏Æ¡ˆ æ ¿Ω)
        //    (cubeFacesIndices πËø≠ º¯º≠ø° µ˚∏ß)
        drawCubeFace(2); // æ∆∑ß∏È (0,1,5,4)
        drawCubeFace(3); // ¿≠∏È (3,2,6,7)
        drawCubeFace(4); // øﬁ¬  ∏È (0,3,7,4)
        drawCubeFace(5); // ø¿∏•¬  ∏È (1,2,6,5)

       
    }

    // --- √ﬂ∞° --- 3∑π¿Œ Ω∫∆Æ∏≥ ±◊∏Æ±‚ ---
       // πŸ¥⁄¿ª 3µÓ∫–«œ¥¬ ºº∑Œ ∂Û¿Œ 2∞≥ ±◊∏Æ±‚
    {
        // ±‚¡ÿ model: ¥‹¿ß«‡∑ƒ (∂Û¿Œ ¿⁄√º∞° ¿ÃπÃ z∑Œ ±Ê∞‘ ∏∏µÈæÓ¡Æ ¿÷¿Ω)
        Mat4 model = identity();

        // 1) øﬁ¬  ∞Ê∞Ëº±
        Mat4 leftLine = model;
        leftLine.m[12] = -0.25f;          // x ¿Ãµø => ∞™¿Ã ¿€¿ªºˆ∑œ ∞°øÓµ• ¡ºæ∆¡¸
        glUniformMatrix4fv(locModel, 1, GL_FALSE, leftLine.m);
        drawLaneLine();

        // 2) ø¿∏•¬  ∞Ê∞Ëº±
        Mat4 rightLine = model;
        rightLine.m[12] = 0.25f;
        glUniformMatrix4fv(locModel, 1, GL_FALSE, rightLine.m);
        drawLaneLine();
=======
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");


    // --- ?∞ÎÑê Í∑∏Î¶¨Í∏?(?êÎ∏å Î≤ΩÎ©¥ Î∞òÎ≥µ) ---
    int tunnelSegments = 300; // ?∞ÎÑê Í∏∏Ïù¥
    float tunnelScaleXY = 2.0f; // ?∞ÎÑê ?àÎπÑ/?íÏù¥ Î∞∞Ïú® (2Î∞?

    for (int i = 0; i < tunnelSegments; i++)
    {
        // 1. Model ?âÎ†¨ Í≥ÑÏÇ∞:

        // ?∞ÎÑê ?¨Í∏∞ Ï°∞Ï†à (X: 2Î∞? Y: 2Î∞? Z: 1Î∞?
        // ZÏ∂??¨Í∏∞??1.0?ºÎ°ú ?†Ï??¥Ïïº ?∞ÎÑê Ï°∞Í∞Å?§Ïù¥ ?úÎ°ú Î∂ôÏäµ?àÎã§.
        Mat4 modelTunnelScale = scale(tunnelScaleXY, tunnelScaleXY, 1.0f);
        // ZÏ∂ïÏúºÎ°??∞ÎÑê Ï°∞Í∞Å Î∞∞Ïπò
        Mat4 modelTunnelTranslate = translate(0.0f, 0.0f, -(float)i * 1.0f);

        // ?¨Í∏∞ Ï°∞Ï†à ???¥Îèô
        Mat4 model = multifly(modelTunnelTranslate, modelTunnelScale);

        // 2. ?∞Ïù¥?îÏóê ???êÎ∏å Ï°∞Í∞Å??Model ?âÎ†¨ ?ÑÏÜ°
        glUniformMatrix4fv(locModel, 1, GL_FALSE, model.m);

        // 3. ?êÎ∏å??4Í∞?Î≤ΩÎ©¥Îß?Í∑∏Î¶¨Í∏?
        drawCubeFace(2); // ?ÑÎû´Î©?
        drawCubeFace(3); // ?óÎ©¥
        drawCubeFace(4); // ?ºÏ™Ω Î©?
        drawCubeFace(5); // ?§Î•∏Ï™?Î©?
    }

    // +++ ?åÎ†à?¥Ïñ¥ ?êÎ∏å Í∑∏Î¶¨Í∏?+++

    // 1. Model ?âÎ†¨ Í≥ÑÏÇ∞:

    // ?åÎ†à?¥Ïñ¥ ?êÎ∏å ?¨Í∏∞ (0.2Î∞?
    float playerScale = 0.5f; // ?åÎ†à?¥Ïñ¥ ?¨Í∏∞Î•?Î≥Ä?òÎ°ú ?Ä??
    Mat4 modelPlayerScale = scale(playerScale, playerScale, playerScale);

    // ?åÎ†à?¥Ïñ¥ ?ÑÏπò Í≥ÑÏÇ∞:
    // 1. ?∞ÎÑê Î∞îÎã• Y ?ÑÏπò:
    //    Í∏∞Î≥∏ ?êÎ∏å Î∞îÎã•: -0.5f
    //    ?∞ÎÑê Y ?§Ï??? tunnelScaleXY (2.0f)
    //    -> ?§Ï†ú ?∞ÎÑê Î∞îÎã• Y = -0.5f * 2.0f = -1.0f
    // 2. ?åÎ†à?¥Ïñ¥ Y ?ÑÏπò:
    //    ?åÎ†à?¥Ïñ¥ ?¨Í∏∞: 0.2f -> ?åÎ†à?¥Ïñ¥ ?àÎ∞ò ?¨Í∏∞: 0.1f
    //    -> ?åÎ†à?¥Ïñ¥ Ï§ëÏã¨ Y = ?§Ï†ú ?∞ÎÑê Î∞îÎã• + ?åÎ†à?¥Ïñ¥ ?àÎ∞ò ?¨Í∏∞
    //    -> -1.0f + 0.1f = -0.9f
    float playerY = (-0.5f * tunnelScaleXY) + (playerScale / 2.0f);

    // 3. ?åÎ†à?¥Ïñ¥ Z ?ÑÏπò:
    //    *** ?ÑÏó≠ Î≥Ä???¨Ïö©?òÎèÑÎ°??òÏ†ï ***
    Mat4 modelPlayerTranslate = translate(playerX, playerY, playerZ);

    // ÏµúÏ¢Ö Model ?âÎ†¨ Í≥ÑÏÇ∞ (?§Ï????ÅÏö© ???¥Îèô)
    Mat4 modelPlayer = multifly(modelPlayerTranslate, modelPlayerScale);

    // 2. ?∞Ïù¥?îÏóê ?åÎ†à?¥Ïñ¥ ?êÎ∏å??Model ?âÎ†¨ ?ÑÏÜ°
    glUniformMatrix4fv(locModel, 1, GL_FALSE, modelPlayer.m);

    // 3. ?êÎ∏å??6Í∞?Î©?Î™®Îëê Í∑∏Î¶¨Í∏?
    for (int f = 0; f < 6; f++) {
        drawCubeFace(f);
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
    }


    glutSwapBuffers();
}

<<<<<<< HEAD

=======
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
int main(int argc, char** argv)
{

    srand((unsigned)time(NULL));

<<<<<<< HEAD
    //--- ¿©µµøÏ ª˝º∫«œ±‚
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // ±Ì¿Ã πˆ∆€ √ﬂ∞°
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 800);
    glutCreateWindow("2025 Coding Test-Computer Graphics");

    //--- GLEW √ ±‚»≠«œ±‚
=======
    //--- ?àÎèÑ???ùÏÑ±?òÍ∏∞
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // ÍπäÏù¥ Î≤ÑÌçº Ï∂îÍ?
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1280, 960);
    glutCreateWindow("2025 Coding Test-Computer Graphics");

    //--- GLEW Ï¥àÍ∏∞?îÌïòÍ∏?
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";


<<<<<<< HEAD
    //--- ºº¿Ã¥ı ¿–æÓøÕº≠ ºº¿Ã¥ı «¡∑Œ±◊∑• ∏∏µÈ±‚ ---
    make_vertexShaders();
    make_fragmentShaders();

    // ------ √ﬂ∞° ----------
=======
    //--- ?∏Ïù¥???ΩÏñ¥?Ä???∏Ïù¥???ÑÎ°úÍ∑∏Îû® ÎßåÎì§Í∏?---
    make_vertexShaders();
    make_fragmentShaders();

    // ------ Ï∂îÍ? ----------
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.f, 1.f, 1.f, 1.f);
    shaderProgramID = createShaderProgram();

<<<<<<< HEAD
    setupCubeVAOs(); // ≈•∫Í ±◊∏≤
=======
    setupCubeVAOs(); // ?êÎ∏å Í∑∏Î¶º
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553



    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
<<<<<<< HEAD
=======

    glutSpecialFunc(specialKeyboard);
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
    glutMainLoop();

    return 0;
}

<<<<<<< HEAD
GLvoid drawScene()             //--- ƒ›πÈ «‘ºˆ: √‚∑¬ ƒ›πÈ «‘ºˆ
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
=======
GLvoid drawScene()             //--- ÏΩúÎ∞± ?®Ïàò: Ï∂úÎ†• ÏΩúÎ∞± ?®Ïàò
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | 0x00000100);
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553

    glUseProgram(shaderProgramID);

    glPointSize(5.0f);
    glDrawArrays(GL_POINTS, 0, 1);

    glutSwapBuffers();

}

<<<<<<< HEAD
GLvoid Reshape(int w, int h)         //--- ƒ›πÈ «‘ºˆ: ¥ŸΩ√ ±◊∏Æ±‚ ƒ›πÈ «‘ºˆ
=======
GLvoid Reshape(int w, int h)         //--- ÏΩúÎ∞± ?®Ïàò: ?§Ïãú Í∑∏Î¶¨Í∏?ÏΩúÎ∞± ?®Ïàò
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
{
    glViewport(0, 0, w, h);
    aspect = (float)w / (float)h;
}
