#define CMAC_NET_H

#ifndef RL_HEADERS_H
#include "RL_headers.h"
#endif

#include "tiles.h"

class CMAC_net {
   public:
    CMAC_net();
    CMAC_net(int num_inputs, float** range_inputs, int num_tilings,
             int tile_resolution, float alpha, int memory_size, int hash);

    void set_params(int num_inputs, float** range_inputs, int num_tilings,
                    int tile_resolution, float alpha, int memory_size,
                    int hash);

    void set_params(int num_inputs, float** range_inputs, int num_tilings,
                    int tile_resolution, float alpha, float lambda, float gamma,
                    int memory_size, int hash);

    void train(float inputs[], float target);
    void train(float* inputs[], int num_datapoints, float* target);
    void sim(float* outputs, float* inputs);
    void sim(float* outputs, float* inputs[], int num_inputs);
    void clear();
    void report();
    ~CMAC_net();

   private:
    float* _interval_width;
    float* _weights;
    float* _traces;
    int _num_inputs;
    float** _range_inputs;
    int _num_tilings;
    int _memory_size;
    int _hash;
    float _alpha;
    float _lambda;
    float _gamma;
    int _tile_resolution;
};
