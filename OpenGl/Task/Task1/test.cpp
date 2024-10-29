#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#define MAX_LINE_LENGTH 128

struct Vertex {
    float x, y, z;
};

struct Face {
    unsigned int v1, v2, v3;
};

struct Model {
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
};

Model __model;

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
            model.vertices.push_back(vertex);
        }
        else if (str.substr(0, 2) == "f ") {
            Face face;
            std::istringstream s(str.substr(2));
            s >> face.v1 >> face.v2 >> face.v3;
            face.v1--; // OBJ indices start at 1
            face.v2--;
            face.v3--;
            model.faces.push_back(face);
        }
    }
}

std::string read_shader_source(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening shader file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint compile_shader(const std::string& source, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error compiling shader: " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    return shader;
}

GLuint create_shader_program(const std::string& vertexSource, const std::string& fragmentSource) {
    GLuint vertexShader = compile_shader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compile_shader(fragmentSource, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Error linking shader program: " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

GLuint VBO, VAO, EBO;
GLuint shaderProgram;

void setup_buffers() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (const auto& vertex : __model.vertices) {
        vertices.push_back(vertex.x);
        vertices.push_back(vertex.y);
        vertices.push_back(vertex.z);
    }

    for (const auto& face : __model.faces) {
        indices.push_back(face.v1);
        indices.push_back(face.v2);
        indices.push_back(face.v3);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "ModelMatrix");
  
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, __model.faces.size() * 3, GL_UNSIGNED_INT, 0);

    glutSwapBuffers();
}

void initOpenGL() {
    glewInit();
    glEnable(GL_DEPTH_TEST);

    std::string vertexSource = read_shader_source("vertex.glsl");
    std::string fragmentSource = read_shader_source("fragment.glsl");
    shaderProgram = create_shader_program(vertexSource, fragmentSource);

    setup_buffers();
}

int main(int argc, char** argv) {
    read_obj_file("cube.obj", __model);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OBJ Model Viewer");

    initOpenGL();

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
