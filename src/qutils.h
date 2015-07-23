#define QUTILS_H

#ifndef RL_HEADERS_H
#include "RL_headers.h"
//#include <cmath>
//#include <cstdio>
//#include <iostream>
//#include <fstream>
//#include <cstdlib>
//#include <cstring>
//#include <unistd.h>
//#include <fcntl.h>
//#include <string>
#endif

namespace qutils{
    int find_max(float* arr, int numel);
    bool with_probability(float p);

}
void print_arr_1d(int n, float * arr);
void print_arr_2d(int n,int  m, float ** arr);
void save_arr_1d(int n, float* arr, char* filename);
void save_arr_2d(int n, int m, float* xarr, float* yarr, float** zarr, char* filename);
