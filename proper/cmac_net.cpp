#include "cmac_net.h"

cmac_net::cmac_net(int num_inputs, float* tile_dimension, int tile_resolution,
                   int memory_size, int num_tilings, int num_hashings,
                   float alpha, float gamma, float lambda) {
    _memory_size = memory_size;
    _num_tilings = num_tilings;
    _num_hashings = num_hashings;
    _tile_resolution = tile_resolution;
    _alpha = alpha / num_tilings;
    _gamma = gamma;
    _lambda = lambda;
    _num_inputs = num_inputs;

    _weights = new float[_memory_size];
    _tile_dimension = new float[num_inputs];
    _tile_sub_dimension = new float[num_inputs];
    _traces = new float[memory_size];

    _hashings = new int* [_num_hashings];

    for (int i = 0; i < num_inputs; i++) {
        _weights[i] = 0.0f;

    }
    for (int i = 0; i < num_inputs; i++) {
        _tile_dimension[i] = tile_dimension[i];
        _tile_sub_dimension[i] = tile_dimension[i] / (float)tile_resolution;
    }
    for (int i = 0; i<memory_size;i++) 
        _traces[i] = 0.0f;
    
    for (int i = 0; i < num_hashings; i++) _hashings[i] = new int[num_tilings];
    for (int i = 0; i < num_hashings; i++){
        for (int j = 0; j < num_tilings;j++){
            _hashings[i][j] = 0;
        }
    }



}

cmac_net::~cmac_net() {
    delete[] _weights;
    _weights = NULL;
    delete[] _tile_dimension;
    _tile_dimension = NULL;
    delete[] _tile_sub_dimension;
    _tile_sub_dimension = NULL;
    delete[] _traces;
    _traces = NULL;



    for (int i = 0; i < _num_hashings; i++) {
        delete[] _hashings[i];
        _hashings[i] = NULL;
    }
    delete[] _hashings;
    _hashings = NULL;
}
void cmac_net::clear_traces(){

    for (int i = 0; i < _memory_size; i++) _traces[i] *= 0;
}
void cmac_net::drop_traces() {
    for (int i = 0; i < _memory_size; i++) _traces[i] *= _gamma * _lambda;

}

void cmac_net::update_traces(int hash) {
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

void cmac_net::report() {
    printf("\n##############\n");
    printf("CMAC report:\n");
    printf("Number of inputs:  %i\n", _num_inputs);
    printf("Tile dimension: ");
    for (int i = 0; i < _num_inputs; i++)
        printf("%i: (%f) ",i, _tile_dimension[i]);
    printf("\n");
    printf("Number of tilings:  %i\n", _num_tilings);
    printf("Memory size:       %i\n", _memory_size);
    printf("alpha:              %.2f\n", _alpha * _num_tilings);
}

int cmac_net::get_num_hashings(){
    return _num_hashings;

}

void cmac_net::write_weights(char * filename){
    int file = open(filename, O_BINARY | O_WRONLY);
    write(file,(char *)_weights, _memory_size*sizeof(float));
    close(file);
}
void cmac_net::read_weights(char * filename){
    int file = open(filename, O_BINARY | O_RDONLY);
    read(file,(char *)_weights, _memory_size*sizeof(float));
    close(file);
}