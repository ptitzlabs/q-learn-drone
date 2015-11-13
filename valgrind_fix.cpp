#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "src/drone_dynamics.hpp"
int main(){

    int * a;
    //int b = 1;
    //int c = 2;

    int * d = new int[3];

    a = &d[0];

    //d[1] = b;
    //d[2] = c;

    *a = 2;
    std::cout<<"a: "<< *a<<" d[0]: " << d[0]<<std::endl;
    d[0] = 3;
    std::cout<<"a: "<< *a<<" d[0]: " << d[0]<<std::endl;

    drone_dynamics drone;

    drone.report();
    drone.input_set(1,0);

    for (int i = 0; i< 1000; i++){
    drone.rk4_step(0.001);
    }
    drone.report();
    drone.reset();
    drone.report();

    drone.input_set(1,0);
    for (int i = 0; i < 1000; i++){
    drone.euler_step(0.001);
    }

    drone.report();
    return 0;
}
