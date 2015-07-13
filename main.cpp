#include <SDL/SDL.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "src/RL_headers.h"
#include "src/joystick.h"
#include "src/glhelper.h"

using namespace std;
cJoystick js;

void joystick_listen() {
    cJoystick js;
    joystick_position jp[2];

    bool active = true;

    while (active) {
        jp[0] = js.joystickPosition(0);
        jp[1] = js.joystickPosition(1);

        cout << "jp1: " << jp[0].x << " " << jp[0].y << " jp2: " << jp[1].x
             << " " << jp[1].y << endl;
        sleep(0.01);
        if (js.buttonPressed(0)) active = false;
    }

}


int main(int argc, char *argv[]) {
    cout << "Hello!\n";


    return 0;
}
