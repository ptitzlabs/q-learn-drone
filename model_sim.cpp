#include "model_sim.h"

model::model(float* init_state, float** state_limits, int nstates,
             int ninputs) {
    _nstates = nstates;
    _ninputs = ninputs;
    _init_state = new float[nstates];
    _curr_state = new float[nstates];
    _state_limits = new float* [nstates];
    for (int i = 0; i < nstates; i++) {
        _init_state[i] = init_state[i];
        _curr_state[i] = init_state[i];
        _state_limits[i] = new float[2];
        _state_limits[i][0] = state_limits[i][0];
        _state_limits[i][1] = state_limits[i][1];
    }
}
model::~model() {
    delete[] _init_state;
    delete[] _curr_state;
    for (int i = 0; i < _nstates; i++) {
        delete[] _state_limits[i];
    }
    delete[] _state_limits;
}
void model::reset() {
    for (int i = 0; i < _nstates; i++) {
        _curr_state[i] = _init_state[i];
    }
}
void model::report() {
    printf("\n#################\n");
    printf("Model parameters:\n");
    printf("nstates:%i,ninputs:%i\n", _nstates, _ninputs);
    printf("Model limits:\n");
    for (int i = 0; i < _nstates; i++) {
        printf("%.2f,%.2f\n", _state_limits[i][0], _state_limits[i][1]);
    }
    printf("Current model state:\n");

    for (int i = 0; i < _nstates; i++) {
        printf("%.2f", _curr_state[i]);
        i < _nstates - 1 ? printf(",") : printf("\n");
    }
}
void model::advance(float* input) { 

    sim(_curr_state, input);
}
void model::sim(float* output, float* input) {
    model_dynamics(output, _curr_state, input);
    //printf("sim: %.2f -- %.2f -- %.2f\n",input[0],output[0],output[1]);
}
void model::model_dynamics(float* output, float* state, float* input) {
    output[0] =
        state[0] + (input[0] - 1.0) * 0.001 + cos(3.0 * state[1]) * (-0.0025);
    //output[0] = state[0]+(input[0]-1.0)*0.001;
    //output[1] = 6;
    if (output[0] < _state_limits[0][0]) output[0] = _state_limits[0][0];
    if (output[0] > _state_limits[0][1]) output[0] = _state_limits[0][1];

    output[1] = state[1] + output[0];
    if (output[1] < _state_limits[1][0]) output[1] = _state_limits[1][0];
    if (output[1] > _state_limits[1][1]) output[1] = _state_limits[1][1];
    //printf("model_dynamics: %.2f -- %.2f -- %.2f\n",input[0],output[0],output[1]);
}

float * model::get_state(){return _curr_state;}
