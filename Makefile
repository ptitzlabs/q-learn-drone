all: lib/joystick.o lib/glhelper.o lib/sdl_gfx_screen.o lib/qutils.o lib/model.o lib/cmac_net.o lib/tiles.o lib/q_learn.o lib/q_controller.o
	g++ main.cpp lib/joystick.o lib/glhelper.o lib/sdl_gfx_screen.o lib/qutils.o lib/model.o lib/cmac_net.o lib/tiles.o lib/q_learn.o lib/q_controller.o -o bin/main -L/usr/lib `sdl-config --cflags --libs` -lSDL_ttf -lSDL_gfx -lSDL_image -lGL -lGLU -lpthread -std=c++11

huge: lib/joystick.o lib/glhelper.o lib/sdl_gfx_screen.o lib/qutils.o lib/tiles.o lib/q_controller.o
	g++ main_huge.cpp lib/joystick.o lib/glhelper.o lib/sdl_gfx_screen.o lib/qutils.o lib/tiles.o lib/q_controller.o -o bin/main -L/usr/lib `sdl-config --cflags --libs` -lSDL_ttf -lSDL_gfx -lSDL_image -lGL -lGLU -lpthread -std=c++11

lib/sdl_gfx_screen.o: src/sdl_gfx_screen.h src/sdl_gfx_screen.cpp
	g++ src/sdl_gfx_screen.cpp -c -o lib/sdl_gfx_screen.o -lSDL_gfx

lib/joystick.o: src/joystick.h src/joystick.cc
	g++ src/joystick.cc -c -o lib/joystick.o

lib/glhelper.o: src/glhelper.h src/glhelper.cc
	g++ src/glhelper.cc -c -o lib/glhelper.o

lib/qutils.o: src/qutils.h src/qutils.cpp
	g++ src/qutils.cpp -c -o lib/qutils.o

lib/model.o: src/model.h src/model.cpp
	g++ src/model.cpp -c -o lib/model.o

lib/cmac_net.o: src/cmac_net.h src/cmac_net.cpp
	g++ src/cmac_net.cpp -c -o lib/cmac_net.o

lib/tiles.o: src/tiles.h src/tiles.c
	g++ src/tiles.c -c -o lib/tiles.o
lib/q_learn.o: src/q_learn.h src/q_learn.cpp
	g++ src/q_learn.cpp -c -o lib/q_learn.o
lib/q_controller.o: src/q_controller.h src/q_controller.c
	g++ src/q_controller.c -c -o lib/q_controller.o

clean:
	@rm -f *~ src/*~ lib/* bin/*
