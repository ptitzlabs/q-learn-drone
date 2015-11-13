#include "policy_search.hpp"

control_policy::control_policy(){};
void control_policy::parm_init(
    cmac_net* net,          // network to store the policy
    drone_dynamics* drone,  // model instance to be used for policy search
    int control_input_id,
    double* action_levels,  // control levels values
    int n_action_levels,    // number of discrete control levels
    int* input_parm_id,     // ids of states supplied to the policy
    int n_input_parms,      // number of states supplied to the policy
    int* goal_state_id,     // goal state ids
    int n_goal_states,      // number of goal states
    double model_stepsize, double epsilon, double gamma, int max_steps,
    double max_overshoot) {
    _net = net;
    _drone = drone;
    _model_stepsize = model_stepsize, _control_input_id = control_input_id;
    _action_levels = action_levels;
    _n_action_levels = n_action_levels;
    _input_parm_id = input_parm_id;
    _n_input_parms = n_input_parms;
    _drone->get_state(&*_input_parms, &*_input_parm_id, n_input_parms);

    _epsilon = epsilon;
    _gamma = gamma;
    _max_steps = max_steps;
    _size_q = _net->get_num_hashings();
    _goal_state_id = goal_state_id;
    _n_goal_states = n_goal_states;
    _q = new double[_size_q];
    _max_overshoot = max_overshoot;
    for (int i = 0; i < _size_q; i++) _q[i] = 0.0f;
    calc_q();
    _action = find_max();
    net->id = 999;
    //_myfile.open ("example.txt");
    //_myfile << "Writing this to a file.\n";
}

