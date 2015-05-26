#include </usr/include/python2.7/Python.h>

class Python_interface {
   public:
    Python_interface(char const* module);
    ~Python_interface();
    PyObject * python_array(float arr[], int dim[]);
    PyObject * python_array(float * arr[], int dim[]);

   private:
    PyObject* _traceback_module, *_module, *_module_name,
        *_traceback_dictionary, *_module_dictionary, *_format_exception,
        *_run_expression, *_run_script;
};
