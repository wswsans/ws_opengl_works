/* -include */
#include <iostream>
#include <math.h>

// http://www.den.t.u-tokyo.ac.jp/ad_prog/graphics/
// deform_disp.c   図形の描画　//以下はコメント
#ifndef __APPLE__  // OSがMacでない (= WindowsやLinux)
#include <GL/glut.h>  // Windows, Linuxの場合のヘッダ
#else
#include <GLUT/glut.h>  // Macの場合のヘッダ
#endif

#include <vector>
#include "Physics.h"

using namespace std;

template<typename T>
void print_vec(string name, vector<T> vec) {
    cout << name << ": (";
    for (T i : vec) {
        cout << i << ", ";
    }
    cout << ")" << endl;
}

/* Physics */
double cube_size = 5; // distance(center - surface)
vector<Physics> cubes(1, Physics(2, cube_size));

/* OpenGL */
int WINDOW_WIDTH  = 500;
int WINDOW_HEIGHT = 500;
int UPDATE_TIME = 1;
bool using_pers = true;
bool using_buffer = false;
bool playing = true;
vector<double> cam_pos(3, 0);

void doing() {
    if (!playing) return;
    for(auto itr = cubes.begin(); itr != cubes.end(); ++itr) {
        itr->action();
    }
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
    // Camera 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (using_pers) {
        // (vertical_rad, horizonal_rate, close_distance, far_distance)
        gluPerspective(90, 1, 1e-2, pow(10, 10));
    } else {
        // (left, right), (bottom, top), (near, far)
        glOrtho(-200, 200, -200, 200, 0, 500);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // camera(x, y, z), look(x, y, z), upward_vec(x, y, z)
    cam_pos = {150, 150, 150};
    // cam_pos = cubes[0].getCart(0);
    // cam_pos[2] += 200;
    gluLookAt(
        cam_pos[0], cam_pos[1], cam_pos[2],
        0, 0, 0,
        // cam_pos[0], cam_pos[1], cam_pos[2] - 200,
        0.0, 1.0, 0.0
    );
    // Window
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT); //背景色で塗る
    glLineWidth(1.5);
    glPointSize(1.0);
    glColor3d(1.0, 1.0, 1.0); // RGB
    // Save Image
    // glPixelStorei(GL_PACK_ALIGNMENT ,1);
    // glReadBuffer(GL_FRONT);
}

void reshape(int width, int height) {
    // OpenGLウィンドウの描画範囲を設定
    // 下記は描画範囲が[0, width] x [0, height]となるように設定している
    glViewport(0, 0, width, height);
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
}

