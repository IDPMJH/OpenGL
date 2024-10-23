#include "Rect_12.h"
#include "global.h"




float Random_0_to_1f();

Rect_p::Rect_p()
	:Polygon(mode_Rect)
	, _RectShape{0}
	, _Colors{0}
{
}

Rect_p::Rect_p(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3)
	:Polygon(mode_Rect)
	, _RectShape{
		{x1,y1, 0.f},
		{x2,y2, 0.f},
		{x3,y3, 0.f},
		{x1,y1, 0.f},
		{x3,y3, 0.f},
		{(x1 + x3) / 2, (y1 + y3) / 2, 0.f}
	}
	,_x1(x1)
	,_y1(y1)
	,_x2(x2)
	,_y2(y2)
	,_x3(x3)
	,_y3(y3)
	,_size(6)
{
	_Colors.resize(6);
}

Rect_p::~Rect_p()
{
}

Rect_p::Rect_p(float xpos, float ypos, float offset)
	:_RectShape{
		{_x1 = xpos - offset, _y1 = ypos + offset, 0.f},
		{_x2 = xpos - offset, _y2 = ypos - offset, 0.f},
		{_x3 = xpos + offset, _y3 = ypos - offset, 0.f},
		{_x4 = xpos + offset, _y4 = ypos + offset, 0.f}
	}
	,_xpos(xpos)
	,_ypos(ypos)
	,_offset(offset)
	,_Colors{
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},		
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()}
	}
	,_size(4)
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
	if(_size == 3)
		glDrawArrays(GL_POLYGON, 0, 2 *  _size);
	else 
		glDrawArrays(GL_POLYGON, 0, _size);


}

void Rect_p::init_buffer_polygon(GLuint* vao, GLuint* vbo)
{
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- ���� triShape���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// rect size = 18 = 3 * 6 [�ﰢ�� 2���� ���], 3 * 4 [�׳� �簢���� ��� - GL_DrawArrays(GL_GL_POLYGON)
	glBufferData(GL_ARRAY_BUFFER, 3 * _size * sizeof(GLfloat), get_vertex_ptr(), GL_STATIC_DRAW);

	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	// 
	// rect size = 18 = 3 * 6 [�ﰢ�� 2���� ���], 3 * 4 [�׳� �簢���� ��� - GL_DrawArrays(GL_GL_POLYGON)
	glBufferData(GL_ARRAY_BUFFER, 3 * _size * sizeof(GLfloat), get_color_ptr(), GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}
void Rect_p::Update()
{
}
void Rect_p::Change_Color(GLfloat r, GLfloat g, GLfloat b)
{
	for (auto& a : _Colors)
	{
		a = { r,g,b };
	}
}

void Rect_p::Left_Click(float mouse_x, float mouse_y)
{
	if (mouse_x < _x4 && mouse_x > _x1 &&
		mouse_y < _y1 && mouse_y > _y2)
		_clicked = true;
}

void Rect_p::Update(glm::vec2 vpos)
{
	_xpos += vpos.x;
	_ypos += vpos.y;

	// ������ �ʱ�ȭ
	_vpos.x = _xpos;
	_vpos.y = _ypos;
	_width = _height = _offset * 2;



	_RectShape = vector<glm::vec3>{
		{_x1 = _xpos - _offset, _y1 = _ypos + _offset, 0.f},
		{_x2 = _xpos - _offset, _y2 = _ypos - _offset, 0.f},
		{_x3 = _xpos + _offset, _y3 = _ypos - _offset, 0.f},
		{_x4 = _xpos + _offset, _y4 = _ypos + _offset, 0.f}
	};
}

GLfloat* Rect_p::get_vertex_ptr()
{
	return (GLfloat*)_RectShape.data();
}

GLfloat* Rect_p::get_color_ptr()
{
	return (GLfloat*)_Colors.data();
}


