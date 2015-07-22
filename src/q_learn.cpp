#include "q_learn.h"

q_learn::q_learn(cmac_net* net, model* m, int n_action_levels,
                 float* action_levels, float* goal, int n_goal_states,
                 int* goal_state_index, float epsilon, float gamma,
                 int max_steps, float max_overshoot) {
    _net = net;
    _m = m;
    _epsilon = epsilon;
    _gamma = gamma;
    _max_steps = max_steps;
    _size_q = _net->get_num_hashings();
    _curr_goal = goal;
    _goal_state_index = goal_state_index;
    _n_goal_states = n_goal_states;
    _q = new float[_size_q];
    _max_overshoot = max_overshoot;
    for (int i = 0; i < _size_q; i++) _q[i] = 0.0f;
    _n_action_levels = n_action_levels;
    _action_levels = action_levels;
    calc_q();
    _action = find_max();
    net->id = 999;
    //_myfile.open ("example.txt");
    //_myfile << "Writing this to a file.\n";
}

q_learn::~q_learn() {
    delete[] _q;
    _q = NULL;
    // delete[] _action_levels;
    //_action_levels = NULL;
}

int q_learn::find_max() {
    // std::cout<<"Finding max:\n";
    int max_index = 0;
    float max_val = _q[0];
    int num_ties = 1;
    for (int i = 1; i < _size_q; i++) {
        // std::cout<<"loop i\n";
        if (_q[i] >= max_val) {
            // std::cout<<"cond 1\n";
            if (_q[i] > max_val) {
                // std::cout<<"cond 2\n";
                max_val = _q[i];
                max_index = i;
            } else {
                // std::cout<<"cond else\n";
                num_ties++;
                if (rand() % num_ties == 0) {
                    // std::cout<<"cond 3\n";
                    max_val = _q[i];
                    max_index = i;
                }
            }
        }
    }
    return max_index;
}
void q_learn::calc_q() {
    for (int i = 0; i < _size_q; i++) {
        _net->return_value(&_q[i], i);
    }
}
void q_learn::calc_q(int hash) { _net->return_value(&_q[hash], hash); }

bool q_learn::with_probability(float p) {
    return p > ((float)rand()) / RAND_MAX;
}

#define GOAL_TESTING
bool q_learn::goal_reached() {
    //#ifdef GOAL_TESTING
    if (_m->get_init_state(1) < _curr_goal[1]) {
        if (_m->get_state(1) > _curr_goal[1]) {
            std::cout << "positive_goal" << std::endl;
            return true;
        }

    } else {
        if (_m->get_state(1) < _curr_goal[1]) {
            std::cout << "negative_goal" << std::endl;
            return true;
        }
    }
    // if (_m->get_init_state(1) < _curr_goal[1]) {
    // if (_m->get_state(1) > _curr_goal[1] &&
    //(_m->get_state(1) - _curr_goal[1]) /
    //(_m->get_init_state(1) - _curr_goal[1]) <
    //_max_overshoot &&
    //_m->get_state(0) < 0) {
    // std::cout << "positive_goal" << std::endl;
    // return true;
    //}

    //} else {
    // if (_m->get_state(1) < _curr_goal[1] &&
    //(_m->get_state(1) - _curr_goal[1]) /
    //(_m->get_init_state(1) - _curr_goal[1]) <
    //_max_overshoot &&
    //_m->get_state(0) > 0) {
    // std::cout << "negative_goal" << std::endl;
    // return true;
    //}
    //}

    //#else
    // if (_m->get_state(1) > _curr_goal[1] &&
    //_m->get_state(1) < _curr_goal[1] + 0.1 &&
    // std::abs(_m->get_state(0)) < 0.01) {
    // std::cout << "velocity state" << abs(_m->get_state(0)) << std::endl;
    // return true;
    //}

    // for (int i = 0; i < _n_goal_states; i++) {
    // if (goal_reached(_goal_state_index[i]) == false) return false;
    //}
    //#endif

    // std::cout << "goal reached" << std::endl;
    return false;
}
bool q_learn::goal_reached(int n) {
    if (_m->get_state(n) > _curr_goal[n])
        return true;
    else
        return false;
}

