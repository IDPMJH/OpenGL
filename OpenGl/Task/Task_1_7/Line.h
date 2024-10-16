#pragma once
#include "Polygon.h"
class Line : public Polygon
{
public:
	Line();
	Line(GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4, GLfloat f5, GLfloat f6);
	~Line();
public:
	virtual void move_On_dir(char dir);
	virtual void Draw_Polygon();
	virtual void init_buffer_polygon(GLuint* vao, GLuint* vbo);
	virtual void Update();
	GLfloat* get_vertex_ptr();
	GLfloat* get_color_ptr();

public:
	GLfloat _LineShape[2][3];
	GLfloat _Colors[2][3]; //--- 두 꼭지점 색상
};

