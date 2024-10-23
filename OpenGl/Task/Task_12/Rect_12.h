#pragma once

#include "Polygon_12.h"



class Rect_p : public Polygon
{
public:
	Rect_p();
	Rect_p(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3);
	~Rect_p();
	Rect_p(float xpos, float ypos, float offset);
public:
	virtual void move_On_dir(char dir);
	virtual void Draw_Polygon();
	virtual void init_buffer_polygon(GLuint* vao, GLuint* vbo);
	virtual void Update();
	virtual void Change_Color(GLfloat r, GLfloat g, GLfloat b);
	virtual void Left_Click(float mouse_x, float mouse_y);
	virtual void Update(glm::vec2 vpos);
	virtual void Merge_Check(vector<Polygon*> Polygons);

	GLfloat* get_vertex_ptr();
	GLfloat* get_color_ptr();

public:
	vector<glm::vec3> _RectShape;
	vector<glm::vec3> _Colors;
	GLfloat _x1;
	GLfloat _y1;
	GLfloat _x2;
	GLfloat _y2;
	GLfloat _x3;
	GLfloat _y3;
	GLfloat _x4;
	GLfloat _y4;

	float _xpos;
	float _ypos;
	float _offset;
	int _size;
	//GLfloat _RectShape[6][3];//	두 개의 삼각형들 좌표
	//GLfloat _Colors[6][3]; //--- 두 삼각형 꼭지점 색상
};

