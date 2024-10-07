#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
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

int main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); //--- glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //--- ���÷��� ��� ����
	glutInitWindowPosition(0, 0); //--- �������� ��ġ ����
	glutInitWindowSize(window_x, window_y); //--- �������� ũ�� ����
	glutCreateWindow("Example1"); //--- ������ ����
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) //--- glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);			//--- ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape);			//--- �ٽ� �׸��� �ݹ��Լ� ����
	glutKeyboardFunc(Keyboard);			//--- Ű���� �Է� �ݹ��Լ� ����
	glutMotionFunc(Mouse_Down_Move);	// ���콺 ���� ä�� �̵��� �� ȣ�� �� �Լ�
	glutMouseFunc(Mouse);				// ���콺 ���� / �� ȣ�� �Լ�
	glutTimerFunc(50,UserTimerFunc, 1);// Ÿ�̸� �ݹ� �Լ� 
	glClearColor(0.15f, 0.15f, 0.15f, 1.f);
	glutMainLoop(); //--- �̺�Ʈ ó�� ����
}

// 10. �ִϸ��̼� ������ ���� Ÿ�̸� ���� �Լ�
// void glutTimerFunc(unsigned int msecs,(*func)(int value), int value);
// Ÿ�� �ƿ��� �߻��� ��� ȣ��� �ݹ� �Լ��� ����Ѵ�.
// msecs: �ݹ� �Լ��� ȣ���ϱ� ������ ��ٸ� �ð� (�и����� ����)
// func : ȣ���� �Լ��� �̸�
// value : �ݹ� �Լ��� ������ ��
// �� �Լ��� �� ���� ����ǹǷ� �������� �ִϸ��̼��� ���ؼ��� Ÿ�̸� �Լ� ���� Ÿ�̸Ӹ� �ٽ� ȣ���ؾ��Ѵ�

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	//glClearColor(0.f, 1.f, 0.f, 1.f); //--- �������� ����
	glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�

	for (Rect& rc : v_rc)
	{
		glColor3f(rc._r, rc._g, rc._b);
		glRectf(rc._left, rc._top, rc._right, rc._bottom);
	}

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

static bool on_timer = true;
static bool can_click = true;
GLvoid Keyboard(unsigned char key, int x, int y)
{
	can_click = false;	// Ŭ�� �Ұ����ϰ� ����
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
	case 'r' : // �簢�� ����
		v_rc.clear();
		can_click = true;	// �ٽ� Ŭ�� ����
		break;
	default:
		can_click = true;
		break;
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
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

	// ���콺 ��ǥ ��ȯ
	Mouse_Convert_Win_To_OpenGL(xpos, ypos, window_x, window_y);
	pre_xpos = xpos;
	pre_ypos = ypos;

	Rect_terminous check;

	switch (button)
	{
	case GLUT_LEFT_BUTTON: // �簢�� ���� - ���� ����, �簢�� �ܺ� - ���� ����
	{
		if (state == GLUT_DOWN)
		{
			if (v_rc.size() < 5 && can_click == true)
			{
				// �簢�� ����
				
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



// 9. ���콺 �̵� �Է�
// void glutMotionFunc(void(*func)(int x, int y));
// ���콺 ��ư�� ���� ä ���콺�� ������ �� ȣ��� �ݹ� �Լ��� ����Ѵ�.
// void glutPassiveMotionFunc(void(*func)(int x,int y));
// ���콺 ��ư�� ������ ���� ä ���콺�� ������ �� ȣ��� �Լ� ���

