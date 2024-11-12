#include "Triangle.h"
#include "global.h"


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
	:
	// 삼각형의 세 꼭지점 계산
	/*float x1 = mouseX;
float y1 = mouseY + (2.0f / 3.0f) * sideLength;
float x2 = mouseX - baseLength / 2;
float y2 = mouseY - (1.0f / 3.0f) * sideLength;
float x3 = mouseX + baseLength / 2;
float y3 = y2;*/
	_quadrant(quadrant)
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
	, _Translation{ glm::mat4(1.0f)}
	, _Rotation{ glm::mat4(1.0f)}
	, _Transform{ glm::mat4(1.0f) }
	
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
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Triangle::init_buffer_polygon(GLuint* vao, GLuint* vbo)
{
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- 변수 triShape에서 버텍스 데이터 값을 버퍼에 복사한다.
	// Triangle size = 9
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), get_vertex_ptr(), GL_STATIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	// 
	//--- colors 배열의 사이즈: 9 = Triangle
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), get_color_ptr(), GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

void Triangle::Update()
{
	switch (_move)
	{
	case move_Default:
		return;
		break;
	case move_Bounce:
		// 경계 체크
		check_Collision_dir_bounce(check_terminous_out());
		_xpos += _xspd;
		_ypos += _yspd;
		for (auto& a : _TriShape)
		{
			a[0] += _xspd;
			a[1] += _yspd;
		}
		break;
	case move_Zigzag:
		check_Collision_dir_zigzag(check_terminous_out());
		if (_ycount > 0)
		{
			_ypos += _yspd;
			for (auto& a : _TriShape)
			{
				a[1] += _yspd;
			}
			--_ycount;
		}
		else
		{
			_xpos += _xspd;
			for (auto& a : _TriShape)
			{
				a[0] += _xspd;
			}
		}
		break;
	case move_Spiral_rect:
	{
		if (_limit_x < 0.1f && _limit_y < 0.1f)
		{
			_limit_x = 0.1f;
			_limit_y = 0.1f;
			_xspd = -_xspd;
			_yspd = -_yspd;
			rotate90DegreesZ();
			rotate90DegreesZ();
			_spiral_in = false;
		}
		else if(_limit_x > 1.0f && _limit_y > 1.0f)
		{
			_limit_x = 1.0f;
			_limit_y = 1.0f;
			_xspd = -_xspd;
			_yspd = -_yspd;
			rotate90DegreesZ();
			rotate90DegreesZ();
			_spiral_in = true;
		}
		if(_spiral_in == false)
			inspiral();
		else 
			spiral();
	}
	break;
	case move_Spiral_circle:
	{
		_theta = 5.f;
		

		_Transform = glm::rotate(_Transform, radians(_theta), glm::vec3(0.f, 0.f, 1.f));
		//_Trimat = glm::rotate(_Trimat, radians(_theta), glm::vec3(0.f, 0.f, 1.f));

		//glm::vec2 cpos = get_center();


		//_Transform = glm::translate(_Transform, glm::vec3(-cpos.x, -cpos.y, 0.f));
		

	
		/*_xtrans = _xpos / 10.f;
		_ytrans = _ypos / 10.f;*/

		//if (/*abs(_xpos) <= abs(_xspd) || */abs(_xpos - _xspd) >= 1.f)
		//	_xspd = -_xspd;
		//if (/*abs(_xpos) <= abs(_xspd)  || */abs(_ypos - _yspd) >= 1.f)
		//	_yspd = -_yspd;

		/*_xpos -= _xspd;
		_ypos -= _yspd;*/

		//_Transform = glm::translate(_Transform, glm::vec3(_xpos, _ypos, 0.f));


		if (abs(_xpos) + abs(_ypos) > 1.f)
		{
			_xspd = -_xspd;
			_yspd = -_yspd;
			while ((abs(_xpos) + abs(_ypos) >= 1.f))
			{
				_xpos += _xspd;

				_ypos += _yspd;
				for (auto& a : _TriShape)
					a[0] += _xspd;
				_ypos += _yspd;
				for (auto& a : _TriShape)
					a[1] += _yspd;
			}
		}
		else
		{
			_xpos += _xspd;
			for (auto& a : _TriShape)
				a[0] += _xspd;
			_ypos += _yspd;
			for (auto& a : _TriShape)
				a[1] += _yspd;
		}
		/*if (abs(_xpos) == 0.f || abs(_xpos) > 1.f)
		{
			_xspd = -_xspd;
			_xpos += _xspd;
		}
		else
		{
			_xpos += _xspd;
			for (auto& a : _TriShape)
				a[0] += _xspd;
		}
		if (abs(_ypos) == 0.f || abs(_ypos) > 1.f)
		{
			_yspd = -_yspd;
			_ypos += _yspd;
		}
		else
		{
			_ypos += _yspd;
			for (auto& a : _TriShape)
				a[1] += _yspd;	
		}*/


		//_Translation = glm::translate(mat4(1.0f), glm::vec3(_xpos, _ypos, 0.f));
		

		break;
	}
	case move_End:
		break;
	default:
		break;
	}





	/*_Rotation * Matrix4f
	{ 0.0f,-1.f, 0.f, 0.f,
	1.f, 0.f, 0.f, 0.f,
	0.f, 0.f, 1.0f, 0.f,
	0.f, 0.f, 0.f,1.0f };*/

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
	return (GLfloat*)_TriShape;
}

GLfloat* Triangle::get_color_ptr()
{
	return (GLfloat*)_Colors;
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


