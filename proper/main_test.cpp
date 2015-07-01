#include "RL_headers.h"
#include "cmac_net.h"
#include "model.h"


int main(int argc, const char ** argv) {

    //int mem_size = 3000;
    //int n_tilings = 10;
    //int n_actions = 3;

    //float epsilon = 0.0;
    //float alpha = 0.5;
    //float lambda = 0.9;
    //float gamma = 1;

    //cmac_net cm (mem_size,n_tilings);
    //cm.report();

    printf("Testing...\n");

    float** state_limits;
    state_limits = new float*[2];
    state_limits[0] = new float[2];
    state_limits[1] = new float[2];

    state_limits[0][0] = -0.07;
    state_limits[0][1] = 0.07;
    state_limits[1][0] = -1.2;
    state_limits[1][1] = 0.6;
    model m(2, state_limits, 1);

    m.report();
    return 0;

}
