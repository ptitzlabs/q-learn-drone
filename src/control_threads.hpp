#ifndef _CONTROL_THREADS_HPP_
#define _CONTROL_THREADS_HPP_
#include "pipe_handler.hpp"
#include "drone_dynamics.hpp"
#include "policy_search.hpp"

class model_thread {
   public:
    model_thread(data_pipe* p);

   private:
    drone_parm drone;
    data_pipe * _p;

};

class simulation_thread {
   public:
    simulation_thread(data_pipe* p);

    void run_sim();

   private:
     double _timestep;
    drone_parm *  _drone;
    data_pipe * _p;


};

class interface_thread {
   public:
    interface_thread(data_pipe* p);

   private:
    double _j_axis;
};

class training_thread {
   public:
    training_thread(data_pipe* p);

   private:
    data_pipe* _p;
    
    drone_parm * sim_parm;
    policy_parm alt_rate_parm;
    policy_parm pitch_rate_parm;
    policy_parm roll_rate_parm;
    policy_parm yaw_rate_parm;

    policy alt_rate_control;
    policy pitch_rate_control;
    policy roll_rate_control;
    policy yaw_rate_control;

};

#endif
