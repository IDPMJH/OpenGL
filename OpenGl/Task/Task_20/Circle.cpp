#include "Circle.h"
#include <glm/gtc/type_ptr.hpp>

Circle::Circle(float radius, int segments, GLuint shaderProgramID)
    : _radius(radius), _segments(segments), _shaderProgramID(shaderProgramID), _color(1.0f, 1.0f, 1.0f) {
    initBuffer();
}

Circle::~Circle() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}

void Circle::setColor(const glm::vec3& color) {
    _color = color;
    _colors.clear();
    for (int i = 0; i <= _segments; ++i) {
        _colors.push_back(_color);
    }

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferSubData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), _colors.size() * sizeof(glm::vec3), &_colors[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Circle::initBuffer() {
    for (int i = 0; i <= _segments; ++i) {
        float angle = 2.0f * glm::pi<float>() * float(i) / float(_segments);
        float x = _radius * cos(angle);
        float y = _radius * sin(angle);
        _vertices.push_back(glm::vec3(x, y, 0.0f));
        _colors.push_back(_color);
    }

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, (_vertices.size() + _colors.size()) * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size() * sizeof(glm::vec3), &_vertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), _colors.size() * sizeof(glm::vec3), &_colors[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(_vertices.size() * sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Circle::draw() const {
    glUseProgram(_shaderProgramID);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_LINE_LOOP, 0, _vertices.size());
    glBindVertexArray(0);
}