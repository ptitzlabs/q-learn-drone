#include "qt_interface.h"
//#include "qt_PlotWindow.h"

int main(int Argc, const char** Argv) {
#ifndef SEARCHING_WALDO
    int res;

    int argc = Argc;
    char** argv;
    argv = new char* [argc];
    for (int i = 0; i < argc; ++i) {
        int len = strlen(Argv[i]) + 1;
        argv[i] = new char[len];
        strncpy(argv[i], Argv[i], len);
    }

   Argc = 1;
   QApplication a(argc,argv);
   qt_interface *q = new qt_interface(argc,argv);
   //interface *q = new interface(argc,argv);
   a.setActiveWindow(q);
   q->setVisible(true);

   res = a.exec();

    for (int i = 0; i< Argc; ++i)
    {
        delete[] argv[i];
    }
    delete[] argv;
    return res;
#endif
    return 0;

}
