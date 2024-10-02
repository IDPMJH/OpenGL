#include <random>
#include "Rect_4.h"
#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <assert.h>


Rect::Rect()
{
}

Rect::Rect(float left, float right, float top, float bottom, float r, float g, float b, int qurdrant)
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

Rect::Rect(vPos<float> center, float width, float height)
	:_center(center),
	_base_center(center),
	_width(width)
	,_prewidth(width)
	,_preheight(height)
	,_height(height),
	_r(Random_Number<float>(0,10) / 10.f),
	_g(Random_Number<float>(0,10) / 10.f),
	_b(Random_Number<float>(0,10) / 10.f),
	_left(_center._x - width/2.f),
	_right(_center._x + width/2.f),
	_top(_center._y + height/2.f),
	_bottom(_center._y - height/2.f),
	_quadrant(1),
	_dir_x(Random_Number<int>(0, 99) % 2 ? 1 : -1),
	_dir_y(Random_Number<int>(0, 99) % 2 ? 1 : -1)
	,_speed(0.05f)
	,_xcount(0)
	,_ycount(0)
	,_sdir(1)
	,_xidx(0)
	,_yidx(0)
	,_dirchanged(false)
{

}

Rect::~Rect()
{
}

void Rect::Left_Click(float mouse_x, float mouse_y)
{
	Rect_terminous rt;
	rt = Check_terminous(mouse_x, mouse_y);
	if (rt == Rect_terminous::Inside)
	{
		_clicked = true;
	}
	else if (rt == Rect_terminous::Outside)
	{
		//random_device rd;
		//mt19937 mt(rd());
		//uniform_int_distribution<int> id(0, 10);
		//_br = (float)id(mt) / 10.f;
		//_bg = (float)id(mt) / 10.f;
		//_bb = (float)id(mt) / 10.f;
		//glClearColor(_br, _bg, _bb, 1.f); //--- 바탕색을 변경
	}
}

void Rect::Right_Click(float mouse_x, float mouse_y)
{
	Rect_terminous rt;
	rt = Check_terminous(mouse_x, mouse_y);
	if (rt == Rect_terminous::Inside)
	{
		// TO DO
	/*	if (_width <= 0.1f)
			return;

		_left += 0.01f;
		_bottom += 0.01f;

		_right -= 0.01f;
		_top -= 0.01f;

		_width = _right - _left;
		_height = _top - _bottom;
		*/
	}
	else if (rt == Rect_terminous::Outside)
	{
		// TO DO
	/*	_left -= 0.01f;
		_bottom -= 0.01f;

		_right += 0.01f;
		_top += 0.01f;*/
	}
		
}

void Rect::Update()
{

	

	switch (_rc_mvstatus)
	{
	case base:
		break;
	case changing_diagonal:
		if (_left <= -1.f || _right >= 1.f)
		{
			_dir_x = -_dir_x;
			_center._x += (float)_dir_x * _speed;
		}

		if (_bottom <= -1.f || _top >= 1.f)
		{
			_dir_y = -_dir_y;
			_center._y += (float)_dir_y * _speed;
		}

		_center._x += (float)_dir_x * _speed;
		_center._y += (float)_dir_y * _speed;
		break;
	case changing_zigzag:
	{

		float offset;
		if (_dir_y == 0)		// 좌우 왕복 
		{

			if (_left < -1.f || _right > 1.f)
			{
				offset = (_left < -1.f) ? (-1.f - _left) :  (1.f - _right);
				_dir_x = -_dir_x;
				_center._x += offset * 1.1f;//(float)_dir_x * _speed;
				_ycount = 5;
			}
			else if (_ycount > 0)
			{
				--_ycount;
				if (_bottom < -1.f || _top > 1.f)
				{
					offset = (_bottom < -1.f) ? (-1.f - _bottom) : (1.f - _top);
					if(_dirchanged == false)
					{
						_sdir = -_sdir;
						_dirchanged = true;
					}
					_center._y += offset;//(float)sdir * _speed;
				}
				else
					_center._y += (float)_sdir * _speed;
			}
			else
			{
				_center._x += (float)_dir_x * _speed;
				_center._y += (float)_dir_y * _speed;
			}
		}
		else if (_dir_x == 0)	// 상하 왕복
		{
			if (_bottom < -1.f || _top > 1.f)
			{
				offset = (_bottom < -1.f) ? (-1.f - _bottom) : (1.f - _top);

				_dir_y = -_dir_y;
				_center._y += offset;// (float)_dir_y* _speed;
				_ycount = 5;
			}
			else if (_ycount > 0)
			{
				--_ycount;
				if (_left < -1.f || _right > 1.f)
				{
					offset = (_left <= -1.f) ? (-1.f - _left) : (1.f - _right);
					_sdir = -_sdir;
					_center._x += offset;// (float)sdir* _speed;
				}
				else
					_center._x += (float)_sdir * _speed;
			}
			else
			{
				_center._x += (float)_dir_x * _speed;
				_center._y += (float)_dir_y * _speed;
			}
		}

		break;
	}
	}

	switch (_rc_status)
	{
	case base:
		break;
	case changing_size:
	{
		static float scale = 0.005f;
		static bool large = true;

		if (large == true)
		{
			if (_width >= 0.2f)
			{
				large = false;
				break;
			}
			_width += scale;
			_height += scale;
			
		}
		else if (large == false)
		{
			if (_width <= 0.05f)
			{
				large = true;
				break;
			}
			_width -= scale;
			_height -= scale;
			
		}
		break;
	}
	case changing_color:
		if (_xcount >= 5)
		{
			_r = (Random_Number<float>(0, 10) / 10.f);
			_g = (Random_Number<float>(0, 10) / 10.f);
			_b = (Random_Number<float>(0, 10) / 10.f);
			_xcount = 0;
		}
		else
			++_xcount;
		break;
	default:
		break;
	}

	_left = (_center._x - _width / 2.f);
	_right = (_center._x + _width / 2.f);
	_top = (_center._y + _height / 2.f);
	_bottom = (_center._y - _height / 2.f);




}

void Rect::Merge_Check(vector<Rect>& vc)
{
	if (this == nullptr)
		assert(this);
	//vector<Rect>::iterator r_i;
	for (auto i = vc.begin();  i !=vc.end(); ++i)
	{
		
		if (&*i == this)
		{
			// 자기 자신인 경우
			continue;
		}
		vPos<float> length = i->_center - this->_center;

		float width = (i->_width > this->_width) ? i->_width / 2 : this->_width / 2;
		float height = (i->_height > this->_height) ? i->_height / 2 : this->_height / 2;
		vPos<float> center = (i->_height > this->_height) ? i->_center : this->_center;
		if (abs(length._x) <= width && abs(length._y) <= height)
		{
			this->_merged = true;
			i->_merged = true;
			// 합쳐질 상황
			

			/*Rect New_Rc(center, i->_width + this->_width, i->_height + this->_height);
			New_Rc._clicked = true;
			*this = New_Rc;
			vc.erase(i);
			return vc.begin();*/
			//return it;
		}
	}
	/*for (auto& i : vc)
		i._clicked = false;*/
}

Rect_terminous Rect::Check_terminous(float mouse_x, float mouse_y)
{
	if (_quadrant == 0)
	{
		if (mouse_x <= _right && mouse_x >= _left &&
			mouse_y <= _top && mouse_y >= _bottom)
			return Inside;
	}
	else
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
	}


	return Non;
}
