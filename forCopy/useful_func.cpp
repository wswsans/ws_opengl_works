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

// require: CommonCoordSystem
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

void glCurve(double center_x, double center_y, double slope, double range_x, double ex_range) {
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
            pos_x1 = center_x + pm * i;
            pos_y1 = center_y + slope*pow(i, 2);
            pos_x2 = center_x + pm * j;
            pos_y2 = center_y + slope*pow(j, 2);
            glVertex2d(pos_x1, pos_y1);
            glVertex2d(pos_x2, pos_y2);
        }
    }
    glEnd();
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