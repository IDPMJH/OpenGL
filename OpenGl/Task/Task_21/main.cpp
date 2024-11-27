#define _CRT_SECURE_NO_WARNINGS
#include "global.h"
#include "Polygon_19.h"
#include "Line_19.h"
#include "component_19.h"
#include "Circle.h"
#include "Model.h"
#include "Rect.h"
#include "Crane.h"
#include "Circle.h"

enum Rotate_Type
{
	Rotate_none,
	Rotate_x_p,
	Rotate_x_m,
	Rotate_y_p,
	Rotate_y_m,
	Rotate_end,
};

enum TTM
{
    Mode_Default,
    Mode_On,
    Mode_Off,
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


int window_x = 1200;
int window_y = 800;



GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수

bool on_timer = false;
bool on_mouse = true;
bool draw_cube = true;
bool on_depth_test = true;
bool on_culling = true;
bool on_revolving = false;
bool on_perspective = true;
bool on_light = true;

float max_x = 3.f;
float max_z = 3.f;

TTM x_trs = Mode_Default;
float b = 0.f;

TTM y_rot = Mode_Default;
float m_theta = 0.f;

TTM leg_rot = Mode_Default;
float leg_theta = 0.f;

TTM leg_trs = Mode_Default;
float leg_distance = 0.f;

TTM cannon_rot = Mode_Default;
float cannon_theta = 0.f;

TTM cam_rot = Mode_Default;
float cm_rot = 0.f;

TTM light_rot = Mode_Default;
float light_theta = 0.f;



float thetax = 0.f;
float thetay = 0.f;
float thetaz = 0.f;


float xoffset = 0.f;
float yoffset = 0.f;
float zoffset = 4.f;
float cm_xoffset = 0.f;
float cm_yoffset = 0.f;
float cm_zoffset = 0.f;

GLuint vao1[4], vao2[3], vao3[3], vao4[3], vaol[2];
GLuint vao[4];
GLuint vaom[12];
GLuint vbo[4];
glm::vec3 clcolor(0.f, 0.f, 0.f);
GLuint VAO[100];
GLuint VBO, EBO;


mat4 rotateT = mat4(1.f);
mat4 translatT = mat4(1.f);




Cube* pcube;
Cube* plight_cube;
Circle* pcircle;
Pyramid* ppyramid;
Rect_p rect;
vector<class::Line*> Lines;
vector<class::Polygon*> vpolygons;
vec3 g_camerapos = vec3(0.f, 0.f, 0.f);

glm::mat4 invertTranslation(const glm::mat4& matrix) {
    glm::mat4 invertedMatrix = matrix;
    invertedMatrix[3][0] = -matrix[3][0]; // x축 이동 거리 음수로 변경
    invertedMatrix[3][1] = -matrix[3][1]; // y축 이동 거리 음수로 변경
    invertedMatrix[3][2] = -matrix[3][2]; // z축 이동 거리 음수로 변경
    return invertedMatrix;
}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
{
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(window_x, window_y); // 윈도우의 크기 지정
	int i = glutCreateWindow("Task_24"); // 윈도우 생성 (윈도우 이름)
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
	make_shaderProgram();
    pcube = new Cube();
    ppyramid = new Pyramid();
    plight_cube = new Cube();
    pcircle = new Circle(4.f, 100, shaderProgramID);
    read_obj_file("cube.obj", *pcube);
    read_obj_file("cube.obj", *plight_cube);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    vpolygons.push_back(&rect);
 
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

    // 각 행렬 초기화
    mat4 identity = mat4(1.0f);
    mat4 fRotT = mat4(1.f);
	mat4 modelT = glm::mat4(1.0f);
	mat4 trsT = glm::mat4(1.0f);
    mat4 viewT = mat4(1.0f);
    mat4 projectionT = mat4(1.0f); //glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // =====================View transformation=====================
    float radius = 5.0f;
    float camX = sin(radians(cm_rot)) * radius;
    float camZ = cos(radians(cm_rot)) * radius;
    vec3 cameraPos = vec3(camX, 0.5f, camZ + 3.f);  //--- 카메라 위치 EYE
    g_camerapos = cameraPos;

    // 카메라 방향 (camerDir) 구하는 방법
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라가 바라보는 곳 AT
    glm::vec3 cameraDir = glm::normalize(cameraPos - cameraTarget); //--- 카메라 방향 벡터: z축 양의 방향: 정규화(EYE - AT)
    // cameraDir = glm::normalize(vec3(0.0f, cm_yoffset-0.1f, -1.0f));// 카메라가 바라보는 방향 (n)
   
   /* 위쪽 벡터와 카메라 방향 벡터와의 외적 = > camera right벡터를 구할 수 있음
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); = UP
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDir)); --- 카메라의 오른쪽 축(u)
    glm::vec3 cameraUp = glm::cross(cameraDir,cameraRight); u x n = v*/ 
    vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);// 카메라의 위쪽 축(v)
	
