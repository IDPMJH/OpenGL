#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>




GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Win_Resize(int w, int h);

int window_x = 800;
int window_y = 600;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
{
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100,100); // 윈도우의 위치 지정
	glutInitWindowSize(window_x, window_y); // 윈도우의 크기 지정
	int i = glutCreateWindow("Example1"); // 윈도우 생성 (윈도우 이름)
	
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
	glutReshapeFunc (Win_Resize);
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
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // 바탕색을 ‘blue’로 지정
	/*r, g, b: red, green, blue 값
	a : alpha 값(1.0값으로 고정)*/
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 특정 버퍼나 혼합된 버퍼의 영역을 glClearColor에서 선택한 값으로 설정한다.
	/* 컬러 버퍼 : GL_COLOR_BUFFER_BIT
	깊이 버퍼 : GL_DEPTH_BUFFER_BIT
	누적 버퍼 : GL_ACCUM_BUFFER_BIT
	스텐실 버퍼 : GL_STENCIL_BUFFER_BIT*/

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.

	glutSwapBuffers(); // 화면에 출력하기 - 더블버퍼이므로 glFlush대신 실행
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
}

GLvoid Win_Resize(int w, int h)
{
	window_x = w;
	window_y = h;
}

// 콜백함수들
// 1. void gludDisplayFunc (void(*func)(void));
// 현재 윈도우의 출력 콜백 함수 설정
// 윈도우의 내용을 다시 출력해야 할 필요가 있을 때마다 이 함수로 등록한 콜백 함수를 호출한다. 
// 장면을 다시 그리는데 필요한 루틴들은 모두 이 함수 안에 넣어둔다.
//
// 2. void glutReshapeFunc (void(*func)(int w, int h));
// 윈도우 크기가 변경될 때 취할 동작 지정
// w : 윈도우의 새로운 폭, h : 윈도우의 새로운 높이
//
// 3.  void glutIdelFunc (void(*func))
// 이벤트가 없을 때 호출되는 함수
// 다른 이벤트가 없을 떄 실행
// 애니메이션 효과를 줄 수 있다.
// 
// 4. void glutKeyBoardFunc (void(*func)(unsigned char key, int x, int y));
// 키보드와 인자로 지정한 루틴을 연결하여 키를 누를 떄 호출되도록 설정한다.
// 키보드 입력이 일어날 때마다 ASCII코드 값이 설정된다.
// 
// 
// 5. ASCII가 아닌 특수 키인 경우
// void glut SpecialFunc (void(*func)(int key, intx, int y));
// Key: GLUT_KEY_F1 ~ GLUT_KEY_F12,
// GLUT_KEY_LEFT, GLUT_KEY_RIGHT, GLUT_KEY_UP, GLUT_KEY_DOWN,
// GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT,
// GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN
// 
// 6. int glutGetModifiers (void);
// GLUT_ACTIVE_CTRL,GLUT_ACTIVE_ALT, GLUT_ACTIVE_SHIFT 값을 리턴
// 
// 7. 키보드를 떼었을 때 호출되는 콜백 함수 설정
// void glutKeyboardUpFunc ( void (*func) ( unsigned char key, int x, int y ) );
// 
// 8. 마우스 입력
// void glutMouseFunc(void(&func)(int button, int state, int x, int y));
// 마우스 버튼과 인자로 지정한 루틴을 연결하여 호출되도록 한다.
// button (버튼 파라미터): GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
// state(상태 파라미터) : GLUT_UP, GLUT_DOWN
// x, y : 윈도우에서 마우스의 위치
// 
// 9. 마우스 이동 입력
// void glutMotionFunc(void(*func)(int x, int y));
// 마우스 버튼을 누른 채 마우스를 움직일 때 호출될 콜백 함수를 등록한다.
// void glutPassiveMotionFunc(void(*func)(int x,int y));
// 마우스 버튼을 누르지 않은 채 마우스를 움직일 때 호출될 함수 등록
// 
// 10. 애니메이션 구현을 위한 타이머 설정 함수
// void glutTimerFunc(unsigned int msecs,(*func)(int value), int value);
// 타임 아웃이 발생할 경우 호출될 콜백 함수를 등록한다.
// msecs: 콜백 함수를 호출하기 전까지 기다릴 시간 (밀리세컨 단위)
// func : 호출할 함수의 이름
// value : 콜백 함수로 전달할 값
// 이 함수는 한 번만 실행되므로 지속적인 애니메이션을 위해서는 타이머 함수 내에 타이머를 다시 호출해야한다
// 
// 11. 현재 윈도우를 refresh할 때 호출하는 함수
// void glutPostRedisplay(void)
//  현재 윈도우를 refresh하게 한다.
// 출력 자료를 변경한 후 화면 다시 그리기를 하기위하여 그리기 콜백 함수를 호출해야 할 때 불러준다.
// Refresh 되기 전에 여러 번 호출해도 단 한번만 refresh한다.
// 
//12. 인자값의 상태 변수로 다양한 값을 읽어오는 함수 
// int glutGet(int state)
// State:
// GLUT_SCREEN_WIDTH : 스크린의 폭(픽셀 단위)
// GLUT_SCREEN_HEIGHT : 스크린의 높이(픽셀 단위)
// GLUT_ELAPSED_TIME : glutInit이 호출된 이후의 밀리세컨 단위의 시간
// GLUT_WINDOW_X, GLUT_WINDOW_Y, GLUT_WINDOW_WIDTH, GLUT_WINDOW_HEIGHT…
// 
// 13. 메뉴 만들기
// 13-1. 팝업 메뉴 만들기
// int glutCreateMenu(void(*func)(int value));
// 리턴값: 유일한 정수타입의 메뉴 구별자(1부터 시작)
// 
// 13-2. 마우스 버튼에 메뉴 삽입하기
// void glutAttachMenu(int button);
// void glutDetachMenu(int button);
// Button: 버튼 (GLUT_LEFT_BUTTON / GLUT_MIDDLE_BUTTON / GLUT_RIGHT_BOTTON)
// 
// 13-3. 메뉴 항목 추가하기
// void glutAddMenyEntry(char *name, int value);
// Name : 메뉴 엔트리의 이름
// value : 메뉴가 선택되면 메뉴의 콜백 함수에 리턴할 값
// 
// 13-4. 메뉴의 서브 메뉴 추가하기
// void glutAddSubMenu (char *name, int menu);
// Name : 서브 메뉴의 이름
// Menu : 메뉴의 구별자
// 
// 13-5. 메뉴 없애기
// void glueDestroyMenu(int menu);
// 
// 
//


