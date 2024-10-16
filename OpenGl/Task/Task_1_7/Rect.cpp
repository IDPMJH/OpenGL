#include "Rect.h"
#include "global.h"

float Random_0_to_1f();

Rect_p::Rect_p()
	:Polygon(mode_Rect)
	, _RectShape{0}
	, _Colors{0}
{
}

Rect_p::~Rect_p()
{
}

Rect_p::Rect_p(float xpos, float ypos, float offset)
	:_RectShape{
		{xpos - offset, ypos + offset, 0.f},
		{xpos - offset, ypos - offset, 0.f},
		{xpos + offset, ypos + offset, 0.f},

		{xpos + offset, ypos + offset, 0.f},
		{xpos - offset, ypos - offset, 0.f},
		{xpos + offset, ypos - offset, 0.f}
	},
	_Colors{
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
		
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()}
	}
{
}

void Rect_p::move_On_dir(char dir)
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
	for (auto& a : _RectShape)
	{
		a[0] += dirx;
		a[1] += diry;
	}
}

void Rect_p::Draw_Polygon()
{
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
}

void Rect_p::init_buffer_polygon(GLuint* vao, GLuint* vbo)
{
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- ���� triShape���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// rect size = 18
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), get_vertex_ptr(), GL_STATIC_DRAW);

	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	// 
	//--- colors �迭�� ������: 18 = rect
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), get_color_ptr(), GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

GLfloat* Rect_p::get_vertex_ptr()
{
	return (GLfloat*)_RectShape;
}

GLfloat* Rect_p::get_color_ptr()
{
	return (GLfloat*)_Colors;
}


