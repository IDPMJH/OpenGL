#pragma once
#include "Polygon.h"
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
	virtual void Split(Line* line, std::vector<Polygon*>& vpolygons) {}
	virtual bool lineIntersect(const vec3& a1, const vec3& a2, const vec3& b1, const vec3& b2, vec3& intersection) { return false; }
	GLfloat* get_vertex_ptr();
	GLfloat* get_color_ptr();
	void set_color_ptr(GLfloat f1, GLfloat f2, GLfloat f3);
public:
	GLfloat _PointShape[3]; //---한 점 색상
	GLfloat _Colors[3]; //--- 한 점 색상
};

