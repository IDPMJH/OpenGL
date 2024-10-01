#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

using namespace std;
GLvoid drawScene(GLvoid);
GLvoid Reshape
(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void UserTimerFunc(int value);



void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); //--- glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //--- 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); //--- 윈도우의 위치 지정
	glutInitWindowSize(800, 600); //--- 윈도우의 크기 지정
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
	glutKeyboardFunc(Keyboard); //--- 키보드 입력 콜백함수 지정
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glutMainLoop(); //--- 이벤트 처리 시작
}



GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	//glClearColor(0.f, 1.f, 0.f, 1.f); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기
	glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

static bool on_timer = false;
GLvoid Keyboard(unsigned char key, int x, int y)
{

	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> id(1, 10);

	double r = id(mt);
	double g = id(mt);
	double b = id(mt);
	switch (key) {
	case 'c'://--- 배경색을 청록색으로 설정
		glClearColor(0.f, 1.f, 1.f, 1.f);
		break; 
	case 'm'://--- 배경색을 자홍색으로 설정
		glClearColor(1.f, 0.f, 0.f, 1.f);
		break; 
	case 'y'://--- 배경색을 노랑색으로 설정
		glClearColor(1.f, 1.f, 0.f, 1.f);
		break; 
	case 'a' : // 랜덤
		glClearColor(1.f / r, 1.f / g, 1.f / b, 1.f);
		break;
	case 'w'://--- 배경색을 하얀색으로 설정
		glClearColor(1.f, 1.f, 1.f, 1.f);
		break;
	case 'k'://--- 배경색을 검은색으로 설정
		glClearColor(0.f, 0.f, 0.f, 1.f);
		break;
	case 't' : // 타이머 설정, 특정 시간마다 랜덤으로 색 변경
		on_timer = true;
		glutTimerFunc(500, UserTimerFunc, 1);
		break;
	case 's' : // 타이머 종료
		on_timer = false;
		break;
	case 'q' :
		glutLeaveMainLoop();
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

void UserTimerFunc(int value)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> id(1, 10);

	double r = id(mt);
	double g = id(mt);
	double b = id(mt);
	glClearColor(1.f / r, 1.f / g, 1.f / b, 1.f);
	glutPostRedisplay();
	if (on_timer)
		glutTimerFunc(500, UserTimerFunc, 1);
}