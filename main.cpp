#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "src/RL_headers.h"
#include "src/joystick.h"
#include "src/glhelper.h"
#include "src/sdl_gfx_screen.h"
#include "src/model.h"
#include <thread>

using namespace std;
// cJoystick js;
//
const int k_window_width = 640;
const int k_window_height = 480;
const char* k_window_title = "Mountain Car";

void joystick_listen(joystick_position jp[2], bool joystick_buttons[5]) {
    cout << "Initializing joystick..."<< endl;
    cJoystick js;
    sleep(1);
    bool active = true;
    while (active) {
        jp[0] = js.joystickPosition(0);
        jp[1] = js.joystickPosition(1);


        sleep(0.01);
        for (int i = 0; i<5; i++){
            joystick_buttons[i] = js.buttonPressed(i);
        }
        if (js.buttonPressed(0)){
            active = false;
        }
    }
}
void control_learn(joystick_position jp[2],bool joystick_buttons[5]) {
    cout << "Initializing learning..."<< endl;
    bool active = true;

    while (active) {
        //cout << "jp1: " << jp[0].x << " " << jp[0].y << " jp2: " << jp[1].x
             //<< " " << jp[1].y << endl;
        sleep(1);

        if (joystick_buttons[0] == true) {active = false;}
    }
}

void sdl_gfx(float car_loc, joystick_position jp[2], bool joystick_buttons[5]) {
    cout << "Initializing visuals..."<< endl;
    sdl_gfx_screen screen;
    sleep(1);
    //screen.sdl_quit();
    screen.objects_init();
    while (screen.sdl_status()) {
        cout<<"car_loc_sdl"<<car_loc<<endl;
        screen.poll_event();
        screen.fill_screen();
        screen.update_car_rt(car_loc);
        screen.objects_draw();
        screen.refresh_screen();
        sleep(1);
        if (joystick_buttons[0] == true) {screen.sdl_quit();}
    }
}

void model_rt_loop(float * model_state, joystick_position jp[2], bool joystick_buttons[5]){
    float dt = 0.1;

    float** state_limits;
    state_limits = new float* [2];
    state_limits[0] = new float[2];
    state_limits[1] = new float[2];

    state_limits[0][0] = -0.07;
    state_limits[0][1] = 0.07;
    state_limits[1][0] = -1.2;
    state_limits[1][1] = 0.6;

    float init_state[] = {0, -0.5};
    model m(2, state_limits, init_state, 1);

    bool active = true;
    float * in = new float;

    float * state_tmp = new float[2];
    while (active){
        *in = jp[0].x+1;
        m.model_step(in);
        state_tmp = m.get_state();
        model_state[1] = state_tmp[1];
        //cout<<jp[0].x<<" "<<in[0]<<" "<<model_state[1]<<endl;
        //sleep(1);
        if (joystick_buttons[0] == true) {active = false;}

    }
    delete[] in;
    delete[] state_tmp;

}

int main(int argc, char* argv[]) {
    cout << "Starting program..."<< endl;


    joystick_position jp[2];
    for (int i = 0; i<2; i++){
        jp[i].theta = 0;
        jp[i].r = 0;
        jp[i].x = 0;
        jp[i].y = 0;
    
    }

    bool joystick_buttons[5];
    float  model_rt_state[2];
    model_rt_state[0] = 0;
    model_rt_state[1] = -0.5;

    for (int i = 0; i<5; i++) joystick_buttons[i] = false;

    thread learning_thread(control_learn, jp, joystick_buttons);
    sleep(0.5);
    thread model_sim_thread(model_rt_loop, model_rt_state, jp, joystick_buttons);
    sleep(0.5);
    thread sdl_thread(sdl_gfx, model_rt_state[1],jp, joystick_buttons);
    sleep(0.5);
    thread joystick_thread(joystick_listen, jp,joystick_buttons);
    




    //int count = 0;
    //while(++count < 5) {
        //sleep(1);
    //}
    //joystick_buttons[0] = true;

    model_sim_thread.join();
    joystick_thread.join();
    learning_thread.join();

    sdl_thread.join();

    return 0;
}
