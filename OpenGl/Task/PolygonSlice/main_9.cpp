#define _CRT_SECURE_NO_WARNINGS
#include "global.h"
#include "Polygon.h"
#include "Triangle.h"
#include "Dot.h"
#include "Line.h"
#include "Rect.h"
#include "component_7.h"
#include "ogldev_math_3d.h"





GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
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


bool on_timer = true;
bool on_mouse = true;

vector<class::Triangle*> Triangles;

Line lx(-1.f, 0.f, 0.f, 1.f, 0.f, 0);
Line ly(0.f, 1.f, 0.f, 0.f, -1.f, 0);
GLuint vao1[4], vao2[3], vao3[3], vao4[3], vaol[2];
GLuint vaom[12];
GLuint vbo[4];

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
{
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(window_x, window_y); // 윈도우의 크기 지정
	int i = glutCreateWindow("Task_1_7"); // 윈도우 생성 (윈도우 이름)

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
	glutMouseFunc(Mouse);
	glutTimerFunc(10, UserTimerFunc, 1);

	//Triangle t1;

	make_shaderProgram();
	//glGenVertexArrays(2, vaol); //--- VAO 를 지정하고 할당하기

	//glBindVertexArray(vaol[0] ); //--- VAO를 바인드하기
	//lx.init_buffer_polygon(vaol, vbo);

	//glBindVertexArray(vaol[1]);
	//ly.init_buffer_polygon(vaol, vbo);

	InitBuffer();
	glLineWidth(2);
	glPointSize(5);



	// 윈도우 크기가 변경될 때 취할 동작 지정
	// w : 윈도우의 새로운 폭, h : 윈도우의 새로운 높이
	glutMainLoop(); // 이벤트 처리 시작
	/*지금까지 생성한 윈도우들과 여기에 그린 그림들을 화면에 출력한다.
	또한, 이벤트 처리가 시작되고 디스플레이 콜백으로 등록된 함수가 호출된다.
	마우스, 키보드 등의 콜백 함수들이 호출된다
	메인 함수는 최소한 한번의 glutMainLoop함수를 호출해야한다.*/

	//void glutLeaveMainLoop(); - 이벤트 프로세싱(프로그램 종료)
}





GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 {
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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

	glUseProgram(shaderProgramID);

	
	GLint location = glGetUniformLocation(shaderProgramID, "ModelMatrix");
	
	for (size_t i = 0; i < Triangles.size(); i++)
	{
		glm::mat4 Rot = Triangles[i]->_Rotation;
		glm::mat4 Trs = Triangles[i]->_Translation;
		//Matrix4f FinalTransform = Triangles[i]->_Rotation * Triangles[i]->_Translation;
		glm::mat4 FinalTransform = Triangles[i]->_Transform;
		//glm::mat4 FinalTransform = Triangles[i]->_Transform;

			/*(cosf(1.5708f), -sinf(1.5708f), 0.f, 0.f,
			sinf(1.5708f), cosf(1.5708f), 0.f, 0.f,
			0.f, 0.f, 1.0f, 0.f,
			0.f, 0.f, 0.f,1.0f);*/
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(FinalTransform));
		glBindVertexArray(vao1[i]);
		Triangles[i]->Draw_Polygon();
	}
	

	/*glBindVertexArray(vaol[0]);
	lx.Draw_Polygon();
	glBindVertexArray(vaol[1]);
	ly.Draw_Polygon();*/
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


