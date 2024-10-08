#include <random>
#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <assert.h>
#include "Rect_6.h"

Rect::Rect()
{
}

Rect::Rect(float left, float right, float top, float bottom, float r, float g, float b, int qurdrant)
	:_left(left)
	, _right(right)
	, _top(top)
	, _bottom(bottom)
	, _r(r)
	, _g(g)
	, _b(b)
	, _br(1.f)
	, _bg(1.f)
	, _bb(1.f)
	, _quadrant(qurdrant)
{
	_width = _right - _left;
	_height = _top - _bottom;
}

// ���� ��� ���� ���� ������ Rect_6
Rect::Rect(vPos<float> center, float width, float height)
	:_center(center),
	_width(width),
	_height(height),
	_r(Random_Number<float>(0, 10) / 10.f),
	_g(Random_Number<float>(0, 10) / 10.f),
	_b(Random_Number<float>(0, 10) / 10.f),
	_left(_center._x - width / 2.f),
	_right(_center._x + width / 2.f),
	_top(_center._y + height / 2.f),
	_bottom(_center._y - height / 2.f),
	_quadrant(1),
	_dir_x(0),
	_dir_y(0)
{

}

// �θ� ����Ͽ� ���� �� �ڽ� - ����� ��
Rect::Rect(Rect& target, int number)
{
	*(this) = target;
	// �ٽ� ���� ���� �� �������� ����
	_rs_die = (Rect_Status_Die)Random_Number<int>(1, Rect_Status_Die::die_Eight);

	// ���������� �������� ����
	_width = target._width / 2.f;
	_height = target._height / 2.f;
	_clicked = false;
	

	// �θ��� ���� ���¿� ���� �з�
	switch (target._rs_die)
	{
	case die_Default:
		break;
	case die_Plus:	
		split_Rect(target, number);
		select_Dir(number, target);
		break;
	case die_X:
		split_Rect(target, number);
		select_Dir(number, target);
		break;
	case die_Straight:
		split_Rect(target, number);
		select_Dir(number, target);
		break;
	case die_Eight:
		_width = target._width / 4.f;
		_height = target._height / 4.f;
		split_Rect_Eight(target, number);
		select_Dir(number, target);
		break;
	default:
		break;
	}


	// split���� _merged��� ������Ʈ ȸ��
	_merged = true;
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
		//glClearColor(_br, _bg, _bb, 1.f); //--- �������� ����
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

void Rect::Update(vPos<float> pos)
{
	_center = _center + pos;

	_left = (_center._x - _width / 2.f);
	_right = (_center._x + _width / 2.f);
	_top = (_center._y + _height / 2.f);
	_bottom = (_center._y - _height / 2.f);
}

void Rect::Update()
{
	_center._x += (float)_dir_x * 0.01f;
	_center._y += (float)_dir_y * 0.01f;

	if (_width > 0.f && _height > 0.f && _merged == true)
	{
		_width -= 0.001f;
		_height -= 0.001f;
	}

	_left = (_center._x - _width / 2.f);
	_right = (_center._x + _width / 2.f);
	_top = (_center._y + _height / 2.f);
	_bottom = (_center._y - _height / 2.f);
}

// ��ü üũ�ϴ� ���
void Rect::Merge_Check(vector<Rect>& vc)
{
	if (this == nullptr)
		assert(this);
	//vector<Rect>::iterator r_i;
	for (auto i = vc.begin(); i != vc.end(); ++i)
	{

		if (&*i == this)
		{
			// �ڱ� �ڽ��� ���
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
			// ������ ��Ȳ


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

// ���� Ŭ�� �� Rect�� �˰��ϱ� ����
void Rect::Merge_Check(vector<Rect>& vc, Rect* clicked)
{
	if (this == nullptr)
		assert(this);
	//if (clicked == nullptr)
	//	return;	�̹� ������ üũ��
	//vector<Rect>::iterator r_i;
	for (auto i = vc.begin(); i != vc.end(); ++i)
	{

		if (&*i == this)
		{
			// �ڱ� �ڽ��� ���
			continue;
		}
		vPos<float> length = i->_center - this->_center;

		float width = i->_width / 2 + this->_width / 2;//(i->_width > this->_width) ? i->_width / 2 : this->_width / 2;
		float height = i->_height / 2 + this->_height / 2;//(i->_height > this->_height) ? i->_height / 2 : this->_height / 2;
		vPos<float> center = (i->_height > this->_height) ? i->_center : this->_center;
		if (abs(length._x) <= width && abs(length._y) <= height)
		{
			this->_merged = true;
			i->_merged = true;
			this->_r = i->_r;
			this->_g = i->_g;
			this->_b = i->_b;
			// ������ ��Ȳ


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

void Rect::split_Rect(Rect& parent, int number)
{
	vector<vPos<float>> vc_pos;
	float x_offset = parent._width / 4.f;
	float y_offset = parent._height / 4.f;
	vc_pos.push_back((vPos<float>(-x_offset, y_offset)));		// �»��
	vc_pos.push_back((vPos<float>(x_offset, y_offset)));		// ����
	vc_pos.push_back((vPos<float>(-x_offset, -y_offset)));		// ���ϴ�
	vc_pos.push_back((vPos<float>(x_offset, -y_offset)));		// ���ϴ�

	_center = parent._center + vc_pos[number];			// �ڽ� ��ġ ����
	Update();											// �����¿� ��ġ ����
}
void Rect::split_Rect_Eight(Rect& parent, int number)
{
	vector<vPos<float>> vc_pos;
	float x_offset = parent._width / 4.f;
	float y_offset = parent._height / 4.f;
	vc_pos.push_back((vPos<float>(-x_offset, y_offset)));		// �»�� 0
	vc_pos.push_back((vPos<float>(0, y_offset)));				// ��� 1

	vc_pos.push_back((vPos<float>(x_offset, y_offset)));		// ���� 2 
	vc_pos.push_back((vPos<float>(x_offset, 0)));				// �� 3

	vc_pos.push_back((vPos<float>(-x_offset, -y_offset)));		// ���ϴ�4
	vc_pos.push_back((vPos<float>(-x_offset,0)));				// ��5

	vc_pos.push_back((vPos<float>(x_offset, -y_offset)));		// ���ϴ� 6
	vc_pos.push_back((vPos<float>(0, -y_offset)));				// �ϴ� 7


	_center = parent._center + vc_pos[number];			// �ڽ� ��ġ ����
	Update();											// �����¿� ��ġ ����
}

void Rect::select_Dir(int number, const Rect& target)
{
	switch (target._rs_die)
	{
	case die_Default:
		break;
	case die_Plus:
		switch (number)
		{
		case 0:
			_dir_x = -1;
			_dir_y = 0;
			break;
		case 1:
			_dir_x = 0;
			_dir_y = 1;
			break;
		case 2:
			_dir_x = 0;
			_dir_y = -1;
			break;
		case 3:
			_dir_x = 1;
			_dir_y = 0;
			break;
		default:
			break;
		}
		break;
	case die_X:
		switch (number)
		{
		case 0:
			_dir_x = -1;
			_dir_y = 1;
			break;
		case 1:
			_dir_x = 1;
			_dir_y = 1;
			break;
		case 2:
			_dir_x = -1;
			_dir_y = -1;
			break;
		case 3:
			_dir_x = 1;
			_dir_y = -1;
			break;
		default:
			break;
		}
		break;
	case die_Straight:
		break;
	case die_Eight:
		// �»�� 0
		// ��� 1

		// ���� 2 
		// �� 3

		// ���ϴ�4
		// ��5

		// ���ϴ� 6
		// �ϴ� 7
		switch (number)
		{
		case 0:
			_dir_x = -1;
			_dir_y = 1;
			break;
		case 1:
			_dir_x = 0;
			_dir_y = 1;
			break;
		case 2:
			_dir_x = 1;
			_dir_y = 1;
			break;
		case 3:
			_dir_x = 1;
			_dir_y = 0;
			break;
		case 4:
			_dir_x = -1;
			_dir_y = -1;
			break;
		case 5:
			_dir_x = -1;
			_dir_y = 0;
			break;
		case 6:
			_dir_x = 1;
			_dir_y = -1;
			break;
		case 7:
			_dir_x = 0;
			_dir_y = -1;
			break;
		default:
			break;
		}
		break;
		break;
	case die_end:
		break;
	default:
		break;
	}
}

Rect_terminous Rect::Check_terminous(float mouse_x, float mouse_y)
{
	if (_quadrant == 0)
	{
		if (mouse_x < _right && mouse_x > _left &&
			mouse_y < _top && mouse_y > _bottom)
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
