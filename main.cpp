
#include <iostream>
#include <thread>
#include "src/drone_dynamics.hpp"
#include "src/policy_search.hpp"
#include "src/console_color.h"
#include "src/control_threads.hpp"
#include "src/pipe_handler.hpp"
#include "src/message_handler.hpp"
#include "src/control_threads.hpp"

int main() {
    //Initializing data pipe
    //std::cout<<"status line goes here"<<std::endl;
    std::cout<<std::endl;
    data_pipe p;

    model_thread m(&p);
    simulation_thread sim(&p);
    training_thread train(&p);

    return 0;
}
