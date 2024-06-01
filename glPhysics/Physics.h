#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include <vector>
using namespace std;

#include "../forCopy/CoordSystem.h"

// class SpacePhysics {
// private:
//     vector<double> electric;
//     vector<double> magnet;
// };

class Physics {
private:
    // max_differential
    int diffs;
    // position, velocity, accelalation, ...
    // CoordSystem coord;
    /* energy */
    vector<double> force;
    // vector<double> electric;
    // vector<double> magnet;
    // double thermo;
    /* other data */
    double obj_mass;
    int internal_time;
    double obj_size;
public:
    // constructor
    Physics(int diff_time, double o_size);
    /* vector */
    // I: input, O: output
    // cart
    vector<double> setCart(int n, double Ix, double Iy, double Iz);
    vector<double> setCart(int n, vector<double> Idata);
    vector<double> getCart(int n);
    vector<double> getCart(int n, double *Ox, double *Oy, double *Oz);
    // polar
    vector<double> setPol(int n, double Inorm, double IhoRad, double IveRad);
    vector<double> setPol(int n, vector<double> Idata);
    vector<double> getPol(int n);
    vector<double> getPol(int n, double *Onorm, double *OhoRad, double *OveRad);
    /* data */
    double getMass();
    double getSize();
    /* action */
    vector<double> action();
    vector<double> collision_check(Physics obj);
};
 
#endif //_PHYSICS_H_