#include "q_learn.h"

q_learn::q_learn(cmac_net * net, model * m, float epsilon, int max_steps){
    _net = net;
    _m = m;
    _epsilon = epsilon;
    _max_steps = max_steps;
    _size_q = _net->get_num_hashings();
    float* _q = new float[_size_q];
    calc_q();

}

q_learn::~q_learn(){
    delete[] _q;
    _q = NULL;
}


int q_learn::find_max() {
    int max_index = 0;
    float max_val = _q[0];
    int num_ties = 1;
    for (int i = 1; i < _size_q; i++) {
        if (_q[i] >= max_val) {
            if (_q[i] > max_val) {
                max_val = _q[i];
                max_index = i;
            } else {
                num_ties++;
                if (rand() % num_ties == 0) {
                    max_val = _q[i];
                    max_index = i;
                }
            }
        }
    }
    return max_index;
}
void q_learn::calc_q(){
    for (int i = 0; i<_size_q; i++){
        _net->return_value(_q[i],i);
    }
}

bool q_learn::with_probability(float p){
    return p >((float)rand()) / RAND_MAX;

}

//bool goal_reached()



void q_learn::run_episode() {
    _net->clear_traces();
    _net->generate_tiles(_m->get_state());

   for (int i = 0; i< _size_q; i++){
       _net->return_value(_q[i],i);
    }

   int action = find_max();

   if(with_probability(_epsilon)){
       action = rand() % _size_q;
   }

   int step = 0;

   
   


}
