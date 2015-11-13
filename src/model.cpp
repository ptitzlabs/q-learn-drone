#include "model.h"

model::model(drone_dynamics* drone) {
    _num_states = 12;
    _num_inputs = 4;
    _curr_state = new double[_num_states];
    _init_state = new double[_num_states];

    _state_limits = new double* [_num_states];
    for (int i = 0; i < _num_states; i++) {
        _state_limits[i] = new double[2];
    }
    _drone = drone;
#ifdef TRAJECTORY_SAVE_ON
    _trajectory_file.open("logs/trajectory.txt",
                          std::ofstream::out | std::ofstream::trunc);
#endif
}

model::model(int num_states, double** state_limits, double* init_state,
             int num_inputs) {
    _num_states = num_states;
    _num_inputs = num_inputs;

    _curr_state = new double[num_states];
    _init_state = new double[num_states];

    _state_limits = new double* [num_states];
    for (int i = 0; i < num_states; i++) {
        _state_limits[i] = new double[2];
        _curr_state[i] = init_state[i];
        _init_state[i] = init_state[i];
    }

    for (int i = 0; i < num_states; i++) {
        _state_limits[i][0] = state_limits[i][0];
        _state_limits[i][1] = state_limits[i][1];
    }
#ifdef TRAJECTORY_SAVE_ON
    _trajectory_file.open("logs/trajectory.txt",
                          std::ofstream::out | std::ofstream::trunc);
#endif
}

model::~model() {
    delete[] _curr_state;
    delete[] _init_state;
    _curr_state = NULL;
    _init_state = NULL;
    for (int i = 0; i < _num_states; i++) {
        delete[] _state_limits[i];
        _state_limits[i] = NULL;
    }
    delete[] _state_limits;
    _state_limits = NULL;
#ifdef TRAJECTORY_SAVE_ON
    _trajectory_file.close();
#endif
}

void model::model_step(double h) {
    _drone.
    model_dynamics(_curr_state, _curr_state, inputs);
#ifdef TRAJECTORY_SAVE_ON
    _trajectory_file << _curr_state[0] << "," << _curr_state[1] << "\n";
#endif
}

void model::model_dynamics(double* state, double* init_state, double* inputs) {
    state[0] = init_state[0] + (inputs[0] - 1.0) * 0.001 +
               cos(3.0 * init_state[1]) * (-0.0025);
    if (state[0] < _state_limits[0][0]) state[0] = _state_limits[0][0];
    if (state[0] > _state_limits[0][1]) state[0] = _state_limits[0][1];

    state[1] = init_state[1] + state[0];
    if (state[1] < _state_limits[1][0]) state[1] = _state_limits[1][0];
    if (state[1] > _state_limits[1][1]) state[1] = _state_limits[1][1];
    if (state[1] == _state_limits[1][0] && state[0] < 0) state[0] = 0;
}

void model::report() {
    printf("\n#################\n");
    printf("Model parameters:\n");
    printf("nstates:%i,ninputs:%i\n", _num_states, _num_inputs);
    printf("Model limits:\n");
    for (int i = 0; i < _num_states; i++) {
        printf("%.2f,%.2f\n", _state_limits[i][0], _state_limits[i][1]);
    }
    printf("Current model state:\n");

    for (int i = 0; i < _num_states; i++) {
        printf("%.2f", _curr_state[i]);
        i < _num_states - 1 ? printf(",") : printf("\n");
    }
}

double* model::get_state() { return _curr_state; }
void model::get_state(double* state) {
    for (int i = 0; i < _num_states; i++) state[i] = _curr_state[i];
}
int model::get_num_states() { return _num_states; }
double** model::get_state_limits() { return _state_limits; }
double model::get_state(int n) { return _curr_state[n]; }
double model::get_init_state(int n) { return _init_state[n]; }

void model::set_state(double* state) {
    for (int i = 0; i < _num_states; i++) _curr_state[i] = state[i];
}
void model::set_init_state(double* state) {
    for (int i = 0; i < _num_states; i++) _init_state[i] = state[i];
}
void model::set_state(int n, double state) { _curr_state[n] = state; }

void model::reset() {
    for (int i = 0; i < _num_states; i++) _curr_state[i] = _init_state[i];
//_curr_state[0] = 0;
//_curr_state[1] = -0.5;
#ifdef TRAJECTORY_SAVE_ON
    _trajectory_file << _curr_state[0] << "," << _curr_state[1] << "\n";
#endif
    //*_curr_state = *_init_state;
}
