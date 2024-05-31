/* include */
#include <iostream>
#include <cmath>
#include <numeric>

// http://www.den.t.u-tokyo.ac.jp/ad_prog/graphics/
#ifndef __APPLE__
#include <GL/glut.h> // Windows, Linux
#else
#include <GLUT/glut.h> // Mac
#endif

#include "../forCopy/CoordSystem.h"
// #include "../forCopy/vec_op.hpp"

using namespace std;

template<typename T>
void print_vec(string name, vector<T> vec) {
    cout << name << ": (";
    for (T i : vec) {
        cout << i << ", ";
    }
    cout << ")" << endl;
}

/* OpenGL */
int WINDOW_WIDTH  = 500;
int WINDOW_HEIGHT = 500;
int UPDATE_TIME = 1;
bool using_pers = true;
bool using_timer = true;
bool using_buffer = false;
bool playing = true;
vector<double> camera_pos(3, 0);
vector<double> camera_temp(3, 0);

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
        gluPerspective(90, 1, 1e-2, pow(10, 10));
        // (left, right), (bottom ,top), (near, far)
        // glFrustum(-10, 10, -10, 10, 1, 11);
    } else {
        // (left, right), (bottom, top), (near, far)
        glOrtho(-10, 10, -10, 10, 1, 11);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // camera(x, y, z), look(x, y, z), upward_vec(x, y, z)
    // camera_pos = {0, 150, 1e-5};
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

void glAxis() {
    /* xyz(brg+) axis */
    glBegin(GL_LINES);
    // x
    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex3d(1000, 0, 0);
    // y
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 1000, 0);
    // z
    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, 1000);
    glEnd();
    glColor3d(1, 1, 1);
}

