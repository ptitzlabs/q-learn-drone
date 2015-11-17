#include "drone_dynamics.hpp"

using namespace std;

drone_parm::drone_parm()
    : b(1.1487e-5),
      d(2.69e-8),
      Ixx(8.5e-3),
      Iyy(8.5e-3),
      Izz(15.8e-3),
      Irotor(6.8e-5),
      m(1.2),
      l(0.24) {
    // Pre-calculating various constants used in dynamic equations
    a1_phi = (Iyy - Izz) / Ixx;
    a1_the = (Izz - Ixx) / Iyy;
    a1_psi = (Ixx - Iyy) / Izz;
    a2_phi = 0;
    a2_the = 0;

    a3_phi = l / Ixx;
    a3_the = l / Iyy;
    a3_psi = 1 / Izz;
}
drone_parm::~drone_parm() {}

drone_dynamics::drone_dynamics(drone_parm drone)
    : _n_out(6), _n_aux_out(1), _n_in(4), _h(0.1) {
    // Map pointers to supplied dynamics constants
    b = &drone.b;
    d = &drone.d;
    Ixx = &drone.Ixx;
    Iyy = &drone.Iyy;
    Izz = &drone.Izz;
    Irotor = &drone.Irotor;
    m = &drone.m;
    l = &drone.l;

    a1_phi = &drone.a1_phi;
    a1_the = &drone.a1_the;
    a1_psi = &drone.a1_psi;

    a2_phi = &drone.a2_phi;
    a2_the = &drone.a2_the;

    a3_phi = &drone.a3_phi;
    a3_the = &drone.a3_the;
    a3_psi = &drone.a3_psi;

    // Initialize variables to store the inputs and outputs
    _n_out_tot = _n_out * 3;
    _n_parms = _n_out_tot + _n_aux_out;  // total number of output parameters
    _parm = new double *[_n_parms];      // vector storing all output
                                         // parameters
    _x = new double[_n_out];             // states
    _xd = new double[_n_out];            // derivatives
    _xdd = new double[_n_out];           // second derivatives

    _aux = new double[_n_aux_out];  // auxillary outputs

    // Mapping pointers to state outputs
    for (int i = 0; i < _n_out; i++) {
        _parm[i] = &_x[i];
        _parm[i + _n_out] = &_xd[i];
        _parm[i + _n_out * 2] = &_xdd[i];
    }

    // Mapping pointers to auxillary states
    for (int i = 0; i < _n_aux_out; i++) {
        _parm[_n_out_tot + i] = &_aux[i];
    }

    // Initialize control inputs
    _u_true = new double[_n_in];
    _u_scaled = new double[_n_in];

    // Initialize default values
    _init_parm = new double[_n_parms];
    _init_u_true = new double[_n_in];
    _init_u_scaled = new double[_n_in];

    for (int i = 0; i < _n_parms; i++) {
        *_parm[i] = 0;
    }
    for (int i = 0; i < _n_in; i++) {
        _u_scaled[i] = 0;
    }

    cout << _x[0] <<" " << _x[1] << " " << _x[2]<< endl;
    input_scale();
    calc_aux();
    set_init_parm();
    set_init_input();
}
drone_dynamics::~drone_dynamics() {
    delete[] _parm;
    delete[] _x;
    delete[] _xd;
    delete[] _xdd;
    delete[] _aux;

    delete[] _u_true;
    delete[] _u_scaled;

    delete[] _init_parm;
    delete[] _init_u_true;
    delete[] _init_u_scaled;
}

// Calc auxillary states
void drone_dynamics::calc_aux() {
    calc_aux(_parm);
}
void drone_dynamics::calc_aux(double **parm) {
    for (int i = 0; i < _n_aux_out; i++) calc_aux(i, parm);
}

void drone_dynamics::calc_aux(int id, double **parm) {
    switch (id) {
        case 0:
            *parm[18] = cos(*parm[3]) * cos(*parm[4]);
            break;
    }
}

// Set parameters
void drone_dynamics::set_parm(double **parm) {
    for (int i = 0; i < _n_parms; i++) set_parm(i, *parm[i]);
}

