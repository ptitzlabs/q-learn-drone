all: lib/joystick.o lib/glhelper.o
	g++ main.cpp lib/joystick.o lib/glhelper.o -o bin/main -L/usr/lib `sdl-config --cflags --libs` -lSDL_ttf -lSDL_image -lGL -lGLU -lpthread

lib/joystick.o: src/joystick.h src/joystick.cc
	g++ src/joystick.cc -c -o lib/joystick.o

lib/glhelper.o: src/glhelper.h src/glhelper.cc
	g++ src/glhelper.cc -c -o lib/glhelper.o

clean:
	@rm -f *~ src/*~ lib/* bin/*
