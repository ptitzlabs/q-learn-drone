#define RL_H

#ifndef RL_HEADERS_H
#include "RL_headers.h"
#endif

#ifndef CMAC_NET_H
#include "cmac_net.h"
#endif

#ifndef MODEL_SIM_H
#include "model_sim.h"
#endif

class RL {
   public:
    RL(model* m, int num_inputs, int num_states, float* actions,
       int num_actions, float* target_state, int* target_flags);
    ~RL();
    void report();
    bool with_probability(float p);
    bool goal_reached();
    void run_episode();

   private:
    model* _m;
    int _num_inputs;
    int _num_states;
    float* _actions;
    int _num_actions;
    float* _curr_state;
    float* _target_state;
    int* _target_flags;
    int _max_steps;
    CMAC_net* _cmac;
    int _cmac_num_inputs;
    float** _cmac_range_inputs;
    int _cmac_num_tilings;
    int _cmac_tile_resolution;
    float _cmac_alpha;
    float _cmac_lambda;
    float _cmac_gamma;
    int _cmac_memory_size;
    int _cmac_hash;
};
