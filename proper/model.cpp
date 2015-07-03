#include "model.h"

model::model(int num_states, float ** state_limits, float * init_state, int num_inputs){
   _num_states = num_states;
   _num_inputs = num_inputs;

   _cur_state = new float [num_states];
   
   _state_limits = new float* [num_states]; 
   for(int i = 0; i<num_states; i++){
       _state_limits[i] = new float[2];
       _cur_state[i] = init_state[i];
   }

   for(int i = 0; i<num_states; i++){
       _state_limits[i][0] = state_limits[i][0];
       _state_limits[i][1] = state_limits[i][1];
   }
}

model::~model(){
    delete[] _cur_state;
    _cur_state = NULL;
    for (int i = 0; i< _num_states; i++){
        delete[] _state_limits[i];
        _state_limits[i] = NULL;
    }
    delete[] _state_limits;
    _state_limits = NULL;

}

void model::model_step(float* inputs ){
    model_dynamics(_cur_state,_cur_state,inputs);
}

void model::model_dynamics(float * state, float *init_state, float* inputs){
    state[0] =
        init_state[0] + (inputs[0] - 1.0) * 0.001 + cos(3.0 * init_state[1]) * (-0.0025);
    if (state[0] < _state_limits[0][0]) state[0] = _state_limits[0][0];
    if (state[0] > _state_limits[0][1]) state[0] = _state_limits[0][1];

    state[1] = init_state[1] + state[0];
    if (state[1] < _state_limits[1][0]) state[1] = _state_limits[1][0];
    if (state[1] > _state_limits[1][1]) state[1] = _state_limits[1][1];
}

void model::report(){

    printf("\n#################\n");
    printf("Model parameters:\n");
    printf("nstates:%i,ninputs:%i\n", _num_states, _num_inputs);
    printf("Model limits:\n");
    for (int i = 0; i < _num_states; i++) {
        printf("%.2f,%.2f\n", _state_limits[i][0], _state_limits[i][1]);
    }
    printf("Current model state:\n");

    for (int i = 0; i < _num_states; i++) {
        printf("%.2f", _cur_state[i]);
        i < _num_states - 1 ? printf(",") : printf("\n");
    }
}

float * model::get_state(){

    return _cur_state;

}
float model::get_state(int n){

    return _cur_state[n];

}
