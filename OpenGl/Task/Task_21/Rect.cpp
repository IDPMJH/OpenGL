#include "Rect.h"

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>


float Random_0_to_1f();

Rect_p::Rect_p()
	:Polygon(mode_Rect)
	, _RectShape
    {
        {-3.f,0.f,-3.f},
        {-3.f,0.f,3.f},
        {3.f,0.f,3.f},
        {3.f,0.f,-3.f}
    }
    , _Colors{
       {Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
        {Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
        {Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},

        {Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
        {Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
        {Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()}
    }
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
{
	_Colors.resize(6);
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
	glDrawArrays(GL_POLYGON, 0, _RectShape.size());
}

void Rect_p::init_buffer_polygon(GLuint* vao, GLuint* vbo)
{
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- 변수 triShape에서 버텍스 데이터 값을 버퍼에 복사한다.
	// rect size = 18
	glBufferData(GL_ARRAY_BUFFER, 3 * _RectShape.size() * sizeof(GLfloat), get_vertex_ptr(), GL_STATIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	// 
	//--- colors 배열의 사이즈: 18 = rect
	glBufferData(GL_ARRAY_BUFFER, 3 * _RectShape.size() * sizeof(GLfloat), get_color_ptr(), GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}
void Rect_p::Update()
{

	if (_RectShape[0].x <= _RectShape[1].x)
	{
		_RectShape[0].x = _x1;
		_RectShape[3].x = _x1;
		_RectShape[5].x = (_x1 + _x3) / 2.f;
		_RectShape[5].y = (_y1 + _y3) / 2.f;


	}
	float _leftsidex = (_x1 - _x2) / 50;
	float _rightupx = (_x3 - _x1) / 100;
	float _rightupy = (_y1 - _y3) / 100;
	_RectShape[0].x -= _leftsidex;
	_RectShape[3].x -= _leftsidex;
	_RectShape[5].x += _rightupx;
	_RectShape[5].y += _rightupy;
}
void Rect_p::Change_Color(GLfloat r, GLfloat g, GLfloat b)
{
	for (auto& a : _Colors)
	{
		a = { r,g,b };
	}
}

GLfloat* Rect_p::get_vertex_ptr()
{
	return (GLfloat*)_RectShape.data();
}

GLfloat* Rect_p::get_color_ptr()
{
	return (GLfloat*)_Colors.data();
}


