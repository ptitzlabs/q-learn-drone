#Makefile
CXX = /usr/bin/c++
EXEEXT = .o 

LIB_TAG = d
PKG_CONFIG_ENV = PKG_CONFIG_PATH="/usr/lib/x86_64-linux-gnu/pkgconfig"
RPATHCMD =
QT_MOC_EXECUTABLE = /usr/lib/x86_64-linux-gnu/qt4/bin/moc
qt_RPATHCMD =

qt_EXECUTABLES_list =\
	qt_example$(EXEEXT)

main_EXECUTABLES_list = \
	main$(EXEEXT)

testing_EXECUTABLES_list = \
	testing$(EXEEXT)

EXECUTABLES_list = \
	$(main_EXECUTABLES_list)
	#$(qt_EXECUTABLES_list)\


all: $(EXECUTABLES_list)

clean:
	rm -f $(EXECUTABLES_list)

testing: $(testing_EXECUTABLES_list)

testing$(EXEEXT): model_testing.cpp model_sim.cpp cmac_net.cpp Q_learn.cpp tiles.c
	$(CXX) model_testing.cpp model_sim.cpp cmac_net.cpp Q_learn.cpp tiles.c -Wall -o $@

#qt_example$(EXEEXT): qt_example.cpp qt_PlotWindow.cpp moc_qt_PlotWindow.cpp
	#$(CXX) qt_example.cpp qt_PlotWindow.cpp moc_qt_PlotWindow.cpp -o $@ $(qt_RPATHCMD) `$(PKG_CONFIG_ENV) pkg-config --cflags --libs plplot$(LIB_TAG)-qt` `$(PKG_CONFIG_ENV) pkg-config --cflags --libs plplotd`

#moc_qt_PlotWindow_interface.cpp: qt_PlotWindow_interface.h
	#$(QT_MOC_EXECUTABLE) `$(PKG_CONFIG_ENV) pkg-config  --cflags-only-I plplot$(LIB_TAG)-qt` $< -o $@
moc_qt_interface.cpp: qt_interface.h
	$(QT_MOC_EXECUTABLE) `$(PKG_CONFIG_ENV) pkg-config  --cflags-only-I plplot$(LIB_TAG)-qt` $< -o $@

main$(EXEEXT): main_test.cpp  
	$(CXX) main_test.cpp model.cpp qutils.cpp cmac_net.cpp q_learn.cpp tiles.c q_controller.c -Wall -o $@ $(qt_RPATHCMD) `$(PKG_CONFIG_ENV) pkg-config --cflags --libs `
#CC=g++
#LIB=-lm -ldislnc -ldiscpp -ldislin
#main: main.cpp tiles.h tiles.c interface.h interface.cpp
	#$(CC) -o bin/main.o main.cpp interface.cpp $(LIB)
