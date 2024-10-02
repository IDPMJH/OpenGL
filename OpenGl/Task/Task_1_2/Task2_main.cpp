#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>
#include "Component.h"
#include "Rect_2.h"

static int window_x = 800;
static int window_y = 600;


using namespace std;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Win_Resize(int w, int h);
void UserTimerFunc(int value);


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
	if (glewInit() != GLEW_OK
		) //--- glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl
			;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	glutDisplayFunc(drawScene); //--- 출력 콜백함수의 지정
	glutReshapeFunc(Reshape); //--- 다시 그리기 콜백함수 지정
	//glutKeyboardFunc(Keyboard); //--- 키보드 입력 콜백함수 지정
	glutReshapeFunc(Win_Resize);
	glutMouseFunc(Mouse); // 마우스 입력 콜백함수 지정
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glutMainLoop(); //--- 이벤트 처리 시작
}

//Rect_2(int left, int right, int top, int bottom, int r, int g, int b, int qurdrant);
Rect_2 rc1(0, 1, 1, 0, 1, 1, 1, 1);
Rect_2 rc2(-1, 0, 1, 0, 1, 1, 1, 2);
Rect_2 rc3(-1, 0, 0, -1, 1, 1, 1, 3);
Rect_2 rc4(0, 1, 0, -1, 1, 1, 1, 4);


GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	//glClearColor(0.f, 1.f, 0.f, 1.f); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

	glColor3f(rc1._r, rc1._g, rc1._b);
	glRectf(rc1._left,rc1._top,rc1._right,rc1._bottom);	// 1사분면

	glColor3f(rc2._r, rc2._g, rc2._b);
	glRectf(rc2._left,rc2._top,rc2._right,rc2._bottom);	// 2사분면

	glColor3f(rc3._r, rc3._g, rc3._b);
	glRectf(rc3._left,rc3._top,rc3._right,rc3._bottom);	// 3사분면

	glColor3f(rc4._r, rc4._g, rc4._b);
	glRectf(rc4._left,rc4._top,rc4._right,rc4._bottom);	// 4사분면
	
	glutSwapBuffers(); //--- 화면에 출력하기
}

// void glColor3f(Glfloat r, Glfloat g, Glfloat b);
// 현재 색상 설정하기
// (r, g, b) : red, green, blue 색상, 0.0 ~1.0 사이의 값으로 glRectf 를 호출하기 직전 설정한다.

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid Win_Resize(int w, int h)
{
	window_x = w;
	window_y = h;
}

static bool on_timer = false;

// 8. 마우스 입력
// void glutMouseFunc(void(&func)(int button, int state, int x, int y));
// 마우스 버튼과 인자로 지정한 루틴을 연결하여 호출되도록 한다.
// button (버튼 파라미터): GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
// state(상태 파라미터) : GLUT_UP, GLUT_DOWN
// x, y : 윈도우에서 마우스의 위치
GLvoid Mouse(int button, int state, int x, int y)
{
	float xpos = x;
	float ypos = y;

	Mouse_Convert_Win_To_OpenGL(xpos, ypos, window_x, window_y);
	
	Rect_terminous check;

	switch (button)
	{
	case GLUT_LEFT_BUTTON: // 사각형 내부 - 색상 랜덤, 사각형 외부 - 배경색 랜덤
		if(state == GLUT_DOWN)
		{
			rc1.Left_Click(xpos, ypos);
			rc2.Left_Click(xpos, ypos);
			rc3.Left_Click(xpos, ypos);
			rc4.Left_Click(xpos, ypos);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if(state == GLUT_DOWN)
		{
			rc1.Right_Click(xpos, ypos);
			rc2.Right_Click(xpos, ypos);
			rc3.Right_Click(xpos, ypos);
			rc4.Right_Click(xpos, ypos);
		}
		break;

	default:
		break;
	}

	glutPostRedisplay();
}


GLvoid Keyboard(unsigned char key, int x, int y)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> id(1, 10);

	float r = (float)id(mt);
	float g = (float)id(mt);
	float b = (float)id(mt);
	switch (key) {
	case 'c'://--- 배경색을 청록색으로 설정
		/*glClearColor(0.f, 1.f, 1.f, 1.f);*/
		
		glColor3f(0, 1, 1);
		break;
	case 'm'://--- 배경색을 자홍색으로 설정
		glClearColor(1.f, 0.f, 0.f, 1.f);
		break;
	case 'y'://--- 배경색을 노랑색으로 설정
		glClearColor(1.f, 1.f, 0.f, 1.f);
		break;
	case 'a': // 랜덤
		glClearColor(1.f / r, 1.f / g, 1.f / b, 1.f);
		break;
	case 'w'://--- 배경색을 하얀색으로 설정
		glClearColor(1.f, 1.f, 1.f, 1.f);
		break;
	case 'k'://--- 배경색을 검은색으로 설정
		glClearColor(0.f, 0.f, 0.f, 1.f);
		break;
	case 't': // 타이머 설정, 특정 시간마다 랜덤으로 색 변경
		on_timer = true;
		glutTimerFunc(500, UserTimerFunc, 1);
		break;
	case 's': // 타이머 종료
		on_timer = false;
		break;
	case 'q':
		glutLeaveMainLoop();
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

void UserTimerFunc(int value)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> id(1, 10);

	float r = (float)id(mt);
	float g = (float)id(mt);
	float b = (float)id(mt);
	glClearColor(1.f / r, 1.f / g, 1.f / b, 1.f);
	glutPostRedisplay();
	if (on_timer)
		glutTimerFunc(500, UserTimerFunc, 1);
}