#define MODEL_SIM_H

#ifndef RL_HEADERS_H
#include "RL_headers.h"
#endif

class model{
    public:
        model(float *init_state, float** state_limits, int nstates, int ninputs);
        ~model();
        void report();
        void sim(float* output, float* input);
        void advance(float * input);
        void reset();
        void model_dynamics(float* output,float* state,float*input);
        float* get_state();
    private:
        int _nstates;
        int _ninputs;
        float* _init_state;
        float* _curr_state;
        float** _state_limits;
};
