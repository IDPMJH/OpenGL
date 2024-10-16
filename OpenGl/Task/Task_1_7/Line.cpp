#include "Line.h"
#include "global.h"

float Random_0_to_1f();

Line::Line()
	:Polygon(mode_Line)
	,_LineShape{0}
	,_Colors{0}
{
}

Line::Line(GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4, GLfloat f5, GLfloat f6)
	:Polygon(mode_Line)
	, _LineShape{ {f1,f2,f3}, {f4,f5,f6} }
	, _Colors{ Random_0_to_1f(),Random_0_to_1f() ,Random_0_to_1f()
	, Random_0_to_1f(),Random_0_to_1f() ,Random_0_to_1f() }
{
}

Line::~Line()
{
}

void Line::move_On_dir(char dir)
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
	for (auto& a : _LineShape)
	{
		a[0] += dirx;
		a[1] += diry;
	}
}

void Line::Draw_Polygon()
{
	glDrawArrays(GL_LINES, 0, 2);
}

void Line::init_buffer_polygon(GLuint* vao, GLuint* vbo)
{
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- 변수 triShape에서 버텍스 데이터 값을 버퍼에 복사한다.
	// Line size = 3
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), get_vertex_ptr(), GL_STATIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	// 
	//--- colors 배열의 사이즈: 6= Line
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), get_color_ptr(), GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

void Line::Update()
{
}

GLfloat* Line::get_vertex_ptr()
{
	return (GLfloat*)_LineShape;
}

GLfloat* Line::get_color_ptr()
{
	return (GLfloat*)_Colors;
}
