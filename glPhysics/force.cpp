/* include */
#include <iostream>
#include <cmath>
#include <vector>

// http://www.den.t.u-tokyo.ac.jp/ad_prog/graphics/
#ifndef __APPLE__
#include <GL/glut.h> // Windows, Linux
#else
#include <GLUT/glut.h> // Mac
#endif

using namespace std;

#include "../forCopy/CoordSystem.h"
#include "../forCopy/useful_func.cpp"
#include "Physics.h"

/* OpenGL */
int WINDOW_WIDTH  = 500;
int WINDOW_HEIGHT = 500;
int UPDATE_TIME = 1;
bool using_pers = false;
bool using_timer = false;
bool using_buffer = true;
bool playing = true;
// camera
double increase_rate = M_PI * 5/180;
vector<double> camera_pos(3, 0);
vector<double> camera_temp(3, 0);
// click
bool left_click, clicked;
vector<int> click_pos(2, 0);
// Physics
double cube_size = 20, cube_mass = 10;
Physics cube(cube_mass, cube_size);
vector<double> temp;

void doing() {
    if (!playing) return;
    cube.action();
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
        glFrustum(- WINDOW_WIDTH/2, WINDOW_WIDTH/2, - WINDOW_HEIGHT/2, WINDOW_HEIGHT/2, -50, 500); // WINDOW SIZE is position
    } else {
        // (left, right), (bottom, top), (near, far)
        // glOrtho(-10, 10, -10, 10, 1, 11);
        glOrtho(- WINDOW_WIDTH/2, WINDOW_WIDTH/2, - WINDOW_HEIGHT/2, WINDOW_HEIGHT/2, -50, 500); // WINDOW SIZE is position
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // camera(x, y, z), look(x, y, z), upward_vec(x, y, z)
    gluLookAt(
        camera_pos[0], camera_pos[1], camera_pos[2],
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
    glAxis();
    temp = cube.getCart(0);
    cube.setCart(0, temp[0], 0, 0);
    print_vec("cube pos", temp);
    glCube(temp[0], temp[1], temp[2], cube.getSize());
    glArrow(temp, cube.getForceCart());
    cube.setForceCart(-temp[0], 0, 0);
    //
    glFlush();
    if (using_buffer) {
        glutSwapBuffers();
    }
}

void keyboard(unsigned char key, int x, int y) {
    // printf("key %c at (%d, %d)\n", key, x, y);
    camera_temp = CommonCoordSystem::cart2pol(camera_pos);
    switch (key) {
        case 'i':
            camera_temp[2] += increase_rate;
            break;
        case 'k':
            camera_temp[2] -= increase_rate;
            break;
        case 'j':
            camera_temp[1] += increase_rate;
            break;
        case 'l':
            camera_temp[1] -= increase_rate;
            break;
        case 'u':
            camera_temp[0] -= 2;
            break;
        case 'o':
            camera_temp[0] += 2;
            break;
        case 'f':
            cube.action();
            break;
        case 'q':
            cout << "Quit" << endl;
            exit(0);
            break;
        case ' ':
            playing = !playing;
            break;
    }
    camera_pos = CommonCoordSystem::pol2cart(camera_temp);
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
            click_pos = {0, 0};
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
    cube.setCart(0, 0, 0, 0);
    cube.setCart(1, 50, 0, 0);
    camera_pos = CommonCoordSystem::pol2cart(150, M_PI/4, M_PI/4);
    // cout << "UPDATE_TIME: " << UPDATE_TIME << endl;
    cout << "Window Size: (" << WINDOW_WIDTH << ", " << WINDOW_HEIGHT << ")" << endl;
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (using_buffer) {
        glutInitDisplayMode(GLUT_DOUBLE);
    }
    glutCreateWindow("Physics - Force");
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
