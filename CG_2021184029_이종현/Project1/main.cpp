#define _CRT_SECURE_NO_WARNINGS
#include "global.h"
#include "component_7.h"

#include <iostream>
#include "Game.h"

#include "Polygon.h"
#include "Triangle.h"
#include "Dot.h"
#include "Line.h"
#include "Rect.h"




using glm::vec2;
using glm::vec3;
using glm::mat4;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Update();
GLvoid Mouse_Down_Move(int x, int y);

void make_shaderProgram();
void InitBuffer();
void make_vertexShaders();
void make_fragmentShaders();


int window_x = 800;
int window_y = 600;

GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수


Game core;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
{
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(window_x, window_y); // 윈도우의 크기 지정
	int i = glutCreateWindow("Polygon_Ninja"); // 윈도우 생성 (윈도우 이름)

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
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정'
	glutIdleFunc(Update);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Mouse_Down_Move);	// 마우스 누른 채로 이동할 때 호출 될 함수


	
	core.Initialize();

	core._polygons.reserve(9);
	core._polygons.emplace_back(new Triangle(-0.5f, 0.f, 0.2f, 0));
	core._polygons.emplace_back(new Rect_p(0.5f, 0.f, 0.2f));
	
	make_shaderProgram();
	InitBuffer();



	// 윈도우 크기가 변경될 때 취할 동작 지정
	// w : 윈도우의 새로운 폭, h : 윈도우의 새로운 높이
	glutMainLoop(); // 이벤트 처리 시작
	/*지금까지 생성한 윈도우들과 여기에 그린 그림들을 화면에 출력한다.
	또한, 이벤트 처리가 시작되고 디스플레이 콜백으로 등록된 함수가 호출된다.
	마우스, 키보드 등의 콜백 함수들이 호출된다
	메인 함수는 최소한 한번의 glutMainLoop함수를 호출해야한다.*/

	//void glutLeaveMainLoop(); - 이벤트 프로세싱(프로그램 종료)
}


// Idle function 
GLvoid Update()
{
	/*for (auto it = core._polygons.begin(); it != core._polygons.end(); )
		if ((*it)->_dead == true)
		{
			it = core._polygons.erase(it);
		}
		else
			++it;*/


	core.Update();
	
	InitBuffer();
	glutPostRedisplay();
}



GLuint vao[10], vbo[4];


GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 {
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 바탕색을 ‘blue’로 지정
	/*r, g, b: red, green, blue 값
	a : alpha 값(1.0값으로 고정)*/
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 특정 버퍼나 혼합된 버퍼의 영역을 glClearColor에서 선택한 값으로 설정한다.
	/* 컬러 버퍼 : GL_COLOR_BUFFER_BIT
	깊이 버퍼 : GL_DEPTH_BUFFER_BIT
	누적 버퍼 : GL_ACCUM_BUFFER_BIT
	스텐실 버퍼 : GL_STENCIL_BUFFER_BIT*/

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.


	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glLineWidth(2);
	glPointSize(5);
	glUseProgram(shaderProgramID);


	GLint location = glGetUniformLocation(shaderProgramID, "ModelMatrix");
	mat4 modelt = mat4(1.f);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelt));

	for (size_t i = 0; i < core._polygons.size(); i++)
	{
		glBindVertexArray(vao[i]);
		if (core._polygons[i] != nullptr)
			core._polygons[i]->Draw_Polygon();
	}

	//--- 사용할 VAO 불러오기
	//glBindVertexArray(vao[1]);
	//// 그리기
	//glDrawArrays(GL_TRIANGLES, 0, 3);


	//void glDrawArrays(GLenum mode, GLint first, GLsizei count);
	/*바인드 한 배열 데이터로부터 프리미티브 렌더링 하기
	 mode : 렌더링 할 프리미티브의 종류
		 -점:GL_POINTS,
		 -선:
		 GL_LINE_STRIP,		1 - 2 - 3 - 4 인덱스 전부 연결
		 GL_LINE_LOOP,		[1] - 2 - 3 - 4 - [1] 끝이 이어지게 루프로 연결
		 GL_LINES			1 - 2번인덱스, 3 - 4번인덱스 등등 따로 연결
		 -삼각형:
		 GL_TRIANGLE_STRIP	이어지는 삼각형 ex)삼각형 두 개로 만드는 사각형 등
		 GL_TRIANGLE_FAN	부채꼴로 이어지는 삼각형
		 GL_TRIANGLES 등		기본 삼각형
	 first : 배열에서 도형의 시작 인덱스
	 count : 렌더링 할 인덱스 개수
	 프리미티브(primitive) : 오픈지엘 렌더링의 기본 단위로 이용 가능한 가장 단순한 요소  점, 선, 삼각형*/

	/*	GL에서 그리기
		void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices);
	 배열 데이터로부터 프리미티브 렌더링 하기, 배열 데이터의 인덱스를 사용
		mode : 렌더링 할 프리미티브의 종류
		GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES 등
		count : 렌더링할 요소의 개수
		type : indices 값의 타입
		GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, 또는 GL_UNSIGNED_INT
		indices : 바인딩 되는 버퍼의 데이터 저장소에 있는 배열의 첫 번째 인덱스 오프셋*/
	
	//기본 속성 바꾸기
	/*점 크기 설정
		void glPointSize(GLfloat size);
		size : 점의 크기(초기값 : 1)

	선의 굵기 조정
		void glLineWidth(GLfloat width);
		width : 선의 굵기(초기값 : 1)

	폴리곤 모드 설정
		void glPolygonMode(GLenum face, GLenum mode);
		face : 모드를 설정할 면(GL_FRONT_AND_BACK)
		mode : 모드
		GL_POINT, GL_LINE, GL_FILL*/

	glutSwapBuffers(); //--- 화면에 출력하기


	//glutSwapBuffers(); // 화면에 출력하기 - 더블버퍼이므로 glFlush대신 실행
	/*그리기를 실행하는 동시에 화면에 나타나지 않는 버퍼(off screen)에 렌더링을 할 수 있다.
	스왑(swap) 명령으로 버퍼에 렌더링한 그림을 스크린 상에 즉시 나타낼 수 있다.

	더블 버퍼 사용
	시간이 오래 걸리는 복잡한 그림을 그린 후 완성된 그림을 화면에 보여줄 수 있다.
	애니메이션에서 사용할 수 있다.

	사용 방법
	메인 함수: 출력 모드를 더블 버퍼링을 위해 설정한다.
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); //--- 출력 모드를 더블 타입으로 설정
	그리기 콜백 함수: 드로잉 명령을 실행하고 버퍼 교체를 설정한다.
	glutSwapBuffers (); */
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 {
{
	glViewport(0, 0, w, h);
	window_x = w;
	window_y = h;
}

static polygon_type draw_mode;
Line* sliceline = nullptr;


bool on_click = false;

GLvoid Mouse(int button, int state, int x, int y)
{
	float xpos = x;
	float ypos = y;

	// 마우스 좌표 변환
	Mouse_Convert_Win_To_OpenGL(xpos, ypos, window_x, window_y);
	/*pre_xpos = xpos;
	pre_ypos = ypos;
	vPos<float> eraser_pos(xpos, ypos);
	*/
	vec3 start(xpos, ypos,0.f);
	switch (button)
	{
	case GLUT_LEFT_BUTTON: // 사각형 내부 - 색상 랜덤, 사각형 외부 - 배경색 랜덤
	{

		// 눌렀을 때
		if (state == GLUT_DOWN)
		{
			on_click = true;
			sliceline = new Line(start, start); // 시작점 기준으로 만듦
			core._polygons.emplace_back(sliceline);
		
		}
		else	// if up key, delete line
		{
			on_click = false;
			int limit = core._polygons.size();
			for(int i = 0; i < limit; ++i)
				{
				auto triit = find_if(core._polygons.begin(), core._polygons.end(), [](auto p) {return (p->_type == mode_Rect || p->_type == mode_Triangle); });
					if (triit != core._polygons.end() && (*triit)->_dead == false && (*triit)->_splited == false)
					{
						(*triit)->Split(sliceline, core._polygons);
						limit = core._polygons.size();
					}
				}
			auto it = find_if(core._polygons.begin(), core._polygons.end(), [](auto p) {return p->_type == mode_Line; });
			if (it != core._polygons.end())
			{
				core._polygons.erase(it);
			}
		}
	}
	break;
	}

	/*InitBuffer();
	glutPostRedisplay();*/
}

static float pre_xpos = 0;
static float pre_ypos = 0;

GLvoid Mouse_Down_Move(int x, int y)
{
	if (on_click == false)
		return;
	// 마우스 좌표 변환
	float cur_xpos = x;
	float cur_ypos = y;
	Mouse_Convert_Win_To_OpenGL(cur_xpos, cur_ypos, window_x, window_y);


	float x_offset = cur_xpos - pre_xpos;
	float y_offset = cur_ypos - pre_ypos;


	
	// 현재 커서 위치
	vec3 cur_pos(cur_xpos, cur_ypos, 0.f);
	

	// find line in core.polygons
	auto it = find_if(core._polygons.begin(), core._polygons.end(), [](auto* p) {return p->_type == mode_Line; });
	if (it == core._polygons.end())
		return;


	Line* ptemp = dynamic_cast<Line*>(*it);
	ptemp->Update(cur_pos);

	pre_xpos = cur_xpos;
	pre_ypos = cur_ypos;

	InitBuffer();
	glutPostRedisplay();
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	//키보드 명령에 따라, 마우스를 누르는 위치에 점, 선, 삼각형 또는 사각형(삼각형 2개 붙이기) 그린다.
	//	색상과 크기는 자율적으로 정하고, 최대 10개의 도형을 그린다.
	//	키보드 명령
	//	p : 점 그리기
	//	l : 선 그리기
	//	t : 삼각형 그리기
	//	r : 사각형 그리기
	//	w / a / s / d : 그린 모든 도형 중 랜덤하게 한 개를 선택한 후,
	// 화면에서 위 / 좌 / 아래 / 우측으로 이동한다.
	// 명령어를 선택할 때마다 다른 도형이 선택되어 이동된다.
	//	c : 모든 도형을 삭제한다.

	if (key == 'm')
	{
		// 도형의 모드 Line, FIll
	}
	else if (key == 'l')
	{
		// 경로 출력하기 on / off
	}
	else if (key == 'w')
	{
		// 도형이 날아오는 속도 변경 ++
	}
	else if (key == 's')
	{
		// 도형이 날아오는 속도 변경 --
	}
	
	InitBuffer();
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

void InitBuffer()
{
	glGenVertexArrays(10, vao); //--- VAO 를 지정하고 할당하기

	for (size_t i = 0; i < core._polygons.size(); i++)
	{
		glBindVertexArray(vao[i]); //--- VAO를 바인드하기
		if (core._polygons[i] != nullptr)
			core._polygons[i]->init_buffer_polygon(vao, vbo);
	}

	//glBindVertexArray(vao[0]); //--- VAO를 바인드하기
	//{
	//	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	//	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//	//--- 변수 triShape에서 버텍스 데이터 값을 버퍼에 복사한다.
	//	//--- triShape 배열의 사이즈: 9 * float
	//	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), l.get_vertex_ptr(), GL_STATIC_DRAW);

	//	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//	//--- attribute 인덱스 0번을 사용가능하게 함
	//	glEnableVertexAttribArray(0);

	//	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//	// 
	//	//--- colors 배열의 사이즈: 9 *float
	//	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), l.get_color_ptr(), GL_STATIC_DRAW);
	//	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//	//--- attribute 인덱스 1번을 사용 가능하게 함.
	//	glEnableVertexAttribArray(1);
	//}

	//glBindVertexArray(vao[1]); //--- VAO를 바인드하기
	//{

	//	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	//	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//	//--- 변수 triShape에서 버텍스 데이터 값을 버퍼에 복사한다.
	//	//--- triShape 배열의 사이즈: 9 * float
	//	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape2, GL_STATIC_DRAW);

	//	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//	//--- attribute 인덱스 0번을 사용가능하게 함
	//	glEnableVertexAttribArray(0);

	//	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//	//--- colors 배열의 사이즈: 9 *float
	//	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	//	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//	//--- attribute 인덱스 1번을 사용 가능하게 함.
	//	glEnableVertexAttribArray(1);
	//}
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

	if(!result)
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







