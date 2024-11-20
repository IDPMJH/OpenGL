#pragma once

#include "Polygon.h"

using std::vector;



class Line : public Polygon
{
public:
	Line();
	Line(GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4, GLfloat f5, GLfloat f6);
	Line(GLfloat x, GLfloat y);
	Line(glm::vec3 p1, glm::vec3 p2);
	~Line();
public:
	virtual void move_On_dir(char dir);
	virtual void Draw_Polygon();
	virtual void init_buffer_polygon(GLuint* vao, GLuint* vbo);
	virtual void Update()override;
	virtual void Split(Line* line, std::vector<Polygon*>& vpolygons) override {}
	virtual bool lineIntersect(const glm::vec3& a1, const glm::vec3& a2, const glm::vec3& b1, const glm::vec3& b2, glm::vec3& intersection) override { return false; }

	void Update(glm::vec3 offset);
	GLfloat* get_vertex_ptr();
	GLfloat* get_color_ptr();
	void init_line(bool ccwon);

public:
	vector<glm::vec3> _lineshape;
	//GLfloat _lineshape[2][3]; //--- 두 꼭지점 색상
	vector<glm::vec3> _Colors;
	//GLfloat _Colors[2][3]; //--- 두 꼭지점 색상
	int _size;
	float _radius;
	float _theta;
	float _mouse_x;
	float _mouse_y;
	bool _linemode;



};

