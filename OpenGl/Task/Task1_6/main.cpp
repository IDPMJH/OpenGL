#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include "Rect_6.h"

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
	glutCreateWindow("Task_6"); //--- ������ ����
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

	int count = Random_Number<int>(5, 10);

	for (int i = 0; i < count; ++i)
	{
		// �簢�� ����
		float a = Random_Number<float>(-90, 90) / 100.f;
		float b = Random_Number<float>(-90, 90) / 100.f;
		vPos<float> vPos_temp(a, b);
		Rect rc_temp(vPos_temp, 0.1f, 0.1f);
		rc_temp._rs_die = (Rect_Status_Die)Random_Number<int>(Rect_Status_Die::die_Default + 1, Rect_Status_Die::die_end - 1);
		v_rc.emplace_back(rc_temp);
	}

	glutDisplayFunc(drawScene); //--- ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape); //--- �ٽ� �׸��� �ݹ��Լ� ����
	glutKeyboardFunc(Keyboard); //--- Ű���� �Է� �ݹ��Լ� ����
//	glutMotionFunc(Mouse_Down_Move);	// ���콺 ���� ä�� �̵��� �� ȣ�� �� �Լ�
	glutMouseFunc(Mouse);		// ���콺 ���� / �� ȣ�� �Լ�
	glutTimerFunc(50, UserTimerFunc, 1);// Ÿ�̸� �ݹ� �Լ� 
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

