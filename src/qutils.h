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
    int find_max(double* arr, int numel);
    bool with_probability(double p);

}
void print_arr_1d(int n, double * arr);
void print_arr_2d(int n,int  m, double ** arr);
void save_arr_1d(int n, double* arr, char* filename);
void save_arr_2d(int n, int m, double* xarr, double* yarr, double** zarr, char* filename);
