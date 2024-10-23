#include "Dot_12.h"
#include "global.h"


Dot::Dot()
	:Polygon(mode_Point)
	, _PointShape{ {0.f,0.f,0.f} }
	, _Colors{ {0.f,0.f,0.f} }
{
}

float Random_0_to_1f();

Dot::Dot(float f1, float f2, float f3)
	:Polygon(mode_Point)
	, _PointShape{ {f1,f2,f3} }
	, _Colors{ {Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()} }
	,_xpos(f1)
	,_ypos(f2)
	,_vcount(1)
	,_offset(0.05)
{
}

Dot::~Dot()
{
}

void Dot::move_On_dir(char dir)
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
	_PointShape[0] += dirx;
	_PointShape[1] += diry;
}

void Dot::Draw_Polygon()
{
	glDrawArrays(GL_POINTS, 0, 1);
}

void Dot::init_buffer_polygon(GLuint* vao, GLuint* vbo)
{
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- ���� triShape���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// dot size = 3
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), get_vertex_ptr(), GL_STATIC_DRAW);

	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	// 
	//--- colors �迭�� ������: 3 = dot
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), get_color_ptr(), GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);

}

void Dot::Change_Color(GLfloat r, GLfloat g, GLfloat b)
{
}

void Dot::Update(glm::vec2 vpos)
{
	_xpos += vpos.x;
	_ypos += vpos.y;


	// ������ �ʱ�ȭ
	_vpos.x = _xpos;
	_vpos.y = _ypos;
	_width = _height = _offset * 2;

	_PointShape = vector<glm::vec3>{ {_xpos,_ypos,0.f } };
}

void Dot::Merge_Check(vector<Polygon*> Polygons)
{
	if (this == nullptr)
		assert(this);
	//if (clicked == nullptr)
	//	return;	�̹� ������ üũ��
	//vector<Rect>::iterator r_i;
	for (auto i = Polygons.begin(); i != Polygons.end(); ++i)
	{

		if (*i == this)
		{
			// �ڱ� �ڽ��� ���
			continue;
		}
		float length_x = i._xpos - this->_center;

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

GLfloat* Dot::get_vertex_ptr()
{
	return (GLfloat * )_PointShape.data();
}

GLfloat* Dot::get_color_ptr()
{
	return (GLfloat * )_Colors.data();
}

void Dot::set_color_ptr(GLfloat f1, GLfloat f2, GLfloat f3)
{
	_Colors[0] = { f1, f2, f3 };
};


void Dot::Left_Click(float mouse_x, float mouse_y)
{
	if (mouse_x < _xpos +_offset && mouse_x > _xpos - _offset &&
		mouse_y < _ypos + _offset && mouse_y > _ypos - _offset)
		_clicked = true;

}
