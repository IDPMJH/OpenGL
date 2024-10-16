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
	_quadrant(quadrant)
	, _xpos(xpos)
	, _ypos(ypos)
	, _xspd(0.02f)
	, _yspd(0.02f)
	, _offset(offset)
	, _angle_radian(0.0f)
	,_TriShape{
		{xpos,ypos + 2 * offset, 0},
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
	_xpos = 0;
	_ypos = 0;
	for (auto& xa : _TriShape)
	{
		_xpos += xa[0];
		_ypos += xa[1];
	}
	
	_xpos = _xpos / 3.f;
	_ypos = _ypos / 3.f;


	switch (_move)
	{
	case move_Default:
		return;
		break;
	case move_Bounce:
		check_Collision_dir_bounce();
		_xpos += _xspd;
		_ypos += _yspd;
		for (auto& a : _TriShape)
		{
			a[0] += _xspd;
			a[1] += _yspd;
		}
		break;
	case move_Zigzag:
		check_Collision_dir_zigzag();
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
		//check_Collision_dir_spiral_rect();
		if (_xtrans + _xpos > 1.f && _spiral_in == false)
		{
			_spiral_in = true;
			_delta_x = -_delta_x;
			
		}
		else if (_xtrans + _xpos < -1.f && _spiral_in == true)
		{
			_spiral_in = false;
			_delta_x = -_delta_x;
		}
		_Rotation = glm::rotate(_Rotation, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
		_xtrans += _delta_x;


		// 이동 행렬 계산
		_Translation = glm::mat4(1.0f); // 단위 행렬로 초기화
		_Translation = glm::translate(_Translation, glm::vec3(_xtrans, 0.0f, 0.0f)); // 이동 변환 적용

		//_Translation = glm::mat4(1.0f);
		//_Translation = glm::translate(_Translation, glm::vec3(_xspd, 0.f, 0.0f));



	}
		break;
		/*if (_horizon == false)
		{
			_ypos += _yspd;
			for (auto& a : _TriShape)
			{
				a[1] += _yspd;
			}
		}
		else
		{
			_xpos += _xspd;
			for (auto& a : _TriShape)
			{
				a[0] += _xspd;
			}
		}
		break;*/
	case move_Spiral_circle:
	{
		if (_xtrans + _xpos > 1.f && _spiral_in == false||
			_ypos > 1.f)
		{
			_spiral_in = true;
			_delta_x = -_delta_x;
			_delta_y = -_delta_y;

		}
		else if (_xtrans + _xpos < -1.f && _spiral_in == true
			|| _ypos < -1.f)
		{
			_spiral_in = false;
			_delta_x = -_delta_x;
			_delta_y = -_delta_y;
		}
		/*if (_ypos > 1.0f || _ypos < -1.f)
		{
			_delta_y = -_delta_y;
		}*/
		
		for (auto& a : _TriShape)
		{
			a[0] += _delta_x;
			a[1] += _delta_y;
		}
		_Translation = glm::mat4(1.0f);
		//_Translation = glm::translate(_Translation, glm::vec3(_xtrans, _xtrans, 0.0f)); // 이동 변환 적용
		_theta = 10.0f;
		_Rotation = glm::rotate(_Rotation, glm::radians(_theta), glm::vec3(0.f, 0.f, 1.f));
		/*	{ cosf(_theta),-sinf(_theta),0,0
			,sinf(_theta), cosf(_theta),0,0,
			0,0,1,0,
			0,0,0,1 };*/
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

void Triangle::check_Collision_dir_bounce()
{
	switch (_dir)
	{
	case dir_up:
		if (_xpos >= 1.f)
		{
			rotate90DegreesZ();
			_dir = dir_left;
			_xspd = -_xspd;
		}
		else if (_xpos <= -1.f)
		{
			rotate_m_90DegreesZ();
			_dir = dir_right;
			_xspd = -_xspd;
		}
		if(_ypos >= 1.f )
		{
			rotate180DegreesZ();
			_dir = dir_down;
			_yspd = -_yspd;
		}
		break;
	case dir_left:
		if (_xpos <= -1.f)
		{
			rotate180DegreesZ();
			_dir = dir_right;
			_xspd = -_xspd;
		}
		if (_ypos >= 1.f)
		{
			rotate90DegreesZ();
			_dir = dir_down;
			_yspd = -_yspd;
		}
		else if (_ypos <= -1.f)
		{
			rotate_m_90DegreesZ();
			_dir = dir_up;
			_yspd = -_yspd;
		}
		break;
	case dir_down:
		if (_xpos >= 1.f)
		{
			rotate_m_90DegreesZ();
			_dir = dir_left;
			_xspd = -_xspd;
		}
		else if (_xpos <= -1.f)
		{
			rotate90DegreesZ();
			_dir = dir_right;
			_xspd = -_xspd;
		}
		if (_ypos <= -1.f)
		{
			rotate180DegreesZ();
			_dir = dir_up;
			_yspd = -_yspd;
		}
		break;
	case dir_right:
		if (_xpos >= 1.f)
		{
			rotate180DegreesZ();
			_dir = dir_left;
			_xspd = -_xspd;
		}
		if (_ypos >= 1.f)
		{
			rotate_m_90DegreesZ();
			_dir = dir_down;
			_yspd = -_yspd;
		}
		else if (_ypos <= -1.f)
		{
			rotate90DegreesZ();
			_dir = dir_up;
			_yspd = -_yspd;
		}
		break;
	default:
		break;
	}
}

void Triangle::check_Collision_dir_zigzag()
{

	switch (_dir)
	{
	case dir_up:
		if (_xpos >= 1.f)
		{
			rotate90DegreesZ();
			_dir = dir_left;
			_xspd = -_xspd;
		}
		else if (_xpos <= -1.f)
		{
			rotate_m_90DegreesZ();
			_dir = dir_right;
			_xspd = -_xspd;
		}
		if (_ypos >= 1.f)
		{
			rotate180DegreesZ();
			_dir = dir_down;
			_yspd = -_yspd;
		}
		break;
	case dir_left:
		if (_xpos <= -1.f)
		{
			rotate180DegreesZ();
			_dir = dir_right;
			_xspd = -_xspd;
			_ycount = 5;
		}
		if (_ypos >= 1.f)
		{
			_yspd = -_yspd;
		}
		else if (_ypos <= -1.f)
		{
			_yspd = -_yspd;
		}
		break;
	case dir_right:
		if (_xpos >= 1.f)
		{
			rotate180DegreesZ();
			_dir = dir_left;
			_xspd = -_xspd;
			_ycount = 5;
		}
		if (_ypos >= 1.f)
		{
			_yspd = -_yspd;
		}
		else if (_ypos <= -1.f)
		{
			_yspd = -_yspd;
		}
		break;
	case dir_down:
		if (_xpos >= 1.f)
		{
			rotate_m_90DegreesZ();
			_dir = dir_left;
			_xspd = -_xspd;
		}
		else if (_xpos <= -1.f)
		{
			rotate90DegreesZ();
			_dir = dir_right;
			_xspd = -_xspd;
		}
		if (_ypos <= -1.f)
		{
			rotate180DegreesZ();
			_dir = dir_up;
			_yspd = -_yspd;
		}
		break;
	default:
		break;
	}
}

void Triangle::check_Collision_dir_spiral_rect()
{
	{
		if (_limit_x < 0.2f)
		{
			_delta_x = -_delta_x;
			_xspd = -_xspd;
			_limit_x = 0.2f; // limit 값을 다시 증가시킴
		}
		else if (_limit_x > 1.f)
		{
			_delta_x = -_delta_x;
			_xspd = -_xspd;
			_limit_x = 1.f; // limit 값을 다시 감소시킴
		}
		if (_limit_y < 0.2f)
		{
			_delta_y = -_delta_y;
			_yspd = -_yspd;
			_limit_y = 0.2f; // limit 값을 다시 증가시킴
		}
		else if (_limit_y > 1.f)
		{
			_delta_y = -_delta_y;
			_yspd = -_yspd;
				_limit_y = 1.f; // limit 값을 다시 감소시킴
		}
	

		switch (_dir)
		{
		case dir_up:
			if (_xpos > _limit_x)
			{
				rotate90DegreesZ();
				_dir = dir_left;
				_xspd = -_xspd;
				_horizon = false;
				_limit_x -= _delta_x;
				for (auto& a : _TriShape)
					a[0] -= _delta_x;
				_xpos -= _delta_x;
			}
			else if (_xpos < -_limit_x)
			{
				rotate_m_90DegreesZ();
				_dir = dir_right;
				_xspd = -_xspd;
				_horizon = false;
				_limit_x -= _delta_x;
				for (auto& a : _TriShape)
					a[0] += _delta_x;
				_xpos += _delta_x;
			}
			if (_ypos > _limit_y)
			{
				rotate180DegreesZ();
				_dir = dir_down;
				_yspd = -_yspd;
				_horizon = true;
				_limit_y -= _delta_y;
				for (auto& a : _TriShape)
					a[1] -= _delta_y;
				_ypos -= _delta_y;
			}
			break;
		case dir_left:
			if (_xpos < -_limit_x)
			{
				rotate180DegreesZ();
				_dir = dir_right;
				_xspd = -_xspd;
				_horizon = false;
				_limit_x -= _delta_x;
				for (auto& a : _TriShape)
					a[0] += _delta_x;
				_xpos += _delta_x;
			}
			if (_ypos > _limit_y)
			{
				rotate90DegreesZ();
				_dir = dir_down;
				_yspd = -_yspd;
				_horizon = true;
				_limit_y -= _delta_y;
				for (auto& a : _TriShape)
					a[1] -= _delta_y;
				_ypos -= _delta_y;
			}
			else if (_ypos < -_limit_y)
			{
				rotate_m_90DegreesZ();
				_dir = dir_up;
				_yspd = -_yspd;
				_horizon = true;
				_limit_y -= _delta_y;
				for (auto& a : _TriShape)
					a[1] += _delta_y;
				_ypos += _delta_y;
			}
			break;
		case dir_down:
			if (_xpos > _limit_x)
			{
				rotate_m_90DegreesZ();
				_dir = dir_left;
				_xspd = -_xspd;
				_horizon = false;
				_limit_x -= _delta_x;
				for (auto& a : _TriShape)
					a[0] -= _delta_x;
				_xpos -= _delta_x;
			}
			else if (_xpos < -_limit_x)
			{
				rotate90DegreesZ();
				_dir = dir_right;
				_xspd = -_xspd;
				_horizon = false;
				_limit_x -= _delta_x;
				for (auto& a : _TriShape)
					a[0] += _delta_x;
				_xpos += _delta_x;
			}
			if (_ypos <= -_limit_y)
			{
				rotate180DegreesZ();
				_dir = dir_up;
				_yspd = -_yspd;
				_horizon = true;
				_limit_y -= _delta_y;
				for (auto& a : _TriShape)
					a[1] += _delta_y;
				_ypos += _delta_y;
			}
			break;
		case dir_right:
			if (_xpos > _limit_x)
			{
				rotate180DegreesZ();
				_dir = dir_left;
				_xspd = -_xspd;
				_horizon = false;
				_limit_x -= _delta_x;
				for (auto& a : _TriShape)
					a[0] -= _delta_x;
				_xpos -= _delta_x;
			}
			if (_ypos >= _limit_y)
			{
				rotate_m_90DegreesZ();
				_dir = dir_down;
				_yspd = -_yspd;
				_horizon = true;
				_limit_y -= _delta_y;
				for (auto& a : _TriShape)
					a[1] -= _delta_y;
				_ypos -= _delta_y;

			}
			else if (_ypos < -_limit_y)
			{
				rotate90DegreesZ();
				_dir = dir_up;
				_yspd = -_yspd;
				_horizon = true;
				_limit_y -= _delta_y;
				for (auto& a : _TriShape)
					a[1] += _delta_y;
				_ypos += _delta_y;
			}
			break;
		default:
			break;
		}
	}
	
//if (_limit_x <= 0.2f)
//{
//	_delta_x = -_delta_x;
//	_xspd = -_xspd;
//	_limit_x = 0.21f;
//}
//else if (_limit_x > 1.f)
//{
//	_delta_x = -_delta_x;
//	_xspd = -_xspd;
//	_limit_x = 0.99f;
//}
//if (_limit_y <= 0.2f)
//{
//	_delta_y = -_delta_y;
//	_yspd = -_yspd;
//	_limit_y = 0.21f;
//}
//else if (_limit_y > 1.f)
//{
//	_delta_y = -_delta_y;
//	_yspd = -_yspd;
//	_limit_y = 0.99f;
//}
//switch (_dir)
//{
//case dir_up:
//	if (_xpos >= _limit_x)
//	{
//		rotate90DegreesZ();
//		_dir = dir_left;
//		_xspd = -_xspd;
//		_horizon = false;
//		_limit_x -= _delta_x;
//		for (auto& a : _TriShape)
//			a[0] -= _delta_x;
//		_xpos -= _delta_x;
//	}
//	else if (_xpos <= -_limit_x)
//	{
//		rotate_m_90DegreesZ();
//		_dir = dir_right;
//		_xspd = -_xspd;
//		_horizon = false;
//		_limit_x -= _delta_x;
//		for (auto& a : _TriShape)
//			a[0] += _delta_x;
//		_xpos += _delta_x;
//	}
//	if (_ypos >= _limit_y)
//	{
//		rotate180DegreesZ();
//		_dir = dir_down;
//		_yspd = -_yspd;
//		_horizon = true;
//		_limit_y -= _delta_y;
//		for (auto& a : _TriShape)
//			a[1] -= _delta_y;
//		_ypos -= _delta_y;
//	}
//	break;
//case dir_left:
//	if (_xpos <= -_limit_x)
//	{
//		rotate180DegreesZ();
//		_dir = dir_right;
//		_xspd = -_xspd;
//		_horizon = false;
//		_limit_x -= _delta_x;
//		for (auto& a : _TriShape)
//			a[0] += _delta_x;
//		_xpos += _delta_x;
//	}
//	if (_ypos >= _limit_y)
//	{
//		rotate90DegreesZ();
//		_dir = dir_down;
//		_yspd = -_yspd;
//		_horizon = true;
//		_limit_y -= _delta_y;
//		for (auto& a : _TriShape)
//			a[1] -= _delta_y;
//		_ypos -= _delta_y;
//	}
//	else if (_ypos <= -_limit_y)
//	{
//		rotate_m_90DegreesZ();
//		_dir = dir_up;
//		_yspd = -_yspd;
//		_horizon = true;
//		_limit_y -= _delta_y;
//		for (auto& a : _TriShape)
//			a[1] += _delta_y;
//		_ypos += _delta_y;
//	}
//	break;
//case dir_down:
//	if (_xpos >= _limit_x)
//	{
//		rotate_m_90DegreesZ();
//		_dir = dir_left;
//		_xspd = -_xspd;
//		_horizon = false;
//		_limit_x -= _delta_x;
//		for (auto& a : _TriShape)
//			a[0] -= _delta_x;
//		_xpos -= _delta_x;
//	}
//	else if (_xpos <= -_limit_x)
//	{
//		rotate90DegreesZ();
//		_dir = dir_right;
//		_xspd = -_xspd;
//		_horizon = false;
//		_limit_x -= _delta_x;
//		for (auto& a : _TriShape)
//			a[0] += _delta_x;
//		_xpos += _delta_x;
//	}
//	if (_ypos <= -_limit_y)
//	{
//		rotate180DegreesZ();
//		_dir = dir_up;
//		_yspd = -_yspd;
//		_horizon = true;
//		_limit_y -= _delta_y;
//		for (auto& a : _TriShape)
//			a[1] += _delta_y;
//		_ypos += _delta_y;
//	}
//	break;
//case dir_right:
//	if (_xpos >= _limit_x)
//	{
//		rotate180DegreesZ();
//		_dir = dir_left;
//		_xspd = -_xspd;
//		_horizon = false;
//		_limit_x -= _delta_x;
//		for (auto& a : _TriShape)
//			a[0] -= _delta_x;
//		_xpos -= _delta_x;
//	}
//	if (_ypos >= _limit_y)
//	{
//		rotate_m_90DegreesZ();
//		_dir = dir_down;
//		_yspd = -_yspd;
//		_horizon = true;
//		_limit_y -= _delta_y;
//		for (auto& a : _TriShape)
//			a[1] -= _delta_y;
//		_ypos -= _delta_y;
//
//	}
//	else if (_ypos <= -_limit_y)
//	{
//		rotate90DegreesZ();
//		_dir = dir_up;
//		_yspd = -_yspd;
//		_horizon = true;
//		_limit_y -= _delta_y;
//		for (auto& a : _TriShape)
//			a[1] += _delta_y;
//		_ypos += _delta_y;
//	}
//	break;
//default:
//	break;
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
	
	for(auto &a : _TriShape)
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
	for (auto& a : _TriShape)
	{
		float x = a[0] - _xpos;
		float y = a[1] - _ypos;
		float x_new = -x;
		float y_new = -y;
		a[0] = x_new + _xpos;
		a[1] = y_new + _ypos;
	}
}

void Triangle::move_pos_()
{

}


