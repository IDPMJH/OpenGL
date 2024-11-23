#pragma once

#include "Polygon_19.h"



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
	
};

