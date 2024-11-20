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

using std::vector;

float Random_0_to_1f();

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
public:
    void model_init_buffer(GLuint& VBO,GLuint& VAO, GLuint& EBO)
    {

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


		// ¸ðµç¸é
		/*for (const auto& face : pmodel_1->faces) {
			indices.push_back(face.v1);
			indices.push_back(face.v2);
			indices.push_back(face.v3);
		}*/

		if(_type == Type_cube)
		{
			if (_face_on_1 == 1)
			{
				// µÞ¸é
				_indices.push_back(faces[0].v1);
				_indices.push_back(faces[0].v2);
				_indices.push_back(faces[0].v3);
				_indices.push_back(faces[1].v1);
				_indices.push_back(faces[1].v2);
				_indices.push_back(faces[1].v3);
			}
			else if (_face_on_1 == 2)
			{
				// ¿ÞÂÊ ¿·¸é
				_indices.push_back(faces[2].v1);
				_indices.push_back(faces[2].v2);
				_indices.push_back(faces[2].v3);
				_indices.push_back(faces[3].v1);
				_indices.push_back(faces[3].v2);
				_indices.push_back(faces[3].v3);
			}
			else if (_face_on_1 == 3)
			{
				// À­¸é
				_indices.push_back(faces[4].v1);
				_indices.push_back(faces[4].v2);
				_indices.push_back(faces[4].v3);
				_indices.push_back(faces[5].v1);
				_indices.push_back(faces[5].v2);
				_indices.push_back(faces[5].v3);
			}
			else if (_face_on_1 == 4)
			{

				// ¿À¸¥ÂÊ ¿·¸é
				_indices.push_back(faces[6].v1);
				_indices.push_back(faces[6].v2);
				_indices.push_back(faces[6].v3);
				_indices.push_back(faces[7].v1);
				_indices.push_back(faces[7].v2);
				_indices.push_back(faces[7].v3);
			}
			else if (_face_on_1 == 5)
			{
				// ¾Æ·§¸é
				_indices.push_back(faces[8].v1);
				_indices.push_back(faces[8].v2);
				_indices.push_back(faces[8].v3);
				_indices.push_back(faces[9].v1);
				_indices.push_back(faces[9].v2);
				_indices.push_back(faces[9].v3);
			}
			else if (_face_on_1 == 6)
			{
				// ¾Õ¸é
				_indices.push_back(faces[10].v1);
				_indices.push_back(faces[10].v2);
				_indices.push_back(faces[10].v3);
				_indices.push_back(faces[11].v1);
				_indices.push_back(faces[11].v2);
				_indices.push_back(faces[11].v3);
			}



			if (_face_on_2 == 1)
			{
				// µÞ¸é
				_indices.push_back(faces[0].v1);
				_indices.push_back(faces[0].v2);
				_indices.push_back(faces[0].v3);
				_indices.push_back(faces[1].v1);
				_indices.push_back(faces[1].v2);
				_indices.push_back(faces[1].v3);
			}
			else if (_face_on_2 == 2)
			{
				// ¿ÞÂÊ ¿·¸é
				_indices.push_back(faces[2].v1);
				_indices.push_back(faces[2].v2);
				_indices.push_back(faces[2].v3);
				_indices.push_back(faces[3].v1);
				_indices.push_back(faces[3].v2);
				_indices.push_back(faces[3].v3);
			}
			else if (_face_on_2 == 3)
			{
				// À­¸é
				_indices.push_back(faces[4].v1);
				_indices.push_back(faces[4].v2);
				_indices.push_back(faces[4].v3);
				_indices.push_back(faces[5].v1);
				_indices.push_back(faces[5].v2);
				_indices.push_back(faces[5].v3);
			}
			else if (_face_on_2 == 4)
			{

				// ¿À¸¥ÂÊ ¿·¸é
				_indices.push_back(faces[6].v1);
				_indices.push_back(faces[6].v2);
				_indices.push_back(faces[6].v3);
				_indices.push_back(faces[7].v1);
				_indices.push_back(faces[7].v2);
				_indices.push_back(faces[7].v3);
			}
			else if (_face_on_2 == 5)
			{
				// ¾Æ·§¸é
				_indices.push_back(faces[8].v1);
				_indices.push_back(faces[8].v2);
				_indices.push_back(faces[8].v3);
				_indices.push_back(faces[9].v1);
				_indices.push_back(faces[9].v2);
				_indices.push_back(faces[9].v3);
			}
			else if (_face_on_2 == 6)
			{
				// ¾Õ¸é
				_indices.push_back(faces[10].v1);
				_indices.push_back(faces[10].v2);
				_indices.push_back(faces[10].v3);
				_indices.push_back(faces[11].v1);
				_indices.push_back(faces[11].v2);
				_indices.push_back(faces[11].v3);
			}
		}

		else if (_type == Type_d4)
		{
			if (_face_on_1 == 7)
			{
				_indices.push_back(faces[0].v1);
				_indices.push_back(faces[0].v2);
				_indices.push_back(faces[0].v3);
				
			}
			else if (_face_on_1 == 8)
			{
				_indices.push_back(faces[1].v1);
				_indices.push_back(faces[1].v2);
				_indices.push_back(faces[1].v3);
			
			}
			else if (_face_on_1 == 9)
			{
				_indices.push_back(faces[2].v1);
				_indices.push_back(faces[2].v2);
				_indices.push_back(faces[2].v3);
			}
			else if (_face_on_1 == 0)
			{

				_indices.push_back(faces[3].v1);
				_indices.push_back(faces[3].v2);
				_indices.push_back(faces[3].v3);
			}
			

			if (_face_on_2 == 7)
			{
				_indices.push_back(faces[0].v1);
				_indices.push_back(faces[0].v2);
				_indices.push_back(faces[0].v3);
			}
			else if (_face_on_2 == 8)
			{
				_indices.push_back(faces[1].v1);
				_indices.push_back(faces[1].v2);
				_indices.push_back(faces[1].v3);
			}
			else if (_face_on_2 == 9)
			{
				_indices.push_back(faces[2].v1);
				_indices.push_back(faces[2].v2);
				_indices.push_back(faces[2].v3);
			}
			else if (_face_on_2 == 0)
			{
				_indices.push_back(faces[3].v1);
				_indices.push_back(faces[3].v2);
				_indices.push_back(faces[3].v3);
			}
		}

	/*	glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);*/

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);


		// »ö»ó ¹öÆÛ
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
    }
};

Model model;

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
            vertex.r = Random_0_to_1f(); //0.5f; // ---------------»ö»ó ¼³Á¤ÇÏ´Â ºÎºÐ------------------
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

            // µð¹ö±ë Ãâ·Â
            std::cout << "Face: " << face.v1 << ", " << face.v2 << ", " << face.v3 << std::endl;
        }
    }
}



