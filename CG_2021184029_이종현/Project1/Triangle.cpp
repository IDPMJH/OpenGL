#include "Triangle.h"
#include "global.h"
#include "Line.h"
#include "Rect.h"


float Random_0_to_1f();

Triangle::Triangle()
	:Polygon(mode_Triangle)
	,_TriShape{0}
	,_Colors{0}
	, _quadrant(0)
{
}

Triangle::Triangle(GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4, GLfloat f5, GLfloat f6, GLfloat f7, GLfloat f8, GLfloat f9)
	:Polygon(mode_Triangle)
	, _TriShape
	{	{f1,f2,f3},
		{f4,f5,f6},
		{f7,f8,f9}
	}
	, _Colors
	{	
		{ Random_0_to_1f(),Random_0_to_1f() ,Random_0_to_1f()},
		{ Random_0_to_1f(),Random_0_to_1f() ,Random_0_to_1f()},
		{ Random_0_to_1f(),Random_0_to_1f() ,Random_0_to_1f() }
	}
	, _quadrant(0)
{
}

Triangle::Triangle(float xpos, float ypos, float offset, int quadrant)
	:Polygon(mode_Triangle)
	// 삼각형의 세 꼭지점 계산
	/*float x1 = mouseX;
float y1 = mouseY + (2.0f / 3.0f) * sideLength;
float x2 = mouseX - baseLength / 2;
float y2 = mouseY - (1.0f / 3.0f) * sideLength;
float x3 = mouseX + baseLength / 2;
float y3 = y2;*/
	,_quadrant(quadrant)
	, _xpos(xpos)
	, _ypos(ypos)
	, _xspd(0.01f)
	, _yspd(0.01f)
	, _offset(offset)
	, _angle_radian(0.0f)
	,_TriShape{
		{xpos,ypos + 1.5f * offset, 0},
		{xpos - offset, ypos - offset, 0},
		{xpos + offset, ypos - offset, 0}
	}
	, _Colors
	{
		{ Random_0_to_1f(),Random_0_to_1f() ,Random_0_to_1f()},
		{ Random_0_to_1f(),Random_0_to_1f() ,Random_0_to_1f()},
		{ Random_0_to_1f(),Random_0_to_1f() ,Random_0_to_1f() }
	}
	, _vertices{
		{ xpos,ypos + 1.5f * offset, 0 },
		{ xpos - offset, ypos - offset, 0 },
		{ xpos + offset, ypos - offset, 0 }
	}
	,_vcolors{
		{ Random_0_to_1f(),Random_0_to_1f() ,Random_0_to_1f()},
		{ Random_0_to_1f(),Random_0_to_1f() ,Random_0_to_1f()},
		{ Random_0_to_1f(),Random_0_to_1f() ,Random_0_to_1f()}
	}

	, _Translation{ glm::mat4(1.0f)}
	, _Rotation{ glm::mat4(1.0f)}
	, _Transform{ glm::mat4(1.0f) }
{
	_dead = false;
}

Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3,vector<vec3> color)
	:Polygon(mode_Triangle)
	, _xpos(p1[0] + p2[0] + p3[0] / 3)
	, _ypos(p1[1] + p2[1] + p3[1] / 3)
	, _TriShape{
		{p1[0],p1[1],p1[2]},
		{p2[0],p2[1],p2[2]},
		{p3[0],p3[1],p3[2]}
	}
	, _vertices{
		{p1[0],p1[1],p1[2]},
		{p2[0],p2[1],p2[2]},
		{p3[0],p3[1],p3[2]}
	}
	, _vcolors{
		{color[0][0],color[0][1],color[0][2]},
		{color[1][0],color[1][1],color[1][2]},
		{color[2][0],color[2][1],color[2][2]}
	}
	, _Colors{
		{color[0][0],color[0][1],color[0][2]},
		{color[1][0],color[1][1],color[1][2]},
		{color[2][0],color[2][1],color[2][2]}
	}
	,_splited(true)
	, _xspd(0.01f)
	, _yspd(0.01f)
{
}



