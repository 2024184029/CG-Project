//*** Çì´õÆÄÀÏ°ú ¶óÀÌºê·¯¸® Æ÷ÇÔ½ÃÅ°±â
// Çì´õÆÄÀÏ µğ·ºÅä¸® Ãß°¡ÇÏ±â: ÇÁ·ÎÁ§Æ® ¸Ş´º -> ¸Ç ¾Æ·¡¿¡ ÀÖ´Â ÇÁ·ÎÁ§Æ® ¼Ó¼º -> VC++ µğ·ºÅä¸® -> ÀÏ¹İÀÇ Æ÷ÇÔ µğ·ºÅä¸® -> ÆíÁıÀ¸·Î °¡¼­ ÇöÀç µğ·ºÅä¸®ÀÇ include µğ·ºÅä¸® Ãß°¡ÇÏ±â
// ¶óÀÌºê·¯¸® µğ·ºÅä¸® Ãß°¡ÇÏ±â: ÇÁ·ÎÁ§Æ® ¸Ş´º -> ¸Ç ¾Æ·¡¿¡ ÀÖ´Â ÇÁ·ÎÁ§Æ® ¼Ó¼º -> VC++ µğ·ºÅä¸® -> ÀÏ¹İÀÇ ¶óÀÌºê·¯¸® µğ·ºÅä¸® -> ÆíÁıÀ¸·Î °¡¼­ ÇöÀç µğ·ºÅä¸®ÀÇ lib µğ·ºÅä¸® Ãß°¡ÇÏ±â


#define _CRT_SECURE_NO_WARNINGS 

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

<<<<<<< HEAD
// ------------ Àü¿ªº¯¼ö -------------
GLuint vaoCube[6];
GLuint vaoLaneLine;   // ¹Ù´Ú ·¹ÀÎ ¸¸µé±â À§ÇÑ VAO
GLuint vaoCoin; 	 // ÄÚÀÎ VAO
const int COIN_SEGMENTS = 32;
const int COIN_VERT_COUNT = COIN_SEGMENTS + 2; // Áß½É + ¼¼±×¸ÕÆ® + ³¡ Á¤Á¡

float coinRotateAngle = 0.0f;   // ÄÚÀÎ È¸Àü °¢µµ

float aspect = 1.0f; // Á¾È¾ºñ

// +++ ÇÃ·¹ÀÌ¾î À§Ä¡ º¯¼ö Ãß°¡ +++
float playerX = 0.0f;
float playerZ = -3.0f; // ÃÊ±â Z À§Ä¡ (±âÁ¸ °ª)
float moveSpeed = 0.2f; // ÇÑ ¹ø¿¡ ÀÌµ¿ÇÒ °Å¸®
//ÅÍ³Î z¹æÇâ ÀÌµ¿·® (Æ®·¹µå¹Ğ ¿ÀÇÁ¼Â)
float tunnelOffsetZ = 0.0f;

//ÀÚµ¿ ÀÌµ¿µÇ´Â º¯¼ö Ãß°¡
bool isAutoMove = false;
//float autoMoveSpeedPerFrame = 0.02f;
float autoMoveSpeedPerFrame = 0.008f;

// [·Îº¿ ¾Ö´Ï¸ŞÀÌ¼Ç¿ë Àü¿ª º¯¼ö Ãß°¡]
float limbAngle = 0.0f;   // ÆÈ´Ù¸® °¢µµ
float limbDir = 1.0f;     // ÆÈ´Ù¸® ¿òÁ÷ÀÓ ¹æÇâ

//Á¡ÇÁ °ü·Ã º¯¼ö
bool isJumping = false;
float jumpY = 0.0f;
float jumpVelocity = 0.0f;
const float GRAVITY = 0.0006f;    // Áß·ÂÀ» Àı¹İ Á¤µµ·Î ³·Ãã (ÃµÃµÈ÷ ¶³¾îÁü)
const float JUMP_POWER = 0.045f;



//ÁÂ¿ì ºÎµå·¯¿î ÀÌµ¿À» À§ÇÑ º¯¼ö
bool isLeftDown = false;  // ¿ŞÂÊ Å°°¡ ´­·ÁÀÖ´Â°¡?
bool isRightDown = false; // ¿À¸¥ÂÊ Å°°¡ ´­·ÁÀÖ´Â°¡?
float playerMoveSpeed = 0.05f; // ÁÂ¿ì ÀÌµ¿ ¼Óµµ (ÀÌ °ªÀ» Å°¿ì¸é ´õ ºü¸£°Ô ¿òÁ÷ÀÓ)
const float PLAYER_LIMIT = 1.8f;

int currentLane = 0;        // ÇöÀç ·¹ÀÎ ¹øÈ£ (-1: ¿ŞÂÊ, 0: °¡¿îµ¥, 1: ¿À¸¥ÂÊ)
float targetPlayerX = 0.0f; // ÇÃ·¹ÀÌ¾î°¡ ÀÌµ¿ÇØ¾ß ÇÒ ¸ñÇ¥ X ÁÂÇ¥
const float LANE_WIDTH = 0.85f; // ·¹ÀÎ °£°İ (¼±ÀÌ 0.25´Ï±î 0.5 °£°İÀÌ¸é µü ¸Â½À´Ï´Ù)
float laneSwitchSpeed = 0.04f;

//---------------------------------------------------
//±âÂ÷ °ü·Ã ±¸Á¶Ã¼ ¹× º¯¼ö
struct Train {
    int lane;
    float zPos;
    int colorType;
    bool isMoving;
    float speed;
    int type;
};

// ======== ¿©±âºÎÅÍ Ãß°¡ ========
struct Coin {
    int lane;       // -1, 0, 1 ·¹ÀÎ
    float zPos;     // ±âÂ÷¿Í °°Àº ¹æ½ÄÀ¸·Î z À§Ä¡
    bool collected; // ¸Ô¾ú´ÂÁö ¿©ºÎ
};

std::vector<Coin> coins; // ÄÚÀÎ ¸ñ·Ï
int coinCount = 0;       // ¸ÔÀº ÄÚÀÎ °³¼ö

int gWidth = 800, gHeight = 800; // ÇöÀç Ã¢ Å©±â (ÅØ½ºÆ®¿ë)

//---------------------------------------------------

std::vector<Train> trains; // ±âÂ÷µéÀ» °ü¸®ÇÒ ¸®½ºÆ®
//----------------------------------------------------------
bool isGameStarted = false; //°ÔÀÓ ½ÃÀÛ ¿©ºÎ
//----------------------------------------
//½½¶óÀÌµù °ü·Ã º¯¼ö
bool isSliding = false;
//int slideTimer = 0;//Áö¼Ó½Ã°£
const int SLIDE_DURATION = 45; //½½¶óÀÌµù À¯Áö ½Ã°£

// ------- À°¸éÃ¼ Á¤Á¡ -------------------------------------------

