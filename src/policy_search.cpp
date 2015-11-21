#include "policy_search.hpp"

policy_parm::policy_parm()
    : max_steps(10000),     // maximum number of steps
      memory_size(3000),    // cmac memory size
      n_tilings(10),        // number of tilings
      tile_resolution(8),   // sub-tilings per tile
      alpha(0.5),           // cmac learning update parameter
      gamma(1),             // discount-rate parameter
      lambda(0.9),          // trace-decay parameter
      id_state(0),          // null-pointer
      id_goal(0),           // null-pointer
      goal_thres(0.1),      // goal threshold
      epsilon(0),           // random action probability
      n_action_levels(5) {  // number of action levels
    action_levels = new double[n_action_levels];
    for (int i = 0; i < 5; i++) action_levels[i] = -1 + i * 0.5;
}
policy_parm::~policy_parm() {
    delete[] action_levels;
    delete[] id_state;
    delete[] id_goal;
}

void policy_parm::set_n_goal(int n) {
    n_goal = n;
    id_goal = new int[n];
}
void policy_parm::set_n_state(int n) {
    n_state = n;
    id_state = new int[n];
}

policy::policy()
    : m(0),  // NULL pointer for model
      p(0),  // NULL pointer for policy parameters
      n(0)   // NULL pointer for CMAC
{}

policy::~policy() {
    delete m;  // clear up the model
    delete n;  // clear up the cmac
    delete[] _q;
    delete[] _cmac_input;
}

void policy::set_parm(policy_parm* policy_parm) {
    std::cout<<BOLDYELLOW<<"SETTING POLICY PARAMETERS "<<RESET<<std::endl;
    p = &*policy_parm;
    // Initializing a CMAC net to store the policy
    // CMAC tile dimensions to accomodate monitored states
    p->n_cmac_parms =
        p->n_state + p->n_goal;  // add up the monitored states and goals
    double tile_dimension[p->n_cmac_parms];  // init tile dimensions

    //for (int i = 0; i < 19; i++) {
        //std::cout << "state: " << i << " " << m->get_scale(i) << "\n";
    //}
    for (int i = 0; i < p->n_state; i++) {
        tile_dimension[i] =
            m->get_scale(p->id_state[i]);  // fill up the state dimensions
        //std::cout << tile_dimension[i];
    }

    for (int i = 0; i < p->n_goal; i++) {
        tile_dimension[p->n_state + i] =
            m->get_scale(p->id_goal[i]);  // fill up the goal dimensions
    }

    // Creating a new cmac instance and initializing the parameters
    n = new cmac_net;
    n->parm_init(
        p->n_cmac_parms,     // both state and goal are used as inputs
        tile_dimension,      // tile dimensions are the expected spread of
                             // values
        p->tile_resolution,  // tile resolution is the number of partitions
        p->memory_size,      // allocated memory size
        p->n_tilings,        // number of overlapping tiles
        p->n_action_levels,  // number of action levels
        p->alpha,            // update rate parameter alpha
        p->gamma,            // trace decay parameter gamma
        p->lambda);          // discount-rate parameter
    n->report();

    // Init cache variables
    _q = new double[p->n_action_levels];
    _cmac_input = new double[p->n_cmac_parms];
}
void policy::set_model(drone_parm * sim_parm) {
    std::cout<<BOLDYELLOW<<"SETTING MODEL"<<RESET<<std::endl;
    m = new drone_dynamics(sim_parm);

    fun_test(0,1);
    //fun_test(1,1);
    //fun_test(2,1);
    //fun_test(3,1);

    m->report();
}

void policy::fun_test(int n, int o){
    m->set_input(n,o);
    m->rk4_step();
}
void policy::set_goal(double* goal) { _curr_goal = goal; }
void policy::set_init(double* init) {
    for (int i = 0; i < p->n_state; i++) {
        m->set_init_parm(p->id_state[i], init[i]);
    }
    m->reset();
}

void policy::set_state_parm(int id[], int n) {
    p->id_state = new int[n];
    p->n_state = n;
    for (int i = 0; i++; i < n) p->id_state[i] = id[i];
}
void policy::set_goal_parm(int id[], int n) {
    p->id_goal = new int[n];
    p->n_goal = n;
    for (int i = 0; i++; i < n) p->id_goal[i] = id[i];
}

