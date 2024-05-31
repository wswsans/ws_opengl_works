#include <iostream>
#include <vector>
#include <algorithm> // vector関連で良いもの
#include <cmath>
using namespace std;

// 時間測定
#include <chrono>

// OpenGL
#ifndef __APPLE__  // OSがMacでない (= WindowsやLinux)
#include <GL/glut.h>  // Windows, Linuxの場合のヘッダ
#else
#include <GLUT/glut.h>  // Macの場合のヘッダ
#endif

template<typename T>
void print_vec(string name, vector<T> vec) {
    cout << name << ": (";
    for (T i : vec) {
        cout << i << ", ";
    }
    cout << ")" << endl;
}

/* debugger */
bool is_playing = true;
int following = 0, per_ct = 1;

/* collision program */
int i, d, col_ct, ct;
double cube_size;
bool is_collision;
// mass[kg], position[m], spped[m/s], energy[root(kg)m/s], length[m]
vector< vector<double> > cube;
// wall(x), floor(y)
vector<double> model_origin(2, 50);
vector<double> models(2);
double v_0 = 0;

void collision () {
    double prev_momentum,a,b,c;
    prev_momentum = 0;
    a = 0;
    b = 0;
    c = 0;
    for (i = 0; i < cube.size(); i++) {
        prev_momentum += cube[i][0] * cube[i][2]; // mass * speed
        a += cube[i][0];
    }
    b = sqrt(cube[1][0]) * prev_momentum;
    c = pow(prev_momentum, 2) - cube[0][0] * cube[1][0] * pow(v_0, 2);
    cube[1][3] = (b + sqrt(pow(b, 2) - a * c)) / a;
    cube[0][3] = (-sqrt(cube[1][0]) * cube[1][3] + prev_momentum) / sqrt(cube[0][0]);
    // energy to speed
    for (i = 0; i < cube.size(); i++) {
        cube[i][2] = cube[i][3] / sqrt(cube[i][0]);
    }
    if (is_collision) {
        // print_vec("cube0", cube[0]);
        // print_vec("cube1", cube[1]);
        col_ct++;
        is_collision = false;
        // cout << "collision: " << col_ct << endl;
    }
}

double move() {
    for (i = 0; i < cube.size(); i++) {
        cube[i][1] += cube[i][2];
        if (cube[i][1] < 0) {
            col_ct++;
            is_collision = true;
            if (i == 0) {
            cube[i][1] = 0;
            cube[i][2] *= -1;
            }
        }
    }
    if (cube[1][1] - cube[0][1] < cube[0][4]) {
        // cube[1][1] = cube[0][1] + cube[0][4];
        collision();
    }
    if (cube[1][1] - cube[0][1] > cube[0][4]) {
        is_collision = true;
    }
    if (cube[1][1] < cube[0][4]) {
        cube[1][1] = cube[0][4];
    }
    ct++;
    return ct;
}

/* OpenGL */
int WINDOW_WIDTH  = 700;
int WINDOW_HEIGHT = 350;

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

int UPDATE_TIME = 1;
void doing() {
    if (is_playing) {
        for (int _ = 0; _ < per_ct; _++) {
            move();
        }
    }
}
void timer(int value) {
    doing();
    glutPostRedisplay();
    glutTimerFunc(UPDATE_TIME, timer, 0);
}
void idle() {
    doing();
    // 再描画
    glutPostRedisplay();
}

void InitDisplay() {
    // Camera 
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // if (using_pers) {
    //     gluPerspective(Pers_array[0], Pers_array[1], Pers_array[2], Pers_array[3]);
    // } else {
    //     glOrtho(Orth_array[0], Orth_array[1], Orth_array[2], Orth_array[3], Orth_array[4], Orth_array[5]);
    // }
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // gluLookAt(
    //     Look_array[0], Look_array[1], Look_array[2], // カメラ位置
    //     Look_array[3], Look_array[4], Look_array[5], // 見る位置
    //     0.0, 1.0, 0.0 // 上方向ベクトル
    // );
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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)width, 0.0, (double)height);
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
}

void glArrow(double pos_x, double pos_y, double length, double theta) {
    glBegin(GL_LINE_STRIP);
    glVertex2d(pos_x, pos_y);
    glVertex2d(pos_x + length * cos(theta), pos_y + length * sin(theta));
    glEnd();
}

