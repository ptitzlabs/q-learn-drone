#include "drone_dynamics.hpp"

drone_dynamics::drone_dynamics() {
    b = 1.1487e-5;
    d = 2.69e-8;
    Ixx = 8.5e-3;
    Iyy = 8.5e-3;
    Izz = 15.8e-3;
    Irotor = 6.8e-5;
    m = 1.2;
    l = 0.24;
    g = 9.81;

    // pre-calc things that should remain constant, unless the model is changed

    a1_phi = (Iyy - Izz) / Ixx;
    a1_the = (Izz - Ixx) / Iyy;
    a1_psi = (Ixx - Iyy) / Izz;

    a2_phi = 0;
    a2_the = 0;

    a3_phi = l / Ixx;
    a3_the = l / Iyy;
    a3_psi = 1 / Izz;

    _n_aux = 1;
    _n_parms = 18 + _n_aux;
    _n_controls = 4;

    _aux = new double *[_n_aux];  // pointers to auxillary values
    _xdd = new double *[6];       // pointers to second derivatives
    _xd = new double *[6];        // pointers to first derivatives
    _x = new double *[6];         // pointers to state values

    _u = new double[_n_controls];  // number of control inputs

    _parm_vector = new double[_n_parms];  // parameter storage
    _init_parm = new double[_n_parms];    // parameter default

    for (int i = 0; i < 6; i++) {
        _x[i] = &_parm_vector[i];         // x assigned index 0:5
        _xd[i] = &_parm_vector[i + 6];    // xd assigned index 6:11
        _xdd[i] = &_parm_vector[i + 12];  // xdd assigned index 12:17
    }

    for (int i = 0; i < _n_aux; i++) {
        _aux[i] =
            &_parm_vector[i + 18];  // auxillary parameters are appended after
                                    // state parameters
    }
    for (int i = 0; i < _n_parms; i++) {
        _init_parm[i] = 0;  // all initial variables are set to 0
    }

    reset();

    _u = new double[4];  // inputs
    for (int i = 0; i < _n_controls; i++) {
        input_set(0,i);  // all control inputs are set to 0
    }

    *_aux[0] =
        cos(*_x[3]) * cos(*_x[4]);  // cos(theta) x cos(phi), for alt. control

    _parm_scale = new double *[_n_parms];  // parameter scaling
    for (int i = 0; i < _n_parms; i++) {
        _parm_scale[i] = new double[3];
        _parm_scale[i][0] = 0;
        _parm_scale[i][1] = 0;
        _parm_scale[i][2] = 0;
    }

    // roll scaling and limits
    _parm_scale[3][0] = PI;
    _parm_scale[3][1] = -PI / 2;
    _parm_scale[3][2] = PI / 2;

    // pitch scaling and limits
    _parm_scale[4][0] = PI;
    _parm_scale[4][1] = -PI / 2;
    _parm_scale[4][2] = PI / 2;

    // cos(theta) x cos(phi) scaling and limits
    _parm_scale[18][0] = 1;
    _parm_scale[18][1] = 0;
    _parm_scale[18][2] = 1;
}

// Copy assignment

drone_dynamics &drone_dynamics::operator=(const drone_dynamics &source) {
    b = source.b;
    d = source.d;
    Ixx = source.Ixx;

    _x[0] = source._x[0];
}

drone_dynamics::~drone_dynamics() {
    delete[] _init_parm;
    delete[] _parm_vector;

    for (int i = 0; i < _n_aux; i++) {
        // delete _aux[i];
    }

    for (int i = 0; i < 6; i++) {
        // delete _x[i];
        // delete _xd[i];
        // delete _xdd[i];
    }

    delete[] _x;
    delete[] _xd;
    delete[] _xdd;
    delete[] _aux;

    delete[] _u;

    for (int i = 0; i < _n_parms; i++) {
        delete[] _parm_scale[i];
    }
    delete[] _parm_scale;
}

double drone_dynamics::get_x() { return *_x[0]; }
double drone_dynamics::get_y() { return *_x[1]; }
double drone_dynamics::get_z() { return *_x[2]; }

double drone_dynamics::get_xd() { return *_xd[0]; }
double drone_dynamics::get_yd() { return *_xd[1]; }
double drone_dynamics::get_zd() { return *_xd[2]; }

double drone_dynamics::get_xdd() { return *_xdd[1]; }
double drone_dynamics::get_ydd() { return *_xdd[2]; }
double drone_dynamics::get_zdd() { return *_xdd[3]; }

double drone_dynamics::get_phi() { return *_x[3]; }
double drone_dynamics::get_the() { return *_x[4]; }
double drone_dynamics::get_psi() { return *_x[5]; }

