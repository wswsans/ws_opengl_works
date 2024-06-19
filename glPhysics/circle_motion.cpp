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
#include "../forCopy/CoordSystem.h"
#include "../forCopy/useful_func.cpp"

using namespace std;

/* Physics */
double cube_size = 5; // distance(center - surface)
vector<Physics> cubes(1, Physics(2, cube_size));

/* OpenGL */
int WINDOW_WIDTH  = 500;
int WINDOW_HEIGHT = 500;
int UPDATE_TIME = 1;
bool using_pers = true;
bool using_buffer = true;
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
    cam_pos = {1e-5, 150, 0};
    // cam_pos = cubes[0].getCart(0);
    // cam_pos[2] += 200;
    gluLookAt(
        cam_pos[0], cam_pos[1], cam_pos[2],
        0, 0, 0,
        // cam_pos[0], cam_pos[1], cam_pos[2] - 200,
        0.0, 1.0, 0.0
    );
    // Window
    glClear(GL_COLOR_BUFFER_BIT); //背景色で塗る
    glClearColor(0.0, 0.0, 0.0, 1.0);
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

// values
vector<double> p0 = {100, 0, 0};
double v0_norm = 15;
double v0_theta = M_PI / 2; // horizonal
double v0_theta2 = 0; // vertical
double accelNorm = pow(v0_norm, 2) / abs(p0[0]);

vector<double> ptemp(3, 0); // position temp
vector<double> vtemp(3, 0); // velocity temp
double center_theta = 0;
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
            center_theta = atan(ptemp[2]/ptemp[0]);
            if (ptemp[0] > 0) {
                center_theta += M_PI;
            }
            itr->setPol(2, accelNorm, center_theta, 0);
            glColor3d(0, 1.0, 1.0); // RGB
            // print_vec("vel", itr->getPol(1));
            glArrow(ptemp[0], ptemp[1], ptemp[2], vtemp[0] * 2, vtemp[1], vtemp[2]);
        }
        //
        glFlush();                    // 画面に出力
}

void keyboard(unsigned char key, int x, int y) {
    // printf("key %c at (%d, %d)\n", key, x, y);
    switch (key) {
        case 'r':
            for(auto itr = cubes.begin(); itr != cubes.end(); ++itr) {
                itr->setCart(0, p0[0], p0[1], p0[2]);
                itr->setPol(1, v0_norm, v0_theta, v0_theta2);
                itr->setPol(2, accelNorm, 0, 0);
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
    cubes[0].setPol(2, accelNorm, 0, 0);
    /* OpenGL */
    cout << "GL Start" << endl;
    // GLUTの初期化
    glutInit(&argc, argv);
    // ウィンドウのサイズを設定
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
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
