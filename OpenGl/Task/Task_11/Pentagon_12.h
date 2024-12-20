#pragma once
#include "Polygon_12.h"
class Pentagon :
    public Polygon
{
public:
	Pentagon();
	Pentagon(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3,GLfloat x4, GLfloat y4);
	Pentagon(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4,GLfloat x5,GLfloat y5);

	~Pentagon();
public:
	virtual void move_On_dir(char dir);
	virtual void Draw_Polygon();
	virtual void init_buffer_polygon(GLuint* vao, GLuint* vbo);
	virtual void Update();
	virtual void Change_Color(GLfloat r, GLfloat g, GLfloat b);

	GLfloat* get_vertex_ptr();
	GLfloat* get_color_ptr();

public:
	vector<glm::vec3> _PentaShape;
	vector<glm::vec3> _Colors;
	GLfloat _x1;
	GLfloat _y1;
	GLfloat _x2;
	GLfloat _y2;
	GLfloat _x3;
	GLfloat _y3;
	GLfloat _x4;
	GLfloat _y4;
	GLfloat _x5;
	GLfloat _y5;

	bool _to_line;
	//GLfloat _RectShape[6][3];//	�� ���� �ﰢ���� ��ǥ
	//GLfloat _Colors[6][3]; //--- �� �ﰢ�� ������ ����
};


