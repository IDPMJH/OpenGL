#pragma once
#include <vector>

enum Rect_terminous
{
	Non = 0,
	Outside,
	Inside,
	Max,
};

enum Rect_Status
{
	base,
	rd_color,
	rd_backgruond,
	reduction,
	enlagerment,
};

using namespace std;
class Rect_2
{
public:
	Rect_2();
	Rect_2(float left, float right, float top, float bottom, float r, float g, float b, int qurdrant);
	~Rect_2();

public:
	void Left_Click(float mouse_x, float mouse_y);
	void Right_Click(float mouse_x, float mouse_y);

	Rect_terminous Check_terminous(float mouse_x, float mouse_y);

public:
	float _left;
	float _right;
	float _top;
	float _bottom;

	float _width;
	float _height;

	float _r;
	float _g;
	float _b;

	float _br;
	float _bg;
	float _bb;

	int _quadrant; // ºÐ¸é

	Rect_Status _rs = base;
	

};

