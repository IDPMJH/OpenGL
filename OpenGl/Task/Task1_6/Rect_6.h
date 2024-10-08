#pragma once
#pragma once
#include <vector>
#include "Component_6.h"


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

enum Rect_Status_Die
{
	die_Default,
	die_Plus,
	die_X,
	die_Straight,
	die_Eight,
	die_end,
};

using namespace std;

class Rect
{
public:
	Rect();
	Rect(float left, float right, float top, float bottom, float r, float g, float b, int qurdrant);
	Rect(vPos<float> center, float width, float height);
	Rect(Rect& target,int number);
	~Rect();

public:
	void Left_Click(float mouse_x, float mouse_y);
	void Right_Click(float mouse_x, float mouse_y);
	void Update(vPos<float> pos);
	void Update();
	void Merge_Check(vector<Rect>& vc);
	void Merge_Check(vector<Rect>& vc, Rect* clicked);
	void split_Rect(Rect& parent,int number);
	void split_Rect_Eight(Rect& parent, int number);
	void select_Dir(int number, const Rect &target);

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

	int _dir_x;
	int _dir_y;

	int _quadrant; // ºÐ¸é

	bool _clicked = false;
	bool _merged = false;;

	Rect_Status _rs = base;
	Rect_Status_Die _rs_die;

};

