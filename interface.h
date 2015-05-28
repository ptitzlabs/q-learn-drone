#include <stdio.h>
#include <math.h>
#include "dislin.h"
#include "discpp.h"
#include <cstring>
#include <string>

class d_interface {
    //Dislin g;
    std::string cl1, cl2;
int id_lis1, id_lis2, id_pbut;
    public: 
    d_interface();

    void widget();
    private: 
    Dislin g;

};