bool policy::with_probability(double p) {
    return p > ((float)rand()) / RAND_MAX;
}

bool policy::goal_reached() {
    double goal_dist = 0;  // normalized goal distance
    int id;
    //std::cout<<"n_goal: "<<p->n_goal<<std::endl;
    for (int i = 0; i < p->n_goal; i++) {
        id = p->id_state[i];  // pick up the state id
        //std::cout<<"goal "<<id<<": ";
        //goal_dist = (m->get_state(id) - _curr_goal[i]) / m->get_scale(id);
        //std::cout<<pow((m->get_state(id) - _curr_goal[i]) / m->get_scale(id),2)<<std::endl;
        //std::cout<<goal_dist;
    }
    //return sqrt(goal_dist) < p->goal_thres;  // check with the goal threshold
    return false;
}

void policy::calc_q() {
    for (int i = 0; i < p->n_action_levels; i++) {
        n->return_value(&_q[i], i);
    }
}
void policy::calc_q(int hash) { n->return_value(&_q[hash], hash); }
int policy::calc_action() {
    int index = 0;
    float max_val = _q[0];
    int num_ties = 1;
    for (int i = 1; i < p->n_action_levels; i++) {
        if (_q[i] >= max_val) {
            if (_q[i] > max_val) {  // find max Q for current action
                max_val = _q[i];
                index = i;
            } else {
                num_ties++;
                if (rand() % num_ties == 0) {  // randomly break ties
                    max_val = _q[i];
                    index = i;
                }
            }
        }
    }
    if (with_probability(p->epsilon)) {  // or take a random guess
        index = rand() % p->n_action_levels;
    }
    return index;
}

void policy::calc_cmac_input() {
    // Supply current state as CMAC input
    for (int i = 0; i < p->n_state; i++) {
        _cmac_input[i] = m->get_state(p->id_state[i]);
    }
    // Supply goal state as CMAC input
    for (int i = 0; i < p->n_goal; i++) {
        _cmac_input[p->n_state + i] = _curr_goal[i];
    }
}

void policy::run_episode() {
    std::cout<<YELLOW<<"Running episode: "<<RESET<<std::endl;
    m->reset();                      // reset model
    n->clear_traces();               // clear traces
    calc_cmac_input();               // update CMAC input
    n->generate_tiles(_cmac_input);  // generate tiles
    calc_q();                        // calculate Q-values
    _action = calc_action();         // find optimal action

    int step = 0;  // initial step number
    //while (!goal_reached() && step < p->max_steps) {
    while (step < p->max_steps) {
        //m->rk4_step();
        //m->report();

        //m->set_input(0,0);
        m->report();
        printf("steponit\n");
        m->rk4_step();
        //m->report();
        //run_step();
        //m->report();
        step++;
    }

    std::cout<<step<<std::endl;
}

void policy::run_step() {
    std::cout<<YELLOW<<"Running step: "<<RESET<<std::endl;
    n->drop_traces();           // drop traces
    n->update_traces(_action);  // update traces for the current action
    std::cout<< "\nACTION: "<<_action<<" INPUT: "<< p->action_levels[_action];
    m->set_input(
        p->id_input,                 // specify model input
        p->action_levels[_action]);  // set model input to current action
    m->report();
    m->rk4_step();                   // execute model step

    double reward = -1;                   // calculate new reward
    double delta = reward - _q[_action];  // substract old Q(k) value
    calc_cmac_input();  // update CMAC input with the new model state
    n->generate_tiles(_cmac_input);  // generate new CMAC tiles
    calc_q();                        // calculate new Q(k+1) values
    _action = calc_action();         // calculate the action for the next step
    if (!goal_reached()) {
        delta += p->gamma * _q[_action];  // calc the change in Q(k+1)
    }
    n->quick_update(delta);  // update Q(k+1) in the CMAC
    calc_q(_action);         // calculate updated Q(k+1)
}

void policy::report() {
    std::cout << "\n=====================" << std::endl;
    std::cout << "CONTROLLER PARAMETERS" << std::endl;
    std::cout << "=====================" << std::endl;
    m->report();
    n->report();
}
