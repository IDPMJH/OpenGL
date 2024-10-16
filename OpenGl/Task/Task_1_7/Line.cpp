#include "Line.h"
#include "global.h"

float Random_0_to_1f();

Line::Line()
	:Polygon(mode_Line)
	,_LineShape{0}
	,_Colors{0}
{
}

Line::Line(GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4, GLfloat f5, GLfloat f6)
	:Polygon(mode_Line)
	, _LineShape{ {f1,f2,f3}, {f4,f5,f6} }
	, _Colors{ Random_0_to_1f(),Random_0_to_1f() ,Random_0_to_1f()
	, Random_0_to_1f(),Random_0_to_1f() ,Random_0_to_1f() }
{
}

Line::~Line()
{
}

void Line::move_On_dir(char dir)
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
	for (auto& a : _LineShape)
	{
		a[0] += dirx;
		a[1] += diry;
	}
}

void Line::Draw_Polygon()
{
	glDrawArrays(GL_LINES, 0, 2);
}

void Line::init_buffer_polygon(GLuint* vao, GLuint* vbo)
{
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- ���� triShape���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// Line size = 3
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), get_vertex_ptr(), GL_STATIC_DRAW);

	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	// 
	//--- colors �迭�� ������: 6= Line
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), get_color_ptr(), GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

void Line::Update()
{
}

GLfloat* Line::get_vertex_ptr()
{
	return (GLfloat*)_LineShape;
}

GLfloat* Line::get_color_ptr()
{
	return (GLfloat*)_Colors;
}
