#include "sdl_gfx_screen.h"

sdl_gfx_screen::sdl_gfx_screen(int window_width, int window_height, char* title,
                               int alloc_objects) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    _screen = SDL_SetVideoMode(window_width, window_height, 0,
                               SDL_HWSURFACE | SDL_DOUBLEBUF);

    _window_title = title;
    SDL_WM_SetCaption(_window_title, 0);
    _font = TTF_OpenFont("fonts/arial.ttf",12);
    //sleep(1);
    _foreground.r = 255;
    _foreground.b = 255;
    _foreground.g = 255;
    
    _background.r = 0;
    _background.b = 0;
    _background.g = 0;

    _text_surface = TTF_RenderText_Shaded(_font, "Manual mode", _foreground, _background);

    _text_location.x = 50;
    _text_location.y = 50;
    _text_location.w = 0;
    _text_location.h = 0;

    _sdl_running = true;
    _shapes.reserve(alloc_objects);
}
void sdl_gfx_screen::sdl_quit() { _sdl_running = false; }

sdl_gfx_screen::~sdl_gfx_screen() {
    SDL_FreeSurface(_screen);
    //SDL_FreeSurface(_text_surface);
    TTF_CloseFont(_font);
    TTF_Quit();
    SDL_Quit();
    for (int i = 0; i < _shapes.size(); i++) {
        delete[] _shapes.at(i).x;
        delete[] _shapes.at(i).y;
        delete[] _shapes.at(i).color;
    }
    _shapes.clear();
    sdl_quit();
}
bool sdl_gfx_screen::sdl_status() { return _sdl_running; }

void sdl_gfx_screen::fill_screen() {
    SDL_FillRect(_screen, NULL, SDL_MapRGB(_screen->format, 0, 0, 0));
}

void sdl_gfx_screen::refresh_screen() { SDL_Flip(_screen); }

void sdl_gfx_screen::poll_event() {
    if (SDL_PollEvent(&_event)) {
        if (_event.type == SDL_QUIT) {
            _sdl_running = false;
        }
    }
}

void sdl_gfx_screen::add_shape(int n, float* x, float* y, int* color,
                               float scale_x, float scale_y) {
    line_object object;
    object.num_vertices = n;
    object.x = new float[n];
    object.y = new float[n];
    for (int i = 0; i < n; i++) {
        object.x[i] = x[i];
        object.y[i] = y[i];
    }
    object.x_pos = 0;
    object.y_pos = 0;
    object.rotation = 0;
    object.scale_x = scale_x;
    object.scale_y = scale_y;
    object.color = new int[4];
    for (int i = 0; i < 4; i++) object.color[i] = color[i];
    _shapes.push_back(object);
    // std::cout << "Object added:" << std::endl;
    // std::cout << "x: ";
    // print_arr_1d(_shapes.back().num_vertices, _shapes.back().x);
    // std::cout << "\ny: ";
    // print_arr_1d(_shapes.back().num_vertices, _shapes.back().y);
    // std::cout << "\n color: " << object.color[0] << " " << object.color[1]
    //<< " " << object.color[2] << " " << object.color[3] << std::endl;
}
void sdl_gfx_screen::update_car_rt(float loc) {
    //std::cout<<loc<<std::endl;
    float car[5][2] = {{-0.05, -0.05},
                       {0.05, -0.05},
                       {0.05, 0.05},
                       {-0.05, 0.05},
                       {-0.05, -0.05}};
    //x = new float[5];
    //y = new float[5];

    for (int i = 0; i < 5; i++) {

        car[i][0] = car[i][0]+loc;
        car[i][1] = car[i][1]+(1 + sin(3 * loc)) / 2;
    }

    for(int i = 0; i<5; i++){
        _shapes.at(1).x[i] = car[i][0];
        _shapes.at(1).y[i] = car[i][1];
    }


}
void sdl_gfx_screen::update_target_flag(float loc) {
    //std::cout<<loc<<std::endl;
    float flag[5][2] = {{0, 0},
                       {0, 0.1},
                       {0.05, 0.1},
                       {0.05, 0.05},
                       {0, 0.05}};
    //x = new float[5];
    //y = new float[5];
    loc = (loc+1)/2-0.5;

    for (int i = 0; i < 5; i++) {

        flag[i][0] = flag[i][0]+loc;
        flag[i][1] = flag[i][1]+(1 + sin(3 * loc)) / 2;
    }

    for(int i = 0; i<5; i++){
        _shapes.at(2).x[i] = flag[i][0];
        _shapes.at(2).y[i] = flag[i][1];
    }


}
void sdl_gfx_screen::objects_init() {
    // Draw a mountain

    float pi3 = M_PI / 3;
    int n = 100;
    float* x = new float[n];
    float* y = new float[n];
    int* color = new int[4];
    color[0] = 255;
    color[1] = 0;
    color[2] = 0;
    color[3] = 255;
    // int color[4] = {255, 0, 0, 255};

    float lim_x[2] = {-1.7, 0.6};
    float lim_y[2] = {0, 1};

    for (int i = 0; i < n; i++) {
        x[i] = lim_x[0] + (float)i / (float)(n - 1) * (lim_x[1] - lim_x[0]);
        y[i] = (1 + sin(3 * x[i])) / 2;
    }

    add_shape(n, x, y, color);
    _shapes.back().scale_x = 200;
    _shapes.back().scale_y = -200;
    _shapes.back().x_pos = 400;
    _shapes.back().y_pos = 300;

    // Mountain car shape
    //
    float car[5][2] = {{-0.05, -0.05},
                       {0.05, -0.05},
                       {0.05, 0.05},
                       {-0.05, 0.05},
                       {-0.05, -0.05}};
    x = new float[5];
    y = new float[5];

    for(int i = 0; i<5; i++){
        x[i] = car[i][0];
        y[i] = car[i][1];
    }

    color[0]=0;
    color[1]=255;

    add_shape(5, x, y, color);
    _shapes.back().scale_x = 200;
    _shapes.back().scale_y = -200;
    _shapes.back().x_pos = 400;
    _shapes.back().y_pos = 300;
    float flag[5][2] = {{0, 0},
                       {0, 0.1},
                       {0.05, 0.1},
                       {-0.05, 0.05},
                       {0, 0.05}};
    //x = new float[5];
    //y = new float[5];
    for(int i = 0; i<5; i++){
        x[i] = flag[i][0];
        y[i] = flag[i][1];
    }

    color[0]=0;
    color[1]=255;

    add_shape(5, x, y, color);
    _shapes.back().scale_x = 200;
    _shapes.back().scale_y = -200;
    _shapes.back().x_pos = 400;
    _shapes.back().y_pos = 300;
    delete[] x;
    delete[] y;
    delete[] color;
}

