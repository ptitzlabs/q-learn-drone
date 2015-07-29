
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
#include "src/tiles.h"
#include "src/qutils.h"
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
    float epsilon;
    int max_steps;
    float max_overshoot;
};
struct q_space {
    int memory_size;
    int num_tilings;
    int resolution;
    int num_vars;
    float* weights;
    float* traces;
    float* sub_dimension;
    float* reward_bandwidth;
};
class model {
   public:
    float** curr_s, **init_s;
    model_params m_parm;
    model();

    void init_set(float* init_state, int id);
    void advance(int in, int id);
    void reset(int id);
    void report();
};
void model::report() {
    cout << "\n####" << endl
         << "Model report:\nnum states: " << m_parm.num_states
         << "\nnum inputs: " << m_parm.num_inputs
         << "\nnum actions: " << m_parm.num_actions << "\n action space: ";
    print_arr_1d(3, m_parm.action_space);

    cout << "\nstate limits: ";
    print_arr_2d(2, 2, m_parm.state_limits);
    cout << "\ncurr state: ";
    print_arr_1d(2, curr_s[0]);
    cout << "\ninit state: ";
    print_arr_1d(2, init_s[0]);
    cout << endl;
}
model::model() {
    m_parm.num_states = 2;
    m_parm.num_inputs = 1;
    m_parm.num_actions = 3;

    m_parm.action_space = new float[3];

    m_parm.action_space[0] = 0;
    m_parm.action_space[1] = 1;
    m_parm.action_space[2] = 2;

    float state_limits[2][2] = {{-0.07, 0.07}, {-1.2, 0.7}};

    m_parm.state_limits = new float* [2];
    for (int i = 0; i < 2; i++) m_parm.state_limits[i] = new float[2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            m_parm.state_limits[i][j] = state_limits[i][j];
        }
    }

    init_s = new float* [10];
    curr_s = new float* [10];

    for (int i = 0; i < 10; i++) {
        init_s[i] = new float[2];
        curr_s[i] = new float[2];
    }

    for (int i = 0; i < 10; i++) {
        init_s[i][0] = 0;
        init_s[i][1] = -0.5;
        curr_s[i][0] = 0;
        curr_s[i][1] = -0.5;
    }
}

void model::init_set(float* new_init_state, int id) {
    for (int i = 0; i < m_parm.num_states; i++)
        init_s[id][i] = new_init_state[i];
}

void model::reset(int id) {
    for (int i = 0; i < m_parm.num_states; i++) curr_s[id][i] = init_s[id][i];
}

void model::advance(int in, int id) {
    curr_s[id][0] += (m_parm.action_space[in] - 1.0) * 0.001 +
                     cos(3.0 * curr_s[id][1]) * (-0.0025);

    if (curr_s[id][0] < m_parm.state_limits[0][0])
        curr_s[id][0] = m_parm.state_limits[0][0];
    if (curr_s[id][0] > m_parm.state_limits[0][1])
        curr_s[id][0] = m_parm.state_limits[0][1];

    curr_s[id][1] += curr_s[id][0];
    if (curr_s[id][1] < m_parm.state_limits[1][0])
        curr_s[id][1] = m_parm.state_limits[1][0];
    if (curr_s[id][1] > m_parm.state_limits[1][1])
        curr_s[id][1] = m_parm.state_limits[1][1];
    // cout << curr_s[id][0] << " " << curr_s[id][1] << endl;
}

class q_learn_instance {
    model* _m;

    q_space _q;
    void run_episode();
    void run_step();
    void set_goal(float* goal_state);
    void set_init(float* init_state);
    void set_net_input();
    void gen_tiles();
    void calc_q();
    void calc_q(int hash);
    float calc_reward();
    void calc_reward_parm();

    void clear_traces();
    void drop_traces();
    void update_traces(int hash);
    float* _goal_state;
    float* _q_val;
    float* _init_state;
    float* _net_input;
    int** _features;

