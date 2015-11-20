#ifndef _POLICY_SEARCH_HPP_
#define _POLICY_SEARCH_HPP_
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "cmac_net.h"
#include "drone_dynamics.hpp"

struct policy_parm {
    // State parameters
    int id_input;           // input id
    int n_action_levels;    // number of discrete action levels
    double* action_levels;  // action levels applied to input

    int* id_state;  // monitored states
    int n_state;    // number of states
    int* id_goal;   // goal states
    int n_goal;     // number of goals

    // Learning parameters
    double gamma;        // discount-rate parameter
    double lambda;       // trace-decay parameter
    int max_steps;  // maximum number of steps
    double goal_thres;   // fraction threshold for goal
    double epsilon;     // probability of random action

    // CMAC parameters
    int memory_size;      // memory size to store the cmac
    int tile_resolution;  // number of segments per tile
    int n_tilings;        // number of overlapping tilings
    double alpha;         // step-size parameter

    int n_cmac_parms;

    void set_n_goal(int n);
    void set_n_state(int n);
    policy_parm();
    ~policy_parm();
};

class policy {
   public:
    policy();
    ~policy();

    void set_parm(policy_parm* parm);
    void set_model(drone_parm* parm);


    void set_state_parm(int id[], int n);
    void set_goal_parm(int id[], int n);

    void set_goal(double* goal);
    void set_init(double* init);

    void train();
    void run_episode();
    void run_step();

    bool with_probability(double p);

    bool goal_reached();

    void calc_q();
    void calc_q(int hash);
    void calc_cmac_input();

    int calc_action();
    void report();

   private:
    policy_parm* p;
    drone_dynamics* m;
    cmac_net* n;

    // cache variables
    double *_q; // stores local Q-values
    double* _curr_goal;
    int _action;
    double * _cmac_input;
};
#endif