void q_learn::set_goal(float* goal) { _curr_goal = goal; }

void q_learn::run_episode() {
    _m->reset();
    _net->clear_traces();
    _net->generate_tiles(_m->get_state());

    calc_q();

    _action = find_max();

    if (with_probability(_epsilon)) {
        _action = rand() % _size_q;
    }

    int step = 0;

    while (!goal_reached() && step < _max_steps) {
        step++;
        run_step();
    }
    printf("%i\n", step);
}
// Learning with target and init state as a factor
void q_learn::run_episode(float* init_state, float* goal_state) {
    float model_init_state[2] = {init_state[0], init_state[1]};
    model_init_state[0] = -0.07 + 0.14 * (float)(rand() % 10) / 10;
    model_init_state[1] = -1.2 + 1.8 * (float)(rand() % 10) / 10;

    //_m->set_state(model_init_state);
    // std::cout << "cur_goal: " << goal_state[1] << " input_init_state "
    //<< init_state[1] << " model_init_state: " << _m->get_state(1);
    // model_init_state[0] = 0;
    // model_init_state[1] = -0.5;
    model_init_state[0] = init_state[0];
    model_init_state[1] = init_state[1];
    //goal_state[1] = 0.5;
    set_goal(goal_state);
    _m->set_init_state(model_init_state);
    _m->reset();

    _net->clear_traces();

    float net_input[3] = {_m->get_state(0), _m->get_state(1), goal_state[1]};
    _net->generate_tiles(net_input);

    calc_q();

    _action = find_max();

    if (with_probability(_epsilon)) {
        _action = rand() % _size_q;
    }

    int step = 0;

    int counter = 0;

    //while (!goal_reached() && step < _max_steps) {
    while (counter < 100 && step < _max_steps){
        if (calc_reward(goal_state,_m->get_state(),2) > -0.2) counter++;
        step++;
        run_step();
    }
    std::cout << "steps: " << step << " goal reached: " << goal_reached()
              << std::endl;

    // std::cout << " state 0 " << _m->get_state(0) << std::endl << "step " <<
    // step
    //<< std::endl;
    ///////////////////////////////////////////////////////
    //_m->reset();
    //_m->set_state(init_state);
    // set_goal(goal_state);

    //_net->clear_traces();

    //// float net_input[3] = {_m->get_state(0), _m->get_state(1),
    /// goal_state[1]};
    // net_input[0] = 0;
    // net_input[1] = -0.5;
    // net_input[2] = 0.5;
    //_net->generate_tiles(net_input);

    // calc_q();

    //_action = find_max();
    // int tiles[10];
    // float var_tmp[] = {0, -0.5, 0.5};
    // get_tiles1(tiles, 10, var_tmp, 3, 160000, 0);
    // float* weights = _net->get_weights();

    // for (int i = 0; i < 10; i++) std::cout << weights[tiles[i]] << " ";
    // int index_chk = 0;
    // float m_chk[] = {0, -0.5};
    // gen_input_index_max_q(&index_chk, 0, m_chk, 0,
    //_net->get_tile_sub_dimension(), _m->get_num_states(),
    // 0, _n_action_levels, _net->get_weights(),
    //_net->get_memory_size(), _net->get_num_tilings());

    // std::cout << " ::: " << index_chk << " " << _action;
    ////////////////////////////////////////////////////////
    // printf("%i\n", step);
}

float q_learn::calc_reward(float* goal, float* curr_state, int num_states) {
    // Using Gaussian function to calculate potential-based reward
    float sum = 0;

    for (int i = 0; i < num_states; i++) {
        sum += pow(
            (goal[i] - curr_state[i]) / _net->get_tile_sub_dimension()[i], 2);
    }

    return -1 + exp(-sum);
}