void display(void) {
        InitDisplay();
        //
        // 衝突回数
        render_string(100, WINDOW_HEIGHT * 0.8, 0, ("collision: " + to_string(col_ct) + ", " + to_string(per_ct) + "/ct, following: " + to_string(following) + ", is_playing: " + to_string(is_playing)).c_str());
        // 原点を変更
        models[1] = model_origin[1];
        if (following == 0) {
            models[0] = model_origin[0];
        } else {
            models[0] = model_origin[0] - cube[following - 1][1];
        }
        // 壁と床
        glBegin(GL_LINE_STRIP);
        glVertex2d(models[0], WINDOW_HEIGHT);
        glVertex2d(models[0], models[1]);
        glVertex2d(WINDOW_WIDTH, models[1]);
        glEnd();
        // 物体
        // mass[kg], position[m], spped[m/s], energy[root(kg)m/s], length[m]
        for (i = 0; i < cube.size(); i++) {
            render_string(100, WINDOW_HEIGHT * (0.7 - 0.1 * i), 0, ("cube" + to_string(i) + "(pos, speed): (" + to_string(cube[i][1]) + ", " + to_string(cube[i][2]) + ")").c_str());
            glArrow(models[0] + cube[i][1] + cube[i][4]/2, models[1] + cube[i][4]/2, cube[i][2], 0);
            glBegin(GL_LINE_LOOP);
            glVertex2d(models[0] + cube[i][1], models[1]);
            glVertex2d(models[0] + cube[i][1] + cube[i][4], models[1]);
            glVertex2d(models[0] + cube[i][1] + cube[i][4], models[1] + cube[i][4]);
            glVertex2d(models[0] + cube[i][1], models[1] + cube[i][4]);
            glEnd();
        }
        //
        glFlush();                    // 画面に出力
}

void keyboard(unsigned char key, int x, int y) {
    // printf("key %c at (%d, %d)\n", key, x, y);
    switch (key) {
        case ' ':
            is_playing = !is_playing;
            break;
        case 'f':
            following++;
            following %= cube.size() + 1;
            break;
        case 'a':
            model_origin[0] -= 10;
            break;
        case 'd':
            model_origin[0] += 10;
            break;
        case 'z':
            per_ct--;
            if (per_ct < 1) {
                per_ct = 1;
            }
            break;
        case 'x':
            per_ct = 1;
            break;
        case 'c':
            per_ct++;
            break;
        case 'r':
            is_playing = true;
            following = 0;
            per_ct = 1;
            model_origin[0] = 50;
            break;
        case 'q':
            exit(0);
            break;
    }
}

int main(int argc, char **argv) {
    // cout << "UPDATE_TIME: " << UPDATE_TIME << endl;
    cout << "Window Size: (" << WINDOW_WIDTH << ", " << WINDOW_HEIGHT << ")" << endl;
    cout << "f: following\n(a, d): model_origin\n(z, c): fps\nx: fps = 1\nr: reset\nq: quit" << endl;
    d = 1;
    v_0 = -1;
    cube_size = 10;
    switch (argc) {
        case 4:
            cube_size = strtod(argv[3], NULL);
        case 3:
            v_0 = -1 * abs(atoi(argv[2]));
        case 2:
            d = atoi(argv[1]);
            break;
        default:
            cout << argv[0] << " 100^d v_0 cube_size" << endl;
            break;
    }
    // mass[kg], position[m], spped[m/s], energy[root(kg)m/s], length[m]
    cube.push_back(vector<double>{1, cube_size * 2, 0, 0, cube_size * 5});
    cube.push_back(vector<double>{pow(100, d), cube_size * 10, v_0, 0, cube_size * 10});
    /* OpenGL */
    // GLUTの初期化
    glutInit(&argc, argv);
    // ウィンドウのサイズを設定
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    // ウィンドウの作成 (引数はウィンドウのタイトル)
    glutCreateWindow("Collision");
    // 描画に使う関数の登録
    glutDisplayFunc(display);
    // ウィンドウのサイズ変更時に呼ばれる関数の登録
    glutReshapeFunc(reshape);
    /* UPDATE_TIME[ms]ごとに描画 */
    glutTimerFunc(UPDATE_TIME, timer, 0);
    // キーボード
    glutKeyboardFunc(keyboard);
    // 描画ループの開始
    glutMainLoop();
}
