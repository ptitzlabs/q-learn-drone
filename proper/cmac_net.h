#ifndef _CMAC_NET_H_
#define _CMAC_NET_H_
#include "RL_headers.h"
#include "tiles.h"


class cmac_net {
   public:
    cmac_net(int num_inputs, float* tile_dimension, int tile_resolution = 8,
             int memory_size = 3000, int num_tilings = 10, int num_hashings = 3,
             float alpha = 0.5, float gamma = 1, float lambda = 0.9);
    ~cmac_net();

    void clear_traces();
    void clear_weights();
    void drop_traces();
    void update_traces(int hash);
    void generate_tiles(float* input);
    void return_value(float* output, int hash);
    void quick_update(float delta);
    void update(float* state, float target, int hash);
    void report();
    int get_num_hashings();
    float * get_weights();
    float * get_tile_sub_dimension();
    int get_memory_size();
    int get_num_tilings();

    void read_weights(char* filename);
    void write_weights(char* filename);
    

   private:
    int _memory_size;
    int _num_tilings;
    int _num_hashings;
    int _num_inputs;
    float _alpha;
    float _gamma;
    float _lambda;
    float* _tile_dimension;
    float* _tile_sub_dimension;
    float* _weights;
    float* _traces;
    int** _hashings;
    int _tile_resolution;

    float* _outputs_tmp;
};
#endif
