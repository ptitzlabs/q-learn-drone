#Makefile
CXX = /usr/bin/c++
EXEEXT = 

LIB_TAG = d
PKG_CONFIG_ENV = PKG_CONFIG_PATH="/usr/lib/x86_64-linux-gnu/pkgconfig"
RPATHCMD =
QT_MOC_EXECUTABLE = /usr/lib/x86_64-linux-gnu/qt4/bin/moc
qt_RPATHCMD =

qt_EXECUTABLES_list =\
	qt_example$(EXEEXT)

EXECUTABLES_list = \
	$(qt_EXECUTABLES_list)

all: $(EXACUTABLES_list)

clean:
	rm -f $(EXACUTABLES_list)

qt_example$(EXEEXT): qt_example.cpp qt_PlotWindow.cpp moc_qt_PlotWindow.cpp
	$(CXX) qt_example.cpp qt_PlotWindow.cpp moc_qt_PlotWindow.cpp -o $@ $(qt_RPATHCMD) `$(PKG_CONFIG_ENV) pkg-config --cflags --libs plplot$(LIB_TAG)-qt` `$(PKG_CONFIG_ENV) pkg-config --cflags --libs plplotd`

#CC=g++
#LIB=-lm -ldislnc -ldiscpp -ldislin
#main: main.cpp tiles.h tiles.c interface.h interface.cpp
	#$(CC) -o bin/main.o main.cpp interface.cpp $(LIB)
