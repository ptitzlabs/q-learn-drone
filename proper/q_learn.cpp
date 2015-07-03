#include "q_learn.h"

q_learn::q_learn(cmac_net* net, model* m, int n_action_levels,
                 float* action_levels, float* goal, int n_goal_states,
                 int* goal_state_index, float epsilon, float gamma,
                 int max_steps) {
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
    for (int i = 0; i < _size_q; i++) _q[i] = 0.0;
    _n_action_levels = n_action_levels;
    _action_levels = action_levels;
    calc_q();
    _action = find_max();
}

q_learn::~q_learn() {
    delete[] _q;
    _q = NULL;
    // delete[] _action_levels;
    //_action_levels = NULL;
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
void q_learn::calc_q() {
    for (int i = 0; i < _size_q; i++) {
        _net->return_value(_q[i], i);
    }
}
void q_learn::calc_q(int hash) { _net->return_value(_q[hash], hash); }

bool q_learn::with_probability(float p) {
    return p > ((float)rand()) / RAND_MAX;
}

bool q_learn::goal_reached() {
    for (int i = 0; i < _n_goal_states; i++) {
        if (goal_reached(_goal_state_index[i]) == false) return false;
    }
    return true;
}
bool q_learn::goal_reached(int n) {
    if (_m->get_state(n) > _curr_goal[n])
        return true;
    else
        return false;
}

void q_learn::set_goal(float* goal) { _curr_goal = goal; }

void q_learn::run_episode() {
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
    printf("Steps: %i\n", step);
}

void q_learn::run_step() {
    _net->drop_traces();
    _net->update_traces(_action);
    _m->model_step(&_action_levels[_action]);
    print_arr_1d(2,_m->get_state());
    std::cout<<std::endl;
    float reward = -1;
    float delta = reward - _q[_action];
    _net->generate_tiles(_m->get_state());
    calc_q();
    _action = find_max();
    if (with_probability(_epsilon)) {
        _action = rand() % _size_q;
    }
    if (!goal_reached()) delta += _q[_action];
    calc_q(_action);
}

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
    std::cout<<"\nGoal states: ";
    for (int i = 0; i < _n_goal_states; i++)
        std::cout << _curr_goal[_goal_state_index[i]] << "  ";

    _m->report();
    _net->report();
    // printf("Number of inputs:  %i\n", _num_inputs);
    // printf("Tile dimension: ");
    // for (int i = 0; i < _num_inputs; i++)
    // printf("%i: (%f) ",i, _tile_dimension[i]);
    // printf("\n");
    // printf("Number of tilings:  %i\n", _num_tilings);
    // printf("Memory size:       %i\n", _memory_size);
    // printf("alpha:              %.2f\n", _alpha * _num_tilings);
}