   public:
    q_params q_parm;
    q_learn_instance(model* m);
    void train(float* init_state, float* goal_state);
    void report();
};

void q_learn_instance::report() {
    _m->report();
    cout << "\n####" << endl << "Q learning report:\ngamma: " << q_parm.gamma
         << "\nlambda: " << q_parm.lambda << "\nepsilon: " << q_parm.epsilon
         << "\nmax steps: " << q_parm.max_steps
         << "\nmemory_size: " << _q.memory_size
         << "\nnum_tilings: " << _q.num_tilings
         << "\nresolution: " << _q.resolution
         << "\nnum variables: " << _q.num_vars << "\ncurrent Q: ";
    print_arr_1d(3, _q_val);
    cout << "\nalpha: " << q_parm.alpha << "\nsub dimension: ";
    print_arr_1d(3, _q.sub_dimension);
    cout << "\ngoal state: ";
    print_arr_1d(2, _goal_state);
    cout << "\ninit state: ";
    print_arr_1d(2, _init_state);
    cout << endl;
}
q_learn_instance::q_learn_instance(model* m) {
    _m = m;

    q_parm.gamma = 1;
    q_parm.lambda = 0.9;
    q_parm.epsilon = 0.0;
    q_parm.max_steps = 10000;
    q_parm.max_overshoot = 0.03;

    _q.memory_size = 160000;
    _q.num_tilings = 10;
    _q.resolution = 8;
    _q.num_vars = 3;
    _q_val = new float[_m->m_parm.num_actions];
    _q.weights = new float[_q.memory_size];
    _q.traces = new float[_q.memory_size];

    _q.reward_bandwidth = new float[2];
    _q.reward_bandwidth[0] =
        (_m->m_parm.state_limits[0][1] - _m->m_parm.state_limits[0][0]) / 2;
    _q.reward_bandwidth[1] =
        fabs(_m->curr_s[0][1] - _m->init_s[0][1]) * q_parm.max_overshoot;

    q_parm.alpha = 0.5 / _q.num_tilings;
    for (int i = 0; i < _q.memory_size; i++) {
        _q.weights[i] = 0;
        _q.traces[i] = 0;
    }
    _q.sub_dimension = new float[3];

    for (int i = 0; i < 2; i++) {
        _q.sub_dimension[i] =
            (_m->m_parm.state_limits[i][1] - _m->m_parm.state_limits[i][0]) /
            _q.resolution;
    }
    _q.sub_dimension[2] = _q.sub_dimension[1];

    _goal_state = new float[_m->m_parm.num_states];
    _init_state = new float[_m->m_parm.num_states];
    _net_input = new float[_q.num_vars];
    _features = new int* [_m->m_parm.num_actions];
    for (int i = 0; i < _m->m_parm.num_actions; i++)
        _features[i] = new int[_q.num_tilings];
}
void q_learn_instance::set_goal(float* goal_state) {
    for (int i = 0; i < _m->m_parm.num_states; i++)
        _goal_state[i] = goal_state[i];
}

void q_learn_instance::set_init(float* init_state) {
    for (int i = 0; i < _m->m_parm.num_states; i++)
        _init_state[i] = init_state[i];
}

void q_learn_instance::set_net_input() {
    // for (int i = 0; i < 3; i++) _net_input[i] = 0;
    _net_input[0] = _m->curr_s[0][0] / _q.sub_dimension[0];
    _net_input[1] = _m->curr_s[0][1] / _q.sub_dimension[1];
    _net_input[2] = _goal_state[1] / _q.sub_dimension[1];
    // cout<<"net input: "<<_net_input[0]<<" "<<_net_input[1]<<"
    // "<<_net_input[2]<<endl;
}

void q_learn_instance::calc_q(int hash) {
    _q_val[hash] = 0;
    for (int i = 0; i < _q.num_tilings; i++)
        _q_val[hash] += _q.weights[_features[hash][i]];
}
void q_learn_instance::calc_q() {
    for (int i = 0; i < _m->m_parm.num_actions; i++) {
        calc_q(i);
    }
}

