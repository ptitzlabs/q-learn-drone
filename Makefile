all: lib/joystick.o lib/glhelper.o lib/sdl_gfx_screen.o lib/qutils.o lib/model.o
	g++ main.cpp lib/joystick.o lib/glhelper.o lib/sdl_gfx_screen.o lib/qutils.o lib/model.o -o bin/main -L/usr/lib `sdl-config --cflags --libs` -lSDL_ttf -lSDL_gfx -lSDL_image -lGL -lGLU -lpthread -std=c++11

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

clean:
	@rm -f *~ src/*~ lib/* bin/*
