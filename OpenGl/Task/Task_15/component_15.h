#pragma once
#include <random>

using namespace std;

template <typename T>
T Random_Number(int start, int end);

template <typename T>
class vPos
{
public:
	vPos()
		:_x(0),
		_y(0)
	{

	}
	vPos(T x, T y)
		:_x(x),
		_y(y)
	{
	}
	~vPos()
	{
	}
public:
	vPos<T> operator-(vPos<T> other)
	{
		vPos<T> temp;
		temp._x = _x - other._x;
		temp._y = _y - other._y;
		return temp;
	}
	vPos<T> operator+(vPos<T> other)
	{
		vPos<T> temp;
		temp._x = _x + other._x;
		temp._y = _y + other._y;
		return temp;
	}


public:
	T _x;	// int, float, double
	T _y;	// int, float, double
};

template <typename T>
T Random_Number(int start, int end)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> id(start, end);

	return (T)id(mt);
}


float Random_0_to_1f()
{
	float f = Random_Number<int>(0, 10) / 10.f;

	return f;
}

inline void Mouse_Convert_Win_To_OpenGL(float& _xpos, float& _ypos, float _width, float _height)
{
	_xpos = (float)_xpos / (float)(_width / 2) - 1;
	_ypos = (float)_ypos / (float)(_height / 2) - 1;
	_ypos = -_ypos;
}
//
//GLfloat* make_Line_Pos_Horizonal(float xpos, float ypos, float offset)
//{
//	GLfloat Line[2][3] = { {xpos - offset,ypos,0.f },{xpos + offset , ypos, 0.f } };
//	return (GLfloat*)Line;
//}



