
#include "src/drone_dynamics.hpp"
#include "src/flight_controller.hpp"

int main() {
    drone_dynamics* main_sim =
        new drone_dynamics;  // init the main drone dynamics

    // five discrete input levels are tested during policy search iteration
    // all control inputs are scaled to accept values from -1 to 1
    double* input_levels = new double[5];
    input_levels[0] = -1.0;  // v. low
    input_levels[1] = -0.5;  // low
    input_levels[2] = 0.0;   // base
    input_levels[3] = 0.5;   // high
    input_levels[4] = 1.0;   // v. high

    // monitored states for altitude control include vertical speed,
    // acceleration
    // and tilt angle of the craft, relative to xy-plane
    int* alt_control_states = new int[3];
    alt_control_states[0] = 2;   // zd
    alt_control_states[1] = 8;   // zdd
    alt_control_states[2] = 18;  // cos(phi) x cos(the)

    // goal states for altitude hold controller include vertical speed and
    // acceleration state values
    int* alt_control_goal_states = new int[2];
    alt_control_goal_states[0] = 2;  // zd
    alt_control_goal_states[1] = 8;  // zdd

    rate_controller alt_control;
    alt_control.parm_init(
        &*main_sim,                 // cloning the drone model
        3,                          // selecting total thrust control (U4)
        &*input_levels,             // providing control levels to be tested
        5,                          // number of discrete control levels
        &*alt_control_states,       // specifying the states being evaluated
        3,                          // number of evaluated states
        &*alt_control_goal_states,  // goal w.r.t which reward is calculated
        2,                          // number of goal states
        3000,                       // memory size to store policy
        8,                          // tile resolution
        10,                         // number of tilings
        0.5,                        // step-size parameter
        1.0,                        // discount-rate parameter
        0.9);                       // trace-decay parameter

    // First test is the freefall condition. The controller is designed to break
    // the freefall and keep the altitude. Total of 5 parameters are fed to the
    // policy: current vertical velocity and acceleration, drone tilt, and
    // desirable vertical velocity and acceleration.

    // specify the initial state to start training
    double* alt_control_init_state = new double[3];
    alt_control_init_state[0] = -10.0;  // initial vertical velocity
    alt_control_init_state[1] = -9.81;  // initial vertical acceleration
    alt_control_init_state[2] = 0.0;    // initial tilt

    // specify the goal state
    double* alt_control_goal_state = new double[2];
    alt_control_goal_state[0] = 0.00;  // vertical velocity should reach 0
    alt_control_goal_state[1] = 0.00;  // vertical acceleration should reach 0

    // a->report();
    // b.report();

    // std::cout<< " copying... " << std::endl;

    // b = *a;
    // a->rk4_step(1);

    // a->report();
    // b.report();
    // b.report_model_parm();
    // std::cout<<std::endl;
    // b.rk4_step(5);

    // a->report();
    // b.report();

    // delete a;
    delete main_sim;
    delete[] input_levels;
    delete[] alt_control_states;
    delete[] alt_control_goal_states;

    return 0;
}