// Set individual parameters
void drone_dynamics::set_parm(int id, double parm) { *_parm[id] = parm; }

// Set input
void drone_dynamics::set_input(double *u_scaled) {
    for (int i = 0; i < _n_in; i++) set_input(i, u_scaled[i]);
}

// Set individual input
void drone_dynamics::set_input(int id, double u_scaled) {
    _u_scaled[id] = u_scaled;
    input_scale(id, u_scaled, &_u_true[id]);
}
// Reset sim
void drone_dynamics::reset() {
    for (int i = 0; i < _n_parms; i++) {
        *_parm[i] = _init_parm[i];
    }
    for (int i = 0; i < _n_in; i++) {
        _u_scaled[i] = _init_u_scaled[i];
        //_u_true[i] = _init_u_true[i];
    }
}

// Set initial parameters to current state
void drone_dynamics::set_init_parm() { set_init_parm(_parm); }
// Specify initial parameters
void drone_dynamics::set_init_parm(double **init_parm) {
    for (int i = 0; i < _n_parms; i++) _init_parm[i] = *init_parm[i];
}
// Set the initial parameters at the specified id
void drone_dynamics::set_init_parm(int id_init_parm, double init_parm) {
    _init_parm[id_init_parm] = init_parm;
}

// Set initial inputs to current input
void drone_dynamics::set_init_input() { set_init_input(_u_scaled); }

// Specify initial inputs
void drone_dynamics::set_init_input(double *init_u_scaled) {
    for (int i = 0; i < _n_in; i++) {
        set_init_input(i, _u_scaled[i]);
    }
}

// Set the initial inputs at the specified id
void drone_dynamics::set_init_input(int id, double init_u_scaled) {
    _init_u_scaled[id] = init_u_scaled;
    input_scale(id, _init_u_scaled[id], &_init_u_true[id]);
}
// Input scaling
void drone_dynamics::input_scale() {
    for (int i = 0; i < _n_in; i++) input_scale(i, _u_scaled[i], &_u_true[i]);
}
void drone_dynamics::input_scale(int id, double u_scaled, double *u_true) {
    switch (id) {
        case 0:
            *u_true = G_ACC * *m * (1 + 0.5 * u_scaled);
            break;
        case 1:
            *u_true = u_scaled * 0.1;
            break;
        case 2:
            *u_true = u_scaled * 0.1;
            break;
        case 3:
            *u_true = u_scaled * 0.1;
            break;
    }
}

// Equations of motion
void drone_dynamics::calc_f(double *xdd, double *xd, double *x,
                            double *u_true) {
    // Pre-calculating sines and cosines for speed
    double cphi = cos(x[3]);
    double sphi = sin(x[3]);

    double cthe = cos(x[4]);
    double sthe = sin(x[4]);

    double cpsi = cos(x[5]);
    double spsi = sin(x[5]);

    double U1m = u_true[0] / *m;

    // xyz acceleration
    xdd[0] = (cphi * sthe * cpsi + sphi * spsi) * U1m;  // x
    xdd[1] = (cphi * sthe * spsi + sphi * cpsi) * U1m;  // y
    xdd[2] = -G_ACC + cphi * cthe * U1m;                // z

    // angle acceleration
    xdd[3] =
        xd[4] * xd[5] * *a1_phi + x[4] * *a2_phi + u_true[1] * *a3_phi;  // phi
    xdd[4] = xd[3] * xd[5] * *a1_the + x[3] * *a2_phi +
             u_true[2] * *a3_phi;                            // theta
    xdd[5] = xd[3] * xd[4] * *a1_psi + u_true[3] * *a3_psi;  // psi
}

void drone_dynamics::calc_g(double *g, double *xd, double *x, double *u) {
    // Since the system is time-invariant, it is sufficient to pass the
    // derivative vector as-is
    for (int i = 0; i < _n_out; i++) {
        g[i] = xd[i];
    }
}
// Euler integration
void drone_dynamics::euler_step() { euler_step(_h); }
void drone_dynamics::euler_step(double h) {
    calc_f(_xdd, _xd, _x, &*_u_true);

    for (int i = 0; i < 6; i++) {
        _xd[i] += h * _xdd[i];
        _x[i] += h * _xd[i];
    }

    _aux[0] =
        cos(_x[3]) *
        cos(_x[4]);  // cos(theta) x cos (phi), necessary for altitude control
}

