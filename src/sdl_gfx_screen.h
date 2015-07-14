#ifndef _SDL_GFX_SCREEN_H_
#define _SDL_GFX_SCREEN_H_

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <iostream>
#include <stdlib.h>
#include "RL_headers.h"
#include <vector>
#include <cmath>
#include "qutils.h"

struct line_object {
    int num_vertices;
    float* x;
    float* y;
    float x_pos;
    float y_pos;
    float rotation;
    float scale_x;
    float scale_y;
    int *color;
};
class sdl_gfx_screen {
    public:
        sdl_gfx_screen(int window_width = 640,int window_height = 480, char* title = (char*)"Mountain Car", int alloc_objects = 10);
        ~sdl_gfx_screen();
        void refresh_screen();
        void get_screen();
        void fill_screen();
        void poll_event();
        void draw_line(int n, float* x, float * y, int *color);
        void draw_shape(line_object obj);
        void draw_mointain();
        void objects_init();
        void objects_draw();
        void add_shape(int n,float* x, float* y,int* color,float scale_x = 1.0f, float scale_y = 1.0f);
        void object_scale(line_object obj, float scale_x, float scale_y);
        void object_move(line_object obj, float x_pos, float y_pos);
        void update_car_rt(float loc);
        void sdl_quit();

        bool sdl_status();
        

    private:
        SDL_Surface * _screen;
        SDL_Event _event;
        bool _sdl_running;
        char* _window_title;
        std::vector<line_object> _shapes;   
};

#endif
