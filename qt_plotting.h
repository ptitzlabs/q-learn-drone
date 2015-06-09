
#include <cstdio>
#include <QtCore/QTextStream>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QAction>
#include <QtGui/QMenuBar>
#include <QtGui/QFileDialog>
#include <QtGui/QPrintDialog>
#include <QtGui/QPrinter>
#include <QtGui/QMessageBox>

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <cstring>
#include <cmath>
#include <float.h>

#include "plstream.h"
#ifndef M_PI
#define M_PI    3.1415926535897932384
#endif

class contourf {
public:
    contourf( int, const char ** );

private:
    // Class data
    plstream *pls;

    // number of shade levels, x data points, y data points,
    // and perimeter points.
    static int           ns;
    static int           nx;
    static int           ny;
    static int           exclude;
    static int           colorbar;
    const static int     PERIMETERPTS;
public:
    static PLFLT         tr[];

    static PLOptionTable options[];
    static const char    *notes[];
};
