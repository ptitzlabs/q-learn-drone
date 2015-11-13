#ifndef _Q_CONTROLLER_H_
#define _Q_CONTROLLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "math.h"
#include "tiles.h"
// void get_weights(double* weights, int * num_weights);

// void gen_input(double* input, int num_inputs, double* state, float* goal,
// double* tile_sub_dimension, double* num_states);

void blabla(int in);
void read_cmac_weights_f(double* weights, int * num_weights, char* filename);
void gen_input_index_max_q(int * input, int num_inputs, double* state, double* goal,
               double* tile_sub_dimension, int num_states, double* input_levels,
               int num_input_levels, double* weights, int num_weights,
               int num_tilings);

#endif