GLfloat cubeVertices[8][3] = {
   {-0.7f, -0.5f, -0.5f},        // {¹Ø¸é ¹Ù´Ú, , }
   {0.7f, -0.5f, -0.5f},
   {0.7f, 0.5f, -0.5f},
   {-0.7f, 0.5f, -0.5f},
   {-0.7f,-0.5f, 0.5f},
   {0.7f, -0.5f, 0.5f},
   {0.7f, 0.5f,0.5f},
   {-0.7f, 0.5f, 0.5f}
=======
// ------------ ?„ì—­ë³€??-------------
GLuint vaoCube[6];

float aspect = 1.0f; // ì¢…íš¡ë¹?
// +++ ?Œë ˆ?´ì–´ ?„ì¹˜ ë³€??ì¶”ê? +++
float playerX = 0.0f;
float playerZ = -5.0f; // ì´ˆê¸° Z ?„ì¹˜ (ê¸°ì¡´ ê°?
float moveSpeed = 0.2f; // ??ë²ˆì— ?´ë™??ê±°ë¦¬

// ------- ?¡ë©´ì²??•ì  -------------------------------------------

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
   {0.0f, 0.0f, 0.0f},        // 0: ¾Õ   (¾È º¸ÀÓ)
   {0.0f, 0.0f, 0.0f},        // 1: µÚ   (¾È º¸ÀÓ)

   {0.0f, 0.0f, 1.0f},        // 2: ¾Æ·¡  (¹Ù´Ú)

   {0.5f, 0.0f, 1.0f},        // 3: À§   (ÃµÀå)

   {0.2f, 0.4f, 1.0f},        // 4: ¿ŞÂÊ º®
   {0.2f, 0.4f, 1.0f},        // 5: ¿À¸¥ÂÊ º®
};



// ------- Çà·Ä °è»ê ------
=======
   {1.0f, 0.0f, 0.0f},
   {0.0f, 1.0f, 0.0f},
   {0.0f, 0.0f, 1.0f},
   {1.0f, 1.0f, 0.0f},
   {1.0f, 0.0f, 1.0f},
   {0.0f, 1.0f, 1.0f},
};


// ------- ?‰ë ¬ ê³„ì‚° ------
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
struct Mat4 {
    float m[16];
};

<<<<<<< HEAD
// ´ÜÀ§Çà·Ä
=======
// ?¨ìœ„?‰ë ¬
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

// --- Ãß°¡: Å©±â º¯È¯ Çà·Ä ---
Mat4 scale(float sx, float sy, float sz)
{
    Mat4 m = { 0 };      // ¸ğµÎ 0À¸·Î ÃÊ±âÈ­
    m.m[0] = sx;       // (0,0)
    m.m[5] = sy;       // (1,1)
    m.m[10] = sz;       // (2,2)
    m.m[15] = 1.0f;     // (3,3) µ¿Â÷ÁÂÇ¥¿ë 1
    return m;
}

// --- Ãß°¡: ÀÌµ¿ Çà·Ä ---
Mat4 translate(float tx, float ty, float tz)
{
    Mat4 m = identity(); // ±âº»Àº ´ÜÀ§Çà·Ä
    m.m[12] = tx;        // 4x4 Çà·Ä¿¡¼­ (3,0) = x ÀÌµ¿
    m.m[13] = ty;        // (3,1) = y ÀÌµ¿
    m.m[14] = tz;        // (3,2) = z ÀÌµ¿
    return m;
}


// ------ Çà·Ä ÇÔ¼ö ---------------------------------

// --------------- ?Œì¼ ë¶ˆëŸ¬?¤ê¸° -----------------------------------------
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
        std::cerr << "ERROR: vertex shader ÄÄÆÄÀÏ ½ÇÆĞ\n" << errorLog << std::endl;
=======
        std::cerr << "ERROR: vertex shader ì»´íŒŒ???¤íŒ¨\n" << errorLog << std::endl;
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
        std::cerr << "ERROR: frag_shader ÄÄÆÄÀÏ ½ÇÆĞ\n" << errorLog << std::endl;
=======
        std::cerr << "ERROR: frag_shader ì»´íŒŒ???¤íŒ¨\n" << errorLog << std::endl;
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
        std::cerr << "ERROR: shader program ¿¬°á ½ÇÆĞ\n" << errorLog << std::endl;
=======
        std::cerr << "ERROR: shader program ?°ê²° ?¤íŒ¨\n" << errorLog << std::endl;
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
        return false;
    }

    glUseProgram(shaderID);

    return shaderID;
}



<<<<<<< HEAD
// --------- µµÇü ÃÊ±âÈ­ -----------------------------
=======
// --------- ?„í˜• ì´ˆê¸°??-----------------------------
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
            glEnableVertexAttribArray(1); // ¹Ù´Ú ·¹ÀÎ¿ë VAO

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

        // ---- 2) ¹Ù´Ú 3µîºĞ¿ë ¼¼·Î ¶óÀÎ VAO ----
        // tunnelSegments = 20, ÇÑ ¼¼±×¸ÕÆ® ±æÀÌ = 1.0f ÀÌ´Ï±î ÀüÃ¼ ±æÀÌ 20.0f
        float lineLength = 20.0f;

        // ¹Ù´Ú y´Â -0.5 ÀÌ´Ï±î »ìÂ¦¸¸ À§·Î (-0.49) ¶ç¿ö¼­ z-fighting ¹æÁö
        GLfloat lineColor[3] = { 1.0f, 1.0f, 1.0f }; // Èò»ö

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

// ------------------------------------------------------
void setupCoinVAO()
{
    // [x, y, z, r, g, b] * (COIN_VERT_COUNT)
    GLfloat data[COIN_VERT_COUNT * 6];

    int idx = 0;

    // Áß½ÉÁ¡
    data[idx++] = 0.0f; // x
    data[idx++] = 0.0f; // y
    data[idx++] = 0.0f; // z
    data[idx++] = 1.0f; // r
    data[idx++] = 1.0f; // g
    data[idx++] = 0.0f; // b (³ë¶û)

    // µÑ·¹ Á¡µé
    for (int i = 0; i <= COIN_SEGMENTS; i++) {
        float theta = (2.0f * 3.141592f * i) / COIN_SEGMENTS;
        float x = cosf(theta);
        float y = sinf(theta);

        data[idx++] = x;
        data[idx++] = y;
        data[idx++] = 0.0f; // z´Â 0

        data[idx++] = 1.0f;
        data[idx++] = 1.0f;
        data[idx++] = 0.0f;
    }

    GLuint vbo;
    glGenVertexArrays(1, &vaoCoin);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vaoCoin);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    // À§Ä¡
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // »ö (»ç½Ç uObjectColor·Î µ¤¾î ¾µ °ÅÁö¸¸ Æ÷¸Ë ¸ÂÃß±â¿ë)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