// Calculating Runge-Kutta coefficients;
void drone_dynamics::rk4_k_calc(double *kf_n, double *kg_n, double *kf_p,
                                double *kg_p, double h) {
    // Allocate memory for temp values
    double *xd_tmp = new double[_n_out];
    double *x_tmp = new double[_n_out];

    for (int i = 0; i < _n_out; i++) {
        x_tmp[i] = _x[i] + h * kg_p[i];
        xd_tmp[i] = _xd[i] + h * kf_p[i];
    }

    calc_f(kf_n, xd_tmp, x_tmp, _u_true);  // second derivatives at t+h
    calc_g(kg_n, xd_tmp, x_tmp, _u_true);  // first derivatives at t+h
    // Clear up temp values
    delete[] xd_tmp;
    delete[] x_tmp;
}

void drone_dynamics::rk4_step() { rk4_step(_h); }
void drone_dynamics::rk4_step(double h) {
    // Pre-calc the timesteps

    double h6 = h / 6;
    double h2 = h / 2;
    double hh[] = {h2, h2, h};

    // Allocate memory for rk-coefficients
    double **kf = new double *[4];
    double **kg = new double *[4];

    kf[0] = &*_xdd;
    kg[0] = &*_xd;

    for (int i = 1; i < 4; i++) {
        kf[i] = new double[_n_out];
        kg[i] = new double[_n_out];
    }
    for (int i = 1; i < 4; i++) {
        rk4_k_calc(kf[i], kg[i], kf[i - 1], kg[i - 1], hh[i - 1]);
    }

    for (int i = 0; i < 6; i++) {
        _x[i] += h6 * (kg[0][i] + 2 * kg[1][i] + 2 * kg[2][i] +
                       kg[3][i]);  // updating state values after timestep h
        _xd[i] +=
            h6 *
            (kf[0][i] + 2 * kf[1][i] + 2 * kf[2][i] +
             kf[3][i]);  // updating first derivative values after timestep h
    }

    calc_f(_xdd, _xd, _x, _u_true);  // updating second derivatives
    calc_aux();                      // updating auxillary values

    // cleaning up
    for (int i = 0; i < 4; i++) {
        // delete kf[i];
        // delete kg[i];
    }
    delete[] kf;
    delete[] kg;
#ifdef BLABLA
#endif
}
void drone_dynamics::report() {
    std::cout << "Current state:\nx: " << _x[0] << "\txd: " << _xd[0]
              << "\txdd: " << _xdd[0] << "\tphi: " << _x[3]
              << "\tphid: " << _xd[3] << "\tphidd: " << _xdd[3]
              << "\ny: " << _x[1] << "\tyd: " << _xd[1] << "\tydd: " << _xdd[1]
              << "\tthe: " << _x[4] << "\tthed: " << _xd[4]
              << "\tthedd: " << _xdd[4] << "\nz: " << _x[2]
              << "\tzd: " << _xd[2] << "\tzdd: " << _xdd[2]
              << "\tpsi: " << _x[5] << "\tpsid: " << _xd[5]
              << "\tpsidd: " << _xdd[5] << std::endl;
    std::cout << "aux[0]: " << _aux[0] << std::endl;
    std::cout << "Current input(scaled):"
              << "\n U1: " << _u_scaled[0] << " U2: " << _u_scaled[1]
              << " U3: " << _u_scaled[2] << " U3: " << _u_scaled[3]
              << std::endl;
    std::cout << "Current input(true):"
              << "\n U1: " << _u_true[0] << " U2: " << _u_true[1]
              << " U3: " << _u_true[2] << " U3: " << _u_true[3] << std::endl;
}

void drone_dynamics::set_timestep(double h){ _h = h; }

