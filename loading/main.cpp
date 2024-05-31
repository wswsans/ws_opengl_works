/* include */
#include <iostream>
#include <cmath>

// http://www.den.t.u-tokyo.ac.jp/ad_prog/graphics/
#ifndef __APPLE__
#include <GL/glut.h> // Windows, Linux
#else
#include <GLUT/glut.h> // Mac
#endif
// https://fujiwaratko.sakura.ne.jp/gcc/glut/glut.html
#include "../forCopy/jfont.h"
using namespace std;

/* OpenGL */
int WINDOW_SCALE = 50;
int WINDOW_WIDTH  = 16 * WINDOW_SCALE;
int WINDOW_HEIGHT = 9 * WINDOW_SCALE;
int UPDATE_TIME = 1;
bool using_pers = false;
bool using_timer = true;
bool using_buffer = false;
bool playing = true;
//
double phase = 0;
double rate_phase = 0;
string loading_str, dots;

void doing() {
    if (!playing) return;
    phase += 0.1;
    rate_phase += 0.01;
    switch ((int)fmod(phase, 3)) {
        case 0:
            dots = ".";
            break;
        case 1:
            dots = "..";
            break;
        case 2:
            dots = "...";
            break;
    }
    loading_str = "Now Loading " + dots;
}
void timer(int value) {
    doing();
    glutPostRedisplay();
    glutTimerFunc(UPDATE_TIME, timer, 0);
}
void idle() {
    doing();
    glutPostRedisplay();
}

void InitDisplay() {
    // Window
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT); //背景色で塗る
    glLineWidth(1.5);
    glPointSize(1);
    glColor3d(1, 1, 1);
    // Camera
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (using_pers) {
        // (fov, aspect, near, far)
        gluPerspective(90, 1, 1e-2, pow(10, 10));
        // (left, right), (bottom ,top), (near, far)
        // glFrustum(-10, 10, -10, 10, 1, 11);
    } else {
        // (left, right), (bottom, top), (near, far)
        glOrtho(- WINDOW_WIDTH/2, WINDOW_WIDTH/2, - WINDOW_HEIGHT/2, WINDOW_HEIGHT/2, -50, 100);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // camera(x, y, z), look(x, y, z), upward_vec(x, y, z)
    gluLookAt(
        0, 0, 100,
        0, 0, 0,
        0, 1, 0
    );
    // Save Image
    // glPixelStorei(GL_PACK_ALIGNMENT ,1);
    // glReadBuffer(GL_FRONT);
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
}

//! @brief 文字列を表示
//! @param [in] x 三次元の座標(X)
//! @param [in] y 三次元の座標(Y)
//! @param [in] z 三次元の座標(Z)
//! @param [in] str 文字列（英語のみ）
void render_string(float x, float y, float z, const char* str) {
    glRasterPos3f(x, y, z);

    const char* c = str;
    while (*c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c++);
    }
}

void glCircle(double center_x, double center_y, double center_z, double radius, double phase, int vertexes, double rate, bool isfill) {
    if (rate >= 1) {
        rate = 1;
    }
    if (isfill) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex3d(center_x, center_y, center_z);
    } else {
        glBegin(GL_LINE_STRIP);
    }
    double cx, cy;
    double rotation = M_PI * 2 / vertexes;
    for (int i = 0; i <= vertexes; i++) {
        cx = radius * cos(i * rotation + phase);
        cy = radius * sin(i * rotation + phase);
        glVertex3d(center_x + cx, center_y + cy, center_z);
        if ((double)i/(double)vertexes >= rate) {
            i = vertexes + 1;
        }
    }
    glEnd();
}

void glAxis() {
    /* xyz axis */
    glBegin(GL_LINES);
    // x: blue
    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex3d(1000, 0, 0);
    // y: red
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 1000, 0);
    // z: green
    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, 1000);
    glEnd();
    glColor3d(1, 1, 1);
}

void display(void) {
        InitDisplay();
        //
        // debug: 軸
        // glAxis();
        // ぐるぐる
        glLineWidth(5);
        glCircle(WINDOW_WIDTH/2*(0.5), WINDOW_HEIGHT/2*(-0.75), 0, 15, phase, 150, pow(sin(rate_phase), 2), false);
        glLineWidth(1.5);
        /*
        Tips:
            豆腐からsin3は去ってしまったけれど、
            彼は豆腐生の心の中で生き続ける
        */
        render_string(WINDOW_WIDTH/2*(-0.4), WINDOW_HEIGHT/2*(0.45) - 7.5, 0, "Tips:");
        render_jstring(WINDOW_WIDTH/2*(-0.3), WINDOW_HEIGHT/2*(0.2) - 7.5, "豆腐からsin3は去ってしまったけれど、");
        render_jstring(WINDOW_WIDTH/2*(-0.3), WINDOW_HEIGHT/2*(0.0) - 7.5, "彼は豆腐生の心の中で生き続ける");
        // Now Loading...
        render_string(WINDOW_WIDTH/2*(0.6), WINDOW_HEIGHT/2*(-0.75) - 7.5, 0, loading_str.c_str());
        //
        glFlush();
        if (using_buffer) {
            glutSwapBuffers();
        }
}

void keyboard(unsigned char key, int x, int y) {
    // printf("key %c at (%d, %d)\n", key, x, y);
    switch (key) {
        case 'q':
            cout << "Quit" << endl;
            exit(0);
            break;
        case ' ':
            playing = !playing;
            break;
    }
}

int main(int argc, char **argv) {
    // cout << "UPDATE_TIME: " << UPDATE_TIME << endl;
    cout << "Window Size: (" << WINDOW_WIDTH << ", " << WINDOW_HEIGHT << ")" << endl;
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (using_buffer) {
        glutInitDisplayMode(GLUT_DOUBLE);
    }
    glutCreateWindow("sin3 loading...");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    if (using_timer) {
        glutIdleFunc(idle);
    } else {
        glutTimerFunc(UPDATE_TIME, timer, 0);
    }
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}