// ·Îº¿ÀÇ °¢ ºÎÀ§(Å¥ºê)¸¦ ±×¸®´Â ÇïÆÛ ÇÔ¼ö
void drawColoredCube(glm::mat4 modelMatrix, glm::vec3 color) {
    // 1. ¼ÎÀÌ´õ À¯´ÏÆû À§Ä¡ °¡Á®¿À±â
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");
    GLint locObjectColor = glGetUniformLocation(shaderProgramID, "uObjectColor");
    GLint locUseObjectColor = glGetUniformLocation(shaderProgramID, "uUseObjectColor");

    // 2. GLM Çà·ÄÀ» ¼ÎÀÌ´õ·Î Àü¼Û (glm::value_ptr »ç¿ë)
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // 3. »ö»ó Àü¼Û ¹× »ö»ó ¸ğµå ÄÑ±â
    glUniform3f(locObjectColor, color.r, color.g, color.b); // ¿øÇÏ´Â »ö»ó
    glUniform1i(locUseObjectColor, 1); // true (·Îº¿ »ö»ó »ç¿ë)

    // 4. À°¸éÃ¼ ±×¸®±â (±âÁ¸ vaoCube È°¿ë)
    // »ö»óÀº ¼ÎÀÌ´õ¿¡¼­ µ¤¾î¾²¹Ç·Î ¾Æ¹« vao³ª ½áµµ µÇÁö¸¸ 6¸éÀ» ´Ù ±×·Á¾ß ÇÔ
    for (int i = 0; i < 6; i++) {
        glBindVertexArray(vaoCube[i]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    // 5. »ö»ó ¸ğµå ²ô±â (ÅÍ³Î ±×¸± ¶§ ¿µÇâÀ» ÁÖÁö ¾Êµµ·Ï)
    glUniform1i(locUseObjectColor, 0);
}

// ------------------------------------------------------
//   ¿øÆÇ ÄÚÀÎ ±×¸®±â ÇïÆÛ
// ------------------------------------------------------
void drawCoinMesh(glm::mat4 modelMatrix, glm::vec3 color) {
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");
    GLint locObjectColor = glGetUniformLocation(shaderProgramID, "uObjectColor");
    GLint locUseObjectColor = glGetUniformLocation(shaderProgramID, "uUseObjectColor");

    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3f(locObjectColor, color.r, color.g, color.b);
    glUniform1i(locUseObjectColor, 1);

    glBindVertexArray(vaoCoin);
    glDrawArrays(GL_TRIANGLE_FAN, 0, COIN_VERT_COUNT);

    glBindVertexArray(0);
    glUniform1i(locUseObjectColor, 0);
}

//=========================================================================
//±âÂ÷ ÃÊ±âÈ­ ÇÔ¼ö 

void initTrains() {
    trains.clear();

    float startZ = -30.0f;
    float gapZ = 40.0f;
    // ¡Ú ¼öÁ¤: ¸Ê ÆĞÅÏÀ» ±æ°Ô ¸¸µé¾î¼­ ¹İº¹µÇ´ÂÁö ¸ğ¸£°Ô ÇÔ (15 -> 50)
    int numberOfSets = 50;

    for (int i = 0; i < numberOfSets; i++) {
        float currentZPos = startZ - (i * gapZ);

        // 1ÁÙ¿¡ 1~2°³ ¹èÄ¡
        int trainCount = (rand() % 2) + 1;

        std::vector<int> lanes = { -1, 0, 1 };
        std::random_shuffle(lanes.begin(), lanes.end());

        for (int k = 0; k < trainCount; k++) {
            Train t;
            t.lane = lanes[k];
            t.zPos = currentZPos;

            // ¡Ú ±âÂ÷(0) vs ¿ïÅ¸¸®(1) °áÁ¤ ¡Ú
            // °°Àº ÀÚ¸®¿¡ ±âÂ÷¶û ¿ïÅ¸¸®°¡ µ¿½Ã¿¡ »ı±æ ¼ö ¾øÀ½ (if-else ±¸Á¶¶ó¼­)
            // 25% È®·ü·Î ¿ïÅ¸¸® »ı¼º
            if (rand() % 4 == 0) {
                t.type = 1; // ¿ïÅ¸¸®
            }
            else {
                t.type = 0; // ±âÂ÷
            }

            t.colorType = rand() % 5;
            trains.push_back(t);
        }
    }
}

// ======== ¿©±âºÎÅÍ Ãß°¡: ÄÚÀÎ ÃÊ±âÈ­ ÇÔ¼ö ========
void initCoins() {
    coins.clear();

    float startZ = -20.0f;   // ÇÃ·¹ÀÌ¾î ±âÁØ ¾ÕÂÊ¿¡¼­ ½ÃÀÛ
    float gapZ = 20.0f;    // ÄÚÀÎ ¼¼Æ® °£°İ
    int   sets = 50;       // ¾ó¸¶³ª ¸Ö¸®±îÁö ¸¸µéÁö

    for (int i = 0; i < sets; i++) {
        float baseZ = startZ - i * gapZ;   // Á¡Á¡ ¸Ö¾îÁöµµ·Ï

        int coinPerSet = rand() % 3;       // 0~2°³ ÄÚÀÎ

        std::vector<int> lanes = { -1, 0, 1 };
        std::random_shuffle(lanes.begin(), lanes.end());

        for (int k = 0; k < coinPerSet; k++) {
            Coin c;
            c.lane = lanes[k];
            c.zPos = baseZ;
            c.collected = false;
            coins.push_back(c);
        }
    }
}
// ======== ÄÚÀÎ ÃÊ±âÈ­ ÇÔ¼ö ³¡ ========

//ÇÃ·¹ÀÌ¾î ¹ß ¹ØÀÇ ³ôÀÌ °è»ê ÇÔ¼ö
float getGroundHeight() {
    float trainHeight = 0.8f;   // ±âÂ÷ ³ôÀÌ
    float trainWidth = 0.3f;    // ±âÂ÷ Æø (Ãæµ¹ ÆÇÁ¤¿ë)
    float trainLength = 15.0f;  // ±âÂ÷ ±æÀÌ

    for (int i = 0; i < trains.size(); i++) {
        // 1. ±âÂ÷ÀÇ ÇöÀç Z À§Ä¡ °è»ê (ÇÃ·¹ÀÌ¾î ÂÊÀ¸·Î ´Ù°¡¿À´Â °Í ¹İ¿µ)
        float currentTrainZ = trains[i].zPos + tunnelOffsetZ;

        // 2. ±âÂ÷ÀÇ X À§Ä¡ °è»ê
        float trainX = trains[i].lane * LANE_WIDTH;

        // 3. Ãæµ¹ °Ë»ç (ÇÃ·¹ÀÌ¾î°¡ ±âÂ÷ ¿µ¿ª ¾È¿¡ ÀÖ´Â°¡?)
        // ÇÃ·¹ÀÌ¾î Z´Â -3.0f·Î °íÁ¤µÇ¾î ÀÖ´Ù°í °¡Á¤ (Àü¿ªº¯¼ö playerZ)

        // ZÃà ÆÇÁ¤: ±âÂ÷ ¾ÕµÚ ±æÀÌ ¾È¿¡ ÇÃ·¹ÀÌ¾î°¡ ÀÖ´Â°¡?
        bool inZ = (playerZ >= currentTrainZ - trainLength / 2.0f) &&
            (playerZ <= currentTrainZ + trainLength / 2.0f);

        // XÃà ÆÇÁ¤: ÇÃ·¹ÀÌ¾î°¡ ±âÂ÷ ·¹ÀÎ(ÁÂ¿ì Æø) ¾È¿¡ ÀÖ´Â°¡?
        // ÇÃ·¹ÀÌ¾î Å¥ºê Å©±â°¡ ÀÖÀ¸´Ï ¾à°£ ¿©À¯¸¦ µÓ´Ï´Ù (+0.1f)
        bool inX = (playerX >= trainX - trainWidth / 2.0f - 0.1f) &&
            (playerX <= trainX + trainWidth / 2.0f + 0.1f);

        // ÇÃ·¹ÀÌ¾î°¡ ±âÂ÷ À§¿¡ ÀÖ°í, ÇÃ·¹ÀÌ¾î ³ôÀÌ°¡ ±âÂ÷º¸´Ù ³ô°Å³ª °°À» ¶§ (¶Õ°í ¿Ã¶ó¿ÀÁö ¾Ê°Ô)
        if (inZ && inX && jumpY >= trainHeight - 0.1f) {
            return trainHeight; // ¿©±â´Â ¹Ù´ÚÀÌ 0.8ÃşÀÌ´Ù!
        }
    }

    return 0.0f;
}
//=============================================================


bool checkCollision() {
    float tHeight = 0.8f;      // ±âÂ÷ ³ôÀÌ
    float tLength = 15.0f;     // ±âÂ÷ ±æÀÌ
    float collisionWidth = 0.4f; // Ãæµ¹ ³Êºñ

    for (int i = 0; i < trains.size(); i++) {
        // 1. °øÅë À§Ä¡ °è»ê
        float currentTrainZ = trains[i].zPos + tunnelOffsetZ;
        float trainX = trains[i].lane * LANE_WIDTH;

        // 2. [XÃà °Ë»ç] ·¹ÀÎÀÌ ´Ù¸£¸é Ãæµ¹ÇÒ ÀÏÀÌ ¾øÀ¸´Ï ¹Ù·Î ÆĞ½º (ÃÖÀûÈ­)
        bool inLane = abs(playerX - trainX) < collisionWidth;
        if (!inLane) continue;


        // ---------------------------------------------------------
        // ¡Ú Å¸ÀÔº° Ãæµ¹ ·ÎÁ÷ ºĞ±â (±âÂ÷ vs Àå¾Ö¹°) ¡Ú
        // ---------------------------------------------------------

        if (trains[i].type == 0) {
            // [TYPE 0: ±âÂ÷] - ±âÁ¸ ·ÎÁ÷ À¯Áö

            float trainFront = currentTrainZ + (tLength / 2.0f);
            float trainBack = currentTrainZ - (tLength / 2.0f);

            // Áøµ¿ ¹æÁö¸¦ À§ÇØ ¾ÕÂÊ ÆÇÁ¤À» ³Ë³ËÇÏ°Ô Àâ¾Ò´ø(+0.8f) ±âÁ¸ ÄÚµå À¯Áö
            bool inZ = (playerZ < trainFront + 0.8f) && (playerZ > trainBack + 0.2f);

            // Á¡ÇÁÇØ¼­ ±âÂ÷ À§¿¡ ¿Ã¶ó°¡Áö ¸øÇÑ °æ¿ì(³·À» ¶§)¸¸ Ãæµ¹
            bool isLow = jumpY < (tHeight - 0.1f);

            if (inZ && isLow) {
                return true; // Äç!
            }
        }
        else {
            // [TYPE 1: Àå¾Ö¹°(¿ïÅ¸¸®)] - »õ·Î Ãß°¡µÈ ·ÎÁ÷

            // Àå¾Ö¹°Àº ±âÂ÷Ã³·³ ±æÁö ¾Ê°í ¾ãÀ½ (¾ÕµÚ Æø 0.4f Á¤µµ ¿©À¯)
            // ¿ïÅ¸¸® À§Ä¡(currentTrainZ)¸¦ ±âÁØÀ¸·Î ÇÃ·¹ÀÌ¾î°¡ Áö³ª°¡´Â ÁßÀÎ°¡?
            bool hitBarrierZ = (playerZ < currentTrainZ + 0.4f) && (playerZ > currentTrainZ - 0.4f);

            if (hitBarrierZ) {
                // 1) ½½¶óÀÌµù ÁßÀÌ¶ó¸é? -> ¹«»ç Åë°ú (Ãæµ¹ ¾Æ´Ô)
                if (isSliding) continue;

                // 2) Á¡ÇÁ¸¦ ¾ÆÁÖ ³ô°Ô ¶Ù¾ú´Ù¸é? (¿ïÅ¸¸® ³ôÀÌ 0.6 ÀÌ»ó) -> ¹«»ç Åë°ú
                if (jumpY > 0.6f) continue;

                // ½½¶óÀÌµùµµ ¾Æ´Ï°í, Á¡ÇÁµµ ³·À¸¸é -> Ãæµ¹!
                return true;
            }
        }
    }
    return false; // Ãæµ¹ ¾øÀ½
}



// ======== ¿©±âºÎÅÍ Ãß°¡: ÄÚÀÎ ¾÷µ¥ÀÌÆ® & ¸Ô±â ========
void updateCoins() {
    float coinWidth = 0.25f;      // XÃà Ãæµ¹ Çã¿ë Æø
    float coinLength = 0.5f;       // ZÃà Ãæµ¹ Çã¿ë Æø

    // ±âÂ÷¿Í ¶È°°ÀÌ Æ®·¢ ±æÀÌ¸¦ µ¹·ÁÁÖ±â À§ÇØ »ç¿ë
    float loopDistance = 50.0f * 40.0f; // (train¿¡¼­ numberOfSets * gapZ)

    for (int i = 0; i < coins.size(); i++) {
        // ÇöÀç È­¸é ±âÁØ Z À§Ä¡
        float currentZ = coins[i].zPos + tunnelOffsetZ;

        // È­¸é µÚ·Î Áö³ª°¬À¸¸é ¸Ö¸® µÚ·Î º¸³»¼­ Àç»ç¿ë
        if (currentZ > 5.0f) {
            coins[i].zPos -= loopDistance;
            coins[i].collected = false;
            continue;
        }

        // ÀÌ¹Ì ¸ÔÈù ÄÚÀÎÀÌ¸é ½ºÅµ
        if (coins[i].collected) continue;

        float coinX = coins[i].lane * LANE_WIDTH;

        bool inX = fabs(playerX - coinX) < coinWidth;
        bool inZ = fabs(playerZ - currentZ) < coinLength;

        if (inX && inZ) {
            // ÄÚÀÎ ¸ÔÀ½!
            coins[i].collected = true;
            coinCount++;
        }
    }
}
// ======== ÄÚÀÎ ¾÷µ¥ÀÌÆ® ³¡ ========

//================================================================
//±âÂ÷ ±×¸®±â ÇÔ¼ö
void drawTrains() {
    float tWidth = 0.5f;
    float tHeight = 0.8f;
    float tLength = 15.0f;

    // ¡Ú ¼öÁ¤: ¸Ê ÀüÃ¼ ±æÀÌ Àç°è»ê (50¼¼Æ® * 40°£°İ = 2000.0f)
    // ÀÌ ±æÀÌ¸¸Å­ Á¤È®È÷ µÚ·Î º¸³»¾ß ÆĞÅÏÀÌ ¾È ±úÁü
    float loopDistance = 50 * 40.0f;

    for (int i = 0; i < trains.size(); i++) {
        float currentZ = trains[i].zPos + tunnelOffsetZ;

        // ¸®½ºÆù ·ÎÁ÷ (È­¸é µÚ·Î ³Ñ¾î°¬À» ¶§)
        if (currentZ > 5.0f) {

            // ·¹ÀÎ(lane)ÀÌ³ª Å¸ÀÔ(type)À» ·£´ıÀ¸·Î ¹Ù²ÙÁö ¸¶¼¼¿ä!
            // À§Ä¡¸¸ ¸Ê ±æÀÌ¸¸Å­ µÚ·Î º¸³À´Ï´Ù.
            trains[i].zPos -= loopDistance;

            // (»ö±ò Á¤µµ´Â ¹Ù²ãµµ À§Ä¡¿¡ ¿µÇâ ¾øÀ¸´Ï OK)
            trains[i].colorType = rand() % 5;

            continue;
        }

        // --- ±×¸®±â ·ÎÁ÷ (±âÁ¸°ú µ¿ÀÏ) ---
        float x = trains[i].lane * LANE_WIDTH;

        if (trains[i].type == 0) {
            // [±âÂ÷ ±×¸®±â]
            float y = -1.0f + (tHeight / 2.0f) + 0.05f;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(x, y, currentZ));
            model = glm::scale(model, glm::vec3(tWidth, tHeight, tLength));

            glm::vec3 color;
            switch (trains[i].colorType) {
            case 0: color = glm::vec3(0.8f, 0.2f, 0.2f); break; // »¡°­
            case 1: color = glm::vec3(0.4f, 0.25f, 0.1f); break; // °¥»ö
            case 2: color = glm::vec3(0.0f, 0.3f, 0.7f); break; // ÆÄ¶û
            case 3: color = glm::vec3(0.1f, 0.6f, 0.2f); break; // ÃÊ·Ï
            case 4: color = glm::vec3(0.9f, 0.6f, 0.0f); break; // ÁÖÈ²
            }
            drawColoredCube(model, color);
        }
        else {
            // [¿ïÅ¸¸® ±×¸®±â]
            float barY = -1.0f + 0.7f; // ³ôÀÌ
            glm::mat4 barModel = glm::mat4(1.0f);
            barModel = glm::translate(barModel, glm::vec3(x, barY, currentZ));
            barModel = glm::scale(barModel, glm::vec3(0.7f, 0.2f, 0.1f));
            drawColoredCube(barModel, glm::vec3(1.0f, 0.8f, 0.0f)); // ³ë¶û

            // ´Ù¸®
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

// ======== ¿©±âºÎÅÍ: ÄÚÀÎ ±×¸®±â (¿øÆÇ + È¸Àü) ========
void drawCoins() {
    float coinRadius = 0.2f;   // µ¿±×¶ó¹Ì ¹İÁö¸§
    float coinThickness = 0.1f;   // µÎ²² (¾ãÀº ¿øÆÇ)
    float coinY = -1.0f + 0.5f;  // ¹Ù´Ú À§·Î ¶ç¿ì´Â ³ôÀÌ

    float loopDistance = 50.0f * 40.0f;

    for (int i = 0; i < coins.size(); i++) {
        float currentZ = coins[i].zPos + tunnelOffsetZ;

        // È­¸é µÚ·Î ³Ñ¾î°¡¸é Àç¹èÄ¡
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

        // ÄÚÀÎÀÌ ¼¼·Î·Î ¼­ ÀÖ°í, YÃà ±âÁØÀ¸·Î ºù±Ûºù±Û
        m = glm::rotate(m, glm::radians(coinRotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));

        // XY Æò¸é¿¡ ¸¸µç ¿øÆÇÀ» ½ÇÁ¦ Å©±â·Î ½ºÄÉÀÏ
        m = glm::scale(m, glm::vec3(coinRadius, coinRadius, coinThickness));

        // ³ë¶õ ÄÚÀÎ
        drawCoinMesh(m, glm::vec3(1.0f, 1.0f, 0.0f));
    }
}
// ======== ÄÚÀÎ ±×¸®±â ³¡ ========


void drawCubeFace(int f) {
    glBindVertexArray(vaoCube[f]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

// --- Ãß°¡ --- ·¹ÀÎ °æ°è¼± ÇÏ³ª ±×¸®±â
void drawLaneLine() {
    glBindVertexArray(vaoLaneLine);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}



//=========================================================
//idle ÇÔ¼ö

void idle() {
    bool needRedisplay = false;


    // ¡Ú Ãæµ¹ Ã¼Å© ·ÎÁ÷ ¼öÁ¤ ¡Ú

    if (isGameStarted) {

        if (checkCollision()) {
            // Ãæµ¹ »óÅÂ
            isAutoMove = false; // ¸ØÃã
            tunnelOffsetZ -= 0.2f;
            //autoMoveSpeedPerFrame = 0.02f;
            autoMoveSpeedPerFrame = 0.008f;
            needRedisplay = true;
        }
        else {
            // Ãæµ¹ »óÅÂ°¡ ¾Æ´Ï¶ó¸é? (¿·À¸·Î ÇÇÇß°Å³ª, ¿ø·¡ ¾ÈÀüÇÏ´Ù¸é)
            // ´Ù½Ã ÀÚµ¿À¸·Î ´Ş¸®±â ½ÃÀÛ!
            isAutoMove = true;
        }
    }

    // 1. ÀÚµ¿ ÀÌµ¿ ·ÎÁ÷
    if (isAutoMove) {
        tunnelOffsetZ += autoMoveSpeedPerFrame;

        // ·Îº¿ ÆÈ´Ù¸® ¾Ö´Ï¸ŞÀÌ¼Ç
        limbAngle += 2.0f * limbDir;
        if (limbAngle > 45.0f || limbAngle < -45.0f) {
            limbDir *= -1.0f;
        }
        needRedisplay = true;
    }
    else {
        // ¸ØÃçÀÖÀ» ¶§´Â Â÷·Ç
        if (abs(limbAngle) > 0.1f) {
            limbAngle *= 0.9f;
            needRedisplay = true;
        }
    }

    // --- ÄÚÀÎ È¸Àü °¢µµ ¾÷µ¥ÀÌÆ® (Ç×»ó »ìÂ¦¾¿ µ·´Ù) ---
    coinRotateAngle += 1.0f;      // ÇÑ ÇÁ·¹ÀÓ´ç _µµ ¾¿ È¸Àü(¼ıÀÚ ³·Ãâ¼ö·Ï ´À¸®°Ô)
    if (coinRotateAngle > 360.0f)
        coinRotateAngle -= 360.0f;

    needRedisplay = true; // ÄÚÀÎ È¸Àü ¶§¹®¿¡ »õ·Î ±×¸®±â


    // 2. Á¡ÇÁ ¹× ÂøÁö ¹°¸® ·ÎÁ÷

    // ³» ¹ß ¹ØÀÇ ¸ñÇ¥ ¹Ù´Ú ³ôÀÌ¸¦ ±¸ÇÔ
    float currentGround = getGroundHeight();

    // ÇÃ·¹ÀÌ¾î°¡ ¹Ù´Úº¸´Ù À§¿¡ ÀÖ°Å³ª, Á¡ÇÁ ÁßÀÌ¶ó¸é ¹°¸® Àû¿ë
    if (jumpY > currentGround || isJumping) {
        jumpY += jumpVelocity;      // À§Ä¡ ÀÌµ¿
        jumpVelocity -= GRAVITY;    // Áß·Â Àû¿ë

        // ¹Ù´Ú(±âÂ÷ À§ È¤Àº ¶¥)¿¡ ´ê¾Ò´ÂÁö Ã¼Å©
        if (jumpY <= currentGround) {
            jumpY = currentGround;   // ¹Ù´Ú ³ôÀÌ¿¡ °íÁ¤
            isJumping = false;       // Á¡ÇÁ ³¡ (¶¥¿¡ ´êÀ½)
            jumpVelocity = 0.0f;     // ¼Óµµ ÃÊ±âÈ­
        }
        else {

            isJumping = true;
        }
        needRedisplay = true;
    }
    // ±âÂ÷ À§¿¡ ÀÖ´Ù°¡ ±âÂ÷°¡ Áö³ª°¡¹ö·Á¼­ Çã°øÀÌ µÈ °æ¿ì Ã³¸®
    else if (jumpY > currentGround) {
        isJumping = true; // ´Ù½Ã ¶³¾îÁö±â ½ÃÀÛÇØ¾ß ÇÔ
    }


    // 3. ·¹ÀÎ º¯°æ ¾Ö´Ï¸ŞÀÌ¼Ç ·ÎÁ÷
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

    // ======== ÄÚÀÎ À§Ä¡/¸Ô±â ¾÷µ¥ÀÌÆ® ========
    updateCoins();

    if (needRedisplay) {
        glutPostRedisplay();
    }
}


// ----------------- ÀÏ¹İ Å°º¸µå ÀÔ·Â  ---------------------
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 32:
        if (!isJumping) // ÀÌ¹Ì Á¡ÇÁ ÁßÀÌ ¾Æ´Ò ¶§¸¸ Á¡ÇÁ °¡´É
        {
            isJumping = true;
            jumpVelocity = JUMP_POWER; // À§·Î ¼Ú±¸Ä¡´Â Èû ºÎ¿©
        }
        break;

        // ³ªÁß¿¡ 'q'¸¦ ´­·¯ Á¾·áÇÏ´Â ±â´É µîÀ» ¿©±â¿¡ Ãß°¡ÇÒ ¼ö ÀÖ½À´Ï´Ù.
    case 'q':
    case 'Q':
    case 27: // ESC key
        exit(0);
        break;
    }
}

// ----------------- Å°º¸µå ÀÔ·Â ---------------------
// +++ Å°º¸µå Äİ¹é ÇÔ¼ö Ãß°¡ +++
void specialKeyboard(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP: // 'Page Up' Å°
        if (!isGameStarted) {
            isGameStarted = true;
            isAutoMove = true;
        }
        break;
    case GLUT_KEY_DOWN: // 'DOWN' È­»ìÇ¥ Å°
        if (isGameStarted) {
            isSliding = true;
        }
        break;
    case GLUT_KEY_LEFT:
        // ¿ŞÂÊ ³¡(-1)ÀÌ ¾Æ´Ò ¶§¸¸ ¿ŞÂÊÀ¸·Î ÇÑ Ä­ ÀÌµ¿ ¸í·É
        if (currentLane > -1) {
            currentLane--; // -1 °¨¼Ò
            targetPlayerX = currentLane * LANE_WIDTH; // ¸ñÇ¥ À§Ä¡ Àç¼³Á¤ (-0.5 or 0.0)
        }
        break;

    case GLUT_KEY_RIGHT:
        // ¿À¸¥ÂÊ ³¡(1)ÀÌ ¾Æ´Ò ¶§¸¸ ¿À¸¥ÂÊÀ¸·Î ÇÑ Ä­ ÀÌµ¿ ¸í·É
        if (currentLane < 1) {
            currentLane++; // +1 Áõ°¡
            targetPlayerX = currentLane * LANE_WIDTH; // ¸ñÇ¥ À§Ä¡ Àç¼³Á¤ (0.0 or 0.5)
        }
        break;

    }


    // È­¸éÀ» °»½ÅÇÏµµ·Ï ¿äÃ»
    glutPostRedisplay();
}


void specialKeyboardUp(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_DOWN:
        // Å°¸¦ ¶¼¸é ½½¶óÀÌµù ³¡! (ÀÏ¾î³²)
        isSliding = false;
        break;
    }
}



// ·Îº¿ ±×¸®±â ÇÔ¼ö
void drawRobot(float x, float y, float z) {
    // ·Îº¿ÀÇ ±âº» À§Ä¡ ¼³Á¤
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, z));

    // ¡Ú ½½¶óÀÌµù Ã³¸® ¡Ú
    if (isSliding) {
        // 1. µÚ·Î ´¯±â (-90µµ XÃà È¸Àü)
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // 2. ´©¿ì¸é Áß½ÉÃàÀÌ ¹Ù²î¹Ç·Î À§Ä¡¸¦ »ìÂ¦ ¾Æ·¡/µÚ·Î Á¶Á¤
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, -0.3f));
    }
    else {

        // ·Îº¿ÀÌ µÚ(Z-)¸¦ º¸°í ÀÖÀ¸¹Ç·Î 180µµ È¸Àü (ÇÃ·¹ÀÌ¾î ½ÃÁ¡)
        // ÇÊ¿ä¿¡ µû¶ó °¢µµ Á¶Àı: glm::radians(180.0f)
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

    // 1. ¸öÅë
    glm::mat4 bodyM = glm::scale(model, glm::vec3(0.4f, 0.5f, 0.2f));
    drawColoredCube(bodyM, glm::vec3(0.0f, 0.0f, 1.0f)); // ÆÄ¶û

    // 2. ¸Ó¸®
    glm::mat4 headM = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.0f));
    glm::mat4 headScaleM = glm::scale(headM, glm::vec3(0.3f, 0.3f, 0.3f));
    drawColoredCube(headScaleM, glm::vec3(1.0f, 0.8f, 0.6f)); // »ì»ö

    // 3. ÄÚ (»¡°­)
    glm::mat4 noseM = glm::translate(headM, glm::vec3(0.0f, 0.0f, 0.18f)); // ¾à°£ ¾ÕÀ¸·Î »­
    noseM = glm::scale(noseM, glm::vec3(0.05f, 0.05f, 0.05f));
    drawColoredCube(noseM, glm::vec3(1.0f, 0.0f, 0.0f));

    // ÆÈ´Ù¸® ¿òÁ÷ÀÓ °¢µµ
    float swing = glm::radians(limbAngle);

    // 4. ¿ŞÆÈ (³ë¶û)
    glm::mat4 lArmM = glm::translate(model, glm::vec3(-0.3f, 0.15f, 0.0f));
    lArmM = glm::rotate(lArmM, swing, glm::vec3(1.0f, 0.0f, 0.0f)); // È¸Àü
    lArmM = glm::translate(lArmM, glm::vec3(0.0f, -0.2f, 0.0f)); // È¸ÀüÃà ¾Æ·¡·Î ³»¸®±â
    drawColoredCube(glm::scale(lArmM, glm::vec3(0.1f, 0.4f, 0.1f)), glm::vec3(1.0f, 1.0f, 0.0f));

    // 5. ¿À¸¥ÆÈ (ÃÊ·Ï)
    glm::mat4 rArmM = glm::translate(model, glm::vec3(0.3f, 0.15f, 0.0f));
    rArmM = glm::rotate(rArmM, -swing, glm::vec3(1.0f, 0.0f, 0.0f));
    rArmM = glm::translate(rArmM, glm::vec3(0.0f, -0.2f, 0.0f));
    drawColoredCube(glm::scale(rArmM, glm::vec3(0.1f, 0.4f, 0.1f)), glm::vec3(0.0f, 1.0f, 0.0f));

    // 6. ¿Ş´Ù¸® (È¸»ö + º¸¶ó »ìÂ¦ ¼¯ÀÎ »öÀ¸·Î ¹Ù²Ù¼Ì³×¿ä)
    glm::mat4 lLegM = glm::translate(model, glm::vec3(-0.12f, -0.25f, 0.0f)); // 1. ¾ûµ¢ÀÌ À§Ä¡·Î ÀÌµ¿
    lLegM = glm::rotate(lLegM, -swing, glm::vec3(1.0f, 0.0f, 0.0f));          // 2. È¸Àü

    lLegM = glm::translate(lLegM, glm::vec3(0.0f, -0.15f, 0.0f));
    drawColoredCube(glm::scale(lLegM, glm::vec3(0.12f, 0.3f, 0.12f)), glm::vec3(0.5f, 0.3f, 0.5f));

    // 7. ¿À¸¥´Ù¸®
    glm::mat4 rLegM = glm::translate(model, glm::vec3(0.12f, -0.25f, 0.0f));
    rLegM = glm::rotate(rLegM, swing, glm::vec3(1.0f, 0.0f, 0.0f));

    rLegM = glm::translate(rLegM, glm::vec3(0.0f, -0.15f, 0.0f));
    drawColoredCube(glm::scale(rLegM, glm::vec3(0.12f, 0.3f, 0.12f)), glm::vec3(0.3f, 0.3f, 0.3f));
}

