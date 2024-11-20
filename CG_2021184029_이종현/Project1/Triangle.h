#pragma once
#include "Polygon.h"
#include "ogldev_math_3d.h"



class Matrix4f;
enum Triangle_dir
{
	dir_none,
	dir_up,
	dir_left,
	dir_down,
	dir_right,
};

enum Terminous_Type
{
	Ter_none,
	Ter_left,
	Ter_top,
	Ter_right,
	Ter_bottom,
};

class Triangle : public Polygon
{
public:
	Triangle();
	Triangle(GLfloat f1, GLfloat f2, GLfloat f3,
		GLfloat f4, GLfloat f5, GLfloat f6,
		GLfloat f7, GLfloat f8, GLfloat f9);
	Triangle(float xpos, float ypos, float offset, int quadrant);
	Triangle(vec3 p1, vec3 p2, vec3 p3,vector<vec3>color);
	~Triangle();
public:
	virtual void move_On_dir(char dir);
	virtual void Draw_Polygon() override;
	virtual void init_buffer_polygon(GLuint* vao, GLuint* vbo);
	virtual void Split(Line* line, std::vector<Polygon*>& vpolygons);

	virtual bool lineIntersect(const vec3& a1, const vec3& a2, const vec3& b1, const vec3& b2, vec3& intersection);
	virtual void Update() override;

	void stand_shape();

	void check_Collision_dir_bounce(Terminous_Type Tt);

	void check_Collision_dir_zigzag(Terminous_Type Tt);

	void check_Collision_dir_spiral_rect();
	Terminous_Type check_terminous_out();
	Terminous_Type check_terminous_out_height(float t);
	Terminous_Type check_terminous_out_width(float t);
	GLfloat* get_vertex_ptr();
	GLfloat* get_color_ptr();
	void rotate90DegreesZ();
	void rotate_m_90DegreesZ();
	void rotate180DegreesZ();
	void rotate45DegreeZ();
	void rotateleft();
	void rotateright();
	void move_pos_();
	void plus_offset_ter();
	void spiral();
	void inspiral();
	void init_mat4();

	glm::vec2 get_center();
public:
	GLfloat _TriShape[3][3];
	GLfloat _Colors[3][3]; //--- 삼각형 꼭지점 색상

	vector<vec3> _vertices;
	vector<vec3> _vcolors;

	glm::mat4 _Translation;
	glm::mat4 _Rotation;
	glm::mat4 _Scaling;
	glm::mat4 _Transform;
	glm::mat4 _Trimat;
	GLint _gTranformMatrix;
	int _quadrant;

	glm::vec2 pos;
	float _xpos;
	float _ypos;
	float _xspd = 0.01f;
	float _yspd = 0.01f;
	float _offset;
	float _angle_radian;
	float _ter_offset = 0.f;

	int _ycount = 0;
	int _xcount = 0;
	int _xmaxcount = 0;
	int _ymaxcount = 0;
	int _xspiral_count = 0;
	int _yspiral_count = 0;

	float _delta_x = 0.1f;
	float _delta_y = 0.1f;
	float _limit_x = 1.f;
	float _limit_y = 1.f;
	bool _horizon = true;

	float _theta = 0.0f;
	float _xtrans = 0.0f;
	float _ytrans = 0.0f;

	float _maxtrans_x = 0.f;
	float _mintans_x = 0.f;

	float _maxtrans_y = 0.f;
	float _mintans_y = 0.f;

	bool _spiral_in = true;
	bool _xclash = false;
	bool _yclash = false;
	bool _splited;
	
	Triangle_dir _dirx = dir_none;
	Triangle_dir _diry = dir_up;
};

