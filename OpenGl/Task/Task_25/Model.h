#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using std::vector;
using glm::mat4;
using glm::radians;
using glm::vec2;
using glm::vec3;
using glm::vec4;

class string;

#define MAX_LINE_LENGTH 128

class Vertex {
public:
    float x, y, z;
    float nx, ny, nz;
    float r, g, b;
};

class Face {
public:
    unsigned int v1, v2, v3;
    unsigned int n1, n2, n3;
};

enum Model_Type {
    Type_non,
    Type_cube,
    Type_d4,
    Type_pyramid,
    Type_end,
};

class Model {
public:
    vector<Vertex> vertices;
    vector<Face> faces;
    vector<vec3> normals;
    vector<float> _vertices;
    vector<unsigned int> _indices;
    unsigned int _face_on_1;
    unsigned int _face_on_2;
    Model_Type _type;
    mat4 _trs;
    mat4 _rot;
    mat4 _scale;
    mat4 _FT;
    GLuint VAOs[6], VBOs[6], EBOs[6];

public:
    Model();
    Model(Model_Type type);
    virtual ~Model();

    virtual void model_init_buffer() = 0;
};

extern bool front_open;

class Cube : public Model {
public:
    enum Cube_FaceType {
        back = 0,
        left,
        top,
        right,
        bottom,
        front,
    };

public:
    Cube();

public:
    void init_matrix();
    void top_revolving();
    void front_opening();
    void front_closing();
    virtual void model_init_buffer () override;
    void Draw(GLuint shaderProgramID);

public:
    vector<vector<unsigned int>> _face_indices;
   
    mat4 _Top_mat;
    mat4 _Front_mat;
    mat4 _Side_mat;
    mat4 _Back_mat;
    float _angle;
};

// Pyramid 클래스 추가
class Pyramid : public Model {
public:
    Pyramid();

public:
    virtual void model_init_buffer() override;
    void Draw(GLuint shaderProgramID);
public:
    vector<vector<unsigned int>> _face_indices;
};

void read_newline(std::string& str);
void read_obj_file(const std::string& filename, Model& model);