void sdl_gfx_screen::object_scale(line_object obj, float scale_x,
                                  float scale_y) {
    obj.scale_x = scale_x;
    obj.scale_y = scale_y;
}
void sdl_gfx_screen::object_move(line_object obj, float x_pos, float y_pos) {
    obj.x_pos = x_pos;
    obj.y_pos = y_pos;
}
void sdl_gfx_screen::objects_draw() {
    for (int i = 0; i < _shapes.size(); i++) {
        draw_shape(_shapes.at(i));
    }
    draw_text();

}
void sdl_gfx_screen::draw_text(){
    SDL_BlitSurface(_text_surface, NULL, _screen, &_text_location);

}
void sdl_gfx_screen::draw_shape(line_object obj) {
    float x[obj.num_vertices];
    float y[obj.num_vertices];
    for (int i = 0; i < obj.num_vertices; i++) {
        x[i] = obj.x[i] * obj.scale_x + obj.x_pos;
        y[i] = obj.y[i] * obj.scale_y + obj.y_pos;
    }

    draw_line(obj.num_vertices, x, y, obj.color);
    // std::cout << "\n color: " << obj.color[0] << " " << obj.color[1] << " "
    // << obj.color[2]
    //<< " " << obj.color[3] << std::endl;
}
void sdl_gfx_screen::draw_line(int n, float* x, float* y, int* color) {
    // print_arr_1d(n, x);
    // std::cout << std::endl;
    // print_arr_1d(n, y);
    // std::cout << "\n color: " << color[0] << " " << color[1] << " "
    //<< color[2]
    //<< " " << color[3] << std::endl;

    // std::cout << "x: ";
    // print_arr_1d(_shapes.back().num_vertices, _shapes.back().x);
    // std::cout << "\ny: ";
    // print_arr_1d(_shapes.back().num_vertices, _shapes.back().y);
    // std::cout << "\n color: " << _shapes.back().color[0] << " "
    //<< _shapes.back().color[1] << " " << _shapes.back().color[2]
    //<< " " << _shapes.back().color[3] << std::endl;

    for (int i = 0; i < n - 1; i++) {
        lineRGBA(_screen, x[i], y[i], x[i + 1], y[i + 1], color[0], color[1],
                 color[2], color[3]);
    }
}
