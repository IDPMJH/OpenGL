#define _CRT_SECURE_NO_WARNINGS
#include "global.h"
#include "Polygon_16.h"
#include "Line_16.h"
#include "component_16.h"






enum Rotate_Type
{
	Rotate_none,
	Rotate_x_p,
	Rotate_x_m,
	Rotate_y_p,
	Rotate_y_m,
	Rotate_end,
};

Rotate_Type rtype;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid sp_Keyboard(int key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
int check_quad(float xpos, float ypos);
void UserTimerFunc(int value);


void make_shaderProgram();
void InitBuffer();
void make_vertexShaders();
void make_fragmentShaders();


int window_x = 800;
int window_y = 800;



GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수


bool on_timer = false;
bool on_mouse = true;
bool _draws_of_all = true;
bool _on_depth_test = true;
bool _on_culling = true;
bool _on_revolving = false;

float theta = 0.f;

GLuint vao1[4], vao2[3], vao3[3], vao4[3], vaol[2];
GLuint vao[4];
GLuint vaom[12];
GLuint vbo[4];
glm::vec3 clcolor(0.f, 0.f, 0.f);
GLuint VAO[10];
GLuint VBO, EBO;


mat4 rotateT = mat4(1.f);
mat4 translatT = mat4(1.f);

vector<vec3> Tri =
{
	{0.f,0.1f,0.f},
	{-0.1f,-0.1f,0.f},
	{0.1f,-0.1f,0.f}
};


vector<class::Polygon*> vpolygons;
GLUquadricObj* qobj;
Model_Object mobj;
GLU_Object gobj;
Model model;

vector<class::Line*> Lines;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
{
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(window_x, window_y); // 윈도우의 크기 지정
	int i = glutCreateWindow("Task_15"); // 윈도우 생성 (윈도우 이름)

	// 윈도우 파괴
	//void glutDestroyWindow(int winID);

	//윈도우의 새로운 넓이와 높이
	//void glutReshapeWindow (int width, int height);

	//윈도우의 위치 변화
	//void glutPositionWindow(int x, int y);
	//glutPositionWindow(1000, 500);

	//Full screen 만들기:
	// void glutFullScreen(void);
	// void glutLeaveFullScreen(void);
	// void glutFullScreenToggle(void);
	// 전체 화면으로 세팅 / 해제한다.

		//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;	//OpenGl extensinon 정보를 가져올 수 있도록 설정
	if (glewInit() != GLEW_OK) // glew(라이브러리) 초기화, 실패 시 에러출력
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;

		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	glutDisplayFunc(drawScene); // 출력 콜백 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(sp_Keyboard);
	// Key: GLUT_KEY_F1 ~ GLUT_KEY_F12,
	// GLUT_KEY_LEFT, GLUT_KEY_RIGHT, GLUT_KEY_UP, GLUT_KEY_DOWN,
	// GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT,
	// GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN
	// 
	glutMouseFunc(Mouse);


	class::Polygon* lx = new Line(-1.f, 0.f, 0.f, 1.f, 0.f, 0);
	class::Polygon* ly = new Line(0.f, 1.f, 0.f, 0.f, -1.f, 0);
	class::Polygon* lz = new Line(0.f, 0.f, -1.f, 0.f, 0.f, 1.f);
	vpolygons.emplace_back(lx);
	vpolygons.emplace_back(ly);
	vpolygons.emplace_back(lz);
	read_obj_file("cube.obj", model);
	model._type = Type_cube;
	mobj._model_p = &model;
	gobj._qobj = qobj;

	Line* temp = new Line(0, 0);
	temp->init_line(1);
	temp->_size = temp->_lineshape.size();
	Lines.emplace_back(temp);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	make_shaderProgram();

	InitBuffer();
	glLineWidth(2);
	glPointSize(2);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_POLYGON_SMOOTH);
	//glEnable(GL_LINE_SMOOTH);
	
	




	// 윈도우 크기가 변경될 때 취할 동작 지정
	// w : 윈도우의 새로운 폭, h : 윈도우의 새로운 높이
	glutMainLoop(); // 이벤트 처리 시작
	/*지금까지 생성한 윈도우들과 여기에 그린 그림들을 화면에 출력한다.
	또한, 이벤트 처리가 시작되고 디스플레이 콜백으로 등록된 함수가 호출된다.
	마우스, 키보드 등의 콜백 함수들이 호출된다
	메인 함수는 최소한 한번의 glutMainLoop함수를 호출해야한다.*/
	glutPostRedisplay();
	//void glutLeaveMainLoop(); - 이벤트 프로세싱(프로그램 종료)
	//glutTimerFunc(10, UserTimerFunc, 1);
}



GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 {
{
	glClearColor(0.f, 0.f, 0.f, 0.0f);
	/*r, g, b: red, green, blue 값
	a : alpha 값(1.0값으로 고정)*/
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 특정 버퍼나 혼합된 버퍼의 영역을 glClearColor에서 선택한 값으로 설정한다.
	/* 컬러 버퍼 : GL_COLOR_BUFFER_BIT
	깊이 버퍼 : GL_DEPTH_BUFFER_BIT
	누적 버퍼 : GL_ACCUM_BUFFER_BIT
	스텐실 버퍼 : GL_STENCIL_BUFFER_BIT*/

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기

	glUseProgram(shaderProgramID);

	mat4 fundT = mat4(1.f);
	glm::mat4 modelT = glm::mat4(1.0f);
	glm::mat4 trsT = glm::mat4(1.0f);
	


	glm::mat4 viewT = glm::mat4(1.0f);// glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	glm::mat4 projectionT = glm::mat4(1.0f); //glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	fundT = glm::rotate(modelT, radians(30.f), vec3(1.f, -1.f, 0.f)); // x축, -y축 기준으로 회전


	// 신축
	mobj._scale = glm::scale(mat4(1.f), vec3(0.5f, 0.5f, 0.5f));

	mobj._rot = glm::rotate(mat4(1.f), radians(30.f), vec3(1.f, -1.f, 0.f)); // x축, -y축 기준으로 회전
	gobj._rot = glm::rotate(mat4(1.f), radians(30.f), vec3(1.f, -1.f, 0.f)); // x축, -y축 기준으로 회전


	if (on_timer == true)
	{
		switch (rtype)
		{
		case Rotate_none:
			break;
		case Rotate_x_p:
			rotateT = glm::rotate(rotateT, radians(5.f), vec3(1.f, 0.f, 0.f));
			break;
		case Rotate_x_m:
			rotateT = glm::rotate(rotateT, radians(-5.f), vec3(1.f, 0.f, 0.f));
			break;
		case Rotate_y_p:
			rotateT = glm::rotate(rotateT, radians(5.f), vec3(0.f, 1.f, 0.f));
			break;
		case Rotate_y_m:
			rotateT = glm::rotate(rotateT, radians(-5.f), vec3(0.f, 1.f, 0.f));
			break;
		case Rotate_end:
			break;
		default:
			break;
		}
	}


	GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
	GLuint viewLoc = glGetUniformLocation(shaderProgramID, "view");
	GLuint projectionLoc = glGetUniformLocation(shaderProgramID, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewT));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionT));


	

	
	fundT = glm::rotate(fundT, radians(90.f), vec3(1.0f, 0.f, 0.f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(fundT));
	for (size_t i = 0; i < Lines.size(); i++)
	{

		glBindVertexArray(vao1[i]);
		if (Lines[i]->_lineshape.size() > Lines[i]->_size)
			Lines[i]->_size++;

		Lines[i]->_linemode = true;
		Lines[i]->Draw_Polygon();
	}
	fundT = glm::rotate(fundT, radians(-90.f), vec3(1.0f, 0.f, 0.f));

	size_t i;
	for (i = 0; i < vpolygons.size(); i++)
	{
		glBindVertexArray(VAO[i]);
		vpolygons[i]->Draw_Polygon();
	}
	glBindVertexArray(VAO[i]);



	// 모델 관련========
	mobj._trs = glm::translate(mat4(1.f), vec3(-1.f, 0.f, 0.f));
	mobj._rot = mobj._rot * rotateT;
	if(_on_revolving == true)
		mobj._FT = mobj._scale * mobj._rot * mobj._trs;
	else
		mobj._FT = mobj._scale * mobj._trs * mobj._rot;

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mobj._FT));
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);




	// glu 관련========
	gobj._trs = glm::translate(mat4(1.f), vec3(0.5f, 0.f, 0.f));
	gobj._rot = gobj._rot * rotateT;
	if(_on_revolving == true)
		gobj._FT = gobj._scale * gobj._rot * gobj._trs;
	else
		gobj._FT = gobj._scale * gobj._trs * gobj._rot;

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(gobj._FT));
	qobj = gluNewQuadric();

	gluQuadricDrawStyle(qobj, GLU_LINE);
	glColor3f(0.5f, 0.5f, 1.f);
	if (model._type == Type_cube)
		gluSphere(qobj, 0.2, 8, 8);
	else if (model._type == Type_pyramid)
		gluCylinder(qobj, 0.2f, 0.2f, 0.5, 10, 8);
	gluDeleteQuadric(qobj);

	glFlush();
	


	glutSwapBuffers(); //--- 화면에 출력하기

}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 {
{
	glViewport(0, 0, w, h);
	window_x = w;
	window_y = h;
}


int make_line_size = 1;

GLvoid Mouse(int button, int state, int x, int y)
{


	if (on_mouse == false)
		return;

	float xpos = x;
	float ypos = y;

	// 마우스 좌표 변환
	Mouse_Convert_Win_To_OpenGL(xpos, ypos, window_x, window_y);
	int quadrant = check_quad(xpos, ypos);
	switch (button)
	{
	case GLUT_RIGHT_BUTTON:
	{
		return;
		if (state == GLUT_DOWN)
		{

		}
		break;
	}
	case GLUT_LEFT_BUTTON:
	{
		if (state == GLUT_DOWN)
		{

		}
	}
	break;
	}
	InitBuffer();
	glutPostRedisplay();
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'W':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'h':
		if (_on_depth_test == true)
		{
			glDisable(GL_DEPTH_TEST);
			_on_depth_test = false;
		}
		else if (_on_depth_test == false)
		{
			glEnable(GL_DEPTH_TEST);
			_on_depth_test = true;
		}
		break;
	case 'x':
		if(on_timer == false)
		{
			on_timer = true;
			_on_revolving = false;
			glutTimerFunc(10, UserTimerFunc, 1);
			rotateT = mat4(1.f);
			rtype = Rotate_x_p;
		}
		else if (on_timer == true)
		{
			on_timer = false;
		}
		break;
	case 'X':
		if (on_timer == false)
		{
			on_timer = true;
			_on_revolving = false;
			glutTimerFunc(10, UserTimerFunc, 1);
			rotateT = mat4(1.f);
			rtype = Rotate_x_m;
		}
		else if (on_timer == true)
		{
			on_timer = false;
		}
		break;
	case 'y':
		if (on_timer == false)
		{
			on_timer = true;
			_on_revolving = false;
			glutTimerFunc(10, UserTimerFunc, 1);
			rotateT = mat4(1.f);
			rtype = Rotate_y_p;
		}
		else if (on_timer == true)
		{
			on_timer = false;
		}
		break;
	case 'Y':
		if (on_timer == false)
		{
			on_timer = true;
			_on_revolving = false;
			glutTimerFunc(10, UserTimerFunc, 1);
			rotateT = mat4(1.f);
			rtype = Rotate_y_m;
		}
		else if (on_timer == true)
		{
			on_timer = false;
		}
		break;
	case 'r':
		if (on_timer == false)
		{
			on_timer = true;
			_on_revolving = true;
			glutTimerFunc(10, UserTimerFunc, 1);
			rotateT = mat4(1.f);
			rtype = Rotate_y_p;
		}
		else if (on_timer == true)
		{
			on_timer = false;
			_on_revolving = false;
		}
		break;
	case 'R':
		if (on_timer == false)
		{
			on_timer = true;
			_on_revolving = true;

			glutTimerFunc(10, UserTimerFunc, 1);
			rotateT = mat4(1.f);
			rtype = Rotate_y_m;
		}
		else if (on_timer == true)
		{
			on_timer = false;
			_on_revolving = false;
		}
		break;
	case 'c':
		if (model._type != Type_cube)
		{
			model.vertices.clear();
			model.faces.clear();
			read_obj_file("cube.obj", model);
			model._type = Type_cube;
		}
		else if (model._type != Type_pyramid)
		{
			model.vertices.clear();
			model.faces.clear();
			read_obj_file("pyramid.obj", model);
			model._type = Type_pyramid;
		}

		break;
	case 's':
		on_timer = false;
		_on_revolving = false;
		rotateT = mat4(1.f);
		translatT = mat4(1.f);
		break;
	default:
		break;
	}


	InitBuffer();
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid sp_Keyboard(int key, int x, int y)
{
}



void InitBuffer()
{
	glGenVertexArrays(10, vao1); //--- VAO 를 지정하고 할당하기
	for (size_t i = 0; i < Lines.size(); i++)
	{
		glBindVertexArray(vao1[i]); //--- VAO를 바인드하기
		Lines[i]->init_buffer_polygon(vao1, vbo);
	}

	glGenVertexArrays(10, VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	/*여러 다각형이 그려지지 않는 이유는
	glGenVertexArrays 함수가 루프 내에서
	반복 호출되기 때문일 수 있습니다.
	이 함수는 새로운 VAO를 생성하고 vao1 배열에 저장합니다.
	그러나 이 배열은 루프가 반복될 때마다 덮어쓰여지므로,
	마지막 다각형만이 올바르게 그려질 수 있습니다.*/


	size_t i;
	for (i = 0; i < vpolygons.size(); i++)
	{
		glBindVertexArray(VAO[i]); //--- VAO를 바인드하기
		vpolygons[i]->init_buffer_polygon(VAO, vbo);
	}

		model.model_init_buffer(VBO, VAO[i], EBO);




}


void make_shaderProgram()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);

	glLinkProgram(shaderProgramID);
	// 정상 링크 확인하는 방법은 Tutorial 4 참고

	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}

