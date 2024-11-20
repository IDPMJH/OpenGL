#include <queue>
#include <algorithm>
#include <iostream>
#include "Rect.h"
#include "global.h"
#include "Line.h"
#include "Triangle.h"




GLfloat Random_0_to_1f();

float r = Random_0_to_1f();
float g = Random_0_to_1f();
float b = Random_0_to_1f();
Rect_p::Rect_p()
	:Polygon(mode_Rect)
	, _RectShape{0}
	, _Colors{0}
{
}

Rect_p::~Rect_p()
{
}

Rect_p::Rect_p(float xpos, float ypos, float offset)
	:Polygon(mode_Rect)
	, _RectShape{
		{xpos - offset, ypos + offset, 0.f},
		{xpos - offset, ypos - offset, 0.f},
		{xpos + offset, ypos + offset, 0.f},

		{xpos + offset, ypos + offset, 0.f},
		{xpos - offset, ypos - offset, 0.f},
		{xpos + offset, ypos - offset, 0.f}
	}
	, _vertices{
		{xpos - offset, ypos + offset, 0.f},
		{xpos - offset, ypos - offset, 0.f},
		{xpos + offset, ypos - offset, 0.f},
		{xpos + offset, ypos + offset, 0.f}
	}
	, _Colors{
		{r,g,b},
		{r,g,b},
		{r,g,b},

		{r,g,b},
		{r,g,b},
		{r,g,b}
	}
	,_colors{ {r,g,b},{r,g,b},{r,g,b},{r,g,b},{r,g,b} }
	,_dirx(0)
{
	_triangles.emplace_back(Triangle(vec3{ xpos - offset, ypos + offset, 0.f }, vec3{ xpos - offset, ypos - offset, 0.f }, vec3{ xpos + offset, ypos + offset, 0.f }, _colors));
	_triangles.emplace_back(Triangle(vec3{ xpos + offset, ypos + offset, 0.f }, vec3{ xpos - offset, ypos - offset, 0.f }, vec3{ xpos + offset, ypos - offset, 0.f }, _colors));
	
}

Rect_p::Rect_p(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vector<vec3> color)
	:Polygon(mode_Rect)
	, _vertices{ p1,p2,p3,p4 }
	, _colors{ color }
{
}

Rect_p::Rect_p(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 p5, vector<vec3> color)
	:Polygon(mode_Rect)
	, _vertices{ p1,p2,p3,p4,p5 }
	, _colors{ color }
{
}

void Rect_p::move_On_dir(char dir)
{
	
}
void Rect_p::Update()
{
	// move_on_dir_x
	if (_dirx > 0)
	{
		for (auto& a : _vertices)
		{
			a[0] += 0.01;
		}
		
	}
	else if (_dirx < 0)
	{
		for (auto& a : _vertices)
		{
			a[0] -= 0.01;
		}
		
	}

	vec3 center = (_vertices[0] + _vertices[2]) / 2.f;

	if (center.y < -1.2 || center.x > 1.2 || center.x < -1.2)
	{
		_dead = true;
	}
}
void Rect_p::Draw_Polygon()
{
	glDrawArrays(GL_POLYGON, 0, _vertices.size());
}

