#pragma once
#include <vector>
#include "Component_4.h"


enum Rect_terminous
{
	Non = 0,
	Outside,
	Inside,
	Max,
};

enum Rect_Moving_Status
{
	base_move,
	changing_diagonal,
	changing_zigzag,
};

enum Rect_Status
{
	base = 4,
	changing_size,
	changing_color,
};

using namespace std;

class Rect
{
public:
	Rect();
	Rect(float left, float right, float top, float bottom, float r, float g, float b, int qurdrant);
	Rect(vPos<float> center, float width, float height);
	~Rect();

public:
	void Left_Click(float mouse_x, float mouse_y);
	void Right_Click(float mouse_x, float mouse_y);
	void Update();
	void Merge_Check(vector<Rect>& vc);

	Rect_terminous Check_terminous(float mouse_x, float mouse_y);

public:
	vPos<float> _center;
	vPos<float> _base_center;
	float _left;
	float _right;
	float _top;
	float _bottom;

	float _width;
	float _prewidth;

	float _height;
	float _preheight;

	float _r;
	float _g;
	float _b;

	float _br;
	float _bg;
	float _bb;

	int _quadrant; // ºÐ¸é

	bool _clicked = false;
	bool _merged = false;

	int _dir_x;
	int _dir_y;

	float _speed;

	int _xcount;
	int _ycount;

	int _xidx;
	int _yidx;
	float _sdir;
	bool _dirchanged;
	Rect_Moving_Status _rc_mvstatus = base_move;
	Rect_Status _rc_status = base;
	Rect_Status _rc_prestatus = base;
	

};

