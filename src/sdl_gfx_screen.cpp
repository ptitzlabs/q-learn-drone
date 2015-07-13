#include "sdl_gfx_screen.h"

sdl_gfx_screen::sdl_gfx_screen(int window_width, int window_height,
                               char* title, int alloc_objects) {
    SDL_Init(SDL_INIT_VIDEO);
    _screen = SDL_SetVideoMode(window_width, window_height, 0,
                               SDL_HWSURFACE | SDL_DOUBLEBUF);
    _window_title = title;
    SDL_WM_SetCaption(_window_title, 0);
    _sdl_running = true;
    _shapes.reserve(alloc_objects);
}
sdl_gfx_screen::~sdl_gfx_screen() { SDL_Quit(); }
bool sdl_gfx_screen::sdl_status(){return _sdl_running;}
void sdl_gfx_screen::draw_shape(line_object obj){
    line_object obj_tmp = obj;
    for (int i = 0; i< obj_tmp.num_vertices; i++){
        obj_tmp.x[i] = obj_tmp.x[i]*obj_tmp.scale_x+obj_tmp.x_pos;
        obj_tmp.y[i] = obj_tmp.y[i]*obj_tmp.scale_y+obj_tmp.y_pos;
    }

    draw_line(obj_tmp.num_vertices,obj_tmp.x, obj_tmp.y, obj_tmp.color);

}
void sdl_gfx_screen::draw_line(int n, float* x, float* y, int* color) {
    for (int i = 0; i < n - 1; i++) {
        lineRGBA(_screen, x[i], y[i], x[i + 1], y[i + 1], color[0], color[1],
                 color[2], color[3]);
    }
}

void sdl_gfx_screen::fill_screen() {
    SDL_FillRect(_screen, NULL,SDL_MapRGB(_screen->format, 0, 0, 0));
}

void sdl_gfx_screen::refresh_screen() { SDL_Flip(_screen); }

void sdl_gfx_screen::poll_event() {
    if (SDL_PollEvent(&_event)) {
        if (_event.type == SDL_QUIT) {
            _sdl_running = false;
        }
    }
}

void sdl_gfx_screen::add_shape(int n, float* x, float* y, int* color, float scale_x, float scale_y){
    line_object object;
   object.num_vertices = n;
   object.x = x;
   object.y = y;
   object.x_pos = 0;
   object.y_pos = 0;
   object.rotation = 0;
   object.scale_x = scale_x;
   object.scale_y = scale_y;
   object.color = color;
   _shapes.push_back(object);

}

void sdl_gfx_screen::objects_init(){
//Draw a mountain
    
    float pi3 = M_PI/3;
    int n = 100;
    float * x = new float[n];
    float *y = new float[n];
    int color[4] = {255,0,0,255};

    float lim_x[2] = {-1.7,0.6};
    float lim_y[2] = {0,1};

    for (int i = 0; i< n; i++){

        x[i] = lim_x[0]+(float)i/(float)(n-1)*(lim_x[1]-lim_x[0]);
        y[i] = (1+sin(3*x[i]))/2;
    
    }
    
    add_shape(n,x,y,color);
    _shapes.back().scale_x = 200;
    _shapes.back().scale_y = 200;
    _shapes.back().x_pos = 400;
    _shapes.back().y_pos = 200;
}

void sdl_gfx_screen::object_scale(line_object obj, float scale_x,float scale_y){
    obj.scale_x = scale_x;
    obj.scale_y = scale_y;
}
void sdl_gfx_screen::object_move(line_object obj, float x_pos,float y_pos){
    obj.x_pos = x_pos;
    obj.y_pos = y_pos;
}
void sdl_gfx_screen::objects_draw(){

    float scalex = 0.9;
    float scaley = 0.7;

}

