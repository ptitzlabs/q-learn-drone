#ifndef _Q_LEARN_H_
#define _Q_LEARN_H_
#include "RL_headers.h"
#include "cmac_net.h"
#include "model.h"
#include "q_controller.h"

class q_learn {
   public:
    q_learn(cmac_net *net, model *m, int n_action_levels, float *action_levels,
            float *goal, int n_goal_states, int *goal_state_index,
            float epsilon = 0.0, float gamma = 1.0, int max_steps = 10000);

    ~q_learn();

    void set_goal(float *goal);
    int find_max();
    void calc_q();
    void calc_q(int hash);
    bool with_probability(float p);
    bool goal_reached();
    bool goal_reached(int n);
    void run_episode();
    void run_step();
    void report();
    void write_contour(char* filename, int id, int n = 100, int m = 100);

   private:

    cmac_net *_net;
    model *_m;

    int _max_steps;
    float _gamma;
    float _epsilon;

    int _size_q;
    float *_q;

    int _action;
    float *_action_levels;
    int _n_action_levels;

    float *_curr_goal;
    int _n_goal_states;
    int *_goal_state_index;

    //std::ofstream _myfile;

};
#endif