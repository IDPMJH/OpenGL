#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Circle;

using glm::mat4;

class GLU_Object {
public:
    GLU_Object();
    ~GLU_Object();

   
    Circle* getCircle() { return _circle; }

    void setTranslation(const glm::vec3& translation);
    mat4 getTranslation() { return _trs; }

    void setRotation(float angle, const glm::vec3& axis);
    mat4 getRotation() { return _rot; }

    void setScale(const glm::vec3& scale);
    mat4 getScale() { return _scale; }

    void setFT();
    glm::mat4 getFT() { return _FT; }
    void draw(GLenum drawStyle) const;

    void setCenter(const glm::vec3& center);
    glm::vec4 getCenter() { return _center; }

private:
    GLUquadricObj* _qobj;
    Circle* _circle;
    glm::mat4 _trs;
    glm::mat4 _rot;
    glm::mat4 _scale;
    glm::mat4 _FT;
    glm::vec4 _center;
};
