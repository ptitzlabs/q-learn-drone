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
#include <thread>

using namespace std;
// cJoystick js;
//
const int k_window_width = 640;
const int k_window_height = 480;
const char* k_window_title = "Mountain Car";

void joystick_listen(joystick_position jp[2]) {
    cJoystick js;
    bool active = true;

    while (active) {
        jp[0] = js.joystickPosition(0);
        jp[1] = js.joystickPosition(1);

        sleep(0.01);
        if (js.buttonPressed(0)) active = false;
    }
}
void control_learn(joystick_position jp[2]) {
    bool active = true;

    while (active) {
        cout << "jp1: " << jp[0].x << " " << jp[0].y << " jp2: " << jp[1].x
             << " " << jp[1].y << endl;
        sleep(1);
    }
}

void sdl_gfx() {
    sdl_gfx_screen screen;

    int n = 2;
    float x[n] = {320, 330};
    float y[n] = {240, 250};
    int color[4] = {255, 0, 0, 255};
    screen.objects_init();
    while (screen.sdl_status()) {
        screen.poll_event();
        screen.fill_screen();
        screen.draw_line(n, x, y, color);
        screen.refresh_screen();
    }
}

int main(int argc, char* argv[]) {
    cout << "Hello!\n";
    joystick_position jp[2];

    thread joystick_thread(joystick_listen, jp);
    thread learning_thread(control_learn, jp);
    thread sdl_thread(sdl_gfx);

    sdl_thread.join();
    joystick_thread.join();
    learning_thread.join();

    return 0;
}