double drone_dynamics::get_phid() { return *_xd[3]; }
double drone_dynamics::get_thed() { return *_xd[4]; }
double drone_dynamics::get_psid() { return *_xd[5]; }

double drone_dynamics::get_phidd() { return *_xdd[3]; }
double drone_dynamics::get_thedd() { return *_xdd[4]; }
double drone_dynamics::get_psidd() { return *_xdd[5]; }

double drone_dynamics::get_parm(int id) { return _parm_vector[id]; }
double drone_dynamics::get_scale(int id) { return _parm_scale[id][0]; }

void drone_dynamics::get_state(double* state, int * id, int n_states){
    std::cout<< "n_states: " << n_states  << std::endl; 
    for (int i = 0; i < n_states; i++)
        std::cout<<"id: "<<id[i]<<" val: "<<_parm_vector[id[i]]<<"\t";
        //state[i] = _parm_vector[id[i]];
}


void drone_dynamics::reset() {
    for (int i = 0; i < _n_parms; i++) {
        _parm_vector[i] = _init_parm[i];
    }

    for (int i = 0; i < 4; i++) {
        input_set(0,i);
    }
}

void drone_dynamics::state_set(double *value, int *index, int n_states) {
    for (int i = 0; i < n_states; i++) {
        _parm_vector[index[i]] = value[i];
    }
}

void drone_dynamics::state_set(double *xdd_vec, double *xd_vec, double *x_vec) {
    for (int i = 0; i < 6; i++) {
        *_x[i] = x_vec[i];
        *_xd[i] = xd_vec[i];
        *_xdd[i] = xdd_vec[i];
    }
}

void drone_dynamics::input_set(double u, int id) {
    switch (id) {
        case 0:
            _u[0] = g * m * (1 + 0.5 * u);
            break;
        case 1:
            _u[1] = u * 0.1;
            break;
        case 2:
            _u[2] = u * 0.1;
            break;
        case 3:
            _u[3] = u * 0.1;
            break;
    }
}

void drone_dynamics::input_set(double *u) {
    // The inputs are scaled to range from -1 to 1 and produce realistic
    // behavior
    _u[0] = g * m * (1 + 0.5 * u[0]);
    _u[1] = u[1] * 0.1;
    _u[2] = u[2] * 0.1;
    _u[3] = u[3] * 0.1;

    // for (int i = 0; i < 4; i++) {
    //_u[i] = u[i];
    //}
}

void drone_dynamics::calc_dynamics() {
    // xdd = (cphi*sthe*cpsi+sphi*spsi)*U1m;
    // ydd = (cphi*sthe*spsi+sphi*cpsi)*U1m;
    // zdd = -g+cphi*cthe*U1m;

    // phidd = thed*psid*
}

void drone_dynamics::calc_f(double *f, double *xd_vec, double *x_vec,
                            double *u) {
    double cphi = cos(x_vec[3]);
    double sphi = sin(x_vec[3]);

    double cthe = cos(x_vec[4]);
    double sthe = sin(x_vec[4]);

    double cpsi = cos(x_vec[5]);
    double spsi = sin(x_vec[5]);

    double U1m = u[0] / m;

    // x_vec[0] x
    // x_vec[1] y
    // x_vec[2] z
    // x_vec[3] phi
    // x_vec[4] the
    // x_vec[5] psi

    // xyz acceleration
    f[0] = (cphi * sthe * cpsi + sphi * spsi) * U1m;  // x
    f[1] = (cphi * sthe * spsi + sphi * cpsi) * U1m;  // y
    f[2] = -g + cphi * cthe * U1m;                    // z

    // angle acceleration
    f[3] = xd_vec[4] * xd_vec[5] * a1_phi + x_vec[4] * a2_phi +
           u[1] * a3_phi;  // phi
    f[4] = xd_vec[3] * xd_vec[5] * a1_the + x_vec[3] * a2_phi +
           u[2] * a3_phi;                                   // theta
    f[5] = xd_vec[3] * xd_vec[4] * a1_psi + u[3] * a3_psi;  // psi
}

void drone_dynamics::calc_g(double *g, double *xd_vec, double *x_vec,
                            double *u) {
    // Since the system is time-invariant, it is sufficient to pass the
    // derivative vector as-is
    g[0] = xd_vec[0];
    g[1] = xd_vec[1];
    g[2] = xd_vec[2];

    g[3] = xd_vec[3];
    g[4] = xd_vec[4];
    g[5] = xd_vec[5];
}

