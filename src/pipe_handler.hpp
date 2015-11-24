#ifndef _PIPE_HANDLER_HPP_
#define _PIPE_HANDLER_HPP_

#include "drone_dynamics.hpp"
#include "message_handler.hpp"

class data_pipe{
    public:
        data_pipe();
        ~data_pipe();

        drone_parm * drone;
    private:
        float** _joystick_axis;
        float** _vehicle_state;
        float * _vehicle_input; 

};


#endif