void q_learn_instance::clear_traces() {
    for (int i = 0; i < _q.memory_size; i++) {
        _q.traces[i] = 0;
    }
}
void q_learn_instance::drop_traces() {
    for (int i = 0; i < _q.memory_size; i++) {
        _q.traces[i] *= q_parm.gamma * q_parm.lambda;
    }
}

void q_learn_instance::update_traces(int hash) {
    for (int i = 0; i < _m->m_parm.num_actions; i++) {
        if (i != hash) {
            for (int j = 0; j < _q.num_tilings; j++) {
                _q.traces[_features[i][j]] = 0.0f;
            }
        }
    }
    for (int i = 0; i < _q.num_tilings; i++)
        _q.traces[_features[hash][i]] = 1.0f;
}
float q_learn_instance::calc_reward() {
    float state_norm[2];
    float r = 0;
    for (int i = 0; i < 2; i++) {
        state_norm[i] =
            (_m->curr_s[0][i] - _goal_state[i]) / _q.reward_bandwidth[i];
        r += state_norm[i] * state_norm[i];
    }
    return -1 + 2*exp(-2 * r);
}

void q_learn_instance::calc_reward_parm() {
    _q.reward_bandwidth[0] =
        (_m->m_parm.state_limits[0][1] - _m->m_parm.state_limits[0][0]) / 2;
    _q.reward_bandwidth[1] =
        fabs(_m->curr_s[0][1] - _m->init_s[0][1]) * q_parm.max_overshoot;
}
void q_learn_instance::run_episode() {
    int action = 0;
    _m->init_set(_init_state, 0);
    _m->reset(0);
    clear_traces();
    gen_tiles();
    calc_q();
    action = qutils::find_max(_q_val, _m->m_parm.num_actions);

    int step = 0;
    float reward = -1;
    float delta = 0;
    float cum_reward = 0;

    int drop_flag = 0;

    while (step < q_parm.max_steps && drop_flag != 2) {
        drop_traces();
        update_traces(action);
        delta = -_q_val[action];
        _m->advance(action, 0);
        // cout<<action<<" "<<_m->curr_s[0][1]<<endl;
        gen_tiles();
        calc_q();
        action = qutils::find_max(_q_val, _m->m_parm.num_actions);
        // action = 0;
        // print_arr_1d(3, _q_val);
        // cout << " " << action << endl;
        // sleep(1);
        if (qutils::with_probability(q_parm.epsilon)) {
            action = rand() % _m->m_parm.num_actions;
        }

        delta += calc_reward();
        delta += q_parm.gamma * _q_val[action];
        delta *= q_parm.alpha;
        // cout<<endl<<"update: "<<delta<<endl;
        for (int i = 0; i < _q.memory_size; i++) {
            _q.weights[i] += delta * _q.traces[i];
        }
        calc_q(action);

        if (calc_reward() > -1) {
            //cum_reward += calc_reward();
            drop_flag =1;
            cout << "current state: " << _m->curr_s[0][0] << " "
                 << _m->curr_s[0][1] << " reward: " << calc_reward() << endl;
        }

        if (drop_flag == 1 && calc_reward() < -0.99999999)
            drop_flag = 2;
        step++;
    }
    cout << step << endl;
}

void q_learn_instance::gen_tiles() {
    set_net_input();
    for (int i = 0; i < _m->m_parm.num_actions; i++) {
        get_tiles1(_features[i], _q.num_tilings, _net_input, _q.num_vars,
                   _q.memory_size, i);
    }
}

void q_learn_instance::train(float* init_state, float* goal_state) {
    set_goal(goal_state);
    set_init(init_state);
    calc_reward_parm();
    report();
    set_net_input();
    gen_tiles();
    run_episode();
}

int main() {
    model m;
    q_learn_instance q(&m);
    float init_state[] = {0, -0.5};
    float goal_state[] = {0, 0.5};

    // q.report();
    for (int i = 0; i < 500; i++) q.train(init_state, goal_state);
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
