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
#include "src/cmac_net.h"
#include "src/q_learn.h"
#include <thread>

using namespace std;
// cJoystick js;
//
const int k_window_width = 640;
const int k_window_height = 481;
const char* k_window_title = "Mountain Car";
int tmp_flag = 0;
void control_learn(float* model_state, cmac_net* net, cJoystick* js) {
    cout << "Initializing learning..." << endl;
    // Creating a model instance
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

    for (int i = 0; i < 2; i++) {
        delete[] state_limits[i];
        state_limits[i] = NULL;
    }
    delete[] state_limits;
    state_limits = NULL;
    // Creating a q-learning algorithm instance
    float action_levels[] = {0, 1, 2};
    float goal[] = {0, 0.5};
    int n_action_levels = 3;
    int n_goal_states = 1;
    int goal_state_index[] = {1};

    q_learn q(&*net, &m, n_action_levels, action_levels, goal, n_goal_states,
              goal_state_index);

    net->clear_weights();
    float goal_state[2] = {0, 0.5};

    for (int i = 0; i < 100; i++) {
    }

    bool active = true;
    while (active) {
        // init_state[0] = model_state[0];
        // init_state[1] = model_state[1];
        q.run_episode(model_state, goal_state);
        int index_chk = 0;
        // float m_chk[] = {0, -0.5};
        float m_chk[] = {model_state[0], model_state[1]};
        gen_input_index_max_q(
            &index_chk, 0, m_chk, 0, net->get_tile_sub_dimension(), 2, 0, 3,
            net->get_weights(), net->get_memory_size(), net->get_num_tilings());
        cout << " " << index_chk << " ";
        if (tmp_flag == 0) tmp_flag = 1;

        // int tiles[10];
        // float var_tmp[] = {0,-0.5,0.5};
        // get_tiles1(tiles,10,var_tmp,3,160000,0);

        // for (int i = 0; i< 10; i++)
        // cout<<net->get_weights()[tiles[i]]<<" ";

        // cout<<endl;

        // for (int i = 0; i < 1000; i++) {
        // cout << net->get_weights()[i] << endl;
        //}
        // cout<<"#############"<<endl;
        // sleep(1);

        if (js->buttonPressed(0)) {
            active = false;
        }
    }
}

void sdl_gfx(float* model_state, cJoystick* js) {
    cout << "Initializing visuals..." << endl;
    sdl_gfx_screen screen;
    sleep(1);
    screen.objects_init();
    while (screen.sdl_status()) {
        screen.poll_event();
        screen.fill_screen();
        screen.update_car_rt(model_state[1]);
        screen.update_target_flag(js->joystickPosition(1).x);
        screen.objects_draw();
        screen.refresh_screen();
        usleep(30000);
        if (js->buttonPressed(0)) {
            screen.sdl_quit();
        }
    }
}
//#define MANUAL_MODE
#define AUTO_MODE
void model_rt_loop(float* model_state, cmac_net* net, cJoystick* js) {
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
    float in;
    int input_index;
    float input_levels[] = {0, 1, 2};

    float* state_tmp = new float[2];
    joystick_position jp[2];
    float goal[] = {0, 0.5};
    while (active) {
        m.get_state(model_state);
#ifdef AUTO_MODE
        int index_chk = 0;
        float kk;
        float m_chk[] = {model_state[0], model_state[1]};

        gen_input_index_max_q(
            &index_chk, 0, m_chk, 0, net->get_tile_sub_dimension(), 2, 0, 3,
            net->get_weights(), net->get_memory_size(), net->get_num_tilings());
        kk = (float)index_chk;
        if (tmp_flag == 1) {
            // float m_chk[] = {0, -0.5};

            cout << " " << index_chk << " " << net->id << endl;

            cout << kk << endl;
            tmp_flag = 0;
        }

        input_index=0;
        gen_input_index_max_q(
            &input_index, 0, model_state, 0, net->get_tile_sub_dimension(), 2, 0, 3,
            net->get_weights(), net->get_memory_size(), net->get_num_tilings());
        //gen_input_index_max_q(&input_index, 0, m_chk, 0,
                              //net->get_tile_sub_dimension(), 2,
                              //0, 3, net->get_weights(), net->get_memory_size(),
                              //net->get_num_tilings());
        cout << "input_index "<< input_index << " index_chk " << index_chk <<  " get_num_states() " << m.get_num_states() <<  endl;
        // cout << net->get_weights()[100] << endl;
        //

        m.model_step(&kk);

#endif
#ifdef MANUAL_MODE
        in = js->joystickPosition(0).x + 1;
        m.model_step(&in);
#endif

        usleep(20000);
        if (js->buttonPressed(1)) {
            m.reset();
        }

        if (js->buttonPressed(0)) {
            active = false;
        }
    }
    for (int i = 0; i > 2; i++) delete[] state_limits[i];
    delete[] state_limits;
    delete[] state_tmp;
}

int main(int argc, char* argv[]) {
    cout << "Starting program..." << endl;

    cJoystick js;

    float* tile_dimension = new float[3];

    tile_dimension[0] = 0.14;
    tile_dimension[1] = 1.7;
    tile_dimension[2] = 1.7;

    cmac_net net(3, tile_dimension, 8, 160000);

    sleep(1);

    joystick_position jp[2];

    float* model_state = new float[2];

    thread model_sim_thread(model_rt_loop, model_state, &net, &js);
    sleep(0.5);
    thread sdl_thread(sdl_gfx, model_state, &js);

    thread learning_thread(control_learn, model_state, &net, &js);

    model_sim_thread.join();
    learning_thread.join();

    sdl_thread.join();
    delete[] model_state;
    delete[] tile_dimension;

    return 0;
}