void drone_dynamics::euler_step(double h) {
    calc_f(*_xdd, *_xd, *_x, _u);

    for (int i = 0; i < 6; i++) {
        *_xd[i] += h * *_xdd[i];
        *_x[i] += h * *_xd[i];
    }

    *_aux[0] =
        cos(*_x[3]) *
        cos(*_x[4]);  // cos(theta) x cos (phi), necessary for altitude control
}
void drone_dynamics::rk4_step(double h) {
    double h2 = h / 2;
    double h6 = h / 6;

    double *kf1 = new double[6];
    double *kf2 = new double[6];
    double *kf3 = new double[6];
    double *kf4 = new double[6];

    double *kg1 = new double[6];
    double *kg2 = new double[6];
    double *kg3 = new double[6];
    double *kg4 = new double[6];

    double *xd_vec_tmp = new double[6];
    double *x_vec_tmp = new double[6];

    for (int i = 0; i < 6; i++) {
        kf1[i] = *_xdd[i];  // second derivatives at t0 are already known
        kg1[i] = *_xd[i];   // first derivatives are also known
    }

    for (int i = 0; i < 6; i++) {
        xd_vec_tmp[i] = *_xd[i] + h2 * kf1[i];
        x_vec_tmp[i] = *_x[i] + h2 * kg1[i];
    }

    calc_f(kf2, xd_vec_tmp, x_vec_tmp, _u);  // second derivatives at t+h/2
    calc_g(kg2, xd_vec_tmp, x_vec_tmp, _u);  // first derivatives at t+h/2


    for (int i = 0; i < 6; i++) {
        xd_vec_tmp[i] = *_xd[i] + h2 * kf2[i];
        x_vec_tmp[i] = *_x[i] + h2 * kg2[i];
    }

    calc_f(kf3, xd_vec_tmp, x_vec_tmp,
           _u);  // second derivatives at t+h/2, refined
    calc_g(kg3, xd_vec_tmp, x_vec_tmp,
           _u);  // first derivatives at t+h/2, refined

    for (int i = 0; i < 6; i++) {
        xd_vec_tmp[i] = *_xd[i] + h * kf3[i];
        x_vec_tmp[i] = *_x[i] + h * kg3[i];
    }

    calc_f(kf4, xd_vec_tmp, x_vec_tmp, _u);  // second derivatives at t+h
    calc_g(kg4, xd_vec_tmp, x_vec_tmp, _u);  // second derivatives at t+h

    for (int i = 0; i < 6; i++) {
        *_xd[i] +=
            h6 * (kf1[i] + 2 * kf2[i] + 2 * kf3[i] +
                  kf4[i]);  // updating first derivative values after timestep h
        *_x[i] += h6 * (kg1[i] + 2 * kg2[i] + 2 * kg3[i] +
                        kg4[i]);  // updating state values after timestep h
    }

    calc_f(*_xdd, *_xd, *_x, _u);  // updating second derivatives
    *_aux[0] =
        cos(*_x[3]) *
        cos(*_x[4]);  // cos(theta) x cos (phi), necessary for altitude control

    // cleaning up

    delete[] kf1;
    delete[] kf2;
    delete[] kf3;
    delete[] kf4;

    delete[] kg1;
    delete[] kg2;
    delete[] kg3;
    delete[] kg4;

    delete[] xd_vec_tmp;
    delete[] x_vec_tmp;
}

void drone_dynamics::report() {
    std::cout << "Current state:\nx: " << *_x[0] << "\txd: " << *_xd[0]
              << "\txdd: " << *_xdd[0] << "\tphi: " << *_x[3]
              << "\tphid: " << *_xd[3] << "\tphidd: " << *_xdd[3]
              << "\ny: " << *_x[1] << "\tyd: " << *_xd[1]
              << "\tydd: " << *_xdd[1] << "\tthe: " << *_x[4]
              << "\tthed: " << *_xd[4] << "\tthedd: " << *_xdd[4]
              << "\nz: " << *_x[2] << "\tzd: " << *_xd[2]
              << "\tzdd: " << *_xdd[2] << "\tpsi: " << *_x[5]
              << "\tpsid: " << *_xd[5] << "\tpsidd: " << *_xdd[5] << std::endl;
    std::cout << "aux[0]: " << *_aux[0] << std::endl;
    std::cout << "Current input:"
              << "\n U1: " << _u[0] << " U2: " << _u[1] << " U3: " << _u[2]
              << " U3: " << _u[3] << std::endl;
}

void drone_dynamics::report_model_parm() {
    std::cout << " b: " << b << " d: " << d << " Ixx: " << Ixx
              << " Iyy: " << Iyy << " Izz: " << Izz << " Irotor: " << Irotor
              << " m: " << m << " l: " << l << std::endl;
}
