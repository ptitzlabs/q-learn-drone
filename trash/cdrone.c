#include <stdio.h>
#include <math.h>
struct agent {
    double x[6];
    double dx[6];
    double u[4];
};

struct dynamics {
    double b, d, Ixx, Iyy, Izz, Irotor, m, l, g;
    double a1_phi, a1_the, a1_psi, a2_phi, a2_the, a3_phi, a3_the, a3_psi;

};

void dynamics_init(struct dynamics dyna){

    dyna.b = 1.1487e-5;
    dyna.d = 2.69e-8;
    dyna.Ixx = 8.5e-3;
    dyna.Iyy = 8.5e-3;
    dyna.Izz = 15.8e-3;
    dyna.Irotor = 6.8e-5;
    dyna.m = 1.2;
    dyna.l = 0.24;
    dyna.g = 9.81;
    dyna.a1_phi = (dyna.Ixx-dyna.Izz) / dyna.Ixx;


}

void drone_init(struct agent drone){


}
void dynamics_set(struct dynamics dyna, double b, double d, double Ixx, double Iyy, double Izz,
                   double Irotor, double m, double l, double g) {
    dyna.b =b;

}

int main() {
    struct dynamics dyna;
    struct agent drone;


    dynamics_init(dyna);
    
    return 0; }
