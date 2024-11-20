#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

float Random_0_to_1f();

using std::vector;

#define MAX_LINE_LENGTH 128

using glm::mat4;
using glm::radians;
using glm::vec2;
using glm::vec3;
using glm::vec4;

class Vertex {
public:
    float x, y, z;
    float r, g, b;
};

class Face {
public:
    unsigned int v1, v2, v3;
};


enum Model_Type 
{
	Type_non,
	Type_cube,
	Type_d4,
	Type_pyramid,
	Type_end,
};


class Model{
public:
   vector<Vertex> vertices;
   vector<Face> faces;
   vector<float> _vertices;
   vector<unsigned int> _indices;
   unsigned int _face_on_1 = 10;
   unsigned int _face_on_2 = 10;
   Model_Type _type;
   mat4 _trs;
   mat4 _rot;
   mat4 _scale;
   mat4 _FT;
   

  

public:
    Model() 
        : _type(Type_non)
        , _trs(mat4(1.f))
        , _rot(mat4(1.f))
        , _scale(mat4(1.f))
        , _FT(mat4(1.f))
    {
    };
    Model(Model_Type type)
        : _type(type)
        , _trs(mat4(1.f))
        , _rot(mat4(1.f))
        , _scale(mat4(1.f))
        , _FT(mat4(1.f))
    {
    };
    ~Model() {};

   virtual void model_init_buffer(GLuint& VBO,GLuint& VAO, GLuint& EBO)
    {
       // exception handler
       if (_type == Type_non) {
           std::cerr << "Error: model type is not set" << std::endl;
           exit(EXIT_FAILURE);
       }

		_vertices.clear();
		_indices.clear();

		for (const auto& vertex : vertices) {
			_vertices.push_back(vertex.x);
			_vertices.push_back(vertex.y);
			_vertices.push_back(vertex.z);
			_vertices.push_back(vertex.r);
			_vertices.push_back(vertex.g);
			_vertices.push_back(vertex.b);
		}

        for (const auto& face : faces) {
            _indices.push_back(face.v1);
            _indices.push_back(face.v2);
            _indices.push_back(face.v3);
        }

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// 색상 버퍼
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
    }
};
extern bool front_open;
class Cube : public Model
{
public:
    enum Cube_FaceType {
      
        back = 0,
        left,
        top,
        right,
        bottom,
        front,
    };
    vector<vector<unsigned int>> _face_indices; // 6면의 인덱스를 저장하는 벡터
    GLuint VAOs[6], VBOs[6], EBOs[6];


    // Task_17 전용
    mat4 _Top_mat;
    mat4 _Front_mat;
    mat4 _Side_mat;
    mat4 _Back_mat;
    float _angle = 0.f;

public:
    Cube()
        : Model(Type_cube)
        , _Top_mat(mat4(1.f))
        , _Front_mat(mat4(1.f))
        , _Side_mat(mat4(1.f))
        , _Back_mat(mat4(1.f))
        , _angle(0.f)
    {
        _face_indices.resize(6);
        glGenVertexArrays(6, VAOs);
        glGenBuffers(6, VBOs);
        glGenBuffers(6, EBOs);
    }
    void init_matrix()
    {
        _Top_mat = mat4(1.f);
        _Front_mat = mat4(1.f);
        _Side_mat = mat4(1.f);
        _Back_mat = mat4(1.f);
    }
    void top_revolving()
    {
        _angle += 1.f;
        if (_angle >= 360.f)
            _angle = 0.f;

        mat4 trs = glm::translate(mat4(1.f), vec3(0.f, -0.5f, 0.f));
        mat4 rot = glm::rotate(mat4(1.f), radians(_angle), vec3(1.f, 0.f, 0.f));
        mat4 trs2 = glm::translate(mat4(1.f), vec3(0.f, 0.5f, 0.f));

        _Top_mat = trs2 * rot * trs;
    }
    void front_opening()
    {
 
        if (_angle <= 90.f)
            _angle += 1.f;
        
     

        mat4 trs = glm::translate(mat4(1.f), vec3(0.f, 0.5f, -0.5f));
        mat4 rot = glm::rotate(mat4(1.f), radians(_angle), vec3(1.f, 0.f, 0.f));
        mat4 trs2 = glm::translate(mat4(1.f), vec3(0.f, -0.5f, 0.5f));

        _Front_mat = trs2 * rot * trs;
    }
    void front_closing()
    {

        if (_angle >= 0.f)
            _angle -= 1.f;
        


        mat4 trs = glm::translate(mat4(1.f), vec3(0.f, 0.5f, -0.5f));
        mat4 rot = glm::rotate(mat4(1.f), radians(_angle), vec3(1.f, 0.f, 0.f));
        mat4 trs2 = glm::translate(mat4(1.f), vec3(0.f, -0.5f, 0.5f));

        _Front_mat = trs2 * rot * trs;
    }

    virtual void model_init_buffer(GLuint& VBO, GLuint& VAO, GLuint& EBO)
    {

        // exception handling
        if (_face_indices.size() != 6) {
            std::cerr << "Error: face indices size is not 6" << std::endl;
            exit(EXIT_FAILURE);
        }

        static int face_count = 0;
        _vertices.clear();
        _indices.clear();

        for (auto a : _face_indices)
            a.clear();

        int j = 0;
        size_t i = 0;
        // 면 별로 정보 저장
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
        // 정점 정보를 저장 ======아마 해당 면의 정점만 있어도 될라나
        for (const auto& vertex : vertices) {
            _vertices.push_back(vertex.x);
            _vertices.push_back(vertex.y);
            _vertices.push_back(vertex.z);
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

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // 색상 버퍼
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
    }
};






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
        std::string str(line);
        read_newline(str);
        if (str.substr(0, 2) == "v ") {
            Vertex vertex;
            std::istringstream s(str.substr(2));
            s >> vertex.x >> vertex.y >> vertex.z;
			vertex.r = Random_0_to_1f(); // 0.5f;  // ---------------색상 설정하는 부분------------------
            vertex.g = Random_0_to_1f(); //0.5f;
            vertex.b = Random_0_to_1f(); //1.0f;

            model.vertices.push_back(vertex);
        }
        else if (str.substr(0, 2) == "f ") {
            Face face;
            unsigned int v1, v2, v3;
            int matches = sscanf_s(str.c_str(), "f %u//%*u %u//%*u %u//%*u", &v1, &v2, &v3);
            if (matches == 3) {
                face.v1 = v1 - 1; // OBJ indices start at 1
                face.v2 = v2 - 1;
                face.v3 = v3 - 1;
                model.faces.push_back(face);
            }
            else {
                std::cerr << "Error parsing face: " << str << std::endl;
            }

            // 디버깅 출력
           // std::cout << "Face: " << face.v1 << ", " << face.v2 << ", " << face.v3 << std::endl;
        }
    }
}



