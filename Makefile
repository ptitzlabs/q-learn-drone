all: lib/drone_dynamics.o lib/qutils.o lib/cmac_net.o lib/tiles.o lib/q_learn.o lib/q_controller.o
	g++ main.cpp lib/drone_dynamics.o lib/qutils.o lib/q_learn.o lib/q_controller.o  lib/model.o lib/cmac_net.o lib/tiles.o -o bin/main -Wall -L/usr/lib `sdl-config --cflags --libs` -lm -lpthread -std=c++11

lib/drone_dynamics.o:
	g++ src/drone_dynamics.cpp -Wall -c -o lib/drone_dynamics.o -lm

lib/qutils.o:
	g++ src/qutils.cpp -Wall -c -o lib/qutils.o -lm

lib/q_learn.o:
	g++ src/q_learn.cpp -Wall -c -o lib/q_learn.o -lm

lib/q_controller.o:
	g++ src/q_controller.c -Wall -c -o lib/q_controller.o

#lib/model.o:
	#g++ src/model.cpp -Wall -c -o lib/model.o -lm

lib/cmac_net.o:
	g++ src/cmac_net.cpp -Wall -c -o lib/cmac_net.o -lm

lib/tiles.o:
	g++ src/tiles.c -Wall -c -o lib/tiles.o -lm

clean:
	rm -f lib/* bin/*

