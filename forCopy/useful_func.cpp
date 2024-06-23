// #include <iostream>
// #include <cmath>
// #include <string>
// #include <vector>
// // http://www.den.t.u-tokyo.ac.jp/ad_prog/graphics/
// #ifndef __APPLE__
// #include <GL/glut.h> // Windows, Linux
// #else
// #include <GLUT/glut.h> // Mac
// #endif
// #include "../forCopy/CoordSystem.h"
// using namespace std;

template<typename T>
void print_vec(string name, vector<T> vec) {
    cout << name << ": (";
    for (T i : vec) {
        cout << i << ", ";
    }
    cout << ")" << endl;
}

//! @brief 文字列を表示
//! @param [in] x 三次元の座標(X)
//! @param [in] y 三次元の座標(Y)
//! @param [in] z 三次元の座標(Z)
//! @param [in] str 文字列（英語のみ）
// フォントの種類: GLUT_BITMAP_8_BY_13, GLUT_BITMAP_9_BY_15,
// GLUT_BITMAP_TIMES_ROMAN_10, GLUT_BITMAP_TIMES_ROMAN_24,
// GLUT_BITMAP_HELVETICA_10, GLUT_BITMAP_HELVETICA_12,
// GLUT_BITMAP_HELVETICA_18
void render_string(float x, float y, float z, const char* str) {
    // best center: -14px * len(str) / 2
    glRasterPos3f(x, y, z);

    const char* c = str;
    while (*c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c++);
    }
}

// require: CommonCoordSystem
void glCube(double x0, double y0, double z0, double length, double horizontal, double vertical) {
    vector<double> diagonal_x = CommonCoordSystem::pol2cart(length/2, horizontal, vertical); // x
    vector<double> diagonal_z = CommonCoordSystem::pol2cart(length/2, horizontal + M_PI/2, 0); // z
    vector<double> diagonal_y = CommonCoordSystem::pol2cart(length/2, horizontal, vertical + M_PI/2); // y
    glBegin(GL_QUADS);
    //
    glVertex3d(
        x0 + diagonal_x[0] + diagonal_z[0] + diagonal_y[0],
        y0 + diagonal_x[1] + diagonal_z[1] + diagonal_y[1],
        z0 + diagonal_x[2] + diagonal_z[2] + diagonal_y[2]
    );
    glVertex3d(
        x0 - diagonal_x[0] + diagonal_z[0] + diagonal_y[0],
        y0 - diagonal_x[1] + diagonal_z[1] + diagonal_y[1],
        z0 - diagonal_x[2] + diagonal_z[2] + diagonal_y[2]
    );
    glVertex3d(
        x0 - diagonal_x[0] - diagonal_z[0] + diagonal_y[0],
        y0 - diagonal_x[1] - diagonal_z[1] + diagonal_y[1],
        z0 - diagonal_x[2] - diagonal_z[2] + diagonal_y[2]
    );
    glVertex3d(
        x0 + diagonal_x[0] - diagonal_z[0] + diagonal_y[0],
        y0 + diagonal_x[1] - diagonal_z[1] + diagonal_y[1],
        z0 + diagonal_x[2] - diagonal_z[2] + diagonal_y[2]
    );
    //
    glVertex3d(
        x0 + diagonal_x[0] + diagonal_z[0] - diagonal_y[0],
        y0 + diagonal_x[1] + diagonal_z[1] - diagonal_y[1],
        z0 + diagonal_x[2] + diagonal_z[2] - diagonal_y[2]
    );
    glVertex3d(
        x0 - diagonal_x[0] + diagonal_z[0] - diagonal_y[0],
        y0 - diagonal_x[1] + diagonal_z[1] - diagonal_y[1],
        z0 - diagonal_x[2] + diagonal_z[2] - diagonal_y[2]
    );
    glVertex3d(
        x0 - diagonal_x[0] - diagonal_z[0] - diagonal_y[0],
        y0 - diagonal_x[1] - diagonal_z[1] - diagonal_y[1],
        z0 - diagonal_x[2] - diagonal_z[2] - diagonal_y[2]
    );
    glVertex3d(
        x0 + diagonal_x[0] - diagonal_z[0] - diagonal_y[0],
        y0 + diagonal_x[1] - diagonal_z[1] - diagonal_y[1],
        z0 + diagonal_x[2] - diagonal_z[2] - diagonal_y[2]
    );
    glEnd();
    glBegin(GL_QUAD_STRIP);
    // 1
    glVertex3d(
        x0 + diagonal_x[0] + diagonal_z[0] + diagonal_y[0],
        y0 + diagonal_x[1] + diagonal_z[1] + diagonal_y[1],
        z0 + diagonal_x[2] + diagonal_z[2] + diagonal_y[2]
    );
    glVertex3d(
        x0 + diagonal_x[0] + diagonal_z[0] - diagonal_y[0],
        y0 + diagonal_x[1] + diagonal_z[1] - diagonal_y[1],
        z0 + diagonal_x[2] + diagonal_z[2] - diagonal_y[2]
    );
    // 2
    glVertex3d(
        x0 - diagonal_x[0] + diagonal_z[0] + diagonal_y[0],
        y0 - diagonal_x[1] + diagonal_z[1] + diagonal_y[1],
        z0 - diagonal_x[2] + diagonal_z[2] + diagonal_y[2]
    );
    glVertex3d(
        x0 - diagonal_x[0] + diagonal_z[0] - diagonal_y[0],
        y0 - diagonal_x[1] + diagonal_z[1] - diagonal_y[1],
        z0 - diagonal_x[2] + diagonal_z[2] - diagonal_y[2]
    );
    // 3
    glVertex3d(
        x0 - diagonal_x[0] - diagonal_z[0] + diagonal_y[0],
        y0 - diagonal_x[1] - diagonal_z[1] + diagonal_y[1],
        z0 - diagonal_x[2] - diagonal_z[2] + diagonal_y[2]
    );
    glVertex3d(
        x0 - diagonal_x[0] - diagonal_z[0] - diagonal_y[0],
        y0 - diagonal_x[1] - diagonal_z[1] - diagonal_y[1],
        z0 - diagonal_x[2] - diagonal_z[2] - diagonal_y[2]
    );
    // 4
    glVertex3d(
        x0 + diagonal_x[0] - diagonal_z[0] + diagonal_y[0],
        y0 + diagonal_x[1] - diagonal_z[1] + diagonal_y[1],
        z0 + diagonal_x[2] - diagonal_z[2] + diagonal_y[2]
    );
    glVertex3d(
        x0 + diagonal_x[0] - diagonal_z[0] - diagonal_y[0],
        y0 + diagonal_x[1] - diagonal_z[1] - diagonal_y[1],
        z0 + diagonal_x[2] - diagonal_z[2] - diagonal_y[2]
    );
    // 1
    glVertex3d(
        x0 + diagonal_x[0] + diagonal_z[0] + diagonal_y[0],
        y0 + diagonal_x[1] + diagonal_z[1] + diagonal_y[1],
        z0 + diagonal_x[2] + diagonal_z[2] + diagonal_y[2]
    );
    glVertex3d(
        x0 + diagonal_x[0] + diagonal_z[0] - diagonal_y[0],
        y0 + diagonal_x[1] + diagonal_z[1] - diagonal_y[1],
        z0 + diagonal_x[2] + diagonal_z[2] - diagonal_y[2]
    );
    glEnd();
}

