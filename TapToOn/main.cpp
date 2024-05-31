/* include */
#include <iostream>
#include <cmath>
#include <vector>

// ファイル読み込み
#include <fstream>
#include <string>

// http://www.den.t.u-tokyo.ac.jp/ad_prog/graphics/
#ifndef __APPLE__
#include <GL/glut.h> // Windows, Linux
#else
#include <GLUT/glut.h> // Mac
#endif

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
bool using_pers = false;
bool using_timer = true;
bool using_buffer = false;
bool playing = true;
// Game
string map_path;
vector< vector<bool> > lights; // (num_lights, vector<bool>(num_lights, true))
double cx, cy, radius = 20, phase = M_PI * 36 / 360;
int num_lights = 10;
int px, py, lighten_ct;
// click
bool left_click, clicked, click_flag;
vector<int> click_pos(2, 0);

int load_file(string filepath) {
    // https://kagasu.hatenablog.com/entry/2017/05/01/215219
    // https://gup.monster/entry/2014/04/03/233500
    ifstream ifs(filepath);
    if (ifs.fail()) {
        cerr << "ファイルの読み込みに失敗しました" << endl;
        return -1;
    }
    string file_content;
    getline(ifs, file_content);
    // 数設定
    num_lights = stoi(file_content);
    vector<bool> temp;
    lights = vector< vector<bool> >(0);
    for (int i = 0; i < num_lights; i++) {
        temp = vector<bool>(0,0);
        getline(ifs, file_content);
        for (int j = 0; j < num_lights; j++) {
            temp.push_back(file_content[j] == '1');
        }
        lights.push_back(temp);
    }
    // cout << "num_lights: " << num_lights << endl;
    // for (std::vector< vector<bool> >::iterator i = lights.begin(); i != lights.end(); ++i) {
    //     print_vec(">>> ", *i);
    // }
    return 1;
}

void doing() {
    if (!playing) return;
    // phase += 0.1;
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
        if (i >= rate * vertexes) {
            i = vertexes + 1;
        }
    }
    glEnd();
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
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c++);
    }
}

void display(void) {
    InitDisplay();
    //
    lighten_ct = 0;
    for (std::vector< vector<bool> >::iterator i = lights.begin(); i != lights.end(); ++i) {
        for (std::vector<bool>::iterator j = i->begin(); j != i->end(); ++j) {
            // vectorの場所
            px = (int)(j - i->begin());
            py = num_lights - 1 - (int)(i - lights.begin());
            // 画面の場所
            cx = (double)WINDOW_WIDTH  * (px - num_lights/2.0 + 0.5) * 0.8 / num_lights;
            cy = (double)WINDOW_HEIGHT * (py - num_lights/2.0 + 0.5) * 0.8 / num_lights;
            // 円(ライト)
            glCircle(cx, cy, 0, radius, phase, 5, 1, *j);
            // クリック
            if (clicked && pow(click_pos[0] - cx - WINDOW_WIDTH/2, 2) + pow(click_pos[1] - cy - WINDOW_HEIGHT/2, 2) <= pow(radius, 2)) {
                if (!click_flag) {
                    // cout << "click: " << px << ", " << py << endl;
                    lights[py][px] = !lights[py][px];
                    if (py + 1 < num_lights) lights[py + 1][px] = !lights[py + 1][px];
                    if (px + 1 < num_lights) lights[py][px + 1] = !lights[py][px + 1];
                    if (py - 1 >= 0) lights[py - 1][px] = !lights[py - 1][px];
                    if (px - 1 >= 0) lights[py][px - 1] = !lights[py][px - 1];
                    click_flag = true;
                }
            }
        }
    }
    // クリアチェック
    for (std::vector< vector<bool> >::iterator i = lights.begin(); i != lights.end(); ++i) {
        for (std::vector<bool>::iterator j = i->begin(); j != i->end(); ++j) {
            // vectorの場所
            px = (int)(j - i->begin());
            py = num_lights - 1 - (int)(i - lights.begin());
            if (lights[py][px]) {
                lighten_ct++;
            }
        }
    }
    if (lighten_ct == num_lights * num_lights) {
        // cout << "Clear!\nRestart: R key" << endl;
        glColor3d(0, 1, 0);
        render_string(-18*11/2, WINDOW_HEIGHT/2 - 25, 0, "Clear! Restart: R key");
        playing = false;
    }
    if (lighten_ct > num_lights * num_lights) {
        cout << "?????????? Restart" << endl;
        load_file(map_path);
    }
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
        case 'r':
            load_file(map_path);
            playing = true;
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
            click_flag = false;
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
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " settings.json" << endl;
        return -1;
    }
    /* 初期ロード */
    map_path = argv[1];
    load_file(map_path);
    // cout << "UPDATE_TIME: " << UPDATE_TIME << endl;
    cout << "Window Size: (" << WINDOW_WIDTH << ", " << WINDOW_HEIGHT << ")" << endl;
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (using_buffer) {
        glutInitDisplayMode(GLUT_DOUBLE);
    }
    glutCreateWindow("Tap To On");
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
