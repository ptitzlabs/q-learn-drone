#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "src/drone_dynamics.hpp"


using namespace std;
int main(){

    int * a;
    //int b = 1;
    //int c = 2;

    int * d = new int[3];

    a = &d[0];

    //d[1] = b;
    //d[2] = c;

    *a = 2;
    std::cout<<"a: "<< *a<<" d[0]: " << d[0]<<std::endl;
    d[0] = 3;
    std::cout<<"a: "<< *a<<" d[0]: " << d[0]<<std::endl;

    drone_parm sim_parm;
    drone_dynamics sim(sim_parm);

    delete[] d;

    sim.set_timestep(0.001);

    sim.report();

    sim.reset();
    sim.set_input(0,1);
    for (int i = 0; i < 1000; i++)
    sim.rk4_step();
    sim.report();

    sim.reset();
    sim.set_input(0,1);
    for (int i = 0; i < 1000; i++)
    sim.euler_step();
    sim.report();
    //sim.report();



#ifdef BLABLA


    double ** kf = new double*[4];
    double ** kg = new double*[4];

    double * ddxx = new double[6];
    double * dxx = new double[6];
    double * xx = new double[6];

    double * uu = new double[4];

    for (int i = 0; i < 4; i++){
        uu[i] = 1+i;
    }

    for (int i = 0; i<6;i++){
        ddxx[i] = i;
        dxx[i] = 10+i;
        xx[i] = 20+i;
    }

    kf[0] = &*ddxx;
    kg[0] = &*dxx;

    for (int i = 0; i<6;i++){
cout<<"kf[0]["<<i<<"]: "<<kf[0][i]<<"\t";
    }

    kf[1] = new double[6];
    kg[1] = new double[6];


    sim.calc_f(kf[1],&*kf[0],xx,uu);
    sim.calc_g(kg[1],&*kf[0],xx,uu);

    cout<<endl<<"resulting xdd: "<<kf[1][0]<<kf[1][1]<<kf[1][2];


    //sim.rk4_step();

    for (int i = 0; i< 1000; i++){
    sim.rk4_step(0.001);
    }
    sim.report();
    sim.reset();
    sim.report();

    sim.set_input(1,0);
    for (int i = 0; i < 1000; i++){
    sim.euler_step(0.001);
    }

    sim.report();
#endif
    return 0;
}