void glAxis() {
    /* xyz axis */
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

void glCube(double x1, double y1, double z1, double x2, double y2, double z2) {
    glBegin(GL_QUADS);
    // xy, z1
    glVertex3d(x1, y1, z1);
    glVertex3d(x2, y1, z1);
    glVertex3d(x2, y2, z1);
    glVertex3d(x1, y2, z1);
    // xy, z2
    glVertex3d(x1, y1, z2);
    glVertex3d(x2, y1, z2);
    glVertex3d(x2, y2, z2);
    glVertex3d(x1, y2, z2);
    // xz, y1
    glVertex3d(x1, y1, z1);
    glVertex3d(x2, y1, z1);
    glVertex3d(x2, y1, z2);
    glVertex3d(x1, y1, z2);
    // xz, y2
    glVertex3d(x1, y2, z1);
    glVertex3d(x2, y2, z1);
    glVertex3d(x2, y2, z2);
    glVertex3d(x1, y2, z2);
    // yz, x1
    glVertex3d(x1, y1, z1);
    glVertex3d(x1, y2, z1);
    glVertex3d(x1, y2, z2);
    glVertex3d(x1, y1, z2);
    // yz, x2
    glVertex3d(x2, y1, z1);
    glVertex3d(x2, y2, z1);
    glVertex3d(x2, y2, z2);
    glVertex3d(x2, y1, z2);
    glEnd();
}

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

vector<double> ptemp(3, 0); // position temp
vector<double> vtemp(3, 0); // velocity temp
void display(void) {
        InitDisplay();
        //
        /* plane */ 
        glColor3d(0.5, 0.5, 0.5); // RGB
        glBegin(GL_QUADS);
        glVertex3d(-1000, 0, -1000);
        glVertex3d(-1000, 0,  1000);
        glVertex3d( 1000, 0,  1000);
        glVertex3d( 1000, 0, -1000);
        glEnd();
        /* xyz axis */
        glAxis();
        /* cube */
        glColor3d(1.0, 1.0, 1.0); // RGB
        for(auto itr = cubes.begin(); itr != cubes.end(); ++itr) {
            ptemp = itr->getCart(0);
            vtemp = itr->getPol(1);
            // cout << "-----" << endl;
            // print_vec("\tcube_pos", ptemp);
            // print_vec("\tcube_vel", itr->getCart(1));
            // print_vec("\tcube_acc", itr->getCart(2));
            glCube(
                ptemp[0] - cube_size, ptemp[1] - cube_size, ptemp[2] - cube_size,
                ptemp[0] + cube_size, ptemp[1] + cube_size, ptemp[2] + cube_size
            );
            glColor3d(0, 1.0, 1.0); // RGB
            // print_vec("vel", itr->getPol(1));
            glArrow(ptemp[0], ptemp[1], ptemp[2], vtemp[0] * 2, vtemp[1], vtemp[2]);
        }
        //
        glFlush();
        if (using_buffer) {
            glutSwapBuffers();
        }
}

vector<double> p0 = {-50, 0, 50};
double v0_norm = 15;
double v0_theta = - M_PI / 6; // horizonal
double v0_theta2 = M_PI / 3; // vertical
void keyboard(unsigned char key, int x, int y) {
    // printf("key %c at (%d, %d)\n", key, x, y);
    switch (key) {
        case 'r':
            for(auto itr = cubes.begin(); itr != cubes.end(); ++itr) {
                itr->setCart(0, p0[0], p0[1], p0[2]);
                itr->setPol(1, v0_norm, v0_theta, v0_theta2);
                itr->setCart(2, 0, -0.98, 0);
            }
            break;
        case ' ':
            playing = !playing;
            break;
        case 'p':
            playing = true;
            doing();
            glutPostRedisplay();
            playing = false;
            break;
        case 'q':
            cout << "Quit" << endl;
            exit(0);
            break;
    }
}

int main(int argc, char **argv) {
    // cout << "UPDATE_TIME: " << UPDATE_TIME << endl;
    cout << "Window Size: (" << WINDOW_WIDTH << ", " << WINDOW_HEIGHT << ")" << endl;
    cubes[0].setCart(0, p0[0], p0[1], p0[2]);
    cubes[0].setPol(1, v0_norm, v0_theta, v0_theta2);
    cubes[0].setCart(2, 0, -0.98, 0);
    /* OpenGL */
    cout << "GL Start" << endl;
    // GLUTの初期化
    glutInit(&argc, argv);
    // ウィンドウのサイズを設定
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (using_buffer) {
        // バッファを用いる
        glutInitDisplayMode(GLUT_DOUBLE);
    }
    // ウィンドウの作成 (引数はウィンドウのタイトル)
    glutCreateWindow("Physics");
    // 描画に使う関数の登録
    glutDisplayFunc(display);
    // ウィンドウのサイズ変更時に呼ばれる関数の登録
    glutReshapeFunc(reshape);
    if (using_buffer) {
        // 計算待ち時間を利用する関数の登録
        glutIdleFunc(idle);
    } else {
        /* UPDATE_TIME[ms]ごとに描画 */
        glutTimerFunc(UPDATE_TIME, timer, 0);
    }
    // キーボード
    glutKeyboardFunc(keyboard);
    // 描画ループの開始
    glutMainLoop();
}
