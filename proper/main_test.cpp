#include "RL_headers.h"
#include "cmac_net.h"
#include "model.h"
#include "q_learn.h"

//int find_max(int size, float* arr) {
    //int max_index = 0;
    //float max_val = arr[0];
    //int num_ties = 1;
    //for (int i = 1; i < size; i++) {
        //if (arr[i] >= max_val) {
            //if (arr[i] > max_val) {
                //max_val = arr[i];
                //max_index = i;
            //} else {
                //num_ties++;
                //if (rand() % num_ties == 0) {
                    //max_val = arr[i];
                    //max_index = i;
                //}
            //}
        //}
    //}
    //return max_index;
//}
//void calc_q(int q_size, float * q, cmac_net * net){
    //for (int i = 0; i<q_size; i++){
        //net->return_value(q[i],i);
    //}
//}

//bool with_probability(float p){
    //return p >((float)rand()) / RAND_MAX;

//}

//bool goal_reached()



//void run_episode(cmac_net* net, model* m) {
    //net->clear_traces();
    //net->generate_tiles(m->get_state());
    //int size_q = net->get_num_hashings();
    //float* q = new float[size_q];

   //for (int i = 0; i< size_q; i++){
       //net->return_value(q[i],i);
    //}

   //int action = find_max(size_q,q);
   //float epsilon = 0.0;

   //if(with_probability(epsilon)){
       //action = rand() % size_q;
   //}

   //int step = 0;

   
   

    //delete[] q;
    //q = NULL;
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

    m.report();

    float* tile_dimension = new float[2];
    tile_dimension[0] = 0.14;
    tile_dimension[1] = 1.7;

    cmac_net net(2, tile_dimension);
    delete[] tile_dimension;
    tile_dimension = NULL;

    net.report();

    //float rand_arr[] = {3,1,2,3,3,1,3};

    //int kk = find_max(7,rand_arr);

    //printf("Checking max_finder: %i\n",kk);

    q_learn q(&net, &m);

    //run_episode(&net, &m);

    return 0;
}