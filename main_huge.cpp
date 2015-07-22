
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "src/RL_headers.h"
#include "src/joystick.h"
#include "src/glhelper.h"
#include "src/sdl_gfx_screen.h"
//#include "src/model.h"
//#include "src/cmac_net.h"
//#include "src/q_learn.h"
#include <thread>

using namespace std;

#define MOUNTAIN_CAR

struct model_params {
    int num_states;
    int num_inputs;
    int num_actions;

    float* action_space;

    float** state_limits;
};

struct state_instance {
    float* x;
    float* dx;
    float in;
};
struct q_params {
    float alpha;
    float gamma;
    float lambda;
};
struct q_space {
    int memory_size;
    int num_tilings;
    int resolution;
    float* weights;
    float* traces;
    float* sub_dimension;
};
class model {
   public:
    float** curr_s, **init_s;
    model_params parm;
    model();

    void init_set(float* init_state,int id);
    void advance(int in, int id);
    void reset(int id);
};

model::model() {
    parm.num_states = 2;
    parm.num_inputs = 1;
    parm.num_actions = 3;

    parm.action_space = new float[3];

    parm.action_space[0] = 0;
    parm.action_space[1] = 1;
    parm.action_space[2] = 2;

    float state_limits[2][2] = {{-0.07, 0.07}, {-1.2, 0.6}};

    parm.state_limits = new float* [2];
    for (int i = 0; i < 2; i++) parm.state_limits[i] = new float[2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            parm.state_limits[i][j] = state_limits[i][j];
        }
    }

    init_s = new float*[10];
    curr_s = new float*[10];

    for (int i = 1; i<10; i++){
        init_s[i] = new float[2];
        curr_s[i] = new float[2];
    }

    for (int i = 1; i<10; i++){
        init_s[i][0] = 0;
        init_s[i][1] = -0.5;
    }
}
void model::init_set(float * new_init_state, int id){
    for (int i = 0; i < parm.num_states; i++) init_s[id][i] = new_init_state[i];
}
void model::reset(int id) {
    for (int i = 0; i < parm.num_states; i++) curr_s[id][i] = init_s[id][i];
}
void model::advance(int in,int id){

    curr_s[id][0] += (parm.action_space[in] - 1.0) * 0.001 +
               cos(3.0 * curr_s[id][1]) * (-0.0025);
    curr_s[id][1] += curr_s[id][0];
}

class q_learn_instance {
    model* _m;
    q_params _parm;
    q_space _q;

   public:
    q_learn_instance(model* m);
    void train(float* init_state, float* goal_state);
    void run_episode();
    void run_step();
};

q_learn_instance::q_learn_instance(model* m) {
    _m = m;
    _parm.alpha = 0.5;
    _parm.gamma = 1;
    _parm.lambda = 0.9;

    _q.memory_size = 160000;
    _q.num_tilings = 10;
    _q.resolution = 8;
    _q.weights = new float[_q.memory_size];
    _q.traces = new float[_q.memory_size];
    for (int i = 0; i < _q.memory_size; i++) {
        _q.weights = 0;
        _q.traces = 0;
    }
    _q.sub_dimension = new float[3];

    for (int i = 0; i < 2; i++) {
        _q.sub_dimension[i] =
            (_m->parm.state_limits[i][1] - _m->parm.state_limits[i][0]) /
            _q.resolution;
    }
    _q.sub_dimension[3] = _q.sub_dimension[2];
}

int main() {
    model m;
    q_learn_instance q(&m);
    return 0;
}
// struct q_space {
// int memory_size;
// float * weights;
// float * traces;
// float * sub_dimension;
//};

// struct q_learn_params{
// int gamma;
// int alpha;
//};

// struct model {
// int num_states;
// int num_inputs;

// float *in;
// float *x;
// float * dx;

// float *actions;
// int *num_actions;
//};

// class dynamics{

// public:
// void init(model * m);
// void init(model * m, float * init_state);
// void calc_derivatives(model * m);
// void model_step(model *  m);
// void model_step(float * in, model * m);
// private:

//};

// class q_learn{

// public:
// void init(float * init_state, float * goal);
// void run_episode(model * m, float * init_state, float * goal);
// void advance();
// void calc_reward();
// private:
//};

// void dynamics::init(model * m){
//#ifdef MOUNTAIN_CAR
// m->num_states = 2;
// m->num_inputs = 1;
// m->in = new float;
// m->in = 0;
// m->x = new float[2];
// m->x[0] = 0;
// m->x[1] = -0.5;
// calc_derivatives(m);
//#endif

//}
// void dynamics::calc_derivatives(model * m){
//#ifdef MOUNTAIN_CAR
// m->dx[0] = (m->in[0]-1)*0.01+cos(3*m->x[1])*(-0.0025);
// m->dx[1] = m->x[0];
//#endif
//}
// void dynamics::model_step(model * m){
// calc_derivatives(m);
// for(int i = 0; i<m->num_states;i++)
// m->x[i]+=m->dx[i];
//}
// void dynamics::model_step(float * in, model * m){
// m->in = in;
// model_step(m);
//}

// void q_learn::run_episode(model* m, float * init_state, float * goal){
// dynamics d;
// d.init(m,init_state);
//}
