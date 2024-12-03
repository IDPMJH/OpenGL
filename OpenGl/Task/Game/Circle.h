#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Circle {
public:
    Circle(float radius, int segments, GLuint shaderProgramID);
    ~Circle();

    void setColor(const glm::vec3& color);
    void draw() const;

private:
    void initBuffer();

    float _radius;
    int _segments;
    GLuint _shaderProgramID;
    GLuint _VAO, _VBO;
    glm::vec3 _color;
    std::vector<glm::vec3> _vertices;
    std::vector<glm::vec3> _colors;
};