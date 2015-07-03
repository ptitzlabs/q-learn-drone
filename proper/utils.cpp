#include "utils.h"

void print_arr_1d(int n, float* arr) {
    std::cout << "[";
    for (int i = 0; i < n; i++) {
        std::cout << arr[i];
        if (i < n - 1) std::cout << ", ";
    }
    std::cout << "]";
}