    viewT = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    //glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    
    // =====================Projection transformation=====================
   if(on_perspective == true)
       projectionT = glm::perspective(glm::radians(45.0f), (float)window_x / (float)window_y, 0.1f, 100.0f);
   else
       projectionT = glm::ortho(-2.f, 2.f, -2.f, 2.f, 0.1f, 100.f); 

    projectionT = glm::translate(projectionT, vec3(0.f, 0.f, -2.f));
    
    // 행렬 위치 받아놓음
	GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
	GLuint viewLoc = glGetUniformLocation(shaderProgramID, "view");
	GLuint projectionLoc = glGetUniformLocation(shaderProgramID, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewT));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionT));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(identity));

    glBindVertexArray(VAO[0]);
    if(vpolygons.size() > 0)
        vpolygons[0]->Draw_Polygon();


    float base_width = 3.f;
    float base_height = 3.f;
    float base_depth = 3.f;
    vec4 base_center = vec4(0.f, 0.f, 0.f, 1.f);

    mat4 model = mat4(1.0f);
    mat4 trs = mat4(1.0f);
    mat4 trs1 = mat4(1.0f);
    mat4 trs2 = mat4(1.0f);
    mat4 rot = mat4(1.0f);
    mat4 rot2 = mat4(1.0f);
    mat4 scale = mat4(1.0f);
    scale = glm::scale(identity, vec3(base_width, base_height, base_depth));
    float body_width = base_width;
    float body_height = base_height;
    float body_depth = base_depth;

    model = trs * rot * scale;  // 기본 단위행렬 집합 (scale만 좀 키움)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    // ======================= Cube =======================

    // 조명
    vec4 light_pos = vec4(0.f, 0.f, zoffset, 1.f);
    mat4 light_rot = glm::rotate(mat4(1.f), radians(light_theta), vec3(0.f, 1.f, 0.f));
    vec4 fpos = light_rot * light_pos;
    unsigned int lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
    glUniform3f(lightPosLocation, fpos.x,fpos.y,fpos.z);


    unsigned int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
    if (on_light == true)
        glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
    else
        glUniform3f(lightColorLocation, 0.1, 0.1, 0.1);
 
  
    unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
    glUniform3f(objColorLocation, 1.0, 0.2f, 0.3);
    unsigned int viewPosLocation = glGetUniformLocation(shaderProgramID, "viewPos"); //--- viewPos 값 전달: 카메라 위치
    glUniform3f(viewPosLocation, g_camerapos.x, g_camerapos.y, g_camerapos.z);


    plight_cube->_trs = glm::translate(mat4(1.f), vec3(0.f, 0.0f, zoffset));
    plight_cube->_rot = glm::rotate(mat4(1.f), radians(light_theta), vec3(0.f, 1.f, 0.f));
    plight_cube->_scale = glm::scale(mat4(1.f), vec3(0.2f, 0.2f, 0.2f));
    plight_cube->_FT = plight_cube->_rot * plight_cube->_trs * plight_cube->_scale* model;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(plight_cube->_FT));
    plight_cube->Draw(shaderProgramID);


    mat4 crot = glm::rotate(mat4(1.f), radians(90.f), vec3(1.f, 0.f, 0.f));
    pcircle->setRadius(zoffset);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(crot));
    if(pcircle != nullptr)
        pcircle->draw();



    pcube->_rot = glm::rotate(mat4(1.f), radians(m_theta), vec3(0.f, 1.f, 0.f));
    pcube->_FT = pcube->_rot * model;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pcube->_FT));
    if (pcube != nullptr && draw_cube == true)
        pcube->Draw(shaderProgramID);
    else if (ppyramid != nullptr && draw_cube == false)
        ppyramid->Draw(shaderProgramID);
 
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
    case 'z':
        zoffset -= 0.1f;
        break;
    case 'Z':
        zoffset += 0.1f;
        break;
    case 'n':
        draw_cube = !draw_cube;
        break;
    case 'X':
        cm_xoffset -= 0.1f;
        break;
    case 'm':
        on_light = !on_light;
        break;
    case 'Y':
        cm_yoffset -= 0.1f;
        break;
    case 'r':
        if (light_rot == Mode_On)
        {
            light_rot = Mode_Default;
            on_timer = false;
        }
        else
        {
            light_rot = Mode_On;
            on_timer = true;
            glutTimerFunc(10, UserTimerFunc, 1);
        }
        break;
    case 'R':
        cm_rot -= 1.f;
        break;
    case 'a':
        if (cam_rot == Mode_On)
        {
            cam_rot = Mode_Default;
            on_timer = false;
        }
        else
        {
            cam_rot = Mode_On;
            on_timer = true;
            glutTimerFunc(10, UserTimerFunc, 1);
        }
        break;
    case 'A':
        if (cam_rot == Mode_Off)
        {
            cam_rot = Mode_Default;
            on_timer = false;
        }
        else
        {
            cam_rot = Mode_Off;
            on_timer = true;
            glutTimerFunc(10, UserTimerFunc, 1);
        }
        break;
    case 'b':
        if (x_trs == Mode_On)
        {
            x_trs = Mode_Default;
            on_timer = false;
        }
        else
        {
            x_trs = Mode_On;
            on_timer = true;
            glutTimerFunc(10, UserTimerFunc, 1);
        }
        break;
    case 'B':
        if (x_trs == Mode_Off)
        {
            x_trs = Mode_Default;
            on_timer = false;
        }
        else
        {
            x_trs = Mode_Off;
            on_timer = true;
            glutTimerFunc(10, UserTimerFunc, 1);
        }
        break;
    case 'y':
        if (y_rot == Mode_On)
        {
            y_rot = Mode_Default;
            on_timer = false;
        }
        else
        {
            y_rot = Mode_On;
            on_timer = true;
            glutTimerFunc(10, UserTimerFunc, 1);
        }
        break;
    case 'M':
        if (y_rot == Mode_Off)
        {
            y_rot = Mode_Default;
            on_timer = false;
        }
        else
        {
            y_rot = Mode_Off;
            on_timer = true;
            glutTimerFunc(10, UserTimerFunc, 1);
        }
        break;
    case 'f':
        if (leg_rot == Mode_On)
        {
            leg_rot = Mode_Default;
            on_timer = false;
        }
        else
        {
            leg_rot = Mode_On;
            on_timer = true;
            glutTimerFunc(10, UserTimerFunc, 1);
        }
        break;
    case 'F':
        if (leg_rot == Mode_Off)
        {
            leg_rot = Mode_Default;
            on_timer = false;
        }
        else
        {
            leg_rot = Mode_Off;
            on_timer = true;
            glutTimerFunc(10, UserTimerFunc, 1);
        }
        break;
    case 'e':
        if (leg_trs == Mode_On)
        {
            leg_trs = Mode_Default;
            on_timer = false;
        }
        else
        {
            leg_trs = Mode_On;
            on_timer = true;
            glutTimerFunc(10, UserTimerFunc, 1);
        }
        break;
    case 'E':
        if (leg_trs == Mode_Off)
        {
            leg_trs = Mode_Default;
            on_timer = false;
        }
        else
        {
            leg_trs = Mode_Off;
            on_timer = true;
            glutTimerFunc(10, UserTimerFunc, 1);
        }
        break;
    case 't':
        if (cannon_rot == Mode_On)
        {
            cannon_rot = Mode_Default;
            on_timer = false;
        }
        else
        {
            cannon_rot = Mode_On;
            on_timer = true;
            glutTimerFunc(10, UserTimerFunc, 1);
        }
        break;
    case 'T':
        if (cannon_rot == Mode_Off)
        {
            cannon_rot = Mode_Default;
            on_timer = false;
        }
        else
        {
            cannon_rot = Mode_Off;
            on_timer = true;
            glutTimerFunc(10, UserTimerFunc, 1);
        }
        break;

    case 's':
        on_timer = false;
        break;
    case 'S':
        on_timer = true;
        break;
    case 'c':
        x_trs = Mode_Default;
        b = 0.f;

        y_rot = Mode_Default;
        m_theta = 0.f;

        leg_rot = Mode_Default;
        leg_theta = 0.f;

        leg_trs = Mode_Default;
        leg_distance = 0.f;

        cannon_rot = Mode_Default;
        cannon_theta = 0.f;

        on_timer = false;
        break;
    case 'q':
        glutLeaveMainLoop();
    }
	InitBuffer();
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid sp_Keyboard(int key, int x, int y)
{
}

