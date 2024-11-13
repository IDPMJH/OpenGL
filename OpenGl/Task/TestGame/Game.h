#ifndef GAME_H
#define GAME_H

#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

class Polygon {
public:
    std::vector<GLfloat> vertices;
    float x, y;
    float speedX, speedY;
    bool isActive;
    GLuint vao, vbo;

    Polygon(float startX, float startY, int sides);
    void Update(double deltaTime);
    void Render(GLuint shaderProgram);
    void InitBuffers();
};

class Game {
public:
    Game();
    ~Game();

    HRESULT Initialize();
    void Run();
    void Update();
    void Render();
    void HandleInput();
    void LoadContent();
    void UnloadContent();
    void Exit();

private:
    LARGE_INTEGER _CurCount;
    LARGE_INTEGER _Frequency;
    LARGE_INTEGER _PrevCount;
    double _dDT;
    double _dAcc;
    int _iCallCount;
    int _iFPS;
    std::vector<class::Polygon> polygons;
    GLuint shaderProgram;
    GLuint LoadShader(const char* vertexPath, const char* fragmentPath);
};

#endif // GAME_H
