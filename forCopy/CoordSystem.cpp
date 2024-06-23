#include "CoordSystem.h"
#include <cmath>
#include <vector>
// #include <iostream>
// vector計算
#include "vec_op.hpp"
using namespace std;

int CoordDim = 3;
vector<double> CoordNoneVec(CoordDim, 0);

/*** Common CoordSystem ***/
/* translate */
// deg - rad
double CommonCoordSystem::deg2rad(double Ideg) {
    return Ideg * M_PI / 180;
}
double CommonCoordSystem::rad2deg(double Irad) {
    return Irad * 180 / M_PI;
}
// cart - pol
// value, vector, number
vector<double> CommonCoordSystem::cart2pol(double Ix, double Iy, double Iz) {
    // norm, hoRad, veRad
    vector<double> temp(CoordDim, 0);
    double hoNorm = pow(Ix, 2) +  pow(Iz, 2);
    temp[0] = sqrt( hoNorm +  pow(Iy, 2) );
    temp[1] = atan(Iz/Ix);
    temp[2] = atan(Iy/sqrt(hoNorm));
    /// M_PI ズレチェック
    // vector<double> checker = (vector<double>{Ix, Iy, Iz});
    // vector<double> tester  = (CommonCoordSystem::pol2cart(temp));
    // if (!erChk(checker[0], tester[0]) && !erChk(checker[2], tester[2])) {
    if (Ix < 0) {
        temp[1] += M_PI;
    }
    // if (checker[1] != tester[1]) {
    //     temp[2] += M_PI;
    //     temp[2] += -1;
    // }
    // for (int i = 0; i < checker.size(); i++) {
    //     if (checker[i] != tester[i]) {
    //         temp[1 + (int)(i/3)] += M_PI;
    //     }
    // }
    // print_vec("cart2pol", temp);
    return temp;
}
vector<double> CommonCoordSystem::pol2cart(double Inorm, double IhoRad, double IveRad) {
    // (x, y, z)
    vector<double> temp(CoordDim, 0);
    double hoNorm = Inorm * cos(IveRad);
    temp[0] = hoNorm * cos(IhoRad);
    temp[1] = Inorm * sin(IveRad);
    temp[2] = hoNorm * sin(IhoRad);
    return temp;
}
vector<double> CommonCoordSystem::cart2pol(vector<double> Idata) {
    return CommonCoordSystem::cart2pol(Idata[0], Idata[1], Idata[2]);
}
vector<double> CommonCoordSystem::pol2cart(vector<double> Idata) {
    return CommonCoordSystem::pol2cart(Idata[0], Idata[1], Idata[2]);
}

/*** CoordSystem ***/
// constructor
CoordSystem::CoordSystem(int diff_time) {
    diffs = diff_time;
    // noneVecは使えない
    cartesians = vector< vector<double> >(diff_time + 1, vector<double>(CoordDim, 0));
    polars = vector< vector<double> >(diff_time + 1, vector<double>(CoordDim, 0));
}
/* translate */
vector<double> CoordSystem::cart2pol(int n) {
    if (n > diffs) {
        return CoordNoneVec;
    }
    return CommonCoordSystem::cart2pol(cartesians[n]);
}
vector<double> CoordSystem::pol2cart(int n) {
    if (n > diffs) {
        return CoordNoneVec;
    }
    return CommonCoordSystem::pol2cart(polars[n]);
}
/* vector */
// I: input, O: output
// cart
vector<double> CoordSystem::setCart(int n, double Ix, double Iy, double Iz) {
    if (n > diffs) {
        return CoordNoneVec;
    }
    cartesians[n][0] = Ix;
    cartesians[n][1] = Iy;
    cartesians[n][2] = Iz;
    polars[n] = CommonCoordSystem::cart2pol(Ix, Iy, Iz);
    return cartesians[n];
}
vector<double> CoordSystem::setCart(int n, vector<double> Idata) {
    if (n > diffs) {
        return CoordNoneVec;
    }
    return setCart(n, Idata[0], Idata[2], Idata[3]);
}
vector<double> CoordSystem::getCart(int n) {
    if (n > diffs) {
        return CoordNoneVec;
    }
    return cartesians[n];
}
vector<double> CoordSystem::getCart(int n, double *Ox, double *Oy, double *Oz) {
    if (n > diffs) {
        return CoordNoneVec;
    }
    vector<double> temp = getCart(n);
    *Ox = temp[0];
    *Oy = temp[1];
    *Oz = temp[2];
    return temp;
}
// polar
vector<double> CoordSystem::setPol(int n, double Inorm, double IhoRad, double IveRad) {
    if (n > diffs) {
        return CoordNoneVec;
    }
    polars[n][0] = Inorm;
    polars[n][1] = IhoRad;
    polars[n][2] = IveRad;
    cartesians[n] = CommonCoordSystem::pol2cart(Inorm, IhoRad, IveRad);
    return polars[n];
}
vector<double> CoordSystem::setPol(int n, vector<double> Idata) {
    if (n > diffs) {
        return CoordNoneVec;
    }
    return setPol(n, Idata[0], Idata[2], Idata[3]);
}
vector<double> CoordSystem::getPol(int n) {
    if (n > diffs) {
        return CoordNoneVec;
    }
    return polars[n];
}
vector<double> CoordSystem::getPol(int n, double *Onorm, double *OhoRad, double *OveRad) {
    if (n > diffs) {
        return CoordNoneVec;
    }
    vector<double> temp = getPol(n);
    *Onorm = temp[0];
    *OhoRad = temp[1];
    *OveRad = temp[2];
    return temp;
}
/* get data */
int CoordSystem::getDimention() {
    return CoordDim;
}
int CoordSystem::getDifferential() {
    return diffs;
}
vector< vector<double> > CoordSystem::getPolars() {
    return polars;
}
vector< vector<double> > CoordSystem::getCarts() {
    return cartesians;
}