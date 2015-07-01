#include "cmac_net.h"

cmac_net::cmac_net(int num_inputs, float** range_inputs, int tile_resolution,
                   int memory_size, int num_tilings, int num_hashings,
                   float alpha, float gamma, float lambda) {
    _memory_size = memory_size;
    _num_tilings = num_tilings;
    _num_hashings = num_hashings;
    _tile_resolution = tile_resolution;
    _alpha = alpha / num_tilings;
    _gamma = gamma;
    _lambda = lambda;

    _weights = new float[_memory_size];
    _interval_width = new float[num_inputs];
    _weights = new float[memory_size];
    _traces = new float[memory_size];

    _hashings = new int* [_num_hashings];
    _range_inputs = new float* [num_inputs];

    for (int i = 0; i < num_inputs; i++) _range_inputs[i] = new float[2];
    for (int i = 0; i < num_inputs; i++) {
        _range_inputs[i][0] = range_inputs[i][0];
        _range_inputs[i][1] = range_inputs[i][1];
        _interval_width[i] =
            (range_inputs[i][1] - range_inputs[i][0]) / (float)tile_resolution;
    }
    for (int i = 0; i < num_tilings; i++) _hashings[i] = new int[num_tilings];

    for (int i = 0; i < num_inputs; i++) {
        _weights[i] = 0.0f;
        _traces[i] = 0.0f;
    }
}

cmac_net::~cmac_net() {
    delete[] _weights;
    _weights = NULL;

    for (int i = 0; i < _num_tilings; i++) {
        delete[] _hashings[i];
        _hashings[i] = NULL;
    }
    delete[] _hashings;
    _hashings = NULL;
}
void cmac_net::drop_traces(int hash){
    for (int i = 0; i < _memory_size; i++) _traces[i] *= _gamma * _lambda;
    for (int i = 0; i < _num_hashings; i++) {
        for (int j = 0; j < _num_tilings; j++) {
            if (i != hash) _traces[_hashings[i][j]] = 0.0f;
        }
    }
    for (int i = 0; i < _num_tilings; i++) _traces[_hashings[hash][i]] = 1.0f;

}
void cmac_net::generate_tiles(float* input) {
    for (int i = 0; i < _num_hashings; i++)
        GetTiles(_hashings[i], _num_tilings, input, _num_inputs, _memory_size,
                 i);
}

void cmac_net::return_value(float output, int hash) {
    // int tiles_array[_num_tilings];
    // GetTiles(tiles_array, _num_tilings, input, _num_inputs, _memory_size,
    // hash);
    output = 0;
    for (int i = 0; i < _num_tilings; i++)
        output += _weights[_hashings[hash][i]];
}

void cmac_net::update(float* input, float target, int hash) {

    int tiles_array[_num_tilings];
    GetTiles(tiles_array, _num_tilings, input, _num_inputs, _memory_size, hash);
    float output = 0;

    for (int i = 0; i < _num_tilings; i++) output += _weights[tiles_array[i]];

    float err = target - output;
    float delta = err * _alpha;
    for (int i = 0; i < _num_tilings; i++) {
        _weights[tiles_array[i]] += delta;
    }
}

void cmac_net::report() { printf("cmac net report\n"); }
