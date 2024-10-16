#include <GL/glut.h>
#include <cmath>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 초기 크기 및 축소율 설정
float size = 1.0f;
float scale = 0.9f;

// 그리기 함수
void drawSpiralSquares() {
    float angle = 0.0f;
    float x = 0.0f, y = 0.0f;

    glClear(GL_COLOR_BUFFER_BIT);

    // 반복해서 사각형을 그림
    for (int i = 0; i < 50; ++i) {
        glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        glScalef(size, size, 1.0f);

        // 사각형 그리기
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
        glEnd();

        glPopMatrix();

        // 각도와 크기를 조절하여 다음 사각형의 위치와 크기 설정
        angle += 30.0f; // 각도 증가
        size *= scale;  // 크기 축소

        // 다음 사각형의 중심 위치 조절
        float distance = size * 0.5f;
        x += distance * cos(angle * M_PI / 180.0f);
        y += distance * sin(angle * M_PI / 180.0f);
    }

    glutSwapBuffers();
}

// 윈도우 초기화 및 설정
void initialize() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 배경색을 검정색으로 설정
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0); // 좌표계를 설정
    glMatrixMode(GL_MODELVIEW);
}

// 메인 함수
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Spiral Squares");

    initialize();

    glutDisplayFunc(drawSpiralSquares);
    glutMainLoop();

    return 0;
}
