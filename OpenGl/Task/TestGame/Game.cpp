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
    // ������ ����
    // ���� ī��Ʈ
    QueryPerformanceCounter(&_CurCount);
    // �ʴ� ī��Ʈ Ƚ�� (�ʴ� ī��Ʈ ���� ��)
    QueryPerformanceFrequency(&_Frequency);

    // FreeGLUT �ʱ�ȭ
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
    // �ð� ����
    // ���� ī��Ʈ ��
    QueryPerformanceCounter(&_CurCount);

    // ���� �������� ī���ð� ���� ������ ī���� ���� ���̸� ���Ѵ�. �̸� �ʴ� ī��Ʈ Ƚ���� ������ -> ������ī��Ʈ�� �ð��� ���´�
    _dDT = (double)(_CurCount.QuadPart - _PrevCount.QuadPart) / (double)(_Frequency.QuadPart);
    
    // ���� ������ī��Ʈ �� ���� ������ī��Ʈ������ ���� (���� ����� ����)
    _PrevCount = _CurCount;

    HandleInput();

    for (auto& polygon : polygons) {
        polygon.Update(_dDT);
        // �浹 ����
        if (true/* �浹 ���� */) {
            polygon.isActive = false;
        }
    }

    // ��Ȱ��ȭ�� �ٰ��� ����
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
    // ���콺 �Է� ó�� ����
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        // ���콺 Ŭ�� ó��
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
    // �ٰ��� ����
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
    // ȭ�� ������ ������ ��Ȱ��ȭ
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
