

#include "qt_interface.h"

#include "testing.h"

qt_interface::qt_interface(int argc, char** argv, QWidget* parent)
    : QMainWindow(parent) {
    setAttribute(Qt::WA_DeleteOnClose);

    QMenu* plotMenu = menuBar()->addMenu("Menu");
    plotMenu->addAction("button", this, SLOT(test_cmac()));
    plotMenu->addAction("shadeplot", this, SLOT(shadeplot()));
    plotMenu->addAction("test_contourf", this, SLOT(test_contourf()));
    plotMenu->addAction("button2", this, SLOT(lineplot()));
    plotMenu->addAction("button2", this, SLOT(plotCurves()));
    // plotMenu->addAction("Interactive Selection", this, SLOT(interactive()));

    plot = new QtExtWidget(QT_DEFAULT_X, QT_DEFAULT_Y, this);
    setCentralWidget(plot);

    // One window = One plot widget = one stream
    plmkstrm(&strm);
    plsdev("extqt");

    // Get all the application arguments as argc, argv

    if (argc == 0 || argv == NULL) {
        plsetqtdev(plot);
    } else {
        plsetqtdev(plot, argc, argv);
    }
    plinit();

    resize(600, 600);

    pladv(0);
}

qt_interface::~qt_interface() {
    PLINT cur_strm;
    plgstrm(&cur_strm);
    plsstrm(strm);
    plfreeqtdev();  // also deletes the device ("plot" variable) !
    plsstrm(cur_strm);
}

void fun_test(PLFLT* xg,PLFLT ** zg){
    xg[0] = 23;
    xg[1] = 45;
    zg[0][0] = 84;

}

void qt_interface::test_cmac() {
    int nx = 2000;
    int ny = 2000;
    PLFLT* xg = new PLFLT[nx];
    PLFLT* yg = new PLFLT[ny];


    PLFLT** zg;
    plAlloc2dGrid(&zg,nx,ny);
    fun_test(xg,zg);


    CMAC_testing(xg, yg, zg, nx, ny);
    printf("testing comm: %.2f, %.2f\n",xg[5],zg[0][0]);

    //for (int i = 0; i < nx; i++) {
        //for (int j = 0; j < ny; j++) {
            //printf("%.2f ", zg[i][j]);
        //}

        //printf("\n");
    //}
    

    printf("done doing stuff!\n");
    contourf(xg, yg, zg, nx, ny);

    // std::cout<<zgrid[1][1]<<endl;
}

// static void mypltr(PLFLT x, PLFLT y, PLFLT* tx, PLFLT* ty, PLFLT tr[]) {
//*tx = tr[0] * x + tr[1] * y + tr[2];
//*ty = tr[3] * x + tr[4] * y + tr[5];
//}
void qt_interface::lineplot() {
    plot->clearWidget();

    NSIZE = 101;

    PLFLT x[NSIZE], y[NSIZE];
    PLFLT xmin = 0., xmax = 1., ymin = 0., ymax = 100.;
    int i;

    for (i = 0; i < NSIZE; i++) {
        x[i] = (PLFLT)(i) / (PLFLT)(NSIZE - 1);
        y[i] = ymax * x[i] * x[i];
    }

    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("x", "y=100 x#u2#d", "Simple PLplot demo of a 2D line plot");

    plline(NSIZE, x, y);
}

// void find_min_2_d(PLFLT x, n) {

//}
//

void qt_interface::test_contourf() {
    const int nx = 6;
    const int ny = 6;
    PLFLT* xg = new PLFLT[nx];
    PLFLT* yg = new PLFLT[ny];

    PLFLT** zg;

    plAlloc2dGrid(&zg, nx, ny);

    for (int i = 0; i < nx; i++) {
        xg[i] = i + 1;
    }
    for (int i = 0; i < ny; i++) {
        yg[i] = i + 1;
    }

    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            zg[i][j] = xg[i] * 2 * yg[j];
        }
    }
    contourf(xg, yg, zg, nx, ny);
}

void qt_interface::contourf(PLFLT* xg, PLFLT* yg, PLFLT** zg, int nx, int ny) {
    plot->clearWidget();
    PLFLT* x = new PLFLT[nx];
    PLFLT* y = new PLFLT[ny];
    PLFLT** z;
    PLFLT** x2d;
    PLFLT** y2d;

    PLFLT xmin, xmax, ymin, ymax;

    int nlevel = 20;
    int i, j;

    PLFLT* shedge = new PLFLT[nlevel+1];

    const PLFLT fill_width = 2.0;
    const int cont_color = 0;
    const PLFLT cont_width = 0.0;

    for (i = 0; i < nx; i++) {
        x[i] = xg[i];
    }
    for (i = 0; i < ny; i++) {
        y[i] = yg[i];
    }
    PLFLT zmin = INFINITY;
    PLFLT zmax = -INFINITY;

    plAlloc2dGrid(&z, nx, ny);

    plAlloc2dGrid(&x2d, nx, ny);
    plAlloc2dGrid(&y2d, nx, ny);

    for (i = 0; i < nx; i++) {
        for (j = 0; j < ny; j++) {
            z[i][j] = zg[i][j];
            x2d[i][j] = xg[i];
            y2d[i][j] = yg[j];
        }
    }

    plMinMax2dGrid(z, nx, ny, &zmax, &zmin);

    xmin = xg[0];
    xmax = xg[nx - 1];
    ymin = yg[0];
    ymax = yg[ny - 1];


    for (i = 0; i < nlevel + 1; i++) {
        shedge[i] = zmin + (zmax - zmin) * (float)i / (float)(nlevel);
    }

    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("X", "Y", "Test 2d plot");
    plshades(z, nx, ny, NULL, xmin, xmax, ymin, ymax, shedge, nlevel + 1,
             fill_width, cont_color, cont_width, plfill, true, NULL, NULL);
}

