/* include */
#include <iostream>
#include <cmath>

// http://www.den.t.u-tokyo.ac.jp/ad_prog/graphics/
#ifndef __APPLE__
#include <GL/glut.h> // Windows, Linux
#else
#include <GLUT/glut.h> // Mac
#endif

using namespace std;

/* OpenGL */
int WINDOW_WIDTH  = 500;
int WINDOW_HEIGHT = 500;
int UPDATE_TIME = 1;
bool using_pers = true;
bool using_timer = true;
bool using_buffer = false;
bool playing = true;
// click
bool left_click, clicked;
vector<double> click_pos(2, 0);

void doing() {
    if (!playing) return;
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
        // gluPerspective(90, 1, 1e-2, pow(10, 10));
        // (left, right), (bottom ,top), (near, far)
        // glFrustum(-10, 10, -10, 10, 1, 11);
        glFrustum(- WINDOW_WIDTH/2, WINDOW_WIDTH/2, - WINDOW_HEIGHT/2, WINDOW_HEIGHT/2, -50, 100); // WINDOW SIZE is position
    } else {
        // (left, right), (bottom, top), (near, far)
        // glOrtho(-10, 10, -10, 10, 1, 11);
        glOrtho(- WINDOW_WIDTH/2, WINDOW_WIDTH/2, - WINDOW_HEIGHT/2, WINDOW_HEIGHT/2, -50, 100); // WINDOW SIZE is position
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // camera(x, y, z), look(x, y, z), upward_vec(x, y, z)
    gluLookAt(
        0, 0, 1,
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

void display(void) {
    InitDisplay();
    //
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

void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            left_click = true;
            break;
        // case GLUT_MIDDLE_BUTTON:
        //     break;
        // case GLUT_RIGHT_BUTTON:
        //     break;
        default:
            left_click = false;
            click_pos = {WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2};
            break;
    }
    switch (state) {
        case GLUT_UP:
            clicked = false;
            break;
        case GLUT_DOWN:
            clicked = true;
            break;
        default:
            break;
    }
    click_pos = {x, y};
}

int main(int argc, char **argv) {
    // cout << "UPDATE_TIME: " << UPDATE_TIME << endl;
    cout << "Window Size: (" << WINDOW_WIDTH << ", " << WINDOW_HEIGHT << ")" << endl;
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (using_buffer) {
        glutInitDisplayMode(GLUT_DOUBLE);
    }
    glutCreateWindow("Title");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    if (using_timer) {
        glutIdleFunc(idle);
    } else {
        glutTimerFunc(UPDATE_TIME, timer, 0);
    }
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
}
