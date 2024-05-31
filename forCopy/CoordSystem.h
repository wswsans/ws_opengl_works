#ifndef _COORD_SYSTEM_H_
#define _COORD_SYSTEM_H_

#include <vector>
using namespace std;

// common
class CommonCoordSystem {
// private:
public:
    /* const */
    /* translate */
    // deg - rad
    static double deg2rad(double Ideg);
    static double rad2deg(double Irad);
    // cart - pol
    // value, vector, number
    static vector<double> cart2pol(double Ix, double Iy, double Iz);
    static vector<double> pol2cart(double Inorm, double IhoRad, double IveRad);
    static vector<double> cart2pol(vector<double> Idata);
    static vector<double> pol2cart(vector<double> Idata);
};

class CoordSystem {
private:
    // max_differential
    int diffs;
    // position, velocity, accelalation, ...
    vector< vector<double> > cartesians; // (x, y)
    vector< vector<double> > polars; // (r, theta)
public:
    // constructor
    CoordSystem(int diff_time);
    /* translate */
    vector<double> cart2pol(int n);
    vector<double> pol2cart(int n);
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
    /* get data */
    int getDimention();
    int getDifferential();
    vector< vector<double> > getPolars();
    vector< vector<double> > getCarts();
};
 
#endif //_COORD_SYSTEM_H_