// -----------------·»´õ¸µ---------------------

// --- Ãß°¡ --- ÄÚÀÎ °³¼ö UI ±×¸®±â (ÁÂÃø »ó´Ü)
void drawCoinUI() {
    // ¼ÎÀÌ´õ Àá±ñ ²ô±â
    glUseProgram(0);
    glDisable(GL_DEPTH_TEST);

    // 2D ¿À½î±×·¡ÇÈ Åõ¿µ ¼³Á¤
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, gWidth, 0, gHeight, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    std::string text = "Coins: " + std::to_string(coinCount);

    glColor3f(0.0f, 0.0f, 0.0f); // °ËÁ¤»ö
    glRasterPos2i(10, gHeight - 30); // ÁÂÃø »ó´Ü

    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // »óÅÂ º¹±¸
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(shaderProgramID);
}


void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramID);
    //Mat4 view = identity();
    //--- Ä«¸Ş¶ó(View)¿Í ¿ø±Ù(Projection) ¼³Á¤ ---
    Mat4 viewTranslate = translate(0.0f, -0.9f, -1.0f);

    // 2. Ä«¸Ş¶ó °¢µµ È¸Àü (XÃà È¸Àü)
    // 20µµ Á¤µµ °í°³¸¦ ¼÷¿©¼­ ¹Ù´ÚÀ» ¹Ù¶óº½
    Mat4 viewRotate = rotateX(20.0f * 3.14159f / 180.0f);

    // 3. Çà·Ä ÇÕÄ¡±â (È¸Àü * ÀÌµ¿)
    Mat4 view = multifly(viewRotate, viewTranslate);
    Mat4 projection = perspective(45.0f * 3.14159f / 180.0f, aspect, 0.1f, 100.0f);
    GLint locView = glGetUniformLocation(shaderProgramID, "view");
    GLint locProj = glGetUniformLocation(shaderProgramID, "projection");
    glUniformMatrix4fv(locView, 1, GL_FALSE, view.m);
    glUniformMatrix4fv(locProj, 1, GL_FALSE, projection.m);
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");


    // ¹à±â À¯´ÏÆû À§Ä¡ ÇÑ ¹ø ±¸ÇØµÎ±â 
    // --- ÅÍ³Î ±×¸®±â (Å¥ºê º®¸é ¹İº¹) ---
    GLint locBrightness = glGetUniformLocation(shaderProgramID, "uBrightness");

    // --- ÅÍ³Î ±×¸®±â (Å¥ºê º®¸é ¹İº¹) ---
    int tunnelSegments = 300; // ÅÍ³Î ±æÀÌ
    float tunnelScaleXY = 2.0f; // ÅÍ³Î ³Êºñ/³ôÀÌ ¹èÀ² (2¹è)

    for (int i = 0; i < tunnelSegments; i++)
    {
        // 1. Model Çà·Ä °è»ê:

        // ÅÍ³Î Å©±â Á¶Àı (X: 2¹è, Y: 2¹è, Z: 1¹è)
        // ZÃà Å©±â´Â 1.0À¸·Î À¯ÁöÇØ¾ß ÅÍ³Î Á¶°¢µéÀÌ ¼­·Î ºÙ½À´Ï´Ù.
        Mat4 modelTunnelScale = scale(tunnelScaleXY, tunnelScaleXY, 1.0f);

        //// ZÃàÀ¸·Î ÅÍ³Î Á¶°¢ ¹èÄ¡
        //Mat4 modelTunnelTranslate = translate(0.0f, 0.0f, -(float)i * 1.0f);
        // i¹øÂ° Á¶°¢ÀÇ ±âº» À§Ä¡ -(float)i ¿¡ ÅÍ³Î ÀÌµ¿·®(tunnelOffsetZ)À» ´õÇØ¼­
        // ÅÍ³Î ÀüÃ¼°¡ ¾Õ/µÚ·Î ¹Ğ·Á³ª°Ô ¸¸µç´Ù.
        Mat4 modelTunnelTranslate = translate(0.0f, 0.0f, -(float)i * 1.0f + tunnelOffsetZ);


        // Å©±â Á¶Àı ÈÄ ÀÌµ¿
        Mat4 model = multifly(modelTunnelTranslate, modelTunnelScale);

        // 2. ¼ÎÀÌ´õ¿¡ ÀÌ Å¥ºê Á¶°¢ÀÇ Model Çà·Ä Àü¼Û
        glUniformMatrix4fv(locModel, 1, GL_FALSE, model.m);

        //¼¼±×¸ÕÆ®¸¶´Ù ¹à±â ¹ø°¥¾Æ ÁÖ±â
        float brightness = (i % 2 == 0) ? 0.4f : 1.0f;
        glUniform1f(locBrightness, brightness);

        // 3. Å¥ºêÀÇ 4°³ º®¸é¸¸ ±×¸®±â
        drawCubeFace(2); // ¾Æ·§¸é

        drawCubeFace(4); // ¿ŞÂÊ ¸é
        drawCubeFace(5); // ¿À¸¥ÂÊ ¸é
    }

    // --- Ãß°¡ --- 3·¹ÀÎ ½ºÆ®¸³ ±×¸®±â ---
       // ¹Ù´ÚÀ» 3µîºĞÇÏ´Â ¼¼·Î ¶óÀÎ 2°³ ±×¸®±â
    {
        // ±âÁØ model: ´ÜÀ§Çà·Ä (¶óÀÎ ÀÚÃ¼°¡ ÀÌ¹Ì z·Î ±æ°Ô ¸¸µé¾îÁ® ÀÖÀ½)
        Mat4 model = identity();

        // 1) ¿ŞÂÊ °æ°è¼±
        Mat4 leftLine = model;
        leftLine.m[12] = -0.25f;          // x ÀÌµ¿ => °ªÀÌ ÀÛÀ»¼ö·Ï °¡¿îµ¥ Á¼¾ÆÁü
        glUniformMatrix4fv(locModel, 1, GL_FALSE, leftLine.m);
        drawLaneLine();

        // 2) ¿À¸¥ÂÊ °æ°è¼±
        Mat4 rightLine = model;
        rightLine.m[12] = 0.25f;
        glUniformMatrix4fv(locModel, 1, GL_FALSE, rightLine.m);
        drawLaneLine();
=======
    GLint locModel = glGetUniformLocation(shaderProgramID, "model");


    // --- ?°ë„ ê·¸ë¦¬ê¸?(?ë¸Œ ë²½ë©´ ë°˜ë³µ) ---
    int tunnelSegments = 300; // ?°ë„ ê¸¸ì´
    float tunnelScaleXY = 2.0f; // ?°ë„ ?ˆë¹„/?’ì´ ë°°ìœ¨ (2ë°?

    for (int i = 0; i < tunnelSegments; i++)
    {
        // 1. Model ?‰ë ¬ ê³„ì‚°:

        // ?°ë„ ?¬ê¸° ì¡°ì ˆ (X: 2ë°? Y: 2ë°? Z: 1ë°?
        // Zì¶??¬ê¸°??1.0?¼ë¡œ ? ì??´ì•¼ ?°ë„ ì¡°ê°?¤ì´ ?œë¡œ ë¶™ìŠµ?ˆë‹¤.
        Mat4 modelTunnelScale = scale(tunnelScaleXY, tunnelScaleXY, 1.0f);
        // Zì¶•ìœ¼ë¡??°ë„ ì¡°ê° ë°°ì¹˜
        Mat4 modelTunnelTranslate = translate(0.0f, 0.0f, -(float)i * 1.0f);

        // ?¬ê¸° ì¡°ì ˆ ???´ë™
        Mat4 model = multifly(modelTunnelTranslate, modelTunnelScale);

        // 2. ?°ì´?”ì— ???ë¸Œ ì¡°ê°??Model ?‰ë ¬ ?„ì†¡
        glUniformMatrix4fv(locModel, 1, GL_FALSE, model.m);

        // 3. ?ë¸Œ??4ê°?ë²½ë©´ë§?ê·¸ë¦¬ê¸?
        drawCubeFace(2); // ?„ë«ë©?
        drawCubeFace(3); // ?—ë©´
        drawCubeFace(4); // ?¼ìª½ ë©?
        drawCubeFace(5); // ?¤ë¥¸ìª?ë©?
    }
	drawCoinUI(); // ÄÚÀÎ UI ±×¸®±â

    glutSwapBuffers();
}

//==========================================================
=======
>>>>>>> 733dd3e1c4b60f2cf891ba0eab19553e4d465553
int main(int argc, char** argv)
{

    srand((unsigned)time(NULL));

    //À©µµ¿ì »ı¼ºÇÏ±â
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // ±íÀÌ ¹öÆÛ Ãß°¡
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 800);
    glutCreateWindow("ÄÄ±× ÇÁ·ÎÁ§Æ®");

    //GLEW ÃÊ±âÈ­ÇÏ±â
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";


    //¼¼ÀÌ´õ ÀĞ¾î¿Í¼­ ¼¼ÀÌ´õ ÇÁ·Î±×·¥ ¸¸µé±â 
    make_vertexShaders();
    make_fragmentShaders();


    glewInit();
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.f, 1.f, 1.f, 1.f);
    shaderProgramID = createShaderProgram();

    setupCubeVAOs(); // Å¥ºê ±×¸²
    setupCoinVAO();  // ÄÚÀÎ ¿øÆÇ VAO ¼³Á¤
    initTrains();
	initCoins(); // ÄÚÀÎ ÃÊ±âÈ­


    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
//====================================================
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

    gWidth = w;
    gHeight = h;
}




