#ifndef _DRONE_DYNAMICS_HPP_
#define _DRONE_DYNAMICS_HPP_
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#define PI 3.1415926535897932384626433832795
#define G_ACC 9.80665

struct drone_parm {
    double b;
    double d;
    double Ixx;
    double Iyy;
    double Izz;
    double Irotor;
    double m;
    double l;

    double a1_phi, a1_the, a1_psi, a2_phi, a2_the, a3_phi, a3_the, a3_psi;

    // Constructor
    drone_parm();
    ~drone_parm();
};

class drone_dynamics {
   public:
    drone_dynamics(drone_parm drone);
    ~drone_dynamics();

    void calc_aux();
    void calc_aux(double** parm);
    void calc_aux(int id, double** parm);

    void set_parm(double** parm);
    void set_parm(int id, double parm);

    void set_input(double * u_scaled);
    void set_input(int id, double u_scaled);

    void reset();

    void set_init_parm();
    void set_init_parm(double** init_parm);
    void set_init_parm(int id, double init_parm);

    void set_init_input();
    void set_init_input(double* init_u_scaled);
    void set_init_input(int id, double init_u_scaled);

    void input_scale();
    void input_scale(int id, double u_scaled, double * u_true);

    void calc_f(double* f, double* xd, double* x, double* u);
    void calc_g(double* g, double* xd, double* x, double* u);
    void rk4_k_calc(double *kf_n, double* kg_n, double* kf_p, double* kg_p,
                    double h);
    void euler_step();
    void euler_step(double h);
    void rk4_step();
    void rk4_step(double h);
    void set_timestep(double h);

    void report();


   private:
    // Pointers to drone constants
    double* b;
    double* d;
    double* Ixx;
    double* Iyy;
    double* Izz;
    double* Irotor;
    double* m;
    double* l;

    double* a1_phi, *a1_the, *a1_psi, *a2_phi, *a2_the, *a3_phi, *a3_the,
        *a3_psi;

    int _n_out;  // number of outputs
    int _n_out_tot;
    int _n_aux_out;  // number of auxillary outputs
    int _n_in;       // number of inputs

    double** _parm;
    int _n_parms;

    double* _x;
    double* _xd;
    double* _xdd;
    double* _aux;

    double* _u_true;
    double* _u_scaled;

    double* _init_parm;

    double* _init_u_true;
    double* _init_u_scaled;

    double _h;  // default timestep
};

#ifdef POPOPO

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
    void init_set(double* values);
    void init_set(double* values, int* index, int n_states);
    v
} id state_set(double* value, int* index, int n_states);
void state_set(double* xdd_vec, double* xd_vec, double* x_vec);
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

void get_state(double* state, int* id, int n_states);

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
double* _parm;  // all state values in one vector
double** _aux;  // auxillary values
double** _xdd;  // second state derivatives
double** _xd;   // first state derivatives
double** _x;    // states
double* _u_true;     // inputs
double* _u_scaled;

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
}
;

#endif
#endif
//#define DRONE_DYNAMICS_HPP
