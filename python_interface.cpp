#include "python_interface.h"

Python_interface::Python_interface(char const* module) {
    printf("importing module %s\n", module);
    // Launch python
    Py_Initialize();

    // Get the current directory path and add it to python
    char full_path[256];
    char p_path[256];
    getcwd(full_path, 255);
    strcpy(p_path, "sys.path.append(\"");
    strcat(p_path, full_path);
    strcat(p_path, "\")");
    PyRun_SimpleString("import sys");
    PyRun_SimpleString(p_path);
    // Load some modules
    _traceback_module = PyImport_ImportModule("traceback");
    _module = PyImport_ImportModule(module);
    // if(_module)    printf("everything is a-ok\n");
    if (_traceback_module && _module) {
        // printf("everything is a-ok\n");
        // Get module dictionaries and some functions
        _traceback_dictionary = PyModule_GetDict(_traceback_module);
        _module_dictionary = PyModule_GetDict(_module);

        _format_exception =
            PyDict_GetItemString(_traceback_dictionary, "format_exception");

        _run_expression =
            PyDict_GetItemString(_module_dictionary, "run_expression");

        _run_script = PyDict_GetItemString(_module_dictionary, "run_script");
        // if(_format_exception) printf("everything is a-ok\n");

        if (_format_exception && _run_expression && _run_script) {
            if (PyCallable_Check(_format_exception) &&
                PyCallable_Check(_run_script) &&
                PyCallable_Check(_run_expression)) {
                //printf("everything is a-ok\n");

            }


        }
    }
}

Python_interface::~Python_interface() {
    printf("dereferencing python modules\n");

    Py_Finalize();
}

PyObject * Python_interface::python_array(float arr[], int dim[]) {
    //return PyArray_SimpleNewFromData(1,(long int*)dim,0,arr);
}



#define PY_TEST
#ifndef PY_TEST
void CMAC_net::plot_contourf() {

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

}
#endif