GLvoid Mouse(int button, int state, int x, int y)
{
	if (Triangles.size() >= 4)
		return;
	if (on_mouse == false)
		return;

	float xpos = x;
	float ypos = y;

	// 마우스 좌표 변환
	Mouse_Convert_Win_To_OpenGL(xpos, ypos, window_x, window_y);
	int quadrant = check_quad(xpos, ypos);
	/*pre_xpos = xpos;
	pre_ypos = ypos;
	vPos<float> eraser_pos(xpos, ypos);
	*/
	draw_mode = mode_Triangle;

	switch (button)
	{
	case GLUT_RIGHT_BUTTON: // 사각형 내부 - 색상 랜덤, 사각형 외부 - 배경색 랜덤
	{
		return;
		if (state == GLUT_DOWN)
		{
		}
		break;
	}
	case GLUT_LEFT_BUTTON: // 사각형 내부 - 색상 랜덤, 사각형 외부 - 배경색 랜덤
	{
		if (state == GLUT_DOWN)
		{
			switch (draw_mode)
			{
			case mode_Default:
				break;
			case mode_Point:
			{
				class::Polygon* temp = new Dot(xpos, ypos, 0.f);
				//polygons.emplace_back(temp);
			}
			break;
			case mode_Line:
			{
				class::Polygon* temp = new Line(xpos - 0.1f, ypos, 0.f, xpos + 0.1f, ypos, 0);
				//polygons.emplace_back(temp);
			}
			break;
			case mode_Triangle:
			{
				float fund = Random_0_to_1f();
				fund = fund * 0.1f + 0.05f;
				if (xpos + fund >= 1.f || xpos - fund <= -1.f ||
					ypos + fund*2 >= 1.f || ypos - fund * 1.5f < -1.f)
					return;
				class::Triangle* temp = new Triangle(xpos, ypos, fund, quadrant);
				Triangles.emplace_back(temp);
			}
			break;
			case mode_Rect:
			{
				class::Polygon* temp = new Rect_p(xpos, ypos, 0.1f);
				//polygons.emplace_back(temp);
			}
			break;
			case mode_End:
				break;
			default:
				break;
			}
		}
	}
	}
	InitBuffer();
	glutPostRedisplay();
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	if (Triangles.empty())
		return;
	/*실습 8에서 그린 삼각형들이 각각 다른 방향과 다른 속도로 이동한다.
		 실습 8과 같이 4개의 삼각형을 그릴 수 있다.
		 삼각형은 이등변 삼각형으로 그린다.
		 키보드 명령으로 4개의 다른 이동을 한다.
		 1: 튕기기
		 2 :좌우로 지그재그
		 3 :사각 스파이럴
		 4 :원 스파이럴
		 가장자리를 만나게 되면 삼각형의 방향이 바뀌게 된다.*/
	for (auto& a : Triangles)
	{
		a->_Transform = mat4(1.f);
	}


	if (key == '1')
	{
		for (auto& a : Triangles)
		{
			if (a->_move != move_Bounce)
			{
				a->stand_shape();
				a->rotate45DegreeZ();
				a->_dirx = dir_right;
				a->_diry = dir_up;
				a->_xspd = 0.01f;
				a->_yspd = 0.01f;
				a->_move = move_Bounce;
			}
		}
		on_mouse = false;
	}
	else if (key == '2')
	{
		for (auto& a : Triangles)
		{
			if (a->_move != move_Zigzag)
			{
				a->stand_shape();
				a->rotate_m_90DegreesZ();
				a->_dirx = dir_right;
				a->_xspd = 0.01f;
				a->_yspd = 0.01f;
				a->_move = move_Zigzag;
				a->_ycount = 0;
			}
		}
		on_mouse = false;

	}
	else if (key == '3')
	{
		for (auto& a : Triangles)
		{
			if(a->_move != move_Spiral_rect)
			{
				a->_move = move_Spiral_rect;
				a->stand_shape();
				a->_dirx = dir_left;
				a->_diry = dir_up;
				a->_xspd = 0.02f;
				a->_yspd = 0.02f;
				a->_xclash = false;
				a->_yclash = false;
				a->_xspiral_count = 0;
				a->_yspiral_count = 0;
				a->_limit_x = 1.f;
				a->_limit_y = 1.f;
				a->_spiral_in = true;
				a->_xcount = abs((1.f - a->_xpos) / a->_xspd);
				a->_ycount = abs((1.f - a->_ypos) / a->_yspd);
				a->_xmaxcount = abs((1.f - a->_xpos) / a->_xspd);
				a->_ymaxcount = abs((1.f - a->_ypos) / a->_yspd);
			}
		}
		on_mouse = false;
	}
	else if (key == '4')
	{
		for (auto& a : Triangles)
		{
			a->_Trimat =
			{
				a->_TriShape[0][0],a->_TriShape[0][1],a->_TriShape[0][2],0.f,
				a->_TriShape[1][0],a->_TriShape[1][1],a->_TriShape[1][2],0.f,
				a->_TriShape[2][0],a->_TriShape[2][1],a->_TriShape[2][2],0.f,
				0.f,0.f,0.f,1.0f
			};
			a->_Transform = mat4(1.f);
			a->init_mat4();
			a->_theta = 0.f;
			a->_xspd = a->_xpos / 100.f;
			a->_yspd = a->_ypos / 100.f;
			//a->_xspd = 0.002f;
			//a->_yspd = 0.002f;
			a->_move = move_Spiral_circle;
		}
		on_mouse = false;

	}
	else if (key == 'c')
	{
		Triangles.clear();
		on_mouse = true;
	}
	else return;

	on_timer = true;
	InitBuffer();
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

void InitBuffer()
{
	/*여러 다각형이 그려지지 않는 이유는
	glGenVertexArrays 함수가 루프 내에서
	반복 호출되기 때문일 수 있습니다.
	이 함수는 새로운 VAO를 생성하고 vao1 배열에 저장합니다.
	그러나 이 배열은 루프가 반복될 때마다 덮어쓰여지므로,
	마지막 다각형만이 올바르게 그려질 수 있습니다.*/

	glGenVertexArrays(10, vao1); //--- VAO 를 지정하고 할당하기
	for (size_t i = 0; i < Triangles.size(); i++)
	{
		glBindVertexArray(vao1[i]); //--- VAO를 바인드하기
		Triangles[i]->init_buffer_polygon(vao1, vbo);
	}
	

	//glGenVertexArrays(2, vaol); //--- VAO 를 지정하고 할당하기

	//glBindVertexArray(vaol[0]); //--- VAO를 바인드하기
	//lx.init_buffer_polygon(vaol, vbo);

	//glBindVertexArray(vaol[1]);
	//ly.init_buffer_polygon(vaol, vbo);

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
	for (auto& a : Triangles)
	{
		a->Update();
		InitBuffer();
	}

	glutPostRedisplay();
	if (on_timer)
		glutTimerFunc(10, UserTimerFunc, 1);
}




