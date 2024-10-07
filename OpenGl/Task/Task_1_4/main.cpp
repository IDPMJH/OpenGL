#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include "Rect_4.h"


using namespace std;
GLvoid drawScene(GLvoid);
GLvoid Reshape
(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Mouse_Down_Move(int x, int y);
void UserTimerFunc(int value);

static vector<Rect> v_rc;
int window_x = 800, window_y = 800;

int main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); //--- glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //--- 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); //--- 윈도우의 위치 지정
	glutInitWindowSize(window_x, window_y); //--- 윈도우의 크기 지정
	glutCreateWindow("Example1"); //--- 윈도우 생성
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) //--- glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);			//--- 출력 콜백함수의 지정
	glutReshapeFunc(Reshape);			//--- 다시 그리기 콜백함수 지정
	glutKeyboardFunc(Keyboard);			//--- 키보드 입력 콜백함수 지정
	glutMotionFunc(Mouse_Down_Move);	// 마우스 누른 채로 이동할 때 호출 될 함수
	glutMouseFunc(Mouse);				// 마우스 누름 / 뗌 호출 함수
	glutTimerFunc(50,UserTimerFunc, 1);// 타이머 콜백 함수 
	glClearColor(0.15f, 0.15f, 0.15f, 1.f);
	glutMainLoop(); //--- 이벤트 처리 시작
}

// 10. 애니메이션 구현을 위한 타이머 설정 함수
// void glutTimerFunc(unsigned int msecs,(*func)(int value), int value);
// 타임 아웃이 발생할 경우 호출될 콜백 함수를 등록한다.
// msecs: 콜백 함수를 호출하기 전까지 기다릴 시간 (밀리세컨 단위)
// func : 호출할 함수의 이름
// value : 콜백 함수로 전달할 값
// 이 함수는 한 번만 실행되므로 지속적인 애니메이션을 위해서는 타이머 함수 내에 타이머를 다시 호출해야한다

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	//glClearColor(0.f, 1.f, 0.f, 1.f); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

	for (Rect& rc : v_rc)
	{
		glColor3f(rc._r, rc._g, rc._b);
		glRectf(rc._left, rc._top, rc._right, rc._bottom);
	}

	glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

static bool on_timer = true;
static bool can_click = true;
GLvoid Keyboard(unsigned char key, int x, int y)
{
	can_click = false;	// 클릭 불가능하게 설정
	switch (key)
	{
	case '1' :	// changing_diagonal
		for (auto& a : v_rc)
		{
			if (a._rc_mvstatus == changing_diagonal)
				a._rc_mvstatus = base_move;
			else
			{
				a._rc_mvstatus = changing_diagonal;
				a._dir_x=(Random_Number<int>(0, 99) % 2 ? 1 : -1);
				a._dir_y=(Random_Number<int>(0, 99) % 2 ? 1 : -1);
			}
		}
		break;
	case '2':	// changing_zig_zag
		for (auto& a : v_rc)
		{
			if (a._rc_mvstatus == changing_zigzag)
				a._rc_mvstatus = base_move;
			else
			{
				Random_Number<int>(0, 99) % 2 ? a._dir_x = 1: a._dir_x = 0;
				if (a._dir_x == 0)
					Random_Number<int>(0, 99) % 2 ? a._dir_y = 1 : a._dir_y = -1;
				else if (a._dir_x != 0)
					a._dir_y = 0;


				a._rc_mvstatus = changing_zigzag;
			}
		}
		break;
	case '3':	// changing_size
		for (auto& a : v_rc)
		{
			if (a._rc_status == changing_size)
				a._rc_status = base;
			else
				a._rc_status = changing_size;
		}
		break;
	case '4':	// changing_color
		for (auto& a : v_rc)
		{
			if (a._rc_status == changing_color)
				a._rc_status = base;
			else
				a._rc_status = changing_color;
		}
		break;
	case 's':	// return to base_status
		if (on_timer == true)
			on_timer = false;
		else if (on_timer == false)
		{
			on_timer = true;
			glutTimerFunc(50, UserTimerFunc, 1);
		}
		/*for (auto& a : v_rc)
		{
			if (a._rc_status != base)
			{
				a._rc_prestatus = a._rc_status;
				a._rc_status = base;
			}
			else
				a._rc_status = a._rc_prestatus;
				
		}*/
		break;
	case 'm':	// return to base_pos
		for (auto& a : v_rc)
			a._center = a._base_center;
		break;
	case 'r' : // 사각형 삭제
		v_rc.clear();
		can_click = true;	// 다시 클릭 가능
		break;
	default:
		can_click = true;
		break;
	}

	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

static float pre_xpos;
static float pre_ypos;

GLvoid Mouse_Down_Move(int x, int y)
{
}
GLvoid Mouse(int button, int state, int x, int y)
{
	float xpos = x;
	float ypos = y;

	// 마우스 좌표 변환
	Mouse_Convert_Win_To_OpenGL(xpos, ypos, window_x, window_y);
	pre_xpos = xpos;
	pre_ypos = ypos;

	Rect_terminous check;

	switch (button)
	{
	case GLUT_LEFT_BUTTON: // 사각형 내부 - 색상 랜덤, 사각형 외부 - 배경색 랜덤
	{
		if (state == GLUT_DOWN)
		{
			if (v_rc.size() < 5 && can_click == true)
			{
				// 사각형 생성
				
				vPos<float> vPos_temp(xpos, ypos);
				Rect rc_temp(vPos_temp, 0.1f, 0.1f);
				v_rc.emplace_back(rc_temp);
			}
		}
	}
	break;
	default:
		break;
	}

	glutPostRedisplay();
}

void UserTimerFunc(int value)
{
	for (auto& a : v_rc)
	{
		a.Update();
	}
	glutPostRedisplay();
	if (on_timer)
		glutTimerFunc(50, UserTimerFunc, 1);
}



// 9. 마우스 이동 입력
// void glutMotionFunc(void(*func)(int x, int y));
// 마우스 버튼을 누른 채 마우스를 움직일 때 호출될 콜백 함수를 등록한다.
// void glutPassiveMotionFunc(void(*func)(int x,int y));
// 마우스 버튼을 누르지 않은 채 마우스를 움직일 때 호출될 함수 등록

