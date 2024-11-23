#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::vector;
using GLfloat = float;
using GLuint = unsigned int;
using GLint = int;


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
	virtual void Change_Color(GLfloat r, GLfloat g, GLfloat b) = 0;

public:
	polygon_type _type;
	move_Mode _move;

};

