#pragma once

using GLfloat = float;
using GLuint = unsigned int;


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
	// ������ ��ü�� ������ ���� ���� => ���������Լ� ����
	// => �߻�Ŭ������ ��ȯ
	virtual void move_On_dir(char dir) {}
	virtual void Draw_Polygon() = 0;
	virtual void init_buffer_polygon(GLuint* vao,GLuint* vbo) = 0;
	virtual void Update() = 0;

public:
	polygon_type _type;
	move_Mode _move;
	bool _dead;

};

