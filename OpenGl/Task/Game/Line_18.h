#pragma once

#include "Polygon_18.h"





class Line : public Polygon
{
public:
	Line();
	Line(GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4, GLfloat f5, GLfloat f6);
	Line(GLfloat x, GLfloat y);
	~Line();
public:
	virtual void move_On_dir(char dir);
	virtual void Draw_Polygon();
	virtual void init_buffer_polygon(GLuint* vao, GLuint* vbo);
	virtual void Update();
	virtual void Change_Color(GLfloat r, GLfloat g, GLfloat b);
	GLfloat* get_vertex_ptr();
	GLfloat* get_color_ptr();
	void init_line(bool ccwon);

public:
	vector<glm::vec3> _lineshape;
	vector<glm::vec3> _Colors;
	//GLfloat _lineshape[2][3]; //--- 두 꼭지점 색상
	//GLfloat _Colors[2][3]; //--- 두 꼭지점 색상
	int _size;
	float _radius;
	float _theta;
	float _mouse_x;
	float _mouse_y;
	bool _linemode;



};

