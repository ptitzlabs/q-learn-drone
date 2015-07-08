#include "RL_headers.h"
#include "cmac_net.h"
#include "model.h"
#include "q_learn.h"
#include "qutils.h"

// int find_max(int size, float* arr) {
// int max_index = 0;
// float max_val = arr[0];
// int num_ties = 1;
// for (int i = 1; i < size; i++) {
// if (arr[i] >= max_val) {
// if (arr[i] > max_val) {
// max_val = arr[i];
// max_index = i;
//} else {
// num_ties++;
// if (rand() % num_ties == 0) {
// max_val = arr[i];
// max_index = i;
//}
//}
//}
//}
// return max_index;
//}
// void calc_q(int q_size, float * q, cmac_net * net){
// for (int i = 0; i<q_size; i++){
// net->return_value(q[i],i);
//}
//}

// bool with_probability(float p){
// return p >((float)rand()) / RAND_MAX;

//}

// bool goal_reached()

// void run_episode(cmac_net* net, model* m) {
// net->clear_traces();
// net->generate_tiles(m->get_state());
// int size_q = net->get_num_hashings();
// float* q = new float[size_q];

// for (int i = 0; i< size_q; i++){
// net->return_value(q[i],i);
//}

// int action = find_max(size_q,q);
// float epsilon = 0.0;

// if(with_probability(epsilon)){
// action = rand() % size_q;
//}

// int step = 0;

// delete[] q;
// q = NULL;
//}
int main(int argc, const char** argv) {
    // int mem_size = 3000;
    // int n_tilings = 10;
    // int n_actions = 3;

    // float epsilon = 0.0;
    // float alpha = 0.5;
    // float lambda = 0.9;
    // float gamma = 1;

    // cmac_net cm (mem_size,n_tilings);
    // cm.report();

    printf("Testing...\n");

    float** state_limits;
    state_limits = new float* [2];
    state_limits[0] = new float[2];
    state_limits[1] = new float[2];

    state_limits[0][0] = -0.07;
    state_limits[0][1] = 0.07;
    state_limits[1][0] = -1.2;
    state_limits[1][1] = 0.6;

    float init_state[] = {0, -0.5};
    model m(2, state_limits, init_state, 1);

    for (int i = 0; i < 2; i++) {
        delete[] state_limits[i];
        state_limits[i] = NULL;
    }
    delete[] state_limits;
    state_limits = NULL;

    // m.report();

    float* tile_dimension = new float[2];
    tile_dimension[0] = 0.14;
    tile_dimension[1] = 1.7;

    cmac_net net(2, tile_dimension);
    delete[] tile_dimension;
    tile_dimension = NULL;

    // net.report();

    // float rand_arr[] = {3,1,2,3,3,1,3};

    // int kk = find_max(7,rand_arr);

    // printf("Checking max_finder: %i\n",kk);

    float action_levels[] = {0, 1, 2};
    float goal[] = {0, 0.5};
    int n_action_levels = 3;
    int n_goal_states = 1;
    int goal_state_index[] = {1};
    q_learn q(&net, &m, n_action_levels, action_levels, goal, n_goal_states,
              goal_state_index);
    q.report();
    // q.run_episode();

    for (int i = 0; i < 2; i++) {
        printf("Doing run %i\n", i);
        net.clear_weights();
        for (int i = 0; i < 100; i++) {
            q.run_episode();
        }
    }

    // run_episode(&net, &m);
    //
    //
    //
    float* xx = new float[4];
    float* yy = new float[4];

    float** zz = new float* [4];

    for (int i = 0; i < 4; i++) zz[i] = new float[4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            xx[i] = i;
            yy[j] = j;
            zz[i][j] = i + j / 10.0f;
        }
    }

    save_arr_2d(4,4,xx,yy,zz,(char*)"check_2d");


    delete[] xx;
    delete[] yy;
    for (int i = 0; i<4; i++) delete[] zz[i];
    delete[] zz;

    q.write_contour((char*)"logs/contour",0,1000,1000);
    q.write_contour((char*)"logs/contour",1,1000,1000);
    q.write_contour((char*)"logs/contour",2,1000,1000);

    return 0;
}
