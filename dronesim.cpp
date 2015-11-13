//#include <cstdlib>
//#include <iostream>
#include "src/drone_dynamics.hpp"
#include "src/q_learn.h"
#include "src/model.h"
#include "src/cmac_net.h"

//using namespace std;

int main(){

    drone_dynamics drone;


    double init_state[6] = {0,0,0,0,0,0};
    double init_state_d[6] = {0,0,0,0,0,0};
    double init_input[4] = {1,0,0,0};
    double h = 1;
    

    drone.report();
    drone.state_set(init_state_d,init_state);
    drone.input_set(init_input);

    drone.report();


    for (int i = 0; i < 10; i++ ){
        drone.rk4_step(0.01);
    drone.report();
    }
    return 0;

}
