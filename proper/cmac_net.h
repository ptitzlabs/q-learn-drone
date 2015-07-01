#ifndef RL_HEADERS_H
#include "RL_headers.h"
#endif

#include "tiles.h"

#define CMAC_NET_H

class cmac_net {
   public:
    cmac_net(int num_inputs, float** range_inputs, int tile_resolution = 8,
             int memory_size = 3000, int num_tilings = 10, int num_hashings = 3,
             float alpha = 0.5, float gamma = 1, float lambda = 0.9);
    ~cmac_net();

    void report();
    void drop_traces(int hash);
    void generate_tiles(float* input);
    void return_value(float output, int hash);
    void update(float* state, float target, int hash);

   private:
    int _memory_size;
    int _num_tilings;
    int _num_hashings;
    int _num_inputs;
    float _alpha;
    float _gamma;
    float _lambda;
    float** _range_inputs;
    float* _interval_width;
    float* _weights;
    float* _traces;
    int** _hashings;
    int _tile_resolution;

    float* _outputs_tmp;
};
