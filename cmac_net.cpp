#include "cmac_net.h"

CMAC_net::CMAC_net() {
    const int k_num_inputs = 2;
    const float k_range_inputs[][2] = {{0.0, 2.0}, {0.0, 2.0}};
    const int k_num_tilings = 32;
    const int k_tile_resolution = 8;
    const float k_alpha = 0.5;
    const int k_memory_size = 3000;
    const int k_hash = 0;

    float** range_inputs = new float* [k_num_inputs];
    for (int i = 0; i < k_num_inputs; i++) range_inputs[i] = new float[2];
    for (int i = 0; i < k_num_inputs; i++) {
        range_inputs[i][0] = k_range_inputs[i][0];
        range_inputs[i][1] = k_range_inputs[i][1];
    }
    set_params(k_num_inputs, range_inputs, k_num_tilings, k_tile_resolution,
               k_alpha, k_memory_size, k_hash);
}
CMAC_net::CMAC_net(int num_inputs, float** range_inputs, int num_tilings,
                   int tile_resolution, float alpha, int memory_size,
                   int hash) {
    set_params(num_inputs, range_inputs,num_tilings,
               tile_resolution,alpha,memory_size,hash);
}
void CMAC_net::set_params(int num_inputs, float** range_inputs, int num_tilings,
                    int tile_resolution, float alpha, float lambda, float gamma,
                    int memory_size, int hash){
    set_params(num_inputs, range_inputs,num_tilings,
               tile_resolution,alpha,memory_size,hash);
    _lambda = lambda;
    _gamma = gamma;

}
void CMAC_net::set_params(int num_inputs, float** range_inputs, int num_tilings,
                          int tile_resolution, float alpha, int memory_size,
                          int hash) {
    _num_inputs = num_inputs;
    _interval_width = new float[num_inputs];
    _range_inputs = new float* [num_inputs];
    for (int i = 0; i < num_inputs; i++) _range_inputs[i] = new float[2];
    for (int i = 0; i < num_inputs; i++) {
        _range_inputs[i][0] = range_inputs[i][0];
        _range_inputs[i][1] = range_inputs[i][1];
        _interval_width[i] =
            (range_inputs[i][1] - range_inputs[i][0]) / (float)tile_resolution;
    }
    _tile_resolution = tile_resolution;
    _num_tilings = num_tilings;
    _alpha = alpha / num_tilings;
    _memory_size = memory_size;
    _hash = hash;
    _weights = new float[memory_size];
    _traces = new float[memory_size];
    for (int i = 0; i < memory_size; i++) {
        _weights[i] = 0.0f;
    }
}
CMAC_net::~CMAC_net() {
    for (int i = 0; i < _num_inputs; i++) {
        delete[] _range_inputs[i];
        _range_inputs[i] = NULL;
    }
    delete[] _range_inputs;
    _range_inputs = NULL;
    delete[] _interval_width;
    _interval_width = NULL;
    delete[] _weights;
    _weights = NULL;

}

void CMAC_net::report() {
    printf("\n##############\n");
    printf("CMAC report:\n");
    printf("Number of inputs:  %i\n", _num_inputs);
    printf("Range of inputs: ");
    for (int i = 0; i < _num_inputs; i++)
        printf("%i: (%f,%f) ",i, _range_inputs[i][0], _range_inputs[i][1]);
    printf("\n");
    printf("Number of tilings:  %i\n", _num_tilings);
    printf("Memory size:       %i\n", _memory_size);
    printf("Hash:              %i\n", _hash);
    printf("alpha:              %.2f\n", _alpha * _num_tilings);
}

void CMAC_net::sim(float* outputs, float* inputs) {
    int tiles_array[_num_tilings];
    GetTiles(tiles_array, _num_tilings, inputs, _num_inputs, _memory_size,
             _hash);
    *outputs = 0;
    for (int i = 0; i < _num_tilings; i++) {
        *outputs += _weights[tiles_array[i]];
    }
}
void CMAC_net::sim(float* outputs, float* inputs[], int size_inputs) {
    for (int i = 0; i < size_inputs; i++) {
        sim(&outputs[i], inputs[i]);
    }
}

void CMAC_net::train(float inputs[], float target) {
    int tiles_array[_num_tilings];
    GetTiles(tiles_array, _num_tilings, inputs, _num_inputs, _memory_size,
             _hash);
    float output = 0;

    for (int i = 0; i < _num_tilings; i++) {
        output += _weights[tiles_array[i]];
    }
    float err = target - output;
    float delta = err * _alpha;
    for (int i = 0; i < _num_tilings; i++) {
        _weights[tiles_array[i]] += delta;
    }
}

void CMAC_net::train(float* inputs[], int num_datapoints, float* target) {
    for (int i = 0; i < num_datapoints; i++) {
        train(inputs[i], target[i]);
    }
}

void CMAC_net::clear() {
    for (int i = 0; i < _memory_size; i++) {
        _weights[i] = 0.0f;
    }
}
