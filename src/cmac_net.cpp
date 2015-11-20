#include "cmac_net.h"

cmac_net::cmac_net() {
    double tile_dimension[3] = {0.15, 1.5, 1.5};
    cmac_net(3, tile_dimension, 8);
}
cmac_net::cmac_net(int num_inputs, double* tile_dimension, int tile_resolution,
                   int memory_size, int num_tilings, int num_hashings,
                   double alpha, double gamma, double lambda) {
    parm_init(num_inputs, tile_dimension, tile_resolution, memory_size,
              num_tilings, num_hashings, alpha, gamma, lambda);
}

void cmac_net::parm_init(int num_inputs, double* tile_dimension,
                         int tile_resolution, int memory_size, int num_tilings,
                         int num_hashings, double alpha, double gamma,
                         double lambda) {
    _memory_size = memory_size;  // allocated memory size
    _num_tilings = num_tilings;  // number of overlaping tilings
    _num_hashings =
        num_hashings;  // number of hashings (individual input levels)
    _tile_resolution =
        tile_resolution;  // individual tile resolution (number of sub-tiles)
    _alpha = alpha / num_tilings;  // learning rate
    _gamma = gamma;                // discount-rate parameter
    _lambda = lambda;              // trace-decay parameter
    _num_inputs = num_inputs;  // number of state values supplied to the network

    _weights = new double[_memory_size];       // initialize weights
    _tile_dimension = new double[num_inputs];  // initialize dimensions (approx.
                                               // size of input interval)
    _tile_sub_dimension = new double[num_inputs];  // initialize sub-dimensions
    _traces = new double[memory_size];             // initialize traces

    _hashings = new int* [_num_hashings];  // initialize hashings

    for (int i = 0; i < _memory_size; i++) {
        _weights[i] = 0.0f;
    }
    for (int i = 0; i < num_inputs; i++) {
        _tile_dimension[i] = tile_dimension[i];  // tile dimensions are stored
        _tile_sub_dimension[i] =
            tile_dimension[i] /
            (double)tile_resolution;  // tile sub-dimensions are calculated
    }
    for (int i = 0; i < memory_size; i++)
        _traces[i] = 0.0f;  // traces are re-set

    for (int i = 0; i < num_hashings; i++)
        _hashings[i] =
            new int[num_tilings];  // allocate enough feature hashings to get
                                   // individual tile outputs for each action
    for (int i = 0; i < num_hashings; i++) {
        for (int j = 0; j < num_tilings; j++) {
            _hashings[i][j] = 0;  // hashings are initialized
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
void cmac_net::clear_traces() {
    for (int i = 0; i < _memory_size; i++) _traces[i] = 0.0f;
}
void cmac_net::clear_weights() {
    for (int i = 0; i < _memory_size; i++) _weights[i] = 0.0f;
}
void cmac_net::drop_traces() {
    for (int i = 0; i < _memory_size; i++) _traces[i] *= _gamma * _lambda;
}

void cmac_net::update_traces(int hash) {
    for (int i = 0; i < _num_hashings; i++) {
        if (i != hash) {
            for (int j = 0; j < _num_tilings; j++) {
                _traces[_hashings[i][j]] = 0.0f;
            }
        }
    }
    for (int i = 0; i < _num_tilings; i++) _traces[_hashings[hash][i]] = 1.0f;
}
void cmac_net::generate_tiles(double* input) {
    double input_tmp[_num_inputs];
    for (int i = 0; i < _num_inputs; i++) {
        input_tmp[i] = input[i] / _tile_sub_dimension[i];
    }
    for (int i = 0; i < _num_hashings; i++) {
        get_tiles1(_hashings[i], _num_tilings, input_tmp, _num_inputs,
                   _memory_size, i);
    }
}

void cmac_net::return_value(double* output, int hash) {
    *output = 0;
    for (int i = 0; i < _num_tilings; i++)
        *output += _weights[_hashings[hash][i]];
}
void cmac_net::quick_update(double delta) {
    double tmp = _alpha * delta;
    for (int i = 0; i < _memory_size; i++) {
        _weights[i] += tmp * _traces[i];
    }
}
void cmac_net::update(double* input, double target, int hash) {
    int tiles_array[_num_tilings];
    get_tiles1(tiles_array, _num_tilings, input, _num_inputs, _memory_size,
               hash);
    double output = 0;

    for (int i = 0; i < _num_tilings; i++) output += _weights[tiles_array[i]];

    double err = target - output;
    double delta = err * _alpha;
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
        printf("%i: (%f) ", i, _tile_dimension[i]);
    printf("\n");
    printf("Number of tilings:  %i\n", _num_tilings);
    printf("Memory size:       %i\n", _memory_size);
    printf("alpha:              %.2f\n", _alpha * _num_tilings);
}

int cmac_net::get_num_hashings() { return _num_hashings; }
double* cmac_net::get_weights() { return _weights; }
int cmac_net::get_memory_size() { return _memory_size; }
int cmac_net::get_num_tilings() { return _num_tilings; }

double* cmac_net::get_tile_sub_dimension() { return _tile_sub_dimension; }

void cmac_net::write_weights(char* filename) {
    // int file = open(filename, O_BINARY | O_WRONLY);
    // write(file,(char *)_weights, _memory_size*sizeof(double));
    // close(file);
    FILE* weights_file = fopen(filename, "wb");
    fwrite(_weights, sizeof(double), _memory_size, weights_file);
    fclose(weights_file);
}
void cmac_net::read_weights(char* filename) {
    double weights[_memory_size];
    FILE* weights_file = fopen(filename, "rb");
    fread(weights, sizeof(double), _memory_size, weights_file);
    fclose(weights_file);

    for (int i = 0; i < _memory_size; i++) {
        std::cout << weights[i] << std::endl;
    }
    // int file = open(filename, O_BINARY | O_RDONLY);
    // read(file,(char *)_weights, _memory_size*sizeof(double));
    // close(file);
}
