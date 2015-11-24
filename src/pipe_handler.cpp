#include "pipe_handler.hpp"

data_pipe::data_pipe(){
    msg::status("Initializing data pipe");

    //for (int i = 0; i < 10000; i++){
        //char status[10];
        //sprintf(status,"%i\n%i",i,i+10);
        //msg::begin_text();
        //std::cout<<"A\nB\nC\n"<<i<<std::endl;
        //msg::end_text();
        //msg::sticky_refresh("abc");
        //sleep(1);
    //}

}
data_pipe::~data_pipe(){
    msg::status("Closing data pipe");
}
