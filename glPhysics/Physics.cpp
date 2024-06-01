#include "Physics.h"
#include <cmath>
#include <string.h>
#include <vector>
#include <iostream>
// vector計算
#include "../forCopy/vec_op.hpp"
using namespace std;
#include "../forCopy/CoordSystem.h"

int PhyDim = 3;
vector<double> PhyNoneVec(PhyDim, 0);
CoordSystem coord(2);

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

template<typename T>
void print_vec(string name, vector<T> vec) {
    cout << name << ": (";
    for (T i : vec) {
        cout << i << ", ";
    }
    cout << ")" << endl;
}

/*** Physics ***/
// constructor
Physics::Physics(int diff_time, double o_size) {
    diffs = diff_time;
    obj_size = o_size;
    // noneVecは使えない
}
/* vector */
// I: input, O: output
// cart
vector<double> Physics::setCart(int n, double Ix, double Iy, double Iz) {
    return coord.setCart(n, Ix, Iy, Iz);
}
vector<double> Physics::setCart(int n, vector<double> Idata) {
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
    return coord.setPol(n, Inorm, IhoRad, IveRad);
}
vector<double> Physics::setPol(int n, vector<double> Idata)  {
    return coord.setPol(n, Idata);
}
vector<double> Physics::getPol(int n)  {
    return coord.getPol(n);
}
vector<double> Physics::getPol(int n, double *Onorm, double *OhoRad, double *OveRad)  {
    return coord.getPol(n, Onorm, OhoRad, OveRad);
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
vector<double> Physics::collision_check(Physics obj) {
    vector<double> obj_diff = obj.getCart(0) - getCart(0);
    double distance = 0;
    for (auto itr = obj_diff.begin(); itr != obj_diff.end(); ++itr) {
        distance += pow(*itr, 2);
    }
    distance = sqrt(distance);
    if (distance <= abs(obj.getSize() - getSize())) {
        // collision!
        // ...how to?
    }
    return PhyNoneVec;
}