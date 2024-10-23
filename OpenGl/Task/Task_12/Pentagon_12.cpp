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

Pentagon::Pentagon(float xpos, float ypos, float offset)
	:_PentaShape(5, { 0,0,0 })
	,_vx(5,0)
	,_vy(5,0)
	,_xpos(xpos)
	,_ypos(ypos)
	,_offset_r(offset)
{
	for (size_t i = 0; i < 5; i++)
	{
		_theta = i * 72.f + 90.f;
		_vx[_vx.size() - 1 - i] = _xpos + _offset_r * cosf(glm::radians(_theta));
		_vy[_vy.size() - 1 - i] = _ypos + _offset_r * sinf(glm::radians(_theta));
		_PentaShape[i] = glm::vec3(_vx[_vx.size() - 1 - i], _vy[_vy.size() - 1 - i], 0.f);
	}
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
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- 변수 triShape에서 버텍스 데이터 값을 버퍼에 복사한다.
	// rect size = 18
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), get_vertex_ptr(), GL_STATIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	// 
	//--- colors 배열의 사이즈: 18 = rect
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), get_color_ptr(), GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

void Pentagon::Update()
{
	/*if(_to_line == false)
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
 	}*/

}

void Pentagon::Change_Color(GLfloat r, GLfloat g, GLfloat b)
{
	for (auto& a : _Colors)
	{
		a = { r,g,b };
	}
}

void Pentagon::Left_Click(float mouse_x, float mouse_y)
{
	if (mouse_x < _xpos + _offset_r && mouse_x > _xpos - _offset_r &&
		mouse_y < _ypos + _offset_r && mouse_y > _ypos - _offset_r)
		_clicked = true;
}

void Pentagon::Update(glm::vec2 vpos)
{
	_xpos += vpos.x;
	_ypos += vpos.y;

	// 폴리곤 초기화
	_vpos.x = _xpos;
	_vpos.y = _ypos;
	_width = _height = _offset_r * 2;


	for (size_t i = 0; i < 5; i++)
	{
		_theta = i * 72.f + 90.f;
		_vx[_vx.size() - 1 - i] = _xpos + _offset_r * cosf(glm::radians(_theta));
		_vy[_vy.size() - 1 - i] = _ypos + _offset_r * sinf(glm::radians(_theta));
		_PentaShape[i] = glm::vec3(_vx[_vx.size() - 1 - i], _vy[_vy.size() - 1 - i], 0.f);
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