// require: CommonCoordSystem
void glArrow(double x0, double y0, double z0, double length, double horizontal, double vertical) {
    // 棒部分
    vector<double> bou = CommonCoordSystem::pol2cart(length, horizontal, vertical);
    bou[0] += x0;
    bou[1] += y0;
    bou[2] += z0;
    glBegin(GL_LINES);
    glVertex3d(x0, y0, z0);
    glVertex3d(bou[0], bou[1], bou[2]);
    glEnd();

    // 矢印
    double theta = M_PI/6;
    vector<double> arrow_u = CommonCoordSystem::pol2cart(-length * 0.25, horizontal, vertical + theta);
    vector<double> arrow_d = CommonCoordSystem::pol2cart(-length * 0.25, horizontal, vertical - theta);
    vector<double> arrow_r = CommonCoordSystem::pol2cart(length / 25, horizontal + M_PI/2, 0);
    vector<double> arrow_l = CommonCoordSystem::pol2cart(length / 25, horizontal - M_PI/2, 0);
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
void glArrow(vector<double> pos0, vector<double> angle) {
    glArrow(pos0[0], pos0[1], pos0[2], angle[0], angle[1], angle[2]);
}

void glCurve(double x0, double y0, double slope, double range_x, double ex_range) {
    glBegin(GL_LINES);
    int j;
    int increment = 1;//2*range_x/vertexes;
    double pos_x1, pos_y1, pos_x2, pos_y2;
    double temp_x;
    for (int pm = -1; pm <= 1; pm+=2) {
        for (int i = ex_range; i <= range_x; i+=increment) {
            j = i + increment;
            // range_x を超えないようにする
            if (j > range_x) {
                j = range_x;
            }
            pos_x1 = x0 + pm * i;
            pos_y1 = y0 + slope*pow(i, 2);
            pos_x2 = x0 + pm * j;
            pos_y2 = y0 + slope*pow(j, 2);
            glVertex2d(pos_x1, pos_y1);
            glVertex2d(pos_x2, pos_y2);
        }
    }
    glEnd();
}

// Recommend: glCircle3D
// 使ってる場所が多すぎるため残っている
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

// require: CommonCoordSystem
void glCircle3D(double x0, double y0, double z0, double radius, double horizontal, double vertical, double phase, int vertexes, double rate, bool isfill) {
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
    vector<double> circle;
    for (int i = 0; i <= vertexes; i++) {
        theta = i * freqency + phase;
        // 楕円を利用
        cx = radius * cos(vertical + M_PI/2) * cos(theta);
        cy = radius * sin(vertical + M_PI/2) * cos(theta);
        cz = radius * sin(theta);
        // horizontalでずらす
        circle = CommonCoordSystem::cart2pol(cx, 0, cz);
        circle[1] += horizontal;
        circle = CommonCoordSystem::pol2cart(circle);
        glVertex3d(x0 + circle[0], y0 + cy, z0 + circle[2]);
        if (i >= rate * vertexes) {
            i = vertexes + 1;
        }
    }
    glEnd();
}

// require: CommonCoordSystem
void glSpiral(double x0, double y0, double z0, double radius, double height, double horizontal, double vertical, double phase, int vertexes, double rate, bool isfill, double turns) {
    if (rate >= 1) {
        rate = 1;
    }
    if (isfill) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex3d(x0, y0, z0);
    } else {
        glBegin(GL_LINE_STRIP);
    }
    double cx, cy, cz, theta, length;
    double freqency = M_PI * 2 * turns / vertexes;
    vector<double> circle, bou;
    for (int i = 0; i <= vertexes; i++) {
        theta = i * freqency + phase;
        length = i * radius / vertexes;
        // 楕円を利用
        cx = radius * cos(vertical + M_PI/2) * cos(theta);
        cy = radius * sin(vertical + M_PI/2) * cos(theta);
        cz = radius * sin(theta);
        // horizontalでずらす
        circle = CommonCoordSystem::cart2pol(cx, 0, cz);
        bou = CommonCoordSystem::pol2cart(i * length / vertexes, horizontal, vertical);
        circle[1] += horizontal;
        circle = CommonCoordSystem::pol2cart(circle);
        glVertex3d(x0 + bou[0] + circle[0], y0 + bou[1] + cy, z0 + bou[2] + circle[2]);
        if (i >= rate * vertexes) {
            i = vertexes + 1;
        }
    }
    glEnd();
}