Triangle::~Triangle()
{
}

void Triangle::move_On_dir(char dir)
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
	for (auto& a : _TriShape)
	{
		a[0] += dirx;
		a[1] += diry;
	}
}



void Triangle::Draw_Polygon()
{
	glDrawArrays(GL_POLYGON, 0, _vertices.size());
}

void Triangle::init_buffer_polygon(GLuint* vao, GLuint* vbo)
{
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- 변수 triShape에서 버텍스 데이터 값을 버퍼에 복사한다.
	// Triangle size = 9
	glBufferData(GL_ARRAY_BUFFER, 3*_vertices.size() * sizeof(GLfloat), get_vertex_ptr(), GL_STATIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	// 
	//--- colors 배열의 사이즈: 9 = Triangle
	glBufferData(GL_ARRAY_BUFFER, 3 * _vertices.size() * sizeof(GLfloat), get_color_ptr(), GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

void Triangle::Split(Line* line, std::vector<Polygon*>& vpolygons)
{
	vec3 intersection1{ 0,0,0 };
	vec3 intersection2{ 0,0,0 };

	
	
	vec3 v1 = vec3{ _TriShape[0][0],_TriShape[0][1],_TriShape[0][2] };
	vec3 v2 = vec3{ _TriShape[1][0],_TriShape[1][1],_TriShape[1][2] };
	vec3 v3 = vec3{ _TriShape[2][0],_TriShape[2][1],_TriShape[2][2] };

	vector<vec3> colors;

	colors.push_back(vec3{ _Colors[0][0],_Colors[0][1],_Colors[0][2] });
	colors.push_back(vec3{ _Colors[1][0],_Colors[1][1],_Colors[1][2] });
	colors.push_back(vec3{ _Colors[2][0],_Colors[2][1],_Colors[2][2] });

	vec3 p1 = line->_lineshape[0];
	vec3 p2 = line->_lineshape[1];

	

	// 교차점 계산
	bool found1 = lineIntersect(v1, v2, p1, p2, intersection1);
	bool found2 = lineIntersect(v2, v3, p1, p2, intersection2);
	bool found3 = false;

	if (!found1 && !found2 && !found3)
		return;

	if (found2 == true)
		found3 = lineIntersect(v3, v1, p1, p2, intersection1);
	else
		found3 = lineIntersect(v3, v1, p1, p2, intersection2);

	vec3 originalCentroid = (v1 + v2 + v3) / 3.0f;
	vec3 avgIntersection = (intersection1 + intersection2) / 2.0f;

	if (found1 && found2)
	{
		

		vec3 newTri1[3] = {intersection1,v2,intersection2  };
		vec3 newQuad[4] = { v1, intersection1, intersection2, v3 };

		vector<vec3> newTriColors = { colors[0], colors[1], colors[2] };
		vector<vec3> newQuadColors = { colors[0], colors[1], colors[2], colors[2] };

		Triangle* ttemp = new Triangle(newTri1[0], newTri1[1], newTri1[2], newTriColors);
		Rect_p* rtemp = new Rect_p(newQuad[0], newQuad[1], newQuad[2], newQuad[3], newQuadColors);

		ttemp->_splited = true;
		rtemp->_splited = true;

		vpolygons.push_back(ttemp);
		vpolygons.push_back(rtemp);


		ttemp->_dirx = dir_left;
		rtemp->_dirx = 1;
		//// 기울기 구하기
		//float m = (p2.y - p1.y) / (p2.x - p1.x);
		//if(m < 0)
		//{
		//	ttemp->_dirx = dir_right;
		//	rtemp->_dirx = -1;
		//}
		//else
		//{
		//	ttemp->_dirx = dir_left;
		//	rtemp->_dirx = 1;
		//}
	}
	else if (found2 && found3)
	{
	

		// 새로운 삼각형과 사각형 생성
		vec3 newTri1[3] = { intersection1,intersection2,v3 };
		vec3 newQuad[4] = { v1, v2, intersection2,intersection1 };

		vector<vec3> newTriColors = { colors[1], colors[2], colors[0] };
		vector<vec3> newQuadColors = { colors[1], colors[2], colors[0], colors[0] };

		Triangle* ttemp = new Triangle(newTri1[0], newTri1[1], newTri1[2], newTriColors);
		Rect_p* rtemp = new Rect_p(newQuad[0], newQuad[1], newQuad[2], newQuad[3], newQuadColors);
		vpolygons.push_back(ttemp);
		vpolygons.push_back(rtemp);

		ttemp->_splited = true;
		rtemp->_splited = true;

		ttemp->_dirx = dir_right;
		rtemp->_dirx = -1;
		//// 기울기 구하기
		//float m = (p2.y - p1.y) / (p2.x - p1.x);
		//if (m < 0)
		//{
		//	ttemp->_dirx = dir_right;
		//	rtemp->_dirx = -1;
		//}
		//else
		//{
		//	ttemp->_dirx = dir_left;
		//	rtemp->_dirx = 1;
		//}
		
	}
	else if (found3 && found1)
	{
	
		// 새로운 삼각형과 사각형 생성
		vec3 newTri1[3] = { v1, intersection1, intersection2};
		vec3 newQuad[4] = {intersection1, v2 , v3 , intersection2};

		vector<vec3> newTriColors = { colors[2], colors[0], colors[1] };
		vector<vec3> newQuadColors = { colors[2], colors[0], colors[1], colors[1] };

		Triangle* ttemp = new Triangle(newTri1[0], newTri1[1], newTri1[2], newTriColors);
		Rect_p* rtemp = new Rect_p(newQuad[0], newQuad[1], newQuad[2], newQuad[3], newQuadColors);

		ttemp->_splited = true;
		rtemp->_splited = true;

		vpolygons.push_back(ttemp);
		vpolygons.push_back(rtemp);

		// 기울기 구하기
		if (intersection1.x > intersection2.x)
			swap(intersection1, intersection2);
		float m = (p2.y - p1.y) / (p2.x - p1.x);
		if (m < 0)
		{
			ttemp->_dirx = dir_right;
			rtemp->_dirx = -1;
		}
		else
		{
			ttemp->_dirx = dir_left;
			rtemp->_dirx = 1;
		}

	}
	else
	{
		return;
	}
	for (auto a = vpolygons.begin(); a < vpolygons.end(); ++a)
	{
		if ((*a) == this)
		{
			vpolygons.erase(a);
			break;
		}
	}
}

bool Triangle::lineIntersect(const vec3& a1, const vec3& a2, const vec3& b1, const vec3& b2, vec3& intersection)
{
	vec3 s1 = a2 - a1;
	vec3 s2 = b2 - b1;

	float denominator = (-s2.x * s1.y + s1.x * s2.y);
	if (denominator == 0) {
		// 선분이 평행하거나 일치하는 경우
		return false;
	}

	float s = (-s1.y * (a1.x - b1.x) + s1.x * (a1.y - b1.y)) / (-s2.x * s1.y + s1.x * s2.y);
	float t = (s2.x * (a1.y - b1.y) - s2.y * (a1.x - b1.x)) / (-s2.x * s1.y + s1.x * s2.y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
		intersection = a1 + (t * s1);
		return true;
	}

	return false;
}


void Triangle::Update()
{
	// move_on_dir_x
	if (_dirx == dir_left)
	{
		_xpos -= _xspd;
		for (auto& a : _vertices)
		{
			a[0] -= _xspd;
		}
		for (auto& a : _TriShape)
		{
			a[0] -= _xspd;
		}
	}
	else if (_dirx == dir_right)
	{
		_xpos += _xspd;
		for (auto& a : _vertices)
		{
			a[0] += _xspd;
		}
		for (auto& a : _TriShape)
		{
			a[0] += _xspd;
		}
	}

	vec3 center = (_vertices[0] + _vertices[1] + _vertices[2]) / 3.f;

	if (center.y < -1.2 || center.x > 1.2 || center.x < -1.2)
	{
		_dead = true;
	}
}

void Triangle::stand_shape()
{
	_TriShape[0][0] = _xpos;
	_TriShape[0][1] = _ypos + 1.5f * _offset;
	_TriShape[1][0] = _xpos - _offset;
	_TriShape[1][1] = _ypos - _offset;
	_TriShape[2][0] = _xpos + _offset;
	_TriShape[2][1] = _ypos - _offset;
}

void Triangle::check_Collision_dir_bounce(Terminous_Type Tt)
{
	switch (Tt)
	{
	case Ter_none:
		return;
		break;
	case Ter_left:
		if(_dirx == dir_right)
		{ }
		else if(_diry == dir_up)
			rotate_m_90DegreesZ();
		else
			rotate90DegreesZ();
		_dirx = dir_right;
		_xspd = -_xspd;
		break;
	case Ter_top:
		_diry = dir_down;
		if (_dirx == dir_right)
			rotate_m_90DegreesZ();
		else
			rotate90DegreesZ();
		_yspd = -_yspd;
		break;
	case Ter_right:
		_dirx = dir_left;
		if (_diry == dir_down)
			rotate_m_90DegreesZ();
		else
			rotate90DegreesZ();
		_xspd = -_xspd;
		break;
	case Ter_bottom:
		_diry = dir_up;
		if(_dirx == dir_left)
			rotate_m_90DegreesZ();
		else
			rotate90DegreesZ();
		_yspd = -_yspd;
		break;
	default:
		break;
	}
	check_terminous_out();
}


void Triangle::check_Collision_dir_zigzag(Terminous_Type Tt)
{
	if (Tt == Ter_none)
		return;

	 //세움
	if(Tt == Ter_left || Tt == Ter_right)
		stand_shape();


	switch (Tt)
	{
	case Ter_left:
		rotateright();
		//rotate_m_90DegreesZ();
		_xspd = -_xspd;
		//_xpos -= _offset + _ter_offset;
		/*for (auto& a : _TriShape)
		{
			a[0] -= _offset + _ter_offset;
		}*/
		
		//plus_offset_ter();
		_ycount = 20;
		break;
	case Ter_top:
		_yspd = -_yspd;
		break;
	case Ter_right:
		rotateleft();
		_xspd = -_xspd;
		//_xpos += _offset + _ter_offset;;
		/*for (auto& a : _TriShape)
		{
			a[0] += _offset + _ter_offset;
		}*/
		//plus_offset_ter();
		_ycount = 20;
		
		break;
	case Ter_bottom:
		_yspd = -_yspd;
		break;
	default:
		break;
	}

}

void Triangle::check_Collision_dir_spiral_rect()
{		
	if(_xclash == false)
	{
		for (auto& a : _TriShape)
		{
			if (a[0] < -1.f && _xclash == false)
			{
				_xclash = true;
				rotate_m_90DegreesZ();
			}
		}
	}
	if(_yclash == false)
	{
		for (auto& a : _TriShape)
		{
			if (a[1] < -1.f && _yclash == false)
			{
				_yclash = true;
				rotate90DegreesZ();
			}
		}
		if(_yclash == true)
		{
			_xspd = -_xspd;
			_yspd = -_yspd;
		}
	}
}

// 경계 충돌 검사
Terminous_Type Triangle::check_terminous_out()
{
	for (auto& a : _TriShape)
	{
		if (a[0] > 1.f)
		{
			_ter_offset = a[0] - 1;
			_xpos -= _ter_offset;
			for (auto& aa : _TriShape)
			{
				aa[0] -= _ter_offset;
				
			}
			return Ter_right;
		}
		if (a[0] < -1)
		{
			_ter_offset = -1 - a[0];
			_xpos += _ter_offset;
			for (auto& aa : _TriShape)
			{
				aa[0] += _ter_offset;
				
			}
			return Ter_left;
		}
		if (a[1] > 1)
		{
			_ter_offset = a[1] - 1;
			_ypos -= _ter_offset;
			for (auto& aa : _TriShape)
			{
				aa[1] -= _ter_offset;
				
			}
			return Ter_top;
		}
		if (a[1] < -1)
		{
			_ter_offset = -1 - a[1];
			_ypos += _ter_offset;
			for (auto& aa : _TriShape)
			{
				aa[1] += _ter_offset;
				

			}
			return Ter_bottom;
		}
	}
	return Ter_none;
}

Terminous_Type Triangle::check_terminous_out_height(float t)
{
	for (auto& a : _TriShape)
	{
		if (a[1] > t)
		{
			_ter_offset = a[1] - t;
			_ypos -= _ter_offset;
			for (auto& aa : _TriShape)
			{
				aa[1] -= _ter_offset;
			}
			return Ter_top;
		}
		if (a[1] < -t)
		{
			_ter_offset = -t - a[1];
			_ypos += _ter_offset;
			for (auto& aa : _TriShape)
			{
				aa[1] += _ter_offset;
			}
			return Ter_bottom;
		}
	}
	return Ter_none;

}
Terminous_Type Triangle::check_terminous_out_width(float t)
{
	for (auto& a : _TriShape)
	{
		if (a[0] > t)
		{
			_ter_offset = a[0] - t;
			_xpos -= _ter_offset;
			for (auto& aa : _TriShape)
			{
				aa[0] -= _ter_offset;
			}
			return Ter_right;
		}
		if (a[0] < -t)
		{
			_ter_offset = -t - a[0];
			_xpos += _ter_offset;
			for (auto& aa : _TriShape)
			{
				aa[0] += _ter_offset;

			}
			return Ter_left;
		}
	}
	return Ter_none;
}
GLfloat* Triangle::get_vertex_ptr()
{
	return (GLfloat*)_vertices.data();
}

GLfloat* Triangle::get_color_ptr()
{
	return (GLfloat*)_vcolors.data();
}

void Triangle::rotate90DegreesZ()
{
	for (auto& a : _TriShape)
	{
		float x = a[0] - _xpos;
		float y = a[1] - _ypos;
		float x_new = -y;
		float y_new = x;
		a[0] = x_new + _xpos;
		a[1] = y_new + _ypos;
	}
}
void Triangle::rotate_m_90DegreesZ()
{

	for (auto& a : _TriShape)
	{
		float x = a[0] - _xpos;
		float y = a[1] - _ypos;
		float x_new = y;
		float y_new = -x;
		a[0] = x_new + _xpos;
		a[1] = y_new + _ypos;
	}
}

void Triangle::rotate180DegreesZ()
{
	
	for (auto& at : _TriShape)
	{
		_xpos += at[0];
		_ypos += at[1];
	}
	
	_xpos = (double)_xpos / (double)3;
	_ypos = (double)_ypos / (double)3;


	for (auto& a : _TriShape)
	{
		double x = a[0] - _xpos;
		double y = a[1] - _ypos;
		double x_new = -x;
		double y_new = -y;
		a[0] = x_new + _xpos;
		a[1] = y_new + _ypos;
	}
}

void Triangle::rotate45DegreeZ()
{
	float angle = glm::radians(-45.0f); // 45도를 라디안으로 변환
	float cosTheta = cos(angle);
	float sinTheta = sin(angle);

	for (auto& a : _TriShape)
	{
		float x = a[0] - _xpos;
		float y = a[1] - _ypos;
		float x_new = x * cosTheta - y * sinTheta;
		float y_new = x * sinTheta + y * cosTheta;
		a[0] = x_new + _xpos;
		a[1] = y_new + _ypos;
	}
}

void Triangle::rotateleft()
{
	_xpos = 1 - _offset;
	_TriShape[0][0] = 1;
	_TriShape[0][1] = _ypos + _offset;
	_TriShape[1][0] = _xpos - 2*_offset;
	_TriShape[1][1] = _ypos;
	_TriShape[2][0] = 1;
	_TriShape[2][1] = _ypos - _offset;
}

void Triangle::rotateright()
{
	_xpos = -1 + _offset;
	_TriShape[0][0] = -1;
	_TriShape[0][1] = _ypos + _offset;
	_TriShape[1][0] = _xpos + 2 * _offset;
	_TriShape[1][1] = _ypos;
	_TriShape[2][0] = -1;
	_TriShape[2][1] = _ypos - _offset;
}

void Triangle::move_pos_()
{

}

void Triangle::plus_offset_ter()
{
	for (auto& a : _TriShape)
	{
		if (a[0] > 1.0f)
		{
			float adjustment = a[0] - 1.0f;
			for (auto& aa : _TriShape)
			{
				aa[0] -= adjustment;
			}
			_xpos -= adjustment;
		}
		if (a[1] > 1.0f)
		{
			float adjustment = a[1] - 1.0f;
			for (auto& aa : _TriShape)
			{
				aa[1] -= adjustment;
			}
			_ypos -= adjustment;
		}
		if (a[0] < -1.0f)
		{
			float adjustment = a[0] + 1.0f;
			for (auto& aa : _TriShape)
			{
				aa[0] -= adjustment;
			}
			_xpos -= adjustment;
		}
		if (a[1] < -1.0f)
		{
			float adjustment = a[1] + 1.0f;
			for (auto& aa : _TriShape)
			{
				aa[1] -= adjustment;
			}
			_ypos -= adjustment;
		}
	}
}

void Triangle::spiral()
{

	if (_horizon == true)
	{
		_xpos += _xspd;
		for (auto& a : _TriShape)
		{
			a[0] += _xspd;
		}

	}
	else
	{
		_ypos += _yspd;
		for (auto& a : _TriShape)
		{
			a[1] += _yspd;;
		}
	}

	if (_horizon == true && Ter_none != check_terminous_out_width(_limit_x - _delta_x))
	{
		_horizon = false;
		_xspd = -_xspd;
		if (_dirx == dir_left)
		{
			_dirx = dir_none;
			rotate_m_90DegreesZ();
		}
		else
			rotate90DegreesZ();
		_limit_x -= _delta_x;
		check_terminous_out_width(_limit_x - _delta_x);
	}
	else if (_horizon == false && Ter_none != check_terminous_out_height(_limit_y - _delta_y))
	{
		_horizon = true;
		_yspd = -_yspd;
		rotate90DegreesZ();
		_limit_y -= _delta_y;
		check_terminous_out_height(_limit_y- _delta_y);
	}

}

void Triangle::inspiral()
{
	if (_horizon == true)
	{
		_xpos += _xspd;
		for (auto& a : _TriShape)
		{
			a[0] += _xspd;
		}
	}
	else
	{
		_ypos += _yspd;
		for (auto& a : _TriShape)
		{
			a[1] += _yspd;;
		}
	}

	if (_horizon == true && Ter_none != check_terminous_out_width(_limit_x + _delta_x))
	{
		_horizon = false;
		_xspd = -_xspd;
		if (_dirx == dir_left)
		{
			_dirx = dir_none;
			rotate_m_90DegreesZ();
		}
		else
			rotate90DegreesZ();
		_limit_x += _delta_x;
		check_terminous_out_width(_limit_x + _delta_x);
	}
	else if (_horizon == false && Ter_none != check_terminous_out_height(_limit_y + _delta_y))
	{
		_horizon = true;
		_yspd = -_yspd;
		rotate90DegreesZ();
		_limit_y += _delta_y;
		check_terminous_out_height(_limit_y + _delta_y);
	}

}

void Triangle::init_mat4()
{
	_Translation = mat4(1.0f);
	_Rotation = mat4(1.0f);
	_Transform = mat4(1.0f);
}

glm::vec2 Triangle::get_center()
{
	glm::vec2 c; 
	c.x = (_Trimat[0][0] + _Trimat[1][0] + _Trimat[2][0]) / 3.0f;
	c.y = (_Trimat[0][1] + _Trimat[1][1] + _Trimat[2][2]) / 3.0f;
	return c;
}


