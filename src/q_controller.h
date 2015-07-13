#ifndef _Q_CONTROLLER_H_
#define _Q_CONTROLLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "math.h"
#include "tiles.h"
// void get_weights(float* weights, int * num_weights);

// void gen_input(float* input, int num_inputs, float* state, float* goal,
// float* tile_sub_dimension, float* num_states);

void blabla(int in);
void read_cmac_weights_f(float* weights, int * num_weights, char* filename);
void gen_input_index_max_q(int * input, int num_inputs, float* state, float* goal,
               float* tile_sub_dimension, int num_states, float* input_levels,
               int num_input_levels, float* weights, int num_weights,
               int num_tilings);

#endif