static bool on_timer = true;
GLvoid Keyboard(unsigned char key, int x, int y)
{
	if (key == 'r')
	{
		v_rc.clear();
		int count = Random_Number<int>(20, 40);

		for (int i = 0; i < count; ++i)
		{
			// �簢�� ����
			float a = Random_Number<float>(-90, 90) / 100.f;
			float b = Random_Number<float>(-90, 90) / 100.f;
			vPos<float> vPos_temp(a, b);
			Rect rc_temp(vPos_temp, 0.1f, 0.1f);
			rc_temp._rs_die = die_Plus;//(Rect_Status_Die)Random_Number<int>(Rect_Status_Die::die_Default + 1, Rect_Status_Die::die_end - 1);
			v_rc.emplace_back(rc_temp);
		}
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

static float pre_xpos = 0;
static float pre_ypos = 0;

GLvoid Mouse_Down_Move(int x, int y)
{
	//// ���콺 ��ǥ ��ȯ
	//float cur_xpos = x;
	//float cur_ypos = y;
	//Mouse_Convert_Win_To_OpenGL(cur_xpos, cur_ypos, window_x, window_y);


	//float x_offset = cur_xpos - pre_xpos;
	//float y_offset = cur_ypos - pre_ypos;


	//// �̵��� ����
	//vPos<float> pos(x_offset, y_offset);
	//Rect* ptemp = nullptr;
	//for (Rect& rc : v_rc)
	//{
	//	rc.Left_Click(cur_xpos, cur_ypos);
	//	if (rc._clicked == true)
	//		ptemp = &rc;
	//}
	//if (ptemp == nullptr)
	//	return;

	///*for (Rect& rc : v_rc)
	//{
	//	if (rc._clicked == true)
	//	{
	//		rc.Update(pos);
	//		rc.Merge_Check(v_rc, ptemp);
	//	}

	//}*/
	//ptemp->Update(pos);
	//ptemp->Merge_Check(v_rc, ptemp);
	//vPos<float> temp_center = ptemp->_center;
	//float temp_width = ptemp->_width;
	//float temp_height = ptemp->_height;
	//float temp_r = ptemp->_r;
	//float temp_g = ptemp->_g;
	//float temp_b = ptemp->_b;
	////for (auto rc = v_rc.begin(); rc != v_rc.end(); ++rc)
	////{
	////	if (rc->_clicked == true)
	////		rc->Merge_Check(v_rc);
	////}
	//// �ʱ�ȭ �� ��
	//float min_width = 1.f, min_height = 1.f;
	//float max_width = 0.f, max_height = 0.f;
	//float merge_count = 0.f;
	//vPos<float> center(0.f, 0.f);
	//for (auto it = v_rc.begin(); it != v_rc.end();)
	//{
	//	if (it->_merged == true)
	//	{
	//		++merge_count;
	//		if (min_width > it->_width)
	//			min_width = it->_width;
	//		if (min_height > it->_height)
	//			min_height = it->_height;
	//		if (max_width < it->_width)
	//			max_width = it->_width;
	//		if (max_height < it->_height)
	//			max_height = it->_height;
	//		center = center + (it->_center);
	//		it = v_rc.erase(it);
	//	}
	//	else
	//		++it;
	//}
	//if (merge_count != 0)
	//{
	//	center._x = center._x / merge_count;
	//	center._y = center._y / merge_count;

	//	Rect temp(temp_center, temp_width + 0.02f, temp_height + 0.02f);
	//	temp._r = temp_r;
	//	temp._g = temp_g;
	//	temp._b = temp_b;
	//	v_rc.emplace_back(temp);
	//}
	///*for (auto ite = v_rc.begin(); ite != v_rc.end();)
	//{
	//	if (ite->_merged == true)
	//	{
	//		std::cout << "Erasing: " << ite->_width << " x " << ite->_height << std::endl;
	//		ite = v_rc.erase(ite);
	//	}
	//	else
	//	{
	//		++ite;
	//	}
	//}
	//std::cout << "v_rc size after erase: " << v_rc.size() << std::endl;*/



	//pre_xpos = cur_xpos;
	//pre_ypos = cur_ypos;
	//glutPostRedisplay();
}
GLvoid Mouse(int button, int state, int x, int y)
{
	float xpos = x;
	float ypos = y;

	// ���콺 ��ǥ ��ȯ
	Mouse_Convert_Win_To_OpenGL(xpos, ypos, window_x, window_y);
	pre_xpos = xpos;
	pre_ypos = ypos;
	vPos<float> eraser_pos(xpos, ypos);
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
			auto it = v_rc.begin();
			for (it = v_rc.begin(); it != v_rc.end(); ++it)
			{
				if (it->_clicked == true)
					break;
			}
			if(it != v_rc.end())
			{
				if (it->_rs_die == die_Plus)
				{
					Rect parent = *it;

					Rect temp0(parent, 0);
					v_rc.push_back(temp0);
					Rect temp1(parent, 1);
					v_rc.push_back(temp1);
					Rect temp2(parent, 2);
					v_rc.push_back(temp2);
					Rect temp3(parent, 3);
					v_rc.push_back(temp3);
					
				}
				else if(it->_rs_die == die_X)
				{
					Rect parent = *it;

					Rect temp0(parent, 0);
					v_rc.push_back(temp0);
					Rect temp1(parent, 1);
					v_rc.push_back(temp1);
					Rect temp2(parent, 2);
					v_rc.push_back(temp2);
					Rect temp3(parent, 3);
					v_rc.push_back(temp3);
				}
				else if (it->_rs_die == die_Straight)
				{
					Rect parent = *it;
					int rdnum = Random_Number<int>(0, 3);
					int dirx = 0;
					int diry = 0;
					switch (rdnum)
					{
					case 0:
						dirx = -1;
						diry = 0;
						break;
					case 1:
						dirx = 0;
						diry = -1;
						break;
					case 2:
						dirx = 1;
						diry = 0;
						break;
					case 3:
						dirx = 0;
						diry = -1;
						break;
					default:
						break;
					}
					Rect temp0(parent, 0);
					temp0._dir_x = dirx;
					temp0._dir_y = diry;
					v_rc.push_back(temp0);

					Rect temp1(parent, 1);
					temp1._dir_x = dirx;
					temp1._dir_y = diry;
					v_rc.push_back(temp1);

					Rect temp2(parent, 2);
					temp2._dir_x = dirx;
					temp2._dir_y = diry;
					v_rc.push_back(temp2);

					Rect temp3(parent, 3);
					temp3._dir_x = dirx;
					temp3._dir_y = diry;
					v_rc.push_back(temp3);
					
				}
				else if (it->_rs_die == die_Eight)
				{
					Rect parent = *it;

					Rect temp0(parent, 0);
					v_rc.push_back(temp0);
					Rect temp1(parent, 1);
					v_rc.push_back(temp1);
					Rect temp2(parent, 2);
					v_rc.push_back(temp2);
					Rect temp3(parent, 3);
					v_rc.push_back(temp3);
					Rect temp4(parent, 4);
					v_rc.push_back(temp4);
					Rect temp5(parent, 5);
					v_rc.push_back(temp5);
					Rect temp6(parent, 6);
					v_rc.push_back(temp6);
					Rect temp7(parent, 7);
					v_rc.push_back(temp7);
				}
				for (auto a = v_rc.begin(); a != v_rc.end(); )
				{
					if (a->_clicked)
					{
						a = v_rc.erase(a);
					}
					else
						a++;
				}
			}
		}
		//else if (state == GLUT_UP)
		//{
		//	for (auto it = v_rc.begin(); it != v_rc.end();)
		//	{
		//		if (it->_clicked == true)
		//			it = v_rc.erase(it);
		//		else
		//			++it;
		//	}
		//	for (auto& a : v_rc)
		//	{
		//		a._clicked = false;
		//	}
		//		for (auto rc = v_rc.begin(); rc != v_rc.end(); ++rc)
		//		{
		//			if (rc->_clicked == true)
		//				rc->Merge_Check(v_rc);
		//		}
		//		// �ʱ�ȭ �� ��
		//		float min_width = 1.f, min_height = 1.f;
		//		float max_width = 0.f, max_height = 0.f;
		//		float merge_count = 0.f;
		//		vPos<float> center(0.f, 0.f);
		//		for (auto it = v_rc.begin(); it != v_rc.end();)
		//		{
		//			if (it->_merged == true)
		//			{
		//				++merge_count;
		//				if (min_width > it->_width)
		//					min_width = it->_width;
		//				if (min_height > it->_height)
		//					min_height = it->_height;
		//				if (max_width < it->_width)
		//					max_width = it->_width;
		//				if (max_height < it->_height)
		//					max_height = it->_height;
		//				center = center + (it->_center);
		//				it = v_rc.erase(it);
		//			}
		//			else
		//				++it;
		//		}
		//		if(merge_count != 0)
		//		{
		//			center._x = center._x / merge_count;
		//			center._y = center._y / merge_count;

		//			Rect temp(center, min_width + max_width, min_height + max_height);
		//			v_rc.emplace_back(temp);
		//		}
		//		for (auto ite = v_rc.begin(); ite != v_rc.end();)
		//		{
		//			if (ite->_merged == true)
		//			{
		//				std::cout << "Erasing: " << ite->_width << " x " << ite->_height << std::endl;
		//				ite = v_rc.erase(ite);
		//			}
		//			else
		//			{
		//				++ite;
		//			}
		//		}
		//		std::cout << "v_rc size after erase: " << v_rc.size() << std::endl;
		//	}
		//}
		break;
	default:
		break;
	}

	glutPostRedisplay();
	}
}

void UserTimerFunc(int value)
{
	for (auto& a : v_rc)
	{
		a.Update();
	}

	for (auto a = v_rc.begin(); a != v_rc.end(); )
	{
		if (a->_width <= 0.f || a->_height <= 0.f && a->_merged == true)
		{
			a = v_rc.erase(a);
		}
		else
			a++;
	}
	glutPostRedisplay();
	if (on_timer)
		glutTimerFunc(50, UserTimerFunc, 1);
}



