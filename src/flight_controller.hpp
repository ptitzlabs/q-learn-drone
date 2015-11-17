#ifndef _FLIGHT_CONTROLLER_HPP_
#define _FLIGHT_CONTROLLER_HPP_

#include "cmac_net.h"
#include "drone_dynamics.hpp"
#include "policy_search.hpp"

struct control_parms {
    // Controller I/O parameters
    int control_input_id;    // input id
    double* control_levels;  // levels of supplied input
    int n_control_levels;    // number of input levels
    int* parm_id;            // ids of state parameters
    int n_input_states;      // number of state parameters
    int* goal_state_id;      // goal state ids
    int n_goal_states;       // number of goal states

    // CMAC parameters
    cmac_net net;
    int n_input_parms;       // number of parameters supplied to CMAC
    int *input_parm_id;      // ids of input parameters
    int memory_size;         // memory size to store the cmac
    int tile_resolution;     // tile resolution
    int num_tilings;         // number of overlapping tilings
    double alpha;            // step-size parameter

    // Learning parameters
    int max_steps;
    double gamma;            // discount-rate parameter
    double lambda;           // trace-decay parameter
    double epsilon;
    double max_overshoot;

    // Simulation parameters
    drone_dynamics drone;
    double sim_stepsize;
};

class rate_controller {
   public:
    rate_controller();
    void parm_init(drone_dynamics* drone,   // drone model
                   int control_input_id,    // input id
                   double* control_levels,  // levels of supplied input
                   int n_control_levels,    // number of input levels
                   int* parm_id,            // ids of state parameters
                   int n_input_states,      // number of state parameters
                   int* goal_state_id,      // goal state ids
                   int n_goal_states,       // number of goal states
                   int memory_size,         // memory size to store the cmac
                   int tile_resolution,     // tile resolution
                   int num_tilings,         // number of overlapping tilings
                   double alpha,            // step-size parameter
                   double gamma,            // discount-rate parameter
                   double lambda);          // trace-decay parameter
    void train(double* init_state, double* goal_state);

   private:
    struct control_parms _parm;
    drone_dynamics _drone;
    int _n_input_parms;
    int* _input_parm_id;
    // int _state_id;

    cmac_net _net;
    control_policy _policy;
    double** state_limits;
};

#endif
