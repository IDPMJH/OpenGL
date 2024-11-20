#pragma once
#include "global.h"




using GLfloat = float;
using GLuint = unsigned int;
using glm::mat4;
using glm::vec3;
using std::vector;

class Line;

enum polygon_type
{
	mode_Default = 0,
	mode_Point,
	mode_Line,
	mode_Triangle,
	mode_Rect,
	mode_End,
};

enum move_Mode
{
	move_Default,
	move_Bounce,
	move_Zigzag,
	move_Spiral_rect,
	move_Spiral_circle,
	move_End,
};

class color
{
public:

};

class Polygon
{
public:
	Polygon();
	Polygon(polygon_type pm);

	virtual ~Polygon();
public:
	// 폴리곤 자체가 움직일 일은 없음 => 순수가상함수 구현
	// => 추상클래스로 전환
	virtual void move_On_dir(char dir) {}
	virtual void Draw_Polygon() = 0;
	virtual void init_buffer_polygon(GLuint* vao,GLuint* vbo) = 0;
	virtual void Update() = 0;
	virtual void Split(Line* line, std::vector<Polygon*>& vpolygons) = 0;
	virtual bool lineIntersect(const vec3& a1, const vec3& a2, const vec3& b1, const vec3& b2, vec3& intersection) = 0;

public:
	polygon_type _type;
	move_Mode _move;
	bool _dead;
	bool _splited;
	mat4 _model;
	mat4 _rotate;
	mat4 _translate;
	mat4 _scale;
};
 
