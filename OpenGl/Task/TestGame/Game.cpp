#include <Windows.h>
#include <timeapi.h>
#include "Game.h"
#include "global.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>



class Polygon;

Game::Game() 
    : _CurCount{}
    , _Frequency{}
    , _PrevCount{}
    , _dDT(0.0)
    , _dAcc(0.0)
    , _iCallCount(0)
    , _iFPS(0)
{
}

Game::~Game() {
    // Destructor implementation
}

HRESULT Game::Initialize() {
    // Initialize method implementation
    // 프레임 제어
    // 현재 카운트
    QueryPerformanceCounter(&_CurCount);
    // 초당 카운트 횟수 (초당 카운트 차이 값)
    QueryPerformanceFrequency(&_Frequency);

    // FreeGLUT 초기화
    int argc = 1;
    char* argv[1] = { (char*)"Something" };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("2D Game");

    glewInit();

    shaderProgram = LoadShader("vertex_shader.glsl", "fragment_shader.glsl");

    glutDisplayFunc([]() { instance->Render(); });
    glutIdleFunc([]() { instance->Update(); });

    return S_OK;
}

void Game::Run() {
    glutMainLoop();
}

void Game::Update() {
    // 시간 갱신
    // 현재 카운트 값
    QueryPerformanceCounter(&_CurCount);

    // 이전 프레임의 카운팅과 현재 프레임 카운팅 값의 차이를 구한다. 이를 초당 카운트 횟수로 나눈다 -> 프레임카운트당 시간이 나온다
    _dDT = (double)(_CurCount.QuadPart - _PrevCount.QuadPart) / (double)(_Frequency.QuadPart);
    
    // 이전 프레임카운트 값 현재 프레임카운트값으로 갱신 (다음 계산을 위해)
    _PrevCount = _CurCount;

    HandleInput();

    for (auto& polygon : polygons) {
        polygon.Update(_dDT);
        // 충돌 감지
        if (true/* 충돌 조건 */) {
            polygon.isActive = false;
        }
    }

    // 비활성화된 다각형 제거
   // polygons.erase(std::remove_if(polygons.begin(), polygons.end(), [](class::Polygon& p) { return !p.isActive; }), polygons.end());
    glutPostRedisplay();
  
}

void Game::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    for (auto& polygon : polygons) {
        polygon.Render(shaderProgram);
    }
    glUseProgram(0);

    glutSwapBuffers();
}

void Game::HandleInput() {
    // 마우스 입력 처리 예제
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        // 마우스 클릭 처리
    }
}

void Game::LoadContent() {
    // LoadContent method implementation
}

void Game::UnloadContent() {
    // UnloadContent method implementation
}

void Game::Exit() {
    // Exit method implementation
}

GLuint Game::LoadShader(const char* vertexPath, const char* fragmentPath) {
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);

    std::stringstream vertexStream, fragmentStream;
    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();

    std::string vertexCode = vertexStream.str();
    std::string fragmentCode = fragmentStream.str();

    const char* vertexShaderCode = vertexCode.c_str();
    const char* fragmentShaderCode = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

Polygon::Polygon(float startX, float startY, int sides) 
    : x(startX), y(startY), speedX(0), speedY(0), isActive(true) {
    // 다각형 생성
    float angle = 2 * 3.14159265358979323846f / sides;
    for (int i = 0; i < sides; ++i) {
        vertices.push_back(cos(i * angle));
        vertices.push_back(sin(i * angle));
    }
    InitBuffers();
}

void Polygon::InitBuffers() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Polygon::Update(double deltaTime) {
    x += speedX * deltaTime;
    y += speedY * deltaTime;
    // 화면 밖으로 나가면 비활성화
    if (y > 600) {
        isActive = false;
    }
}

void Polygon::Render(GLuint shaderProgram) {
    if (isActive) {
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_POLYGON, 0, vertices.size() / 2);
        glBindVertexArray(0);
        glUseProgram(0);
    }
}