// BETA, debug
// 完成版は forCopy/useful_func.cpp
/*
 * pol2cartで動くものは, 水平角固定とすると, 仰角はそこから円を描く動作をする
 * だからpol2cartのみで作ることは厳しい
 * そのためpol2cartで上下分を作り, それをまたさらに水平角で動かす必要がある
 * これ多分xy平面の回転だけなんだよな
 * つか, glTranslated使えば?
 * 無理だ, と言うわけで平面の回転でなんとかするしかなさそう
*/ 
void glArrow(double x0, double y0, double z0, double length, double horizonal_t, double vertical_t) {
    // 棒部分
    vector<double> bou = CommonCoordSystem::pol2cart(length, horizonal_t, vertical_t);
    bou[0] += x0;
    bou[1] += y0;
    bou[2] += z0;
    glBegin(GL_LINES);
    glVertex3d(x0, y0, z0);
    glVertex3d(bou[0], bou[1], bou[2]);
    glEnd();

    // 矢印
    double theta = M_PI/6;
    vector<double> arrow_u = CommonCoordSystem::pol2cart(-length * 0.25, horizonal_t + 0*theta/2, vertical_t + theta);
    vector<double> arrow_d = CommonCoordSystem::pol2cart(-length * 0.25, horizonal_t + 0*theta/2, vertical_t - theta);
    vector<double> arrow_r = CommonCoordSystem::pol2cart(length / 25, horizonal_t + M_PI/2, 0);
    vector<double> arrow_l = CommonCoordSystem::pol2cart(length / 25, horizonal_t - M_PI/2, 0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(bou[0], bou[1], bou[2]);
    glVertex3d(
        bou[0] + arrow_u[0] + arrow_r[0],
        bou[1] + arrow_u[1] + arrow_r[1],
        bou[2] + arrow_u[2] + arrow_r[2]
    );
    glVertex3d(
        bou[0] + arrow_d[0] + arrow_r[0],
        bou[1] + arrow_d[1] + arrow_r[1],
        bou[2] + arrow_d[2] + arrow_r[2]
    );
    glVertex3d(
        bou[0] + arrow_d[0] + arrow_l[0],
        bou[1] + arrow_d[1] + arrow_l[1],
        bou[2] + arrow_d[2] + arrow_l[2]
    );
    glVertex3d(
        bou[0] + arrow_u[0] + arrow_l[0],
        bou[1] + arrow_u[1] + arrow_l[1],
        bou[2] + arrow_u[2] + arrow_l[2]
    );
    glVertex3d(
        bou[0] + arrow_u[0] + arrow_r[0],
        bou[1] + arrow_u[1] + arrow_r[1],
        bou[2] + arrow_u[2] + arrow_r[2]
    );
    glEnd();
}
// glRotated使った版
// void glArrow(double x0, double y0, double z0, double length, double horizonal_t, double vertical_t) {
//     vector<double> bou = CommonCoordSystem::pol2cart(length, 0, 0);
//     double theta = M_PI/6;
//     vector<double> arrow_1 = CommonCoordSystem::pol2cart(-length * 0.25, +theta/2, +theta);
//     vector<double> arrow_2 = CommonCoordSystem::pol2cart(-length * 0.25, +theta/2, -theta);
//     vector<double> arrow_3 = CommonCoordSystem::pol2cart(-length * 0.25, -theta/2, -theta);
//     vector<double> arrow_4 = CommonCoordSystem::pol2cart(-length * 0.25, -theta/2, +theta);
//     // 棒部分
//     glBegin(GL_LINES);
//     glVertex3d(0, 0, 0);
//     glVertex3d(bou[0], bou[1], bou[2]);
//     glEnd();
//     // 矢印
//     glBegin(GL_TRIANGLE_FAN);
//     glVertex3d(bou[0], bou[1], bou[2]);
//     glVertex3d(
//         bou[0] + arrow_1[0],
//         bou[1] + arrow_1[1],
//         bou[2] + arrow_1[2]
//     );
//     glVertex3d(
//         bou[0] + arrow_2[0],
//         bou[1] + arrow_2[1],
//         bou[2] + arrow_2[2]
//     );
//     glVertex3d(
//         bou[0] + arrow_3[0],
//         bou[1] + arrow_3[1],
//         bou[2] + arrow_3[2]
//     );
//     glVertex3d(
//         bou[0] + arrow_4[0],
//         bou[1] + arrow_4[1],
//         bou[2] + arrow_4[2]
//     );
//     glVertex3d(
//         bou[0] + arrow_1[0],
//         bou[1] + arrow_1[1],
//         bou[2] + arrow_1[2]
//     );
//     glEnd();
//     vector<double> rotated_bou = CommonCoordSystem::pol2cart(length, horizonal_t, vertical_t); // 回転したときの座標
//     vector<double> normal_bou = CommonCoordSystem::cart2pol(rotated_bou - bou); // 法線
//     normal_bou[1] += M_PI / 2;
//     normal_bou[2] += M_PI / 2;
//     vector<double> dot_bou = rotated_bou * bou; // 内積
//     double rotated_theta = acos( reduce(begin(dot_bou), end(dot_bou)) / (bou[0] * bou[0]) );
//     normal_bou = CommonCoordSystem::pol2cart(normal_bou); // 直交座標に変換
//     glRotated(rotated_theta, normal_bou[0], normal_bou[1], normal_bou[2]);
// }
// pol2cartの本来の使い方を理解した版
// double prev_ho = 0, prev_ve = 0;
// void glArrow(double x0, double y0, double z0, double length, double horizonal_t, double vertical_t) {
//     vector<double> bou = CommonCoordSystem::pol2cart(length, horizonal_t, vertical_t);
//     double theta = M_PI/6;
//     vector<double> arrow_1 = CommonCoordSystem::pol2cart(-length * 0.25, horizonal_t*0 + theta/2, vertical_t*0 + theta);
//     vector<double> arrow_2 = CommonCoordSystem::pol2cart(-length * 0.25, horizonal_t*0 + theta/2, vertical_t*0 - theta);
//     vector<double> arrow_3 = CommonCoordSystem::pol2cart(-length * 0.25, horizonal_t*0 - theta/2, vertical_t*0 - theta);
//     vector<double> arrow_4 = CommonCoordSystem::pol2cart(-length * 0.25, horizonal_t*0 - theta/2, vertical_t*0 + theta);
//     // if (prev_ho != horizonal_t || prev_ve != vertical_t) {
//     //     cout << "(ho, ve): " << horizonal_t << ", " << vertical_t << endl;
//     //     cout << "\t1. (hor, ver, (x, y, z)): " << horizonal_t + theta/2 << ", " << vertical_t + theta << ", ";
//     //     print_vec("", arrow_1);
//     //     cout << "\t2. (hor, ver, (x, y, z)): " << horizonal_t + theta/2 << ", " << vertical_t - theta << ", ";
//     //     print_vec("", arrow_2);
//     //     cout << "\t3. (hor, ver, (x, y, z)): " << horizonal_t - theta/2 << ", " << vertical_t - theta << ", ";
//     //     print_vec("", arrow_3);
//     //     cout << "\t4. (hor, ver, (x, y, z)): " << horizonal_t - theta/2 << ", " << vertical_t + theta << ", ";
//     //     print_vec("", arrow_4);

//     //     prev_ho = horizonal_t;
//     //     prev_ve = vertical_t;
//     // }
//     glBegin(GL_LINES);
//     glVertex3d(x0, y0, z0);
//     glVertex3d(
//         x0 + bou[0],
//         y0 + bou[1],
//         z0 + bou[2]
//     );
//     glEnd();
//     glColor3d(1, 1, 1);
//     glBegin(GL_TRIANGLE_FAN);
//     // glBegin(GL_LINES);
//     // glColor3d(1, 0, 0);
//     glVertex3d(
//         x0 + bou[0],
//         y0 + bou[1],
//         z0 + bou[2]
//     );
//     glVertex3d(
//         x0 + bou[0] + arrow_1[0],
//         y0 + bou[1] + arrow_1[1],
//         z0 + bou[2] + arrow_1[2]
//     );

//     // glColor3d(1, 1, 1);
//     // glVertex3d(
//     //     x0 + bou[0],
//     //     y0 + bou[1],
//     //     z0 + bou[2]
//     // );
//     glVertex3d(
//         x0 + bou[0] + arrow_2[0],
//         y0 + bou[1] + arrow_2[1],
//         z0 + bou[2] + arrow_2[2]
//     );

//     // glColor3d(0, 0, 1);
//     // glVertex3d(
//     //     x0 + bou[0],
//     //     y0 + bou[1],
//     //     z0 + bou[2]
//     // );
//     glVertex3d(
//         x0 + bou[0] + arrow_3[0],
//         y0 + bou[1] + arrow_3[1],
//         z0 + bou[2] + arrow_3[2]
//     );

//     // glColor3d(1, 1, 1);
//     // glVertex3d(
//     //     x0 + bou[0],
//     //     y0 + bou[1],
//     //     z0 + bou[2]
//     // );
//     glVertex3d(
//         x0 + bou[0] + arrow_4[0],
//         y0 + bou[1] + arrow_4[1],
//         z0 + bou[2] + arrow_4[2]
//     );

//     // glColor3d(0, 1, 1);
//     glVertex3d(
//         x0 + bou[0] + arrow_1[0],
//         y0 + bou[1] + arrow_1[1],
//         z0 + bou[2] + arrow_1[2]
//     );
//     glEnd();
//     glTranslated();
//     glColor3d(1, 1, 1);
// }

double horizonal, vertical, arrow_length = 50;
double t_radius = 50;
vector<double> t_temp(3,0);
void display(void) {
        InitDisplay();
        //
        glAxis();
        // theta
        // camera_pos = CommonCoordSystem::pol2cart(150, horizonal, vertical);
        // print_vec("camera", camera_pos);
        t_temp = CommonCoordSystem::pol2cart(t_radius, horizonal, vertical);
        glBegin(GL_LINE_STRIP);
        glColor3d(0.3, 0.3, 0.3);
        glVertex3d(t_radius, 0, 0);
        glVertex3d(t_radius * cos(horizonal), 0, t_radius * sin(horizonal));
        glColor3d(0.7, 0.7, 0.7);
        glVertex3dv(&t_temp[0]);
        glEnd();
        // arrow
        glColor3d(1,1,1);
        glArrow(0,0,0, arrow_length, horizonal, vertical);
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
        case 'a':
            horizonal += M_PI * (10.0 / 360.0);
            break;
        case 'd':
            horizonal -= M_PI * (10.0 / 360.0);
            break;
        case 'w':
            vertical += M_PI * (10.0 / 360.0);
            break;
        case 's':
            vertical -= M_PI * (10.0 / 360.0);
            break;
        case 't':
            arrow_length += 1;
            break;
        case 'g':
            arrow_length -= 1;
            break;
        case 'r':
            horizonal = 0;
            vertical = 0;
            camera_temp = {150, M_PI/4, M_PI/4};
            break;
        case 'i':
            camera_temp[2] += M_PI * (10.0 / 360.0);
            break;
        case 'k':
            camera_temp[2] -= M_PI * (10.0 / 360.0);
            break;
        case 'j':
            camera_temp[1] += M_PI * (10.0 / 360.0);
            break;
        case 'l':
            camera_temp[1] -= M_PI * (10.0 / 360.0);
            break;
        case 'u':
            camera_temp[0] -= 2;
            break;
        case 'o':
            camera_temp[0] += 2;
            break;
        case 'q':
            cout << "Quit" << endl;
            exit(0);
            break;
    }
    camera_pos = CommonCoordSystem::pol2cart(camera_temp);
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
    glutCreateWindow("Arrow");
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
