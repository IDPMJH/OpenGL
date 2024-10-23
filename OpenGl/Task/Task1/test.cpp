#include <GL/glut.h>
#include <cmath>

// 마우스 좌표와 변의 길이
float mouseX = 0.0f;
float mouseY = 0.0f;
float baseLength = 40.0f; // 밑변의 길이
float sideLength = 60.0f;  // 다른 두 변의 길이

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 삼각형의 세 꼭지점 계산
    float x1 = mouseX;
    float y1 = mouseY + (2.0f / 3.0f) * sideLength;
    float x2 = mouseX - baseLength / 2;
    float y2 = mouseY - (1.0f / 3.0f) * sideLength;
    float x3 = mouseX + baseLength / 2;
    float y3 = y2;

    // 삼각형 그리기
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 윈도우 좌표를 OpenGL 좌표로 변환
        mouseX = (float)x;
        mouseY = (float)(glutGet(GLUT_WINDOW_HEIGHT) - y);
        glutPostRedisplay();
    }
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Isosceles Triangle with Centroid at Mouse Position");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
