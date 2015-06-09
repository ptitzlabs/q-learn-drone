#include "Q_learn.h"

RL::RL(model* m, int num_inputs, int num_states, float* actions,
       int num_actions, float* target_state, int* target_flags) {
    _m = m;
    _num_inputs = num_inputs;
    _num_states = num_states;
    _actions = new float[num_actions];
    _num_actions = num_actions;
    _target_flags = new int[num_states];
    _target_state = new float[num_states];
    for (int i = 0; i<num_states; i++){
        _target_flags[i] = target_flags[i];
        _target_state[i] = target_state[i];
    }

    _max_steps = 10000;
    _cmac = new CMAC_net[num_actions];
    //_cmac = new CMAC_net [1];
    // CMAC defaults
    _cmac_num_inputs = num_states;
    _cmac_range_inputs = new float* [num_actions];
    for (int i = 0; i < num_states; i++) _cmac_range_inputs[i] = new float[2];
    _cmac_range_inputs[0][0] = 0.0;
    _cmac_range_inputs[0][1] = 2.0;
    _cmac_range_inputs[1][0] = 0.0;
    _cmac_range_inputs[1][1] = 2.0;
    _cmac_num_tilings = 32;
    _cmac_tile_resolution = 8;
    _cmac_alpha = 0.5;
    _cmac_memory_size = 3000;
    _cmac_hash = 0;

    for (int i = 0; i < num_actions; i++)
        _cmac[i].set_params(_cmac_num_inputs, _cmac_range_inputs,
                            _cmac_num_tilings, _cmac_tile_resolution,
                            _cmac_alpha, _cmac_memory_size, _cmac_hash);

#ifdef DEBUG_MODE
    _cmac[0].report();
#endif

    // for (int i = 0; i < num_actions; i++)
    //_cmac[i] = new CMAC_net(_cmac_num_inputs, _cmac_range_inputs,
    //_cmac_num_tilings, _cmac_tile_resolution,
    //_cmac_alpha, _cmac_memory_size, _cmac_hash);
}

RL::~RL() {
    delete[] _cmac;
    delete[] _actions;
    delete[] _target_state;

    for (int i = 0; i < _num_actions; i++) {
        delete[] _cmac_range_inputs[i];
        _cmac_range_inputs[i] = NULL;
    }
    delete[] _cmac_range_inputs;
    delete[] _target_flags;
    delete[] _target_state;

    _cmac = NULL;
    _actions = NULL;
    _target_state = NULL;
    _cmac_range_inputs = NULL;
    _target_flags = NULL;
    _target_state = NULL;
}

void RL::report() {
    printf("\n#################\n");
    printf("RL report:\n");
    printf("num_inputs: %i\nnum_states: %i\nnum_actions: %i\n", _num_inputs,
           _num_states, _num_actions);
    _m->report();
    printf("state reached: ");
    printf("%s\n", goal_reached() ? "true" : "false");
}
bool RL::with_probability(float p) { return p > ((float)rand()) / RAND_MAX; }
bool RL::goal_reached() {
    bool reached = true;
    float* _curr_state = _m->get_state();
    for (int i = 0; i < _num_states; i++) {
        //std::cout << "state " << i << " is " << _curr_state[i] << "\n flag is" <<_target_flags[i]
                  //<< std::endl;

        if (_target_flags[i] == 1) {

            if (_curr_state[i] < _target_state[i]) reached = false;
        }
    }

    return reached;
}

void RL::run_episode() {
   float * Q = new float[_num_actions]; 
   for (int i = 0; i<_num_actions; i++)
       _cmac->sim(&Q[i],_curr_state);

   int step = 0;

   while (!goal_reached() && step < _max_steps){

   
   }
   
   delete[] Q;
   Q = NULL;
}
