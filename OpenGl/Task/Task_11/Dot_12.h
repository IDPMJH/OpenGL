#pragma once
#include "Polygon_12.h"
class Dot : public Polygon
{
public:
	Dot();
	Dot(GLfloat f1, GLfloat f2, GLfloat f3);
	~Dot();
public:
	virtual void move_On_dir(char dir) override;
	virtual void Draw_Polygon();
	virtual void init_buffer_polygon(GLuint* vao,GLuint* vbo);
	virtual void Update() {}
	virtual void Change_Color(GLfloat r, GLfloat g, GLfloat b);
	GLfloat* get_vertex_ptr();
	GLfloat* get_color_ptr();
	void set_color_ptr(GLfloat f1, GLfloat f2, GLfloat f3);
public:
	GLfloat _PointShape[3]; //---한 점 색상
	GLfloat _Colors[3]; //--- 한 점 색상
};

