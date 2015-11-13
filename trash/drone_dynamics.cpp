#include "drone_dynamics.h"

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

    a1_phi = (Iyy - Izz) / Ixx;
    a1_the = (Izz - Ixx) / Iyy;
    a1_psi = (Ixx - Iyy) / Izz;

    a2_phi = 0;
    a2_the = 0;

    a3_phi = l / Ixx;
    a3_the = l / Iyy;
    a3_psi = 1 / Izz;

    double * _xd_vec = new double[6];
    double * _x_vec = new double[6];

    

    //x = 0;
    //y = 0;
    //z = 0;

    //phi = 0;
    //the = 0;
    //psi = 0;

    //U1 = 0;
    //U2 = 0;
    //U3 = 0;
    //U4 = 0;
}

drone_dynamics::~drone_dynamics(){
    delete[] _xd_vec;
    delete[] _x_vec;
}

void drone_dynamics::state_set(double * xd_vec, double * x_vec){
    for(int i = 0; i<6; i++){
        _xd_vec[i] = xd_vec[i];
        _x_vec[i] = x_vec[i];
    }

}

void drone_dynamics::input_set(double * u_vec){
    for(int i = 0; i<6; i++){
        _u_vec[i] = u_vec[i];
    }

}

void drone_dynamics::calc_dynamics() {
    // xdd = (cphi*sthe*cpsi+sphi*spsi)*U1m;
    // ydd = (cphi*sthe*spsi+sphi*cpsi)*U1m;
    // zdd = -g+cphi*cthe*U1m;

    // phidd = thed*psid*
}

void drone_dynamics::calc_f(double *f, double *xd_vec, double *x_vec,
                            double *u_vec) {
    double cphi = cos(x_vec[3]);
    double sphi = sin(x_vec[3]);

    double cthe = cos(x_vec[4]);
    double sthe = sin(x_vec[4]);

    double cpsi = cos(x_vec[5]);
    double spsi = sin(x_vec[5]);

    double U1m = u_vec[0] / m;

    // x_vec[0] x
    // x_vec[1] y
    // x_vec[2] z
    // x_vec[3] phi
    // x_vec[4] the
    // x_vec[5] psi

    // xyz acceleration
    f[0] = (cphi * sthe * cpsi + sphi * spsi) * U1m;
    f[1] = (cphi * sthe * spsi + sphi * cpsi) * U1m;
    f[2] = -g + cphi * cthe * U1m;

    // angle acceleration
    f[3] =
        xd_vec[4] * xd_vec[5] * a1_phi + x_vec[4] * a2_phi + u_vec[1] * a3_phi;
    f[4] =
        xd_vec[3] * xd_vec[5] * a1_the + x_vec[3] * a2_phi + u_vec[2] * a3_phi;
    f[5] = xd_vec[3] * xd_vec[4] * a1_psi + u_vec[1] * a3_phi;
}

void drone_dynamics::calc_g(double *g, double *xd_vec, double *x_vec,
                            double *u_vec) {
    g[0] = xd_vec[0];
    g[1] = xd_vec[1];
    g[2] = xd_vec[2];

    g[3] = xd_vec[3];
    g[4] = xd_vec[4];
    g[5] = xd_vec[5];
}

void drone_dynamics::rk4_step(double h) {
    double h2 = h/2;
    double h6 = h/6;

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

    calc_f(kf1, _xd_vec, _x_vec, _u_vec);
    calc_g(kg1, _xd_vec, _x_vec, _u_vec);

    for (int i = 0; i < 6; i++) {
        x_vec_tmp[i] = _x_vec[i] + h2 * kg1[i];
        xd_vec_tmp[i] = _xd_vec[i] + h2 * kf1[i];
    }

    calc_f(kf2, xd_vec_tmp, x_vec_tmp, _u_vec);
    calc_g(kg2, xd_vec_tmp, x_vec_tmp, _u_vec);

    for (int i = 0; i < 6; i++) {
        x_vec_tmp[i] = _x_vec[i] + h2 * kg2[i];
        xd_vec_tmp[i] = _xd_vec[i] + h2 * kf2[i];
    }

    calc_f(kf3, xd_vec_tmp, x_vec_tmp, _u_vec);
    calc_g(kg3, xd_vec_tmp, x_vec_tmp, _u_vec);

    for (int i = 0; i < 6; i++) {
        x_vec_tmp[i] = _x_vec[i] + h * kg3[i];
        xd_vec_tmp[i] = _xd_vec[i] + h * kf3[i];
    }

    calc_f(kf4, xd_vec_tmp, x_vec_tmp, _u_vec);
    calc_g(kg4, xd_vec_tmp, x_vec_tmp, _u_vec);

    for (int i = 0; i<6; i++){
        _x_vec[i] = _x_vec[i] + h6*(kg1[i]+2*kg2[i]+2*kg3[i]+kg4[i]);
        _xd_vec[i] = _xd_vec[i] + h6*(kf1[i]+2*kf2[i]+2*kf3[i]+kf4[i]);
    }

    delete[] kf1;
    delete[] kf2;
    delete[] kf3;
    delete[] kf4;

    delete[] kg1;
    delete[] kg2;
    delete[] kg3;
    delete[] kg4;

}
