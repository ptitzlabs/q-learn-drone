#include <stdexcept>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "tiles.h"
#include <stdio.h>
#include <string.h>
//#include "python_interface.h"

class CMAC_net {
   public:
    CMAC_net(int num_inputs, float range_inputs[][2], int num_tilings,
             int tile_resolution, float alpha, int memory_size, int hash);
    void train(float inputs[], float target);
    void train(float* inputs[], int num_datapoints, float* target);
    void sim(float* outputs, float* inputs);
    void sim(float* outputs, float* inputs[], int num_inputs);
    void report();
    void plot_contourf();
    ~CMAC_net();

   private:
    float* _interval_width;
    float* _weights;
    int _num_inputs;
    float** _range_inputs;
    int _num_tilings;
    int _memory_size;
    int _hash;
    float _alpha;
    int _tile_resolution;
};

void CMAC_testing();
