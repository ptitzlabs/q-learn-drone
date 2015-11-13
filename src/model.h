#ifndef _MODEL_H_
#define _MODEL_H_
#include "RL_headers.h"
#include "drone_dynamics.hpp"

class model {
   public:
    model(drone_dynamics *drone);
    model(int num_states, double **state_limits, double *init_state,
          int num_inputs = 1);
    ~model();

    void model_sim(double *state, double *init_state, double *inputs);

    //void model_step(double *input);
    void model_step(double h);

    void model_dynamics(double *state, double *init_state, double *inputs);

    void report();
    double *get_state();
    double get_state(int n);
    double get_init_state(int n);
    void get_state(double *state);
    double **get_state_limits();
    void set_state(int n, double state);
    void set_state(double *state);
    void set_init_state(double *state);
    int get_num_states();

    void reset();

   private:
    int _num_states;
    int _num_inputs;

    double *_init_state;
    double *_curr_state;
    double **_state_limits;

    drone_dynamics * _drone;
    double _h_step;
#ifdef TRAJECTORY_SAVE_ON
    std::ofstream _trajectory_file;
#endif
};
#endif