void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);

	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	/*세이더 정보 가져오기
	 void glGetShaderiv (GLuint shader, GLenum pname, GLint *params);
	 shader : 세이더 객체
	 pname : 객체 파라미터
		GL_SHADER_TYPE, GL_DELETE_STATUS, GL_COMPILE_STATUS, GL_INFO_LOG_LENGTH, GL_SHADER_SOURCE_LENGTH)
	 params : 리턴 값
		GL_SHADER_TYPE : 세이더 타입 리턴(GL_VERTEX_SHADER / GL_FRAGMENT_SHADER)
		GL_DELETE_STATUS : 세이더가 삭제됐으면 GL_TRUE, 아니면 GL_FALSE
		GL_COMPILE_STATUS : 컴파일이 성공했으면 GL_TRUE, 아니면 GL_FALSE
		GL_INFO_LOG_LENGTH : 세이더의 INFORMATION LOG 크기
		GL_SHADER_SOURCE_LENGTH : 세이더 소스 크기
		에러 발생 시, GL_INVALID_VALUE, GL_INVALID_OPERATON, GL_INVALID_ENUM*/

	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		/*void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei * length, GLchar * infoLog);
		 세이더 객체의 information log 가져오기
			 shader : 세이더 객체
			 maxLength : information log 크기
			 length : infoLog 길이
			 infoLog : information log*/
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	// 버텍스 쉐이더 설명 참조
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		// 버텍스 쉐이더 설명 참조
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

int check_quad(float xpos, float ypos)
{
	if (xpos >= 0)
	{
		if (ypos >= 0)
			return 1;
		else
			return 4;
	}
	else
	{
		if (ypos >= 0)
			return 2;
		else
			return 3;
	}
}

void UserTimerFunc(int value)
{
	
	glutPostRedisplay();
	if (on_timer)
		glutTimerFunc(10, UserTimerFunc, 1);
}




