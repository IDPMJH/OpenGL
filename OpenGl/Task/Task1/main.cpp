#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

using namespace std;
GLvoid drawScene(GLvoid);
GLvoid Reshape
(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void UserTimerFunc(int value);



void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); //--- glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //--- ���÷��� ��� ����
	glutInitWindowPosition(0, 0); //--- �������� ��ġ ����
	glutInitWindowSize(800, 600); //--- �������� ũ�� ����
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
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glutMainLoop(); //--- �̺�Ʈ ó�� ����
}



GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	//glClearColor(0.f, 1.f, 0.f, 1.f); //--- �������� ����
	glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
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
	case 'c'://--- ������ û�ϻ����� ����
		glClearColor(0.f, 1.f, 1.f, 1.f);
		break; 
	case 'm'://--- ������ ��ȫ������ ����
		glClearColor(1.f, 0.f, 0.f, 1.f);
		break; 
	case 'y'://--- ������ ��������� ����
		glClearColor(1.f, 1.f, 0.f, 1.f);
		break; 
	case 'a' : // ����
		glClearColor(1.f / r, 1.f / g, 1.f / b, 1.f);
		break;
	case 'w'://--- ������ �Ͼ������ ����
		glClearColor(1.f, 1.f, 1.f, 1.f);
		break;
	case 'k'://--- ������ ���������� ����
		glClearColor(0.f, 0.f, 0.f, 1.f);
		break;
	case 't' : // Ÿ�̸� ����, Ư�� �ð����� �������� �� ����
		on_timer = true;
		glutTimerFunc(500, UserTimerFunc, 1);
		break;
	case 's' : // Ÿ�̸� ����
		on_timer = false;
		break;
	case 'q' :
		glutLeaveMainLoop();
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
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