void Rect_p::init_buffer_polygon(GLuint* vao, GLuint* vbo)
{
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- 변수 triShape에서 버텍스 데이터 값을 버퍼에 복사한다.
	// rect size = 18
	glBufferData(GL_ARRAY_BUFFER, 3 * _vertices.size() * sizeof(GLfloat), get_vertex_ptr(), GL_STATIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	// 
	//--- colors 배열의 사이즈: 18 = rect
	glBufferData(GL_ARRAY_BUFFER, 3* _vertices.size() * sizeof(GLfloat), get_color_ptr(), GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

void Rect_p::Split(Line* line, std::vector<Polygon*>& vpolygons)
{
	std::vector<vec3> intersections;

	// 직사각형과 선의 교차점 계산
	if (!lineIntersectRect(line, intersections) || intersections.size() != 2)
	{
		// 교차점이 없거나 두 개가 아닌 경우 분할 불가
		return;
	}

	// 교차점 정렬
	vec3 inter1 = intersections[0];
	vec3 inter2 = intersections[1];

	vec3 p1 = line->_lineshape[0];
	vec3 p2 = line->_lineshape[1];

	// 교차점이 수평선에 있는지 확인
    if ((inter1.y < _vertices[0].y  && inter1.y > _vertices[2].y)
        && (inter2.y < _vertices[0].y&& inter2.y > _vertices[2].y))
	{
		if (inter1.x > inter2.x)
			swap(inter1, inter2);
		// 기울기 계산
		if (p2.x - p1.x == 0)
		{
			p2.x += 0.0001;
		}
		float m = (p2.y - p1.y) / (p2.x - p1.x);
		
		// 수평선
		std::vector<vec3> topVertices = { _vertices[0], inter1, inter2, _vertices[3] };
		std::vector<vec3> bottomVertices = { inter1, _vertices[1], _vertices[2], inter2 };

		Rect_p* temp1 = new Rect_p(topVertices[0], topVertices[1], topVertices[2], topVertices[3], _colors);
		Rect_p* temp2 = new Rect_p(bottomVertices[0], bottomVertices[1], bottomVertices[2], bottomVertices[3], _colors);

		if (m > 0)
		{
			temp1->_dirx = -1;
			temp2->_dirx = 1;
		}
		else
		{
			temp1->_dirx = 1;
			temp2->_dirx = -1;
		}

		temp1->_splited = true;
		temp2->_splited = true;

		vpolygons.push_back(temp1);
		vpolygons.push_back(temp2);
	}
	// 교차점이 수직선에 있는지 확인
	else if ((inter1.x > _vertices[0].x && inter1.x < _vertices[2].x)
			&& (inter2.x > _vertices[0].x && inter1.x < _vertices[2].x))
	{
		if (inter1.y < inter2.y)
			swap(inter1, inter2);
		// 기울기 계산
		float m = (p2.y - p1.y) / (p2.x - p1.x);
		// 수직선
		std::vector<vec3> leftVertices = { _vertices[0], _vertices[1], inter2, inter1 };
		std::vector<vec3> rightVertices = { inter1, inter2, _vertices[2], _vertices[3] };

		Rect_p* temp1 = new Rect_p(leftVertices[0], leftVertices[1], leftVertices[2], leftVertices[3], _colors);
		Rect_p* temp2 = new Rect_p(rightVertices[0], rightVertices[1], rightVertices[2], rightVertices[3], _colors);


		if (m > 0)
		{
			temp1->_dirx = -1;
			temp2->_dirx = 1;
		}
		else
		{
			temp1->_dirx = -1;
			temp2->_dirx = 1;
		}

		temp1->_splited = true;
		temp2->_splited = true;
		vpolygons.push_back(temp1);
		vpolygons.push_back(temp2);
	}
    else
    {
		// 사선
		if (inter1.x > inter2.x)
			swap(inter1, inter2);

		if (inter1.y < _vertices[0].y && inter1.y > _vertices[1].y)
		{
			if (inter1.y < inter2.y)
			{
				Triangle* temp1 = new Triangle(_vertices[0], inter1, inter2, _colors);
				Rect_p* temp2 = new Rect_p(inter1, _vertices[1], _vertices[2], _vertices[3], inter2, _colors);
				temp1->_dirx = dir_left;
				temp2->_dirx = 1;

				temp1->_splited = true;
				temp2->_splited = true;
				vpolygons.push_back(temp1);
				vpolygons.push_back(temp2);
			}
			else if (inter1.y > inter2.y)
			{
				Triangle* temp1 = new Triangle(_vertices[1], inter2, inter1, _colors);
				Rect_p* temp2 = new Rect_p(inter1, inter2, _vertices[2], _vertices[3], _vertices[0], _colors);
				
				temp1->_dirx = dir_left;
				temp2->_dirx = 1;

				vpolygons.push_back(temp1);
				vpolygons.push_back(temp2);

				temp1->_splited = true;
				temp2->_splited = true;
			}
		}
		else if (inter2.y < _vertices[3].y && inter2.y > _vertices[2].y)
		{
			if (inter1.y > inter2.y)
			{
				Triangle* temp1 = new Triangle(inter1, inter2,_vertices[3], _colors);
				Rect_p* temp2 = new Rect_p(_vertices[0], _vertices[1], _vertices[2], inter2,inter1, _colors);
				temp1->_dirx = dir_right;
				temp2->_dirx = -1;

				temp1->_splited = true;
				temp2->_splited = true;
				vpolygons.push_back(temp1);
				vpolygons.push_back(temp2);
			}
			else if (inter1.y < inter2.y)
			{
				Triangle* temp1 = new Triangle(_vertices[2], inter1, inter2, _colors);
				Rect_p* temp2 = new Rect_p(_vertices[0], _vertices[1], inter1, inter2, _vertices[3], _colors);
				temp1->_dirx = dir_right;
				temp2->_dirx = -1;

				temp1->_splited = true;
				temp2->_splited = true;
				vpolygons.push_back(temp1);
				vpolygons.push_back(temp2);
			}
		}
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

bool Rect_p::lineIntersect(const vec3& a1, const vec3& a2, const vec3& b1, const vec3& b2, vec3& intersection)
{
	// 선분 교차점 계산
	vec3 s1 = a2 - a1;
	vec3 s2 = b2 - b1;

	float s, t;
	s = (-s1.y * (a1.x - b1.x) + s1.x * (a1.y - b1.y)) / (-s2.x * s1.y + s1.x * s2.y);
	t = (s2.x * (a1.y - b1.y) - s2.y * (a1.x - b1.x)) / (-s2.x * s1.y + s1.x * s2.y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// 교차점 계산
		intersection = a1 + (t * s1);
		return true;
	}

	return false; // 교차점 없음
}

bool Rect_p::lineIntersectRect(Line* line, std::vector<vec3>& intersections)
{
	vec3 p1 = line->_lineshape[0];
	vec3 p2 = line->_lineshape[1];

	vec3 intersection;
	bool found = false;

	// 사각형의 각 변과 선분의 교차점 계산
	for (int i = 0; i < 4; ++i)
	{
		vec3 v1 = _vertices[i];
		vec3 v2 = _vertices[(i + 1) % 4];

		if (lineIntersect(v1, v2, p1, p2, intersection))
		{
			intersections.push_back(intersection);
			found = true;
		}
	}

	return found;
}


GLfloat* Rect_p::get_vertex_ptr()
{
	return (GLfloat*)_vertices.data();
}

GLfloat* Rect_p::get_color_ptr()
{
	return (GLfloat*)_colors.data();
}


