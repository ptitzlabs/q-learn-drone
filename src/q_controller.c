/*#include "tiles.h"*/
#include "q_controller.h"
void blabla(int in){
    in = 0;
}
void read_cmac_weights_f(double* weights, int * num_weights, char* filename){
    


}
void gen_input_index_max_q(int *input_index, int num_inputs, double* state, double* goal,
               double *tile_sub_dimension, int num_states, double* input_levels,
               int num_input_levels, double* weights, int num_weights,
               int num_tilings) {
    double q[num_input_levels];
    double variables[num_states+1];
    int tiles[num_tilings];



    /*variables[num_states] = 0.5;*/
    variables[0] = state[0];
    variables[1] = state[1];
    variables[2] = goal[1];
    for (int i = 0; i < num_states; i++)
        variables[i] = state[i] / tile_sub_dimension[i];
    
variables[2] = goal[1]/tile_sub_dimension[2];
    

//printf("%.2f %.2f %.2f\n",variables[0],variables[1],variables[2]);

    for (int i = 0; i < num_input_levels; i++) {
        q[i] = 0;
        get_tiles1(tiles, num_tilings, variables, 3, num_weights,
                   i);
        for (int j = 0; j < num_tilings; j++) q[i] += weights[tiles[j]];
    }

    /*int max_index = 0;*/
    double max_val = q[0];
    int num_ties = 1;
    for (int i = 1; i < num_input_levels; i++) {
        if (q[i] >= max_val) {
            if (q[i] > max_val) {
                max_val = q[i];
                *input_index = i;
            } else {
                num_ties++;
                if (rand() % num_ties == 0) {
                    max_val = q[i];
                    *input_index = i;
                }
            }
        }
    }
    /**input_index = 999;*/
}
