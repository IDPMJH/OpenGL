#pragma once
#include "Polygon.h"


class Triangle;

class Rect_p : public Polygon
{
public:
	Rect_p();
	~Rect_p();
	Rect_p(float xpos, float ypos, float offset);
	Rect_p(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vector<vec3>color);
	Rect_p(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 p5, vector<vec3>color);
public:
	virtual void move_On_dir(char dir);
	virtual void Draw_Polygon();
	virtual void init_buffer_polygon(GLuint* vao, GLuint* vbo);
	virtual void Update();
	virtual void Split(Line* line, std::vector<Polygon*>& vpolygons);
	virtual bool lineIntersect(const vec3& a1, const vec3& a2, const vec3& b1, const vec3& b2, vec3& intersection);
	bool lineIntersectRect(Line* line, std::vector<vec3>& intersections);
	GLfloat* get_vertex_ptr();
	GLfloat* get_color_ptr();

public:
	GLfloat _RectShape[6][3];//	두 개의 삼각형들 좌표

	vector<Triangle> _triangles;
	vector<vec3> _vertices;
	vector<vec3> _colors;
	int _dirx;
	

	GLfloat _Colors[6][3]; //--- 두 삼각형 꼭지점 색상



};