void control_policy::calc_q() {
    for (int i = 0; i < _size_q; i++) {
        _net->return_value(&_q[i], i);
    }
}
int control_policy::find_max() {
    // std::cout<<"Finding max:\n";
    int max_index = 0;
    double max_val = _q[0];
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
control_policy::~control_policy() {
    delete[] _q;
    _q = NULL;
    // delete[] _action_levels;
    //_action_levels = NULL;
}

void control_policy::set_goal(double* goal) { _curr_goal = goal; }
void control_policy::run_episode() {
    _drone->reset();
    _net->clear_traces();
    _drone->get_state(&*_input_parms, &*_input_parm_id, _n_input_parms);
    _net->generate_tiles(_input_parms);

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
bool control_policy::with_probability(double p) {
    return p > ((double)rand()) / RAND_MAX;
}
void control_policy::run_step() {
    _net->drop_traces();
    _net->update_traces(_action);

    _drone->input_set(_action_levels[_control_input_id], _control_input_id);

    //_m->model_step(&_action_levels[_action]);
    _drone->rk4_step(_model_stepsize);
    double reward;
    _drone->get_state(_input_parms, _input_parm_id, _n_input_parms);
    _drone->get_state(_goal_parms, _goal_state_id, _n_goal_states);
    double delta = calc_reward() - _q[_action];
    double net_input[_n_input_parms];
    for (int i = 0; i < _n_input_parms; i++) net_input[i] = _input_parms[i];
    _net->generate_tiles(net_input);
    calc_q();

    _action = find_max();
    if (with_probability(_epsilon)) {
        _action = rand() % _size_q;
    }
    delta += _gamma * _q[_action];
    _net->quick_update(delta);
    calc_q(_action);
}

double control_policy::goal_dist() {
    double dist = 0;
    for (int i = 0; i < _n_goal_states; i++)
        dist += pow((_goal_parms[i] - _curr_goal[i]) /
                        _drone->get_scale(_goal_state_id[i]),
                    2);
    return dist;
}
bool control_policy::goal_reached() {
    if (goal_dist() < 0.1)
        return true;
    else
        return false;
}
void control_policy::calc_q(int hash) { _net->return_value(&_q[hash], hash); }
double control_policy::calc_reward() {
    return -goal_dist();
}

#ifdef POLICY_SEARCH_ON

int control_policy::find_max() {
    // std::cout<<"Finding max:\n";
    int max_index = 0;
    double max_val = _q[0];
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

#define GOAL_TESTING
bool control_policy::goal_reached() {
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
    return false;
}
bool control_policy::goal_reached(int n) {
    if (_m->get_state(n) > _curr_goal[n])
        return true;
    else
        return false;
}

void control_policy::set_goal(double* goal) { _curr_goal = goal; }

// Learning with target and init state as a factor
void control_policy::run_episode(double* init_state, double* goal_state) {
    double model_init_state[2] = {init_state[0], init_state[1]};
    model_init_state[0] = -0.07 + 0.14 * (double)(rand() % 10) / 10;
    model_init_state[1] = -1.2 + 1.8 * (double)(rand() % 10) / 10;

    //_m->set_state(model_init_state);
    // std::cout << "cur_goal: " << goal_state[1] << " input_init_state "
    //<< init_state[1] << " model_init_state: " << _m->get_state(1);
    // model_init_state[0] = 0;
    // model_init_state[1] = -0.5;
    model_init_state[0] = init_state[0];
    model_init_state[1] = init_state[1];
    // goal_state[1] = 0.5;
    set_goal(goal_state);
    _m->set_init_state(model_init_state);
    _m->reset();

    _net->clear_traces();

    double net_input[3] = {_m->get_state(0), _m->get_state(1), goal_state[1]};
    _net->generate_tiles(net_input);

    calc_q();

    _action = find_max();

    if (with_probability(_epsilon)) {
        _action = rand() % _size_q;
    }

    int step = 0;

    int counter = 0;

    // while (!goal_reached() && step < _max_steps) {
    while (counter < 100 && step < _max_steps) {
        if (calc_reward(goal_state, _m->get_state(), 2) > -0.2) counter++;
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

    //// double net_input[3] = {_m->get_state(0), _m->get_state(1),
    /// goal_state[1]};
    // net_input[0] = 0;
    // net_input[1] = -0.5;
    // net_input[2] = 0.5;
    //_net->generate_tiles(net_input);

    // calc_q();

    //_action = find_max();
    // int tiles[10];
    // double var_tmp[] = {0, -0.5, 0.5};
    // get_tiles1(tiles, 10, var_tmp, 3, 160000, 0);
    // double* weights = _net->get_weights();

    // for (int i = 0; i < 10; i++) std::cout << weights[tiles[i]] << " ";
    // int index_chk = 0;
    // double m_chk[] = {0, -0.5};
    // gen_input_index_max_q(&index_chk, 0, m_chk, 0,
    //_net->get_tile_sub_dimension(), _m->get_num_states(),
    // 0, _n_action_levels, _net->get_weights(),
    //_net->get_memory_size(), _net->get_num_tilings());

    // std::cout << " ::: " << index_chk << " " << _action;
    ////////////////////////////////////////////////////////
    // printf("%i\n", step);
}

double control_policy::calc_reward(double* goal, double* curr_state,
                                   int num_states) {
    // Using Gaussian function to calculate potential-based reward
    double sum = 0;

    for (int i = 0; i < num_states; i++) {
        sum += pow(
            (goal[i] - curr_state[i]) / _net->get_tile_sub_dimension()[i], 2);
    }

    return -1 + exp(-sum);
}

// void control_policy::run_step() {
//_net->drop_traces();
//_net->update_traces(_action);
//_m->model_step(&_action_levels[_action]);
// double reward = -1;
// double delta = reward - _q[_action];
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

void control_policy::report() {
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
void control_policy::write_contour(char* filename, int id, int n, int m) {
    char buffer[256];
    sprintf(buffer, "%s%i", filename, id);

    double* xx = new double[n];
    double* yy = new double[m];
    double** zz = new double* [m];
    for (int i = 0; i < m; i++) zz[i] = new double[n];

    double** limits = _m->get_state_limits();

    double x_step = (limits[0][1] - limits[0][0]) / double(n - 1);
    double y_step = (limits[1][1] - limits[1][0]) / double(n - 1);
    std::cout << "xstep: " << x_step << " ystep: " << y_step
              << " limits: " << limits[0][0] << " " << limits[0][1] << "; "
              << " " << limits[1][0] << " " << limits[1][1] << std::endl;

    for (int i = 0; i < n; i++) {
        xx[i] = limits[0][0] + x_step * (double)i;
        std::cout << xx[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < m; i++) {
        yy[i] = limits[1][0] + y_step * (double)i;
        std::cout << yy[i] << " ";
    }
    double in_tmp[2];

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

#endif
