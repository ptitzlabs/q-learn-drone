#ifndef _DRONE_DYNAMICS_HPP_
#define _DRONE_DYNAMICS_HPP_
#include <cmath>
class drone_dynamics {
   public:
    drone_dynamics();
    ~drone_dynamics();
    void euler_step(double h);
    void rk4_step(double h);

    void calc_dynamics();

    // Second derivative
    void calc_f(double* xdd, double* xd_vec, double* x_vec,
                double* u_vec);
    // First derivative
    void calc_g(double* xd, double* xd_vec, double* x_vec,
                double* u_vec);

    void state_set(double * xd_vec, double * x_vec);
    void input_set(double * u_vec);

   private:
    double* _xdd_vec;
    double* _xd_vec;
    double* _x_vec;
    double* _u_vec;
    double b;
    double d;
    double Ixx;
    double Iyy;
    double Izz;
    double Irotor;
    double m;
    double l;
    double g;

    double a1_phi;
    double a1_the;
    double a1_psi;

    double a2_phi;
    double a2_the;
    double a2_psi;

    double a3_phi;
    double a3_the;
    double a3_psi;

    //double x;
    //double y;
    //double z;

    //double xd;
    //double yd;
    //double zd;

    //// double xdd;
    //// double ydd;
    //// double zdd;

    //double phi;
    //double the;
    //double psi;

    //double phid;
    //double thed;
    //double psid;

    //double phidd;
    //double thedd;
    //double psidd;

    //double U1;
    //double U2;
    //double U3;
    //double U4;
};

#endif
//#define DRONE_DYNAMICS_HPP
