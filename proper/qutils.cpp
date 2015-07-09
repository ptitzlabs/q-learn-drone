#include "qutils.h"

void print_arr_1d(int n, float* arr) {
    std::cout << "[";
    for (int i = 0; i < n; i++) {
        std::cout << arr[i];
        if (i < n - 1) std::cout << ", ";
    }
    std::cout << "]";
}

void save_arr_2d(int n, int m, float* xarr, float* yarr, float** zarr,
                 char* filename) {
    size_t len = strlen(filename);
    std::string filename_s;
    filename_s = filename;
    std::ofstream file_x;
    std::ofstream file_y;
    std::ofstream file_z;

    char * filename_tmp = new char[strlen(filename) +7];

    file_x.open(filename_s.insert(len,"_x.csv").c_str(),
                std::ofstream::out | std::ofstream::trunc);
    file_y.open(filename_s.replace(len,6,"_y.csv").c_str(),
                std::ofstream::out | std::ofstream::trunc);
    file_z.open(filename_s.replace(len,6,"_z.csv").c_str(),
                std::ofstream::out | std::ofstream::trunc);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            file_x << xarr[i];
            file_y << yarr[j];
            file_z << zarr[i][j];
            if (j < m - 1) {
                file_x << ", ";
                file_y << ", ";
                file_z << ", ";
            } else {
                file_x << "\n";
                file_y << "\n";
                file_z << "\n";
            }
        }
    }
    file_x.close();
    file_y.close();
    file_z.close();
    delete[] filename_tmp;
    filename_tmp = NULL;
}
