#include <iostream>
#include <thread>
void model_id(){
    std::cout<<"Hello1"<<std::endl;
}
void plant_sim(){
    std::cout<<"Hello2"<<std::endl;
}

int main(){
    std::thread model_id_thread(model_id);
    std::thread plant_sim_thread(plant_sim);

    model_id_thread.join();
    plant_sim_thread.join();
    return 0;

}
