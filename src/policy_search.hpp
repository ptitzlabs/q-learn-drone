#ifndef _POLICY_SEARCH_HPP_
#define _POLICY_SEARCH_HPP_
#include "RL_headers.h"
#include "cmac_net.h"
//#include "model.h"
#include "drone_dynamics.hpp"
#include "q_controller.h"

class control_policy {
   public:
    control_policy();
    void parm_init(
        cmac_net *net,          // network to store the policy
        drone_dynamics *drone,  // model instance to be used for policy search
        int control_input_id,
        double *action_levels,  // control levels values
        int n_action_levels,    // number of discrete control levels
        int *input_parm_id,   // ids of states supplied to the policy
        int n_input_parms,       // number of states supplied to the policy
        int *goal_state_id,      // goal state ids
        int n_goal_states,     // number of goal states
        double model_stepsize = 0.1, double epsilon = 0, double gamma = 1, int max_steps = 10000, double max_overshoot = 0.1);

    ~control_policy();

    void set_goal(double *goal);
    int find_max();
    void calc_q();
    void calc_q(int hash);
    bool with_probability(double p);
    bool goal_reached();
    bool goal_reached(int n);
    void run_episode();
    void run_episode(double *init_state, double *goal_state);
    void run_step();
    void report();
    void write_contour(char *filename, int id, int n = 100, int m = 100);
    double calc_reward();

    double goal_dist();

   private:
    cmac_net *_net;
    drone_dynamics *_drone;

    double _model_stepsize;
    int _max_steps;
    double _max_overshoot;
    double _gamma;
    double _epsilon;

    int _size_q;
    double *_q;


    int _control_input_id;
    int _action;
    double *_action_levels;
    int _n_action_levels;

    double *_input_parms;
    int * _input_parm_id;
    int _n_input_parms;

    double *_goal_parms;
    double *_curr_goal;
    int _n_goal_states;
    int *_goal_state_id;

    // std::ofstream _myfile;
};
#endif
