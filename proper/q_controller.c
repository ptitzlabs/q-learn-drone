/*#include "tiles.h"*/
#include "q_controller.h"
void blabla(int in){
    in = 0;
}
void gen_input_index(int input_index, int num_inputs, float* state, float* goal,
               float *tile_sub_dimension, int num_states, float* input_levels,
               int num_input_levels, float* weights, int num_weights,
               int num_tilings) {
    float q[num_input_levels];
    float variables[num_states];
    int tiles[num_tilings];

    for (int i = 0; i < num_states; i++)
        variables[i] = state[i] / tile_sub_dimension[i];

    for (int i = 0; i < num_input_levels; i++) {
        q[i] = 0;
        get_tiles1(tiles, num_tilings, variables, num_states, num_weights,
                   i);
        for (int j = 0; j < num_tilings; j++) q[i] += weights[tiles[j]];
    }

    /*int max_index = 0;*/
    float max_val = q[0];
    int num_ties = 1;
    for (int i = 1; i < num_input_levels; i++) {
        if (q[i] >= max_val) {
            if (q[i] > max_val) {
                max_val = q[i];
                input_index = i;
            } else {
                num_ties++;
                if (rand() % num_ties == 0) {
                    max_val = q[i];
                    input_index = i;
                }
            }
        }
    }
}
