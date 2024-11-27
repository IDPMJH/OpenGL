#include "GLU_Object.h"
#include "Circle.h"

extern GLuint shaderProgramID;

GLU_Object::GLU_Object()
    : _trs(glm::mat4(1.0f))
    , _rot(glm::mat4(1.0f))
    , _scale(glm::mat4(1.0f))
    , _FT(glm::mat4(1.0f))
    , _qobj(gluNewQuadric())
    , _circle(new Circle(0.5, 32, shaderProgramID))
    , _center(0.0f, 0.0f, 0.0f, 1.0f)
{

}

GLU_Object::~GLU_Object() {
    gluDeleteQuadric(_qobj);
}

void GLU_Object::setTranslation(const glm::vec3& translation) {
    _trs = glm::translate(glm::mat4(1.0f), translation);
    _center = _trs * _center;       // 중심 좌표에 변환 적용
}

void GLU_Object::setRotation(float angle, const glm::vec3& axis) {
    _rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    _center = _rot * _center;       // 중심 좌표에 변환 적용
}

void GLU_Object::setScale(const glm::vec3& scale) 
{
    _scale = glm::scale(glm::mat4(1.0f), scale);
    _center = _scale * _center;     // 중심 좌표에 변환 적용
}

void GLU_Object::setCenter(const glm::vec3& center) 
{
    _center = glm::vec4(center, 1.0f);
}

void GLU_Object::setFT()
{
    _FT = _trs * _rot * _scale;
}

void GLU_Object::draw(GLenum drawStyle) const {
    
    glColor3f(0.0, 0.0, 1.0);
    // 조명 속성 설정
    GLfloat material_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat material_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat material_shininess[] = { 50.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    gluQuadricNormals(_qobj, GLU_SMOOTH);
    gluQuadricDrawStyle(_qobj, drawStyle);
    gluSphere(_qobj, 0.3, 32, 32); // 예시로 구를 그립니다.
    glFlush();
}