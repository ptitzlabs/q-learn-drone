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

void print_arr_1d(int n, float * arr);
void save_arr_1d(int n, float* arr, char* filename);
void save_arr_2d(int n, int m, float* xarr, float* yarr, float** zarr, char* filename);
