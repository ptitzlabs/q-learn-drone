#ifndef _DRONE_DYNAMICS_HPP_
#define _DRONE_DYNAMICS_HPP_
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#define PI 3.1415926535897932384626433832795

class drone_dynamics {
   public:
    drone_dynamics();
    drone_dynamics& operator=(
        const drone_dynamics& source);  // copy assignment operator
    ~drone_dynamics();
    void euler_step(double h);
    void rk4_step(double h);

    void calc_dynamics();

    // Second derivative
    void calc_f(double* xdd, double* xd_vec, double* x_vec, double* u);
    // First derivative
    void calc_g(double* xd, double* xd_vec, double* x_vec, double* u);

    
    void reset();
    void init_set(double * values);
    void init_set(double * values, int * index, int n_states);
    void state_set(double * value, int * index, int n_states);
    void state_set(double *xdd_vec, double* xd_vec, double* x_vec);
    void input_set(double u, int id);
    void input_set(double* u);

    double get_x();
    double get_y();
    double get_z();

    double get_xd();
    double get_yd();
    double get_zd();

    double get_xdd();
    double get_ydd();
    double get_zdd();

    double get_phi();
    double get_the();
    double get_psi();

    double get_phid();
    double get_thed();
    double get_psid();

    double get_phidd();
    double get_thedd();
    double get_psidd();

    void get_state(double* state, int * id, int n_states);

    double get_parm(int id);
    double get_scale(int id);

    void report();
    void report_model_parm();

   private:
    // double _xdd_vec;

    int _n_parms;
    int _n_aux;
    int _n_controls;
    double* _init_parm;
    double* _parm_vector;      // all state values in one vector
    double** _aux;  // auxillary values
    double** _xdd; // second state derivatives
    double** _xd; //first state derivatives
    double** _x; // states 
    double* _u; // inputs

    double** _parm_scale;  // scale of the parameter values (min to max)

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

    // double _cphi;
    // double _sphi;
    // double _cthe;
    // double _sthe;
    // double _cpsi;
    // double _spsi;

    // double x;
    // double y;
    // double z;

    // double xd;
    // double yd;
    // double zd;

    //// double xdd;
    //// double ydd;
    //// double zdd;

    // double phi;
    // double the;
    // double psi;

    // double phid;
    // double thed;
    // double psid;

    // double phidd;
    // double thedd;
    // double psidd;

    // double U1;
    // double U2;
    // double U3;
    // double U4;
};

#endif
//#define DRONE_DYNAMICS_HPP
