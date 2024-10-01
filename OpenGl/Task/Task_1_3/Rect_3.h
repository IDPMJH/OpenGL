#pragma once
#include <vector>
#include "Component_3.h"


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
	void Update(vPos<float> pos);
	void Merge_Check(vector<Rect>& vc);

	Rect_terminous Check_terminous(float mouse_x, float mouse_y);

public:
	vPos<float> _center;

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

	bool _clicked = false;

	Rect_Status _rs = base;
	

};

