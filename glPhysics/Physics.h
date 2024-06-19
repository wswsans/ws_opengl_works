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
    // vector<double> force;
    // vector<double> electric;
    // vector<double> magnet;
    // double thermo;
    /* other data */
    double obj_size;
    double obj_mass;
    int internal_time;
public:
    // constructor
    Physics(double o_mass, double o_size);
    vector<double> Accel2ForceCart(vector<double> accelCart);
    vector<double> Accel2ForcePol(vector<double> accelPol);
    vector<double> Force2AccelCart(vector<double> forceCart);
    vector<double> Force2AccelPol(vector<double> forcePol);
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
    // F = ma
    // cart
    vector<double> setForceCart(double Ix, double Iy, double Iz);
    vector<double> setForceCart(vector<double> Idata);
    vector<double> getForceCart();
    vector<double> getForceCart(double *Ox, double *Oy, double *Oz);
    // polar
    vector<double> setForcePol(double Inorm, double IhoRad, double IveRad);
    vector<double> setForcePol(vector<double> Idata);
    vector<double> getForcePol();
    vector<double> getForcePol(double *Onorm, double *OhoRad, double *OveRad);
    /* data */
    double getMass();
    double getSize();
    /* action */
    vector<double> action();
    // vector<double> collision_check(Physics obj);
};
 
#endif //_PHYSICS_H_