void InitBuffer()
{
    glUseProgram(shaderProgramID);
	glGenVertexArrays(10, vao1); //--- VAO 를 지정하고 할당하기

	glGenVertexArrays(10, VAO);
	glGenBuffers(10, vbo);
	glGenBuffers(1, &EBO);
    
   

	/*여러 다각형이 그려지지 않는 이유는
	glGenVertexArrays 함수가 루프 내에서
	반복 호출되기 때문일 수 있습니다.
	이 함수는 새로운 VAO를 생성하고 vao1 배열에 저장합니다.
	그러나 이 배열은 루프가 반복될 때마다 덮어쓰여지므로,
	마지막 다각형만이 올바르게 그려질 수 있습니다.*/

	//size_t i;
    //size_t j;
	//for (i = 0; i < vpolygons.size(); i++)
	//{
	//	glBindVertexArray(VAO[i]); //--- VAO를 바인드하기
	//	vpolygons[i]->init_buffer_polygon(VAO, vbo);
	//}
    if (plight_cube != nullptr)
        plight_cube->model_init_buffer();

    if (pcircle != nullptr)
        pcircle->initBuffer();

    if (pcube != nullptr && draw_cube == true)
    {
        pcube->model_init_buffer();
    }
    else
        ppyramid->model_init_buffer();


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
    if (x_trs == Mode_On)
    {
        b += 0.1f;
        if (b >= max_x)
        {
            b = max_x;
            x_trs = Mode_Default;
            on_timer = false;

        }
    }
    else if (x_trs == Mode_Off)
    {
        b -= 0.1f;
        if (b <= -max_x)
        {
            b = -max_x;
            x_trs = Mode_Default;
            on_timer = false;

        }
    }
    if (y_rot == Mode_On)
    {
        m_theta += 1.f;
        if (m_theta >= 360.f)
        {
            m_theta = 0.f;
            y_rot = Mode_Default;
            on_timer = false;
        }
    }
    else if (y_rot == Mode_Off)
    {
        m_theta -= 1.f;
        if (m_theta <= -360.f)
        {
            m_theta = 0.f;
            y_rot = Mode_Default;
            on_timer = false;
        }
    }
    if (leg_rot == Mode_On)
    {
        leg_theta += 1.f;
        if (leg_theta >= 360.f)
        {
            leg_theta = 0.f;
            leg_rot = Mode_Default;
            on_timer = false;
        }
    }
    else if (leg_rot == Mode_Off)
    {
        leg_theta -= 1.f;
        if (leg_theta <= -360.f)
        {
            leg_theta = 0.f;
            leg_rot = Mode_Default;
            on_timer = false;
        }
    }
    

    if (leg_trs == Mode_On && leg_theta == 0.f)
    {
        leg_distance += 0.1f;
        if (leg_distance >= 0.75f - 0.1f)
        {
            leg_distance = 0.75f - 0.1f;
            leg_trs = Mode_Default;
            on_timer = false;
        }
    }
    else if (leg_trs == Mode_Off && leg_theta == 0.f)
    {
        leg_distance -= 0.1f;
        if (leg_distance <= 0.f)
        {
            leg_distance = 0.f;
            leg_trs = Mode_Default;
            on_timer = false;
        }
    }
    if (cannon_rot == Mode_On)
    {
        cannon_theta += 1.f;
        if (cannon_theta >= 90.f)
        {
            cannon_theta = 90.f;
            cannon_rot = Mode_Default;
            on_timer = false;
        }
    }
    else if (cannon_rot == Mode_Off)
    {
        cannon_theta -= 1.f;
        if (cannon_theta <= 0)
        {
            cannon_theta = 0.f;
            cannon_rot = Mode_Default;
            on_timer = false;
        }
    }

    if (cam_rot == Mode_On)
    {
        cm_rot += 1.f;
        if (cm_rot >= 360.f)
        {
            cm_rot = 0.f;
            cam_rot = Mode_Default;
            on_timer = false;
        }
    }
    else if (cam_rot == Mode_Off)
    {
        cm_rot -= 1.f;
        if (cm_rot <= -360.f)
        {
            cm_rot = 0.f;
            cam_rot = Mode_Default;
            on_timer = false;
        }
    }
    if (light_rot == Mode_On)
    {
        light_theta += 1.f;
        if (light_theta >= 360.f)
        {
            light_theta = 0.f;
            light_rot = Mode_Default;
            on_timer = false;
        }
    }
    else if (light_rot == Mode_Off)
    {
        light_theta -= 1.f;
        if (light_theta <= -360.f)
        {
            light_theta = 0.f;
            light_rot = Mode_Default;
            on_timer = false;
        }
    }
    InitBuffer();
	glutPostRedisplay();
	if (on_timer)
		glutTimerFunc(10, UserTimerFunc, 1);
}