// ****************************GLM 수학 함수****************************
//
// 절대값
//genType glm::abs(genType x);

// 절대반올림
//vec4f glm::ceil(vec4f x);

// 범위내 값 집게 (없으면 최대 혹은 최소 반환)
//genType glm::clamp(genType x, genType minVal, genType maxVal);

// 실수를 정수로 변환
//int glm::floatBitsToInt(float const& v);

// 절대반내림
//vec4f glm::floor(vec4f const& x);

// 외적
//vec3f glm::cross(vec3f const& x, vec3f const& y);

// 두 벡터사이의 거리
//float glm::distance(vec4f const& p0, vec4f const& p1);

// 내적
//float glm::dot(vec4f const& x, vec4f const& y);

// 벡터의 크기
//float glm::length(vec4f const& x);

// 정규화
//vec4f glm::normalize(vec4f const& x);

// 삼각함수
//float glm::sin(float const& angle);
//float glm::cos(float const& angle);
//float glm::tan(float const& angle);
//float glm::degrees(float const& radians);
//float glm::radians(float const& degrees);
//
//대부분의 상태들은(라이팅, 텍스처링, 은면 제거, 안개 효과 등) 디폴트로 비활성화(disable)되어 있다.
//상태를 활성화(켜거나)하거나 비활성화(끄는)하는 명령어
//void glEnable(GLenum cap);
//지정한 기능을 활성화한다.
//void glDisable(GLenum cap);
//지정한 기능을 비활성화 한다.
//활성화 여부를 체크하는 명령어
//GLboolean glIsEnabled(GLenum cap);
//cap :
//GL_BLEND : 픽셀 블렌딩 연산을 수행(glBlendFunc)
//GL_CULL_FACE : 앞면 혹은 뒷면을 향하는 폴리곤을 선별(glCullFace) --------------컬링
//GL_DEPTH_TEST : 깊이를 비교
//GL_DITHER : 컬러의 디더링 수행
//GL_LINE_SMOOTH : 선의 안티알리아싱 효과
//GL_STENCIL_TEST : 스텐실 테스트
//GL_LINE_SMOOTH, GL_POLYGON_SMOOTH : 선, 면 안티앨리어싱

//void glFrontFace (glenum mode);
// 폴리곤의 어느 면이 앞면, 뒷면인지 정의
// GL_CW - 시계방향으로 정렬된 정점들을 앞면으로
// GL_CCW - 반시계


//응용 프로그램에서 깊이 검사(depth test)를 설정한다.
//윈도우 초기화 시 깊이 검사 모드 설정
//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
//깊이 버퍼를 클리어한다
//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//깊이 검사를 설정 :
//glEnable(GL_DEPTH_TEST);
//깊이 검사를 해제 :
//glDisable(GL_DEPTH_TEST);


//사용 예)
//void main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //--- 깊이 검사 모드도 같이 설정
//	…
//		glEnable(GL_DEPTH_TEST); //--- 상태 설정은 필요한 곳에서 하면 된다.
//}
//void drawScene()
//{
//	glClearColor(0.0, 0.0, 0.0, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //--- 깊이 버퍼를 클리어한다.
//	…
//}




