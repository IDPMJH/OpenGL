#pragma once
#include <vector>

class Face;
class Vertex;
class Model;
class Cube;


class Crane
{
public:
    Crane();
    ~Crane();


public:
    void Draw(GLuint shaderProgramID);
    void init_buffer();
public:
    Cube body;
    Cube frontLeg1;
    Cube frontLeg2;
    Cube head;
    Cube cannon1;
    Cube cannon2;
};

