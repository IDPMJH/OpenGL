#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include "Rect_3.h"

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
	glutMotionFunc(Mouse_Down_Move);	// 마우스 누른 채로 이동할 때 호출 될 함수
	glutMouseFunc(Mouse);		// 마우스 누름 / 뗌 호출 함수
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glutMainLoop(); //--- 이벤트 처리 시작
}



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

static bool on_timer = false;
GLvoid Keyboard(unsigned char key, int x, int y)
{
	if(key == 'a')
	{
		if(v_rc.size() <= 9)
		{
			// 사각형 생성
			float a = Random_Number<float>(-90, 90) / 100.f;
			float b = Random_Number<float>(-90, 90) / 100.f;
			vPos<float> vPos_temp(a,b);
			Rect rc_temp(vPos_temp, 0.1f, 0.1f);
			v_rc.emplace_back(rc_temp);
		}

	}

	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

static float pre_xpos;
static float pre_ypos;

GLvoid Mouse_Down_Move(int x, int y)
{
	// 마우스 좌표 변환
	float cur_xpos = x;
	float cur_ypos = y;
	Mouse_Convert_Win_To_OpenGL(cur_xpos, cur_ypos, window_x, window_y);
	 

	float x_offset = cur_xpos - pre_xpos;
	float y_offset = cur_ypos - pre_ypos;

	
	vPos<float> pos(x_offset, y_offset);

	for (Rect& rc : v_rc)
	{
		if (rc._clicked == true)
			rc.Update(pos);
	}

	pre_xpos = cur_xpos;
	pre_ypos = cur_ypos;

	glutPostRedisplay();
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
			for (Rect& rc : v_rc)
			{
				rc.Left_Click(xpos, ypos);
			}
		}
		else if (state == GLUT_UP)
		{
			for (auto rc = v_rc.begin(); rc != v_rc.end(); ++rc)
			{
				if (rc->_clicked == true)
					rc->Merge_Check(v_rc);
			}
			float min_width = 1.f, min_height = 1.f;
			float max_width = 0.f, max_height = 0.f;
			float merge_count = 0.f;
			vPos<float> center(0.f, 0.f);
			for (auto it = v_rc.begin(); it != v_rc.end();)
			{
				if (it->_merged == true)
				{
					++merge_count;
					if (min_width > it->_width)
						min_width = it->_width;
					if (min_height > it->_height)
						min_height = it->_height;
					if (max_width < it->_width)
						max_width = it->_width;
					if (max_height < it->_height)
						max_height = it->_height;
					center = center + (it->_center);
					it = v_rc.erase(it);
				}
				else
					++it;
			}
			if(merge_count != 0)
			{
				center._x = center._x / merge_count;
				center._y = center._y / merge_count;
				{
				}

				Rect temp(center, min_width + max_width, min_height + max_height);
				v_rc.emplace_back(temp);
			}
			for (auto ite = v_rc.begin(); ite != v_rc.end();)
			{
				if (ite->_merged == true)
				{
					std::cout << "Erasing: " << ite->_width << " x " << ite->_height << std::endl;
					ite = v_rc.erase(ite);
				}
				else
				{
					++ite;
				}
			}
			std::cout << "v_rc size after erase: " << v_rc.size() << std::endl;
		}
	}
		break;
	default:
		break;
	}

	glutPostRedisplay();
}



// 9. 마우스 이동 입력
// void glutMotionFunc(void(*func)(int x, int y));
// 마우스 버튼을 누른 채 마우스를 움직일 때 호출될 콜백 함수를 등록한다.
// void glutPassiveMotionFunc(void(*func)(int x,int y));
// 마우스 버튼을 누르지 않은 채 마우스를 움직일 때 호출될 함수 등록


//void UserTimerFunc(int value)
//{
//	random_device rd;
//	mt19937 mt(rd());
//	uniform_int_distribution<int> id(1, 10);
//
//	float r = (float)id(mt);
//	float g = (float)id(mt);
//	float b = (float)id(mt);
//	glClearColor(1.f / r, 1.f / g, 1.f / b, 1.f);
//	glutPostRedisplay();
//	if (on_timer)
//		glutTimerFunc(500, UserTimerFunc, 1);
//}