#include "Rect.h"
#include "global.h"

float Random_0_to_1f();

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
	:_RectShape{
		{xpos - offset, ypos + offset, 0.f},
		{xpos - offset, ypos - offset, 0.f},
		{xpos + offset, ypos + offset, 0.f},

		{xpos + offset, ypos + offset, 0.f},
		{xpos - offset, ypos - offset, 0.f},
		{xpos + offset, ypos - offset, 0.f}
	},
	_Colors{
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
		
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()},
		{Random_0_to_1f(),Random_0_to_1f(),Random_0_to_1f()}
	}
{
}

void Rect_p::move_On_dir(char dir)
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
	for (auto& a : _RectShape)
	{
		a[0] += dirx;
		a[1] += diry;
	}
}

void Rect_p::Draw_Polygon()
{
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
}

void Rect_p::init_buffer_polygon(GLuint* vao, GLuint* vbo)
{
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- 변수 triShape에서 버텍스 데이터 값을 버퍼에 복사한다.
	// rect size = 18
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), get_vertex_ptr(), GL_STATIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	// 
	//--- colors 배열의 사이즈: 18 = rect
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), get_color_ptr(), GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

GLfloat* Rect_p::get_vertex_ptr()
{
	return (GLfloat*)_RectShape;
}

GLfloat* Rect_p::get_color_ptr()
{
	return (GLfloat*)_Colors;
}


