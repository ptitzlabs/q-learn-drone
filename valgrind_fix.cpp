#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "src/drone_dynamics.hpp"
#include "src/policy_search.hpp"

using namespace std;
int main() {
    //////////////////////////////
    // Initializing drone dynamics
    //////////////////////////////
    drone_parm sim_parm;           // providing dynamic parameters

    drone_dynamics sim(&sim_parm);
    sim.rk4_step();
    sim_parm.m = 100;
    std::cout<<BOLDWHITE<<"#######################"<<RESET<<std::endl;
    sim.rk4_step();
    //sim.reset();
    //sim.rk4_step();

    ///////////////////////////////////////////////////
    // Initializing altitude rate controller parameters
    ///////////////////////////////////////////////////
    policy_parm alt_rate_parm;

    alt_rate_parm.set_n_goal(1);   // 1 goal state
    alt_rate_parm.set_n_state(3);  // 3 monitored states

    // Goal states
    alt_rate_parm.id_goal[0] = 8;  // zd

    // Monitored states
    alt_rate_parm.id_state[0] = 8;   // zd
    alt_rate_parm.id_state[1] = 14;  // zdd
    alt_rate_parm.id_state[2] = 18;  // aux0

    // Control input parameters
    alt_rate_parm.id_input = 0;  // U1

    //Learning parameters
    alt_rate_parm.max_steps = 1;

    //////////////////////////////
    // Initializing control policy
    //////////////////////////////

    policy alt_rate_control;
    // Provide FIRST the model and THEN the controller
    alt_rate_control.set_model(&sim_parm);  // providing the model
    alt_rate_control.set_parm(
        &alt_rate_parm);  // providing the controller parameters


    sim_parm.m = 100;
    alt_rate_control.fun_test(1,1);


    ////////////////////////
    // Initializing training
    ////////////////////////
    
    // Initial simulation state
    double init_state[] = {-10,    // xd
                           -9.81,  // xdd
                           0};     // aux0

    // Goal state
    double goal[] = {0};           // xd


    alt_rate_control.set_init(init_state);
    alt_rate_control.set_goal(goal);
    //alt_rate_control.report();


    // Training test
    //
    //alt_rate_control.calc_cmac_input();
    
    alt_rate_control.run_episode();
    sim_parm.m = 200;
    alt_rate_control.run_episode();
    //alt_rate_control.report();

#ifdef BLABLA
#endif
#ifdef BLABLA
    sim.set_timestep(0.001);

    sim.report();

    sim.reset();
    sim.set_input(0, 1);
    for (int i = 0; i < 1000; i++) sim.rk4_step();
    sim.report();

    sim.reset();
    sim.set_input(0, 1);
    for (int i = 0; i < 1000; i++) sim.euler_step();
    sim.report();
    // sim.report();

    double** kf = new double* [4];
    double** kg = new double* [4];

    double* ddxx = new double[6];
    double* dxx = new double[6];
    double* xx = new double[6];

    double* uu = new double[4];

    for (int i = 0; i < 4; i++) {
        uu[i] = 1 + i;
    }

    for (int i = 0; i < 6; i++) {
        ddxx[i] = i;
        dxx[i] = 10 + i;
        xx[i] = 20 + i;
    }

    kf[0] = &*ddxx;
    kg[0] = &*dxx;

    for (int i = 0; i < 6; i++) {
        cout << "kf[0][" << i << "]: " << kf[0][i] << "\t";
    }

    kf[1] = new double[6];
    kg[1] = new double[6];

    sim.calc_f(kf[1], &*kf[0], xx, uu);
    sim.calc_g(kg[1], &*kf[0], xx, uu);

    cout << endl << "resulting xdd: " << kf[1][0] << kf[1][1] << kf[1][2];

    // sim.rk4_step();

    for (int i = 0; i < 1000; i++) {
        sim.rk4_step(0.001);
    }
    sim.report();
    sim.reset();
    sim.report();

    sim.set_input(1, 0);
    for (int i = 0; i < 1000; i++) {
        sim.euler_step(0.001);
    }

    sim.report();
#endif
    return 0;
}
