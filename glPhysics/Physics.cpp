#include "Physics.h"
#include <cmath>
#include <string.h>
#include <vector>
#include <iostream>
// vector計算
#include "../forCopy/vec_op.hpp"
using namespace std;
// #include "../forCopy/CoordSystem.h"
// #include "../forCopy/useful_func.cpp"

int PhyDim = 3;
int diffs = 2;
vector<double> PhyNoneVec(PhyDim, 0);
CoordSystem coord(2), force(0);

// vector<double> errorCut(vector<double> input) {
//     double gosa = 1e3;
//     for(auto itr = input.begin(); itr != input.end(); ++itr) {
//         *itr = floor(*itr/gosa)*gosa;
//     }
//     return input;
// }

// bool erChk(double A, double B) {
//     double ErVal = 1e-3;
//     return (abs(A - B) <= ErVal);
// }

/*** Physics ***/
// constructor
Physics::Physics(double o_mass, double o_size) {
    obj_mass = o_mass;
    obj_size = o_size;
    // noneVecは使えない
}
//
vector<double> Physics::Accel2ForceCart(vector<double> accelCart) {
    return accelCart * obj_mass;
}
vector<double> Physics::Accel2ForcePol(vector<double> accelPol) {
    accelPol[0] *= obj_mass;
    return accelPol;
}
vector<double> Physics::Force2AccelCart(vector<double> forceCart) {
    return forceCart / (double)obj_mass;
}
vector<double> Physics::Force2AccelPol(vector<double> forcePol) {
    forcePol[0] /= (double)obj_mass;
    return forcePol;
}
/* vector */
// I: input, O: output
// cart
vector<double> Physics::setCart(int n, double Ix, double Iy, double Iz) {
    if (n == 2) {
        force.setCart(0, Physics::Accel2ForceCart(vector<double>{Ix, Iy, Iz}));
    }
    return coord.setCart(n, Ix, Iy, Iz);
}
vector<double> Physics::setCart(int n, vector<double> Idata) {
    if (n == 2) {
        force.setCart(0, Physics::Accel2ForceCart(Idata));
    }
    return coord.setCart(n, Idata);
}
vector<double> Physics::getCart(int n) {
    return coord.getCart(n);
}
vector<double> Physics::getCart(int n, double *Ox, double *Oy, double *Oz) {
    return coord.getCart(n, Ox, Oy, Oz);
}
// polar
vector<double> Physics::setPol(int n, double Inorm, double IhoRad, double IveRad)  {
    if (n == 2) {
        force.setPol(0, Physics::Accel2ForcePol(vector<double>{Inorm, IhoRad, IveRad}));
    }
    return coord.setPol(n, Inorm, IhoRad, IveRad);
}
vector<double> Physics::setPol(int n, vector<double> Idata)  {
    if (n == 2) {
        force.setPol(0, Physics::Accel2ForcePol(Idata));
    }
    return coord.setPol(n, Idata);
}
vector<double> Physics::getPol(int n)  {
    return coord.getPol(n);
}
vector<double> Physics::getPol(int n, double *Onorm, double *OhoRad, double *OveRad)  {
    return coord.getPol(n, Onorm, OhoRad, OveRad);
}

// F = ma
// cart
vector<double> Physics::setForceCart(double Ix, double Iy, double Iz) {
    coord.setCart(2, Physics::Force2AccelCart({Ix, Iy, Iz}));
    return force.setCart(0, Ix, Iy, Iz);
}
vector<double> Physics::setForceCart(vector<double> Idata) {
    coord.setCart(2, Physics::Force2AccelCart(Idata));
    return force.setCart(0, Idata);
}
vector<double> Physics::getForceCart() {
    return force.getCart(0);
}
vector<double> Physics::getForceCart(double *Ox, double *Oy, double *Oz) {
    return force.getCart(0, Ox, Oy, Oz);
}
// polar
vector<double> Physics::setForcePol(double Inorm, double IhoRad, double IveRad)  {
    coord.setPol(2, Physics::Force2AccelPol({Inorm, IhoRad, IveRad}));
    return force.setPol(0, Inorm, IhoRad, IveRad);
}
vector<double> Physics::setForcePol(vector<double> Idata)  {
    coord.setPol(2, Physics::Force2AccelPol(Idata));
    return force.setPol(0, Idata);
}
vector<double> Physics::getForcePol()  {
    return force.getPol(0);
}
vector<double> Physics::getForcePol(double *Onorm, double *OhoRad, double *OveRad)  {
    return force.getPol(0, Onorm, OhoRad, OveRad);
}

/* data */
double Physics::getMass() {
    return obj_mass;
}
double Physics::getSize() {
    return obj_size;
}
/* action */
vector<double> Physics::action() {
    internal_time++;
    vector< vector<double> > carts = coord.getCarts();
    for (std::vector< vector<double> >::iterator i = carts.end() - 2; i != carts.begin() - 1; --i) {
        // cout << "no. " << (int)(i - carts.begin());
        // print_vec("", *i);
        // print_vec("\t+1", *(i + 1));
        // print_vec("\tsum", *i + *(i + 1));
        *i += *(i + 1); // vec_opを利用しない場合, それぞれ全部で和をとる
        coord.setCart((int)(i - carts.begin()), (*i)[0], (*i)[1], (*i)[2]);
    }
    // cout << "----------" << endl;
    return coord.getCart(0);
}
// vector<double> Physics::collision_check(Physics obj) {
//     vector<double> obj_diff = obj.getCart(0) - getCart(0);
//     double distance = 0;
//     for (auto itr = obj_diff.begin(); itr != obj_diff.end(); ++itr) {
//         distance += pow(*itr, 2);
//     }
//     distance = sqrt(distance);
//     if (distance <= abs(obj.getSize() - getSize())) {
//         // collision!
//         // ...how to?
//     }
//     return PhyNoneVec;
// }