void qt_interface::shadeplot() {
    plot->clearWidget();
    int dim = 6;
    PLFLT* x = new PLFLT[dim];
    PLFLT* y = new PLFLT[dim];
    PLFLT** z;
    PLFLT** z2;
    PLFLT** x2d;
    PLFLT** y2d;

    // PLFLT z[dim][dim], x2d[dim][dim], y2d[dim][dim], z2[dim][dim];
    PLFLT xmin, xmax, ymin, ymax;
    // PLFLT * clevel = new PLFLT[10];
    // PLFLT clevel[10];

    // PLFLT step;
    int nlevel;
    int i, j;

    // static const char defined[];
    PLFLT* shedge = new PLFLT[11];
    PLFLT* shedge2 = new PLFLT[11];

    // PLFLT shedge[11], shedge2[11];

    const PLFLT fill_width = 2.0;
    const int cont_color = 0;
    const PLFLT cont_width = 0.0;

    for (i = 0; i < dim; i++) {
        x[i] = i + 1;
        y[i] = x[i];
    }
    PLFLT zmin = INFINITY;
    PLFLT zmax = -INFINITY;
    PLFLT z2min = INFINITY;
    PLFLT z2max = -INFINITY;

    plAlloc2dGrid(&z, dim, dim);
    plAlloc2dGrid(&z2, dim, dim);

    plAlloc2dGrid(&x2d, dim, dim);
    plAlloc2dGrid(&y2d, dim, dim);

    plAlloc2dGrid(&z2, dim, dim);

    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            z[i][j] = x[i] * 2 * y[j];
            z2[i][j] = x[i] * x[i] * y[j];
            x2d[i][j] = x[i];
            y2d[i][j] = y[j];
        }
    }

    plMinMax2dGrid(z, dim, dim, &zmax, &zmin);
    plMinMax2dGrid(z2, dim, dim, &z2max, &z2min);

    xmin = 1.0;
    xmax = 6.0;
    ymin = 1.0;
    ymax = 6.0;
    nlevel = 10;

    for (i = 0; i < nlevel + 1; i++) {
        shedge[i] = zmin + (zmax - zmin) * (float)i / (float)(nlevel);
        shedge2[i] = z2min + (z2max - z2min) * (float)i / (float)(nlevel);
    }

    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("X", "Y", "Test 2d plot");
    plshades(z, dim, dim, NULL, 1., 6., 1., 6., shedge, nlevel + 1, fill_width,
             cont_color, cont_width, plfill, true, NULL, NULL);
}

void qt_interface::plotCurves() {
    plot->clearWidget();

    PLFLT sine[360];
    PLFLT cosine[360];
    PLFLT indexes[360];
    PLFLT square[360];
    PLFLT cubic[360];

    // 1st plot

    for (int i = 0; i < 360; ++i) {
        sine[i] = sin((PLFLT)i / 180. * M_PI);
        cosine[i] = cos((PLFLT)i / 180. * M_PI);
        indexes[i] = (PLFLT)i;
    }

    pladv(0);
    plvpor(0.05, 0.95, 0.05, 0.45);
    plwind(0., 360., -1.2, 1.2);

    plcol0(2);
    plbox("bcnst", 0., 0, "bcnst", 0., 0);

    plcol0(1);
    plwidth(2);
    plline(360, indexes, sine);

    plcol0(3);
    plwidth(1);
    pllsty(2);
    plline(360, indexes, cosine);
    pllsty(1);

    plcol0(2);

    plmtex("t", 1., 0.5, 0.5, "Sines");

    // 2nd plot

    for (int i = -180; i < 180; ++i) {
        square[i + 180] = (((PLFLT)i) / 180. * ((PLFLT)i) / 180.);
        cubic[i + 180] = square[i + 180] * ((PLFLT)i) / 180.;
        indexes[i + 180] = ((PLFLT)i) / 180.;
    }

    plvpor(0.05, 0.95, 0.55, 0.95);
    plwind(-1., 1., -1., 1.);

    plcol0(2);
    plbox("bcnst", 0., 0, "bcnst", 0., 0);

    plcol0(1);
    plwidth(2);
    plline(360, indexes, square);

    plcol0(3);
    plwidth(1);
    pllsty(2);
    plline(360, indexes, cubic);
    pllsty(1);

    plcol0(2);

    plmtex("t", 1., 0.5, 0.5, "Square & Cubic");
}

void plfbox(PLFLT x0, PLFLT y0) {
    PLFLT x[4], y[4];

    x[0] = x0;
    y[0] = 0.;
    x[1] = x0;
    y[1] = y0;
    x[2] = x0 + 1.;
    y[2] = y0;
    x[3] = x0 + 1.;
    y[3] = 0.;
    plfill(4, x, y);
    plcol0(1);
    pllsty(1);
    plline(4, x, y);
}

void qt_interface::interactive() {
    PLFLT x, y;
    plot->captureMousePlotCoords(&x, &y);

    char buf[100];
    sprintf(buf, "Selection: (%f, %f)", x, y);

    QMessageBox msgBox;
    msgBox.setText(buf);
    msgBox.exec();
}
