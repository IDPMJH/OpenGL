#include <random>
#include "Rect_2.h"
#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>


Rect_2::Rect_2()
{
}

Rect_2::Rect_2(float left, float right, float top, float bottom, float r, float g, float b, int qurdrant)
	:_left(left)
	,_right(right)
	,_top(top)
	,_bottom(bottom)
	,_r(r)
	,_g(g)
	,_b(b)
	,_br(1.f)
	,_bg(1.f)
	,_bb(1.f)
	,_quadrant(qurdrant)
{
	_width = _right - _left;
	_height = _top - _bottom;
}

Rect_2::~Rect_2()
{
}

void Rect_2::Left_Click(float mouse_x, float mouse_y)
{
	Rect_terminous rt;
	rt = Check_terminous(mouse_x, mouse_y);
	if (rt == Rect_terminous::Inside)
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> id(0, 10);
		_r = (float)id(mt) / 10.f;
		_g = (float)id(mt) / 10.f;
		_b = (float)id(mt) / 10.f;
	}
	else if (rt == Rect_terminous::Outside)
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> id(0, 10);
		_br = (float)id(mt) / 10.f;
		_bg = (float)id(mt) / 10.f;
		_bb = (float)id(mt) / 10.f;
		glClearColor(_br, _bg, _bb, 1.f); //--- 바탕색을 변경
	}
}

void Rect_2::Right_Click(float mouse_x, float mouse_y)
{
	Rect_terminous rt;
	rt = Check_terminous(mouse_x, mouse_y);
	if (rt == Rect_terminous::Inside)
	{
		if (_width <= 0.1f)
			return;

		_left += 0.01f;
		_bottom += 0.01f;

		_right -= 0.01f;
		_top -= 0.01f;

		_width = _right - _left;
		_height = _top - _bottom;
		
	}
	else if (rt == Rect_terminous::Outside)
	{
		_left -= 0.01f;
		_bottom -= 0.01f;

		_right += 0.01f;
		_top += 0.01f;
	}
		
}

Rect_terminous Rect_2::Check_terminous(float mouse_x, float mouse_y)
{
	switch (_quadrant)
	{
	case 1:
		if (mouse_x <= _right && mouse_x >= _left &&
			mouse_y <= _top && mouse_y >= _bottom)
			return Inside;
		else if (mouse_x >= 0 && mouse_x <= 1 &&
			mouse_y >= 0 && mouse_y <= 1)
			return Outside;
		else
			Non;
		break;
	case 2:
		if (mouse_x <= _right && mouse_x >= _left &&
			mouse_y <= _top && mouse_y >= _bottom)
			return Inside;
		else if (mouse_x >= -1 && mouse_x <= 0 &&
			mouse_y >= 0 && mouse_y <= 1)
			return Outside;
		else
			Non;
		break;
	case 3:
		if (mouse_x <= _right && mouse_x >= _left &&
			mouse_y <= _top && mouse_y >= _bottom)
			return Inside;
		else if (mouse_x >= -1 && mouse_x <= 0 &&
			mouse_y >= -1 && mouse_y <= 0)
			return Outside;
		else
		break;
	case 4:
		if (mouse_x <= _right && mouse_x >= _left &&
			mouse_y <= _top && mouse_y >= _bottom)
			return Inside;
		else if (mouse_x >= 0 && mouse_x <= 1 &&
			mouse_y >= -1 && mouse_y <= 0)
			return Outside;
		else
		break;
	default:
		break;
	}


	return Non;
}
