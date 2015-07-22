#ifndef _MODEL_H_
#define _MODEL_H_
#include "RL_headers.h"


class model{
   public:
    model(int num_states, float ** state_limits, float * init_state, int num_inputs = 1);
    ~model();

    void model_sim(float* state, float* init_state, float* inputs);

    void model_step(float *input);

    void model_dynamics(float *state, float *init_state, float* inputs);

    void report();
    float * get_state();
    float get_state(int n);
    float get_init_state(int n);
    void get_state(float * state);
    float **get_state_limits();
    void set_state(int n, float state);
    void set_state(float *state);
    void set_init_state(float *state);
    int get_num_states();

    void reset();
   private:
    int _num_states;
    int _num_inputs;

    float * _init_state;
    float * _cur_state;
    float ** _state_limits;
#ifdef TRAJECTORY_SAVE_ON
    std::ofstream _trajectory_file;
#endif
};
#endif
