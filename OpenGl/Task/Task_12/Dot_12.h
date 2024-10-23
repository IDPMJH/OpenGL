#pragma once
#include "Polygon_12.h"
class Dot : public Polygon
{
public:
	Dot();
	Dot(float f1, float f2, float f3);
	~Dot();
public:
	virtual void move_On_dir(char dir) override;
	virtual void Draw_Polygon();
	virtual void init_buffer_polygon(GLuint* vao,GLuint* vbo);
	virtual void Update() {}
	virtual void Change_Color(GLfloat r, GLfloat g, GLfloat b);
	virtual void Update(glm::vec2 vpos);
	virtual void Merge_Check(vector<Polygon*> Polygons);
	virtual void Left_Click(float mouse_x, float mouse_y);


	GLfloat* get_vertex_ptr();
	GLfloat* get_color_ptr();
	void set_color_ptr(GLfloat f1, GLfloat f2, GLfloat f3);
	
public:
	float _xpos;
	float _ypos;
	int _vcount;
	float _offset;
	//GLfloat _PointShape[3]; //---한 점 색상
	//GLfloat _Colors[3]; //--- 한 점 색상
	vector<glm::vec3> _PointShape;
	vector<glm::vec3> _Colors;
};

