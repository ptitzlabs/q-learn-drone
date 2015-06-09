#include "model_testing.h"

//using namespace std;

int main() {
#ifdef DEBUG_MODE
//#pragma message(__LOC__"Running in debug mode")
#endif
    int nstates = 2;
    int ninputs = 1;
    float *init_state = new float[nstates];
    init_state[0] = 0.0;
    init_state[1] = -0.5;
    float **state_limits = new float *[nstates];

    for (int i = 0; i < nstates; i++) {
        state_limits[i] = new float[2];
    }
    state_limits[0][0] = -0.07;
    state_limits[0][1] = 0.07;
    state_limits[1][0] = -1.2;
    state_limits[1][1] = 0.6;

    model m(init_state, state_limits, nstates, ninputs);
#ifdef DEBUG_MODE
    m.report();
#endif
    float *input = new float[1];
    input[0] = 1.0;
    // m.sim(output,input);
    for (long i = 0; i < 300; i++) m.advance(input);
    //m.report();
    // cout<<output[0]<<","<<output[1]<<endl;
    delete[] input;


    int nactions = 3;
    float * actions = new float[nactions];
    actions[0] = 0.0;
    actions[1] = 1.0;
    actions[2] = 2.0;
    int ntargets = 1;
    float * target_state = new float [ntargets];
    int * target_flags = new int[ntargets];
    target_state[0] = 0;
    target_state[1] = 0.5;

    target_flags[0] = 0;
    target_flags[1] = 1;

    RL rl(&m,ninputs,nstates,actions,nactions,target_state,target_flags);
#ifdef DEBUG_MODE
    rl.report();
#endif



    delete[] actions;
    delete[] target_flags;
    return 0;
}