void q_learn::run_step() {
    _net->drop_traces();
    _net->update_traces(_action);
    _m->model_step(&_action_levels[_action]);
    float reward;
    // if (!goal_reached()) {
    // reward = -1;
    //} else {
    // reward = 0;
    //}
    float delta = calc_reward(_curr_goal, _m->get_state(), 2) - _q[_action];
    float net_input[3] = {_m->get_state(0), _m->get_state(1), _curr_goal[1]};
    _net->generate_tiles(net_input);
    calc_q();

    _action = find_max();
    // int index_chk = 0;
    // gen_input_index_max_q(&index_chk, 0, _m->get_state(), 0,
    //_net->get_tile_sub_dimension(), _m->get_num_states(),
    // 0, _n_action_levels, _net->get_weights(),
    //_net->get_memory_size(), _net->get_num_tilings());

    // std::cout<<"action check: "<<_action <<" " <<index_chk<<std::endl;
    if (with_probability(_epsilon)) {
        _action = rand() % _size_q;
    }
    //if (!goal_reached()) delta += _gamma * _q[_action];
    delta+= _gamma * _q[_action];
    _net->quick_update(delta);
    calc_q(_action);
}
// void q_learn::run_step() {
//_net->drop_traces();
//_net->update_traces(_action);
//_m->model_step(&_action_levels[_action]);
// float reward = -1;
// float delta = reward - _q[_action];
//_net->generate_tiles(_m->get_state());
// calc_q();

//_action = find_max();
// int index_chk = 0;
// gen_input_index_max_q(&index_chk, 0, _m->get_state(), 0,
//_net->get_tile_sub_dimension(), _m->get_num_states(),
// 0, _n_action_levels, _net->get_weights(),
//_net->get_memory_size(), _net->get_num_tilings());
// if (with_probability(_epsilon)) {
//_action = rand() % _size_q;
//}
// if (!goal_reached()) delta += _gamma * _q[_action];
//_net->quick_update(delta);
// calc_q(_action);
//}

void q_learn::report() {
    printf("\n##############\n");
    printf("Q-learn report:\n");
    std::cout << "Max steps: " << _max_steps << std::endl << "gamma: " << _gamma
              << "\nepsilon: " << _epsilon
              << "\nSize of Q (number of actions): " << _size_q
              << "\nCurrent Q: ";
    for (int i = 0; i < _size_q; i++) std::cout << _q[i] << "  ";

    std::cout << "\nCurrent action:" << _action
              << "\nNumber of action values: " << _n_action_levels
              << "\nAction values:";
    for (int i = 0; i < _n_action_levels; i++)
        std::cout << _action_levels[i] << "  ";
    std::cout << "\nNumber of goal states: " << _n_goal_states
              << "\nGoal state inices: ";
    for (int i = 0; i < _n_goal_states; i++)
        std::cout << _goal_state_index[i] << "  ";
    std::cout << "\nGoal states: ";
    for (int i = 0; i < _n_goal_states; i++)
        std::cout << _curr_goal[_goal_state_index[i]] << "  ";

    _m->report();
    _net->report();
}
void q_learn::write_contour(char* filename, int id, int n, int m) {
    char buffer[256];
    sprintf(buffer, "%s%i", filename, id);

    float* xx = new float[n];
    float* yy = new float[m];
    float** zz = new float* [m];
    for (int i = 0; i < m; i++) zz[i] = new float[n];

    float** limits = _m->get_state_limits();

    float x_step = (limits[0][1] - limits[0][0]) / float(n - 1);
    float y_step = (limits[1][1] - limits[1][0]) / float(n - 1);
    std::cout << "xstep: " << x_step << " ystep: " << y_step
              << " limits: " << limits[0][0] << " " << limits[0][1] << "; "
              << " " << limits[1][0] << " " << limits[1][1] << std::endl;

    for (int i = 0; i < n; i++) {
        xx[i] = limits[0][0] + x_step * (float)i;
        std::cout << xx[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < m; i++) {
        yy[i] = limits[1][0] + y_step * (float)i;
        std::cout << yy[i] << " ";
    }
    float in_tmp[2];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            in_tmp[0] = xx[i];
            in_tmp[1] = yy[j];

            _net->generate_tiles(in_tmp);
            _net->return_value(&zz[j][i], id);
        }
    }

    save_arr_2d(m, n, yy, xx, zz, buffer);
}
