#define _CRT_SECURE_NO_WARNINGS
#include "Model.h"`
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <random>

float Random_0_to_1f_model() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}

Model::Model()
    : _face_on_1(10)
    , _face_on_2(10)
    , _type(Model_Type::Type_non)
    , _trs(mat4(1.f))
    , _rot(mat4(1.f))
    , _scale(mat4(1.f))
    , _FT(mat4(1.f))
{
    glGenVertexArrays(6, VAOs);
    glGenBuffers(6, VBOs);
    glGenBuffers(6, EBOs);
}

Model::Model(Model_Type type)
    : _face_on_1(10)
    , _face_on_2(10)
    , _type(type)
    , _trs(mat4(1.f))
    , _rot(mat4(1.f))
    , _scale(mat4(1.f))
    , _FT(mat4(1.f))
{
    glGenVertexArrays(6, VAOs);
    glGenBuffers(6, VBOs);
    glGenBuffers(6, EBOs);
}

Model::~Model() {}


Cube::Cube()
    : Model(Model_Type::Type_cube)
    , _Top_mat(mat4(1.f))
    , _Front_mat(mat4(1.f))
    , _Side_mat(mat4(1.f))
    , _Back_mat(mat4(1.f))
    , _angle(0.f)
{
    glGenVertexArrays(6, VAOs);
    glGenBuffers(6, VBOs);
    glGenBuffers(6, EBOs);
    _face_indices.resize(6);
  
}

void Cube::init_matrix()
{
    _Top_mat = mat4(1.f);
    _Front_mat = mat4(1.f);
    _Side_mat = mat4(1.f);
    _Back_mat = mat4(1.f);
}

void Cube::top_revolving()
{
    _angle += 1.f;
    if (_angle >= 360.f)
        _angle = 0.f;

    mat4 trs = glm::translate(mat4(1.f), vec3(0.f, -0.5f, 0.f));
    mat4 rot = glm::rotate(mat4(1.f), radians(_angle), vec3(1.f, 0.f, 0.f));
    mat4 trs2 = glm::translate(mat4(1.f), vec3(0.f, 0.5f, 0.f));

    _Top_mat = trs2 * rot * trs;
}

void Cube::front_opening()
{
    if (_angle <= 90.f)
        _angle += 1.f;

    mat4 trs = glm::translate(mat4(1.f), vec3(0.f, 0.5f, -0.5f));
    mat4 rot = glm::rotate(mat4(1.f), radians(_angle), vec3(1.f, 0.f, 0.f));
    mat4 trs2 = glm::translate(mat4(1.f), vec3(0.f, -0.5f, 0.5f));

    _Front_mat = trs2 * rot * trs;
}

void Cube::front_closing()
{
    if (_angle >= 0.f)
        _angle -= 1.f;

    mat4 trs = glm::translate(mat4(1.f), vec3(0.f, 0.5f, -0.5f));
    mat4 rot = glm::rotate(mat4(1.f), radians(_angle), vec3(1.f, 0.f, 0.f));
    mat4 trs2 = glm::translate(mat4(1.f), vec3(0.f, -0.5f, 0.5f));

    _Front_mat = trs2 * rot * trs;
}

