#include "Pentagon_12.h"
#include "global.h"

Pentagon::Pentagon()
	:_PentaShape{0}
	,_Colors{0}
{
}

Pentagon::Pentagon(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4)
	:_PentaShape
	{ 
		{(x1+x4)/2.f,(y1+y4)/2.f,0.f},
		{x1,y1,0.f},
		{x2,y2,0.f},
		{x3,y3,0.f},
		{x4,y4,0.f}
	}
	,_x1(x1)
	,_y1(y1)
	,_x2(x2)
	,_y2(y2)
	,_x3(x3)
	,_y3(y3)
	,_x4(x4)
	,_y4(y4)
	, _to_line(false)
{
	_Colors.resize(6);
}

Pentagon::Pentagon(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4, GLfloat x5, GLfloat y5)
	:_PentaShape
	{
		{x1,y1,0.f},
		{x2,y2,0.f},
		{x3,y3,0.f},
		{x4,y4,0.f},
		{x5,y5,0.f}
	}
	, _x1(x1)
	, _y1(y1)
	, _x2(x2)
	, _y2(y2)
	, _x3(x3)
	, _y3(y3)
	, _x4(x4)
	, _y4(y4)
	, _x5(x5)
	, _y5(y5)
	, _to_line(true)
{
	_Colors.resize(5);
}

Pentagon::~Pentagon()
{
}


void Pentagon::move_On_dir(char dir)
{
}

void Pentagon::Draw_Polygon()
{
	glDrawArrays(GL_POLYGON, 0, 5);

}

void Pentagon::init_buffer_polygon(GLuint* vao, GLuint* vbo)
{
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- ���� triShape���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// rect size = 18
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), get_vertex_ptr(), GL_STATIC_DRAW);

	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	// 
	//--- colors �迭�� ������: 18 = rect
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), get_color_ptr(), GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

void Pentagon::Update()
{
	if(_to_line == false)
	{
		if (_PentaShape[0].y >= _y1 + 0.2f)
		{
			_PentaShape[0].y = _y1;
			_PentaShape[1].x = _x1;
			_PentaShape[2].x = _x2;
			_PentaShape[3].x = _x3;
			_PentaShape[4].x = _x4;
		}
		_PentaShape[0].y += 0.004f;
		_PentaShape[1].x -= 0.002f;
		_PentaShape[2].x += 0.002f;
		_PentaShape[3].x -= 0.002f;
		_PentaShape[4].x += 0.002f;
	}
	else
	{
		if (_PentaShape[1].x >= _x1)
		{ 
			_PentaShape[1].x = _x2;
			_PentaShape[2].x = _x3;
			_PentaShape[3].x = _x4;
			_PentaShape[4].x = _x5;
		}
		float long_offset = _x1 - _x2;
		float short_offset = _x1 - _x3;
		_PentaShape[1].x += long_offset / 50.f;
		_PentaShape[2].x += short_offset / 50.f;
		_PentaShape[3].x -= short_offset / 50.f;
		_PentaShape[4].x -= long_offset / 50.f;
 	}

}

void Pentagon::Change_Color(GLfloat r, GLfloat g, GLfloat b)
{
	for (auto& a : _Colors)
	{
		a = { r,g,b };
	}
}

GLfloat* Pentagon::get_vertex_ptr()
{
	return (GLfloat*)(_PentaShape.data());
}

GLfloat* Pentagon::get_color_ptr()
{
	return (GLfloat*)(_Colors.data());
}
