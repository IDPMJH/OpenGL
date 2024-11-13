#pragma once
#include "Polygon.h"

class Rect_p : public Polygon
{
public:
	Rect_p();
	~Rect_p();
	Rect_p(float xpos, float ypos, float offset);
public:
	virtual void move_On_dir(char dir);
	virtual void Draw_Polygon();
	virtual void init_buffer_polygon(GLuint* vao, GLuint* vbo);
	virtual void Update() {}
	GLfloat* get_vertex_ptr();
	GLfloat* get_color_ptr();

public:
	GLfloat _RectShape[6][3];//	두 개의 삼각형들 좌표
	GLfloat _Colors[6][3]; //--- 두 삼각형 꼭지점 색상
};

