#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
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
	if (glewInit() != GLEW_OK
		) //--- glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl
			;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); //--- ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape); //--- �ٽ� �׸��� �ݹ��Լ� ����
	glutKeyboardFunc(Keyboard); //--- Ű���� �Է� �ݹ��Լ� ����
	glutMotionFunc(Mouse_Down_Move);	// ���콺 ���� ä�� �̵��� �� ȣ�� �� �Լ�
	glutMouseFunc(Mouse);		// ���콺 ���� / �� ȣ�� �Լ�
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glutMainLoop(); //--- �̺�Ʈ ó�� ����
}



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

static bool on_timer = false;
GLvoid Keyboard(unsigned char key, int x, int y)
{
	if(key == 'a')
	{
		if(v_rc.size() <= 9)
		{
			// �簢�� ����
			float a = Random_Number<float>(-90, 90) / 100.f;
			float b = Random_Number<float>(-90, 90) / 100.f;
			vPos<float> vPos_temp(a,b);
			Rect rc_temp(vPos_temp, 0.1f, 0.1f);
			v_rc.emplace_back(rc_temp);
		}

	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

static float pre_xpos;
static float pre_ypos;

GLvoid Mouse_Down_Move(int x, int y)
{
	// ���콺 ��ǥ ��ȯ
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



// 9. ���콺 �̵� �Է�
// void glutMotionFunc(void(*func)(int x, int y));
// ���콺 ��ư�� ���� ä ���콺�� ������ �� ȣ��� �ݹ� �Լ��� ����Ѵ�.
// void glutPassiveMotionFunc(void(*func)(int x,int y));
// ���콺 ��ư�� ������ ���� ä ���콺�� ������ �� ȣ��� �Լ� ���


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