void Cube::model_init_buffer()
{
    if (_face_indices.size() != 6) {
        std::cerr << "Error: face indices size is not 6" << std::endl;
        exit(EXIT_FAILURE);
    }

    _vertices.clear();
    _indices.clear();

    for (auto& a : _face_indices)
        a.clear();

    int j = 0;
    size_t i = 0;
    for (i = 0; i < 6; i++) 
    {
        _face_indices[i].push_back(faces[j].v1);
        _face_indices[i].push_back(faces[j].v2);
        _face_indices[i].push_back(faces[j].v3);
        _face_indices[i].push_back(faces[++j].v1);
        _face_indices[i].push_back(faces[j].v2);
        _face_indices[i].push_back(faces[j].v3);
        ++j;
    }

    for (const auto& vertex : vertices) 
    {
        _vertices.push_back(vertex.x);
        _vertices.push_back(vertex.y);
        _vertices.push_back(vertex.z);
        _vertices.push_back(vertex.nx); // 노말 벡터 추가
        _vertices.push_back(vertex.ny); // 노말 벡터 추가
        _vertices.push_back(vertex.nz); // 노말 벡터 추가
        _vertices.push_back(vertex.r);
        _vertices.push_back(vertex.g);
        _vertices.push_back(vertex.b);
    }

    for (i = 0; i < _face_indices.size(); ++i) 
    {
        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _face_indices[i].size() * sizeof(unsigned int), _face_indices[i].data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void Cube::Draw(GLuint shaderProgramID)
{
    glUseProgram(shaderProgramID);


    for (int i = 0; i < _face_indices.size(); ++i) 
    {
        glBindVertexArray(VAOs[i]);

        glDrawElements(GL_TRIANGLES, _face_indices[i].size(), GL_UNSIGNED_INT, 0);
    }
}

Pyramid::Pyramid() : Model(Type_pyramid) {
    // 피라미드 모델 초기화
    read_obj_file("pyramid.obj", *this);
    _face_indices.resize(5);
    glGenVertexArrays(5, VAOs);
    glGenBuffers(5, VBOs);
    glGenBuffers(5, EBOs);
}

void Pyramid::model_init_buffer() {
    _vertices.clear();
    _indices.clear();

    int j = 0;
    size_t i = 0;
    for (auto& a : _face_indices)
        a.clear();
     for (i = 0; i < 5; i++) 
    {
        _face_indices[i].push_back(faces[j].v1);
        _face_indices[i].push_back(faces[j].v2);
        _face_indices[i].push_back(faces[j].v3);
        if (i == 4)
        {
            _face_indices[i].push_back(faces[++j].v1);
            _face_indices[i].push_back(faces[j].v2);
            _face_indices[i].push_back(faces[j].v3);
        }
        else
            ++j;
    }

    // 각 면의 정점 데이터를 _vertices에 저장
    for (const auto& vertex : vertices) {
        _vertices.push_back(vertex.x);
        _vertices.push_back(vertex.y);
        _vertices.push_back(vertex.z);
        _vertices.push_back(vertex.nx); // 노말 벡터 추가
        _vertices.push_back(vertex.ny); // 노말 벡터 추가
        _vertices.push_back(vertex.nz); // 노말 벡터 추가
        _vertices.push_back(vertex.r);
        _vertices.push_back(vertex.g);
        _vertices.push_back(vertex.b);
    }


    // 각 면에 대해 별도의 VAO, VBO, EBO를 바인딩하고 데이터를 버퍼에 올림
    for (i = 0; i < _face_indices.size(); ++i)
    {
        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _face_indices[i].size() * sizeof(unsigned int), _face_indices[i].data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}


void Pyramid::Draw(GLuint shaderProgramID) {
   
    glUseProgram(shaderProgramID);

    for (int i = 0; i < _face_indices.size(); ++i)
    {
        glBindVertexArray(VAOs[i]);
        glDrawElements(GL_TRIANGLES, _face_indices[i].size(), GL_UNSIGNED_INT, 0);
    }
}
  

void read_newline(std::string& str) {
    if (!str.empty() && str.back() == '\n') {
        str.pop_back();
    }
}

void read_obj_file(const std::string& filename, Model& model) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (file.getline(line, MAX_LINE_LENGTH)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            Vertex vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            vertex.r = Random_0_to_1f_model();
            vertex.g = Random_0_to_1f_model();
            vertex.b = Random_0_to_1f_model();
            model.vertices.push_back(vertex);
        }
        else if (prefix == "vn") {
            vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            model.normals.push_back(normal);
        }
        else if (prefix == "f") {
            Face face;
            char slash;
            ss >> face.v1 >> slash >> slash >> face.n1
               >> face.v2 >> slash >> slash >> face.n2
               >> face.v3 >> slash >> slash >> face.n3;

            // 인덱스를 1 감소시켜 0부터 시작하도록 수정
            face.v1 -= 1;
            face.v2 -= 1;
            face.v3 -= 1;
            face.n1 -= 1;
            face.n2 -= 1;
            face.n3 -= 1;
            model.faces.push_back(face);
        }
    }


    // 각 정점에 대해 노멀 벡터의 평균을 계산하여 저장
    std::vector<std::vector<vec3>> vertex_normals(model.vertices.size());
    for (const auto& face : model.faces) {
        vertex_normals[face.v1].push_back(model.normals[face.n1]);
        vertex_normals[face.v2].push_back(model.normals[face.n2]);
        vertex_normals[face.v3].push_back(model.normals[face.n3]);
    }

    for (size_t i = 0; i < model.vertices.size(); ++i) {
        vec3 avg_normal(0.0f, 0.0f, 0.0f);
        for (const auto& normal : vertex_normals[i]) {
            avg_normal.x += normal.x;
            avg_normal.y += normal.y;
            avg_normal.z += normal.z;
        }
        avg_normal.x /= vertex_normals[i].size();
        avg_normal.y /= vertex_normals[i].size();
        avg_normal.z /= vertex_normals[i].size();

        model.vertices[i].nx = avg_normal.x;
        model.vertices[i].ny = avg_normal.y;
        model.vertices[i].nz = avg_normal.z;
    }
}