void glVortex3D(double x0, double y0, double z0, double radius, double height, double horizontal, double vertical, double phase, int vertexes, double rate, bool isfill, double turns) {
    if (rate >= 1) {
        rate = 1;
    }
    if (isfill) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex3d(x0, y0, z0);
    } else {
        glBegin(GL_LINE_STRIP);
    }
    double cx, cy, cz, theta, length;
    double freqency = M_PI * 2 * turns / vertexes;
    vector<double> circle, bou;
    for (int i = 0; i <= vertexes; i++) {
        theta = i * freqency + phase;
        length = i * radius / vertexes;
        // 楕円を利用
        cx = length * cos(vertical + M_PI/2) * cos(theta);
        cy = length * sin(vertical + M_PI/2) * cos(theta);
        cz = length * sin(theta);
        // horizontalでずらす
        circle = CommonCoordSystem::cart2pol(cx, 0, cz);
        bou = CommonCoordSystem::pol2cart(i * length / vertexes, horizontal, vertical);
        circle[1] += horizontal;
        circle = CommonCoordSystem::pol2cart(circle);
        glVertex3d(x0 + bou[0] + circle[0], y0 + bou[1] + cy, z0 + bou[2] + circle[2]);
        if (i >= rate * vertexes) {
            i = vertexes + 1;
        }
    }
    glEnd();
}

void glAxis(double length = 1000) {
    /* xyz axis */
    glBegin(GL_LINES);
    // x
    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex3d(length, 0, 0);
    // y
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, length, 0);
    // z
    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, length);
    glEnd();
    glColor3d(1, 1, 1);
}