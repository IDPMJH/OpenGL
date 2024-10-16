#pragma once
#include "Polygon.h"
#include "ogldev_math_3d.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Matrix4f;
enum Triangle_dir
{
	dir_up,
	dir_left,
	dir_down,
	dir_right,
};

class Triangle : public Polygon
{
public:
	Triangle();
	Triangle(GLfloat f1, GLfloat f2, GLfloat f3,
		GLfloat f4, GLfloat f5, GLfloat f6,
		GLfloat f7, GLfloat f8, GLfloat f9);
	Triangle(float xpos, float ypos, float offset,int quadrant);
	~Triangle();
public:
	virtual void move_On_dir(char dir);
	virtual void Draw_Polygon() override;
	virtual void init_buffer_polygon(GLuint* vao, GLuint* vbo);
	virtual void Update() override;
	void check_Collision_dir_bounce();
	void check_Collision_dir_zigzag();
	void check_Collision_dir_spiral_rect();
	GLfloat* get_vertex_ptr();
	GLfloat* get_color_ptr() ;
	void rotate90DegreesZ();
	void rotate_m_90DegreesZ();
	void rotate180DegreesZ();
	void move_pos_();
public:
	GLfloat _TriShape[3][3];
	GLfloat _Colors[3][3]; //--- 삼각형 꼭지점 색상
	glm::mat4 _Translation;
	glm::mat4 _Rotation;
	glm::mat4 _Scaling;
	GLint _gTranformMatrix;
	int _quadrant;
	float _xpos;
	float _ypos;
	float _xspd;
	float _yspd;
	float _offset;
	float _angle_radian;
	int _ycount = 0;
	float _delta_x = 0.01f;
	float _delta_y = 0.01f;
	float _limit_x = 1.f;
	float _limit_y = 1.f;
	bool _horizon = true;
	float _theta = 0.0f;
	float _xtrans = 0.0f;
	bool _spiral_in = false;
	Triangle_dir _dir = dir_up;
};

