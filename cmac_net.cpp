#include "cmac_net.h"

CMAC_net::CMAC_net(int num_inputs, float range_inputs[][2], int num_tilings,
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
    printf("CMAC report:\n");
    printf("Number of inputs:  %i\n", _num_inputs);
    printf("Range of inputs: ");
    for (int i = 0; i < _num_inputs; i++)
        printf("i %i: (%f,%f) ", i, _range_inputs[i][0], _range_inputs[i][1]);
    printf("\n");
    printf("Number of tilings:  %i\n", _num_tilings);
    printf("Memory size:       %i\n", _memory_size);
    printf("Hash:              %i\n", _hash);
    printf("alpha:              %.2f\n", _alpha * _num_tilings);
}

#define PY_TEST
void CMAC_net::plot_contourf() {
#ifndef PY_TEST
    Py_Initialize();
    import_array1();
    PyObject* p_name, *p_module, *p_dict, *p_func, *p_value, *format_expression,
        *run_expression, *run_script, *traceback_module, *python_module,
        *traceback_dictionary, *python_dictionary;
    traceback_module = PyImport_ImportModule("traceback");
    python_module = PyImport_ImportModule("python_fun");

    if (traceback_module && python_module) {
        traceback_dictionary = PyModule_GetDict(traceback_module);
        python_dictionary = PyModule_GetDict(python_module);

        format_expression =
            PyDict_GetItemString(traceback_dictionary, "format_expression");

        run_expression =
            PyDict_GetItemString(python_dictionary, "run_expression");

        run_script = PyDict_GetItemString(python_dictionary, "run_script");

        if (format_expression && run_expression && run_script) {
            if (PyCallable_Check(format_expression) &&
                PyCallable_Check(run_expression) &&
                PyCallable_Check(run_script)) {
            }
        }
    }
    fexcp = PyDict_GetItemString(dict1)

        const int res = 4;
    float** x = new float* [res];
    float** y = new float* [res];
    float** z = new float* [res];

    x[0] = new float[res];
    x[1] = x[0] + res;
    x[2] = x[1] + res;

    y[0] = new float[res];
    y[1] = y[0] + res;
    y[2] = y[1] + res;

    z[0] = new float[res];
    z[1] = z[0] + res;
    z[2] = z[1] + res;

    int mdim[] = {res, res};

    float x_step =
        interval_width[0] * (float)_tile_resolution / ((float)res - 1);
    float y_step =
        interval_width[1] * (float)_tile_resolution / ((float)res - 1);
    float input_vals[2];
    std::ofstream fout;
    fout.open("plot_contourf_tmp.txt");

    for (int i = 0; i < res; i++) {
        for (int j = 0; j < res; j++) {
            x[i][j] = _range_inputs[0][0] + x_step * (float)i;
            y[i][j] = _range_inputs[0][0] + y_step * (float)j;
            input_vals[0] = x[i][j];
            input_vals[1] = y[i][j];
            sim(&z[i][j], input_vals);
            fout << x[i][j] << "," << y[i][j] << "," << z[i][j];

            printf("(%.2f,%.2f,%.2f)", x[i][j], y[i][j], z[i][j]);
            if (j < res - 1) {
                fout << ",";
                std::cout << ",";
            }
        }
        fout << std::endl;
        printf("\n");
    }

    fout.close();

    PyObject* x_mat = PyArray_SimpleNewFromData(2, mdim, PyArray_DOUBLE, x[0]);

    char full_path[256];
    char p_path[256];
    getcwd(full_path, 255);
    strcpy(p_path, "sys.path.append(\"");
    strcat(p_path, full_path);
    strcat(p_path, "\")");

    PyRun_SimpleString("import sys");
    PyRun_SimpleString(p_path);

    p_name = PyString_FromString("py_plot");
    p_module = PyImport_Import(p_name);
    if (p_module == NULL) {
        printf("Failed to open py_plot.py");
    }
    p_dict = PyModule_GetDict(p_module);
    p_func = PyDict_GetItemString(p_dict, "plot_contourf");
    if (PyCallable_Check(p_func)) {
        PyObject_CallObject(p_func, NULL);
    } else {
        PyErr_Print();
    }

    Py_DECREF(p_module);
    Py_DECREF(p_name);
    Py_Finalize();
#endif
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
const int k_num_inputs = 2;
const float k_range_inputs[][2] = {{0.0, 2.0}, {0.0, 2.0}};
const int k_num_tilings = 32;
const int k_tile_resolution = 8;
const float k_alpha = 0.5;
const int k_memory_size = 3000;
const int k_hash = 0;

void CMAC_testing() {
    CMAC_net net(k_num_inputs, const_cast<float(*)[2]>(k_range_inputs),
                 k_num_tilings, k_tile_resolution, k_alpha, k_memory_size,
                 k_hash);
    net.report();

    float inputs[][2] = {{0, 0}, {2, 0}, {2, 2}, {0, 2}};
    float targets[] = {1, 0, 1, 0};
    for (int i = 0; i < 4; i++) {
        net.train(inputs[i], targets[i]);
    }
    net.plot_contourf();

    Python_interface python("python_fun");
    printf("doing stuff...\n");
}
