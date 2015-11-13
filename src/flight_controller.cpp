#include "flight_controller.hpp"

rate_controller::rate_controller(){};

void rate_controller::parm_init(
    drone_dynamics* drone,  // drone model
    int control_input_id,   // input id
    double* action_levels,  // levels of supplied input
    int n_action_levels,    // number of input levels
    int* parm_id,           // ids of state parameters
    int n_input_states,     // number of state parameters
    int* goal_state_id,     // goal state ids
    int n_goal_states,      // number of goal states
    int memory_size,        // memory size to store the cmac
    int tile_resolution,    // tile resolution
    int num_tilings,        // number of overlapping tilings
    double alpha,           // step-size parameter
    double gamma,           // discount-rate parameter
    double lambda) {        // trace-decay parameter

    _drone = *drone;  // make a private copy of drone model
    _n_input_parms =
        n_input_states +
        n_goal_states;  // total number of parameters supplied to CMAC
    int* _input_parm_id = new int[_n_input_parms];

    for (int i = 0; i < n_input_states; i++) _input_parm_id[i] = parm_id[i];
    for (int i = 0; i < n_goal_states; i++)
        _input_parm_id[i + n_input_states] = goal_state_id[i];

    double tile_dimension[_n_input_parms];  // pick up expected
                                            // spread of state
                                            // levels
    // the inputs include current state
    for (int i = 0; i < n_input_states; i++) {
        tile_dimension[i] = _drone.get_scale(parm_id[i]);
    }
    // and the current goal value
    for (int i = 0; i < n_goal_states; i++) {
        tile_dimension[n_input_states + i] = _drone.get_scale(goal_state_id[i]);
    }
    // initialize the CMAC net to store the Q-values, using supplied parameters
    _net.parm_init(n_input_states + n_goal_states,  // both the drone state and
                                                    // the goal are used as
                                                    // inputs
                   tile_dimension,  // scale the resulting network appropriately
                   tile_resolution, memory_size, num_tilings,
                   n_action_levels,  // each control level has its own hashing
                   alpha, gamma, lambda);

    double* init_goal = new double[n_goal_states];
    for (int i = 0; i < n_goal_states; i++) {
        init_goal[i] = 0;  // init the goal states at 0
    }

    _policy.parm_init(
        &_net,    // network to store the policy
        &_drone,  // model instance to be used for policy search
        control_input_id,
        action_levels,    // control levels values
        n_action_levels,  // number of discrete control levels
        _input_parm_id,   // ids of states supplied to the policy
        _n_input_parms,   // number of states supplied to the policy
        goal_state_id,    // goal state ids
        n_goal_states);   // number of goal states
    delete[] init_goal;
}

void rate_controller::train(double* init_state, double* goal_state) {
    _policy.set_goal(goal_state);
}
