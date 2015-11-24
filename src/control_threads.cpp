#include "control_threads.hpp"

model_thread::model_thread(data_pipe * p){
    msg::status("Initializing model thread");
    _p = p;
    _p->drone = &drone;
}

simulation_thread::simulation_thread(data_pipe * p){
    msg::status("Initializing simulation thread");
    _p = p;
    _drone = _p->drone;
}

void simulation_thread::run_sim(){
}

training_thread::training_thread(data_pipe * p){
    msg::status("Initializing training thread");

    _p = p;
    ///////////////////////////////
    // Controller initialization //
    ///////////////////////////////

    // Altitude control
    alt_rate_parm.name = "alt_rate";
    alt_rate_parm.id = 0;
    alt_rate_parm.set_n_goal(1); // 1 goal state
    alt_rate_parm.set_n_state(3); // 3 monitored states

    // Goal states
    alt_rate_parm.id_goal[0] = 8; // zd

    // Monitored states

    alt_rate_parm.id_state[0] = 8; // zd
    alt_rate_parm.id_state[1] = 14; // zdd
    alt_rate_parm.id_state[2] = 18; // aux0

    // Control input parameters
    alt_rate_parm.id_input = 0; // U1

    // Learning parameters
    alt_rate_parm.max_steps = 10000;

    /////////////////////////////////
    // Initializing control policy //
    /////////////////////////////////

    // Altitude control
    alt_rate_control.set_model(_p->drone);
    alt_rate_control.set_parm(&alt_rate_parm);


    
}

