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

#include "../forCopy/CoordSystem.h"
#include "../forCopy/useful_func.cpp"

using namespace std;

/* OpenGL */
int WINDOW_WIDTH  = 500;
int WINDOW_HEIGHT = 500;
int UPDATE_TIME = 1;
bool using_pers = false;
bool using_timer = true;
bool using_buffer = false;
bool playing = true;
// click
bool left_click, clicked;
vector<int> click_pos(2, 0);
// camera
vector<double> camera_pos(3, 0);
vector<double> camera_temp(3, 0);
// glCircle
double horizontal, vertical, phase, rate = 1, increase_rate = M_PI * 5/180, t_radius = 50;
bool isfill, hv;
vector<double> t_temp;

void doing() {
    if (!playing) return;
    phase += 0.1;
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

vector<double> daen(double radius, double theta, double a, double b) {
    vector<double> res(2, 0);
    res[0] = radius * a * cos(theta);
    res[1] = radius * b * sin(theta);
    return res;
}

void glCircle(double x0, double y0, double z0, double radius, double horizontal, double vertical, double phase, int vertexes, double rate, bool isfill) { //, bool hv
    if (rate >= 1) {
        rate = 1;
    }
    if (isfill) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex3d(x0, y0, z0);
    } else {
        glBegin(GL_LINE_STRIP);
    }
    double cx, cy, cz, theta;
    double freqency = M_PI * 2 / vertexes;
    vector<double> circle_hol, circle_ver;
    for (int i = 0; i <= vertexes; i++) {
        theta = i * freqency + phase;
        // old
        // cx = radius * cos(theta);
        // cy = radius * sin(theta);
        // new
        // cx = radius * cos(i * freqency + horizontal);
        // cy = radius * sin(vertical);
        // cz = radius * sin(i * freqency + horizontal);
        // vector
        // normal = CommonCoordSystem::pol2cart(1, horizontal + M_PI/2, vertical + M_PI/2);
        // circle_ver = CommonCoordSystem::pol2cart(radius, horizontal, theta);
        // circle_hol = CommonCoordSystem::pol2cart(radius, horizontal + theta, vertical);
        // circle_ver = CommonCoordSystem::pol2cart(radius, horizontal, vertical + theta);
        // circle_hol = CommonCoordSystem::pol2cart(radius, horizontal + (theta)*(!hv), vertical + (theta)*hv);
        // print_vec("vertical", circle_ver);
        // print_vec("horizontal", circle_hol);
        // only
        // glVertex3d(x0 + circle_hol[0], y0 + circle_hol[1], z0 + circle_hol[2]);
        // glVertex3d(x0 + circle_ver[0], y0 + circle_ver[1], z0 + circle_ver[2]);
        // glVertex3d(x0 + circle_hol[0], y0 + circle_hol[1], z0 + circle_hol[2] * cos(vertical));
        // glVertex3d(x0 + circle_ver[0], y0 + circle_ver[1] * sin(vertical), z0 + circle_ver[2]);
        //
        // glVertex3d(x0 + circle_hol[0] + circle_ver[0], y0 + circle_hol[1] + circle_ver[1], z0 + circle_hol[2] + circle_ver[2]);
        // glVertex3d(x0 + (circle_hol[0] + circle_ver[0])/2, y0 + (circle_hol[1] + circle_ver[1])/2, z0 + (circle_hol[2] + circle_ver[2])/2);
        // glVertex3d(x0 + (circle_hol[0] + circle_ver[0])/2, y0 + circle_ver[1] * sin(vertical), z0 + circle_hol[2] * cos(vertical));
        // glVertex3d(x0 + circle_hol[0], y0 + circle_ver[1] * sin(vertical), z0 + circle_hol[2] * cos(vertical));
        // glVertex3d(x0 + circle_hol[0], y0 + circle_ver[1], z0 + circle_hol[2]);
        // glVertex3d(x0 + circle_hol[0], y0 + circle_hol[1] * sin(theta), z0 + circle_hol[2]);
        // glVertex3d(x0 + circle_ver[0], y0 + circle_hol[1] * sin(theta), z0 + circle_ver[2]);
        //
        // glVertex3d(x0, y0, z0);

        // daen
        cx = daen(radius, theta, cos(vertical + M_PI/2), 1)[0];
        cy = daen(radius, theta, sin(vertical + M_PI/2), 1)[0];
        cz = daen(radius, theta, cos(vertical + M_PI/2), 1)[1];
        // print_vec("ccc_c", vector<double>{cx, cy, cz});
        circle_hol = CommonCoordSystem::cart2pol(cx, 0, cz);
        // print_vec("hol_p", circle_hol);
        circle_hol[1] += horizontal;
        circle_hol = CommonCoordSystem::pol2cart(circle_hol);
        // print_vec("hol_c", circle_hol);
        cx = circle_hol[0];
        cz = circle_hol[2];
        glVertex3d(x0 + cx, y0 + cy, z0 + cz);

        /* 片方ずつなら */
        // circle_ver = CommonCoordSystem::pol2cart(radius, horizontal, theta);
        // circle_hol = CommonCoordSystem::pol2cart(radius, horizontal + theta, 0);
        // glVertex3d(x0 + circle_hol[0], y0 + circle_ver[1] * sin(vertical), z0 + circle_hol[2] * cos(vertical));

        if (i >= rate * vertexes) {
            i = vertexes + 1;
        }
    }
    glEnd();
}

void display(void) {
    InitDisplay();
    //
    // theta check
    glArrow(0, 0, 0, 100, horizontal, vertical);
    // camera_pos = CommonCoordSystem::cart2pol(150, horizontal + M_PI/2, vertical + M_PI/2);
    //
    glAxis();
    // glCircle(0, 0, 0, 100, horizontal, vertical, phase, 50, rate, isfill);
    glCircle3D(0, 0, 0, 100, horizontal, vertical, phase, 50, rate, isfill);
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
        case 'a':
            horizontal += increase_rate;
            break;
        case 'd':
            horizontal -= increase_rate;
            break;
        case 'w':
            vertical += increase_rate;
            break;
        case 's':
            vertical -= increase_rate;
            break;
        case 't':
            rate += 0.1;
            if (rate > 1) {
                rate = 1;
            }
            break;
        case 'y':
            rate -= 0.1;
            if (rate < 0) {
                rate = 0;
            }
            break;
        case 'r':
            horizontal = 0;
            vertical = 0;
            camera_temp = {150, M_PI/4, M_PI/4};
            phase = 0;
            break;
        case 'f':
            isfill = !isfill;
            break;
        case 'v':
            hv = !hv;
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
    camera_pos = CommonCoordSystem::pol2cart(150, M_PI/4, M_PI/4);
    // cout << "UPDATE_TIME: " << UPDATE_TIME << endl;
    cout << "Window Size: (" << WINDOW_WIDTH << ", " << WINDOW_HEIGHT << ")" << endl;
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (using_buffer) {
        glutInitDisplayMode(GLUT_DOUBLE);
    }
    glutCreateWindow("glCircle");
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
