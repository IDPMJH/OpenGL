#include <GL/glut.h>
#include <cmath>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// �ʱ� ũ�� �� ����� ����
float size = 1.0f;
float scale = 0.9f;

// �׸��� �Լ�
void drawSpiralSquares() {
    float angle = 0.0f;
    float x = 0.0f, y = 0.0f;

    glClear(GL_COLOR_BUFFER_BIT);

    // �ݺ��ؼ� �簢���� �׸�
    for (int i = 0; i < 50; ++i) {
        glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        glScalef(size, size, 1.0f);

        // �簢�� �׸���
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
        glEnd();

        glPopMatrix();

        // ������ ũ�⸦ �����Ͽ� ���� �簢���� ��ġ�� ũ�� ����
        angle += 30.0f; // ���� ����
        size *= scale;  // ũ�� ���

        // ���� �簢���� �߽� ��ġ ����
        float distance = size * 0.5f;
        x += distance * cos(angle * M_PI / 180.0f);
        y += distance * sin(angle * M_PI / 180.0f);
    }

    glutSwapBuffers();
}

// ������ �ʱ�ȭ �� ����
void initialize() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // ������ ���������� ����
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0); // ��ǥ�踦 ����
    glMatrixMode(GL_MODELVIEW);
}

// ���� �Լ�
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
