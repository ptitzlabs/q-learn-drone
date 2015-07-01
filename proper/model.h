
#ifndef RL_HEADERS_H
#include "RL_headers.h"
#endif
#define MODEL_H

class model{
   public:
    model(int num_states, float ** state_limits, int num_inputs = 1);
    ~model();

    void model_sim(float* state, float* init_state, float* inputs);

    void model_dynamics(float *state, float *init_state, float* inputs);

    void report();
   private:
    int _num_states;
    int _num_inputs;

    float * _cur_state;
    float ** _state_limits;
};
