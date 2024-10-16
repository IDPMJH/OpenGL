#include "Dot.h"
#include "global.h"

Dot::Dot()
	:Polygon(mode_Point)
	,_PointShape{0.f}
	,_Colors{0.f}
{
}

float Random_0_to_1f();

Dot::Dot(GLfloat f1, GLfloat f2, GLfloat f3)
	:Polygon(mode_Point)
	, _PointShape{f1,f2,f3}
	, _Colors{ Random_0_to_1f() }
{
}

Dot::~Dot()
{
}

void Dot::move_On_dir(char dir)
{
	float dirx = 0;
	float diry = 0;
	switch (dir)
	{
	case 'w':
		diry = 0.05f;
		break;
	case 'a':
		dirx = -0.05f;
		break;
	case 's':
		diry = -0.05f;
		break;
	case 'd':
		dirx = 0.05f;
		break;
	default:
		break;
	}
	_PointShape[0] += dirx;
	_PointShape[1] += diry;
}

void Dot::Draw_Polygon()
{
	glDrawArrays(GL_POINTS, 0, 1);
}

void Dot::init_buffer_polygon(GLuint* vao, GLuint* vbo)
{
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- ���� triShape���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// dot size = 3
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), get_vertex_ptr(), GL_STATIC_DRAW);

	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	// 
	//--- colors �迭�� ������: 3 = dot
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), get_color_ptr(), GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);

}

GLfloat* Dot::get_vertex_ptr()
{
	return _PointShape;
}

GLfloat* Dot::get_color_ptr()
{
	return _Colors;
}

void Dot::set_color_ptr(GLfloat f1, GLfloat f2, GLfloat f3)
{
	_Colors[0] = f1;
	_Colors[1] = f2;
	_Colors[2] = f3;
}
