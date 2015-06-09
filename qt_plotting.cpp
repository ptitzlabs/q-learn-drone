#include "qt_plotting.h"

int contourf::ns = 20;
int contourf::nx = 35;
int contourf::ny = 46;
int contourf::exclude = 0;
int contourf::colorbar = 1;
const int contourf::PERIMETERPTS = 100;

PLFLT contourf::tr[] = {2. / (nx - 1), 0.0, -1.0, 0.0, 2. / (ny - 1), -1.0};


PLOptionTable contourf::options[] = {
    {
        "exclude",              // Turns on page showing exclusion
        NULL,
        NULL,
        &exclude,
        PL_OPT_BOOL,
        "-exclude",
        "Plot the \"exclusion\" page."
    },
    {
        "ns",                   // Number of shade levels
        NULL,
        NULL,
        &ns,
        PL_OPT_INT,
        "-ns levels",
        "Sets number of shade levels"
    },
    {
        "nx",                   // Number of data points in x
        NULL,
        NULL,
        &nx,
        PL_OPT_INT,
        "-nx xpts",
        "Sets number of data points in x"
    },
    {
        "ny",                   // Number of data points in y
        NULL,
        NULL,
        &ny,
        PL_OPT_INT,
        "-ny ypts",
        "Sets number of data points in y"
    },
    {
        NULL,                   // option
        NULL,                   // handler
        NULL,                   // client data
        NULL,                   // address of variable to set
        0,                      // mode flag
        NULL,                   // short syntax
        NULL
    }                           // long syntax
};


const char    *contourf::  notes[] = {
    "to get smoother color variation, increase ns, nx, and ny.  to get faster",
    "response (especially on a serial link), decrease them.  a decent but quick",
    "test results from ns around 5 and nx, ny around 25.",
    NULL
};


// Transformation function

static void mypltr( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void * /* pltr_data */ )
{
    *tx = contourf::tr[0] * x + contourf::tr[1] * y + contourf::tr[2];
    *ty = contourf::tr[3] * x + contourf::tr[4] * y + contourf::tr[5];
}


PLINT zdefined( PLFLT x, PLFLT y )
{
    PLFLT z = sqrt( x * x + y * y );

    return z < 0.4 || z > 0.6;
}
int waldo = 0;
void finding_waldo()
{
    printf("%i\n",waldo++);
}
contourf::contourf(int argc, const char ** argv) {

    PLFLT       x, y, argx, argy, distort, r, t, zmin, zmax;
    PLFLT       *px  = new PLFLT[PERIMETERPTS];
    PLFLT       *py  = new PLFLT[PERIMETERPTS];
    PLFLT       *xg1 = new PLFLT[nx];
    PLFLT       *yg1 = new PLFLT[ny];
    PLFLT       **z;
    PLFLT       **w;
    PLcGrid     cgrid1;
    PLcGrid2    cgrid2;
    PLFLT       *clevel    = new PLFLT[ns];
    PLFLT       *shedge    = new PLFLT[ns + 1];
    const int   cont_color = 0;
    const PLFLT fill_width = 2.0, cont_width = 0.0;
    PLFLT       colorbar_width, colorbar_height;

#define NUM_AXES    1
    PLINT      n_axis_opts  = NUM_AXES;
    const char *axis_opts[] = {
        "bcvtm",
    };
    PLINT      num_values[NUM_AXES];
    PLFLT      *values[NUM_AXES];
    PLFLT      axis_ticks[NUM_AXES] = {
        0.0,
    };
    PLINT      axis_subticks[NUM_AXES] = {
        0,
    };
#define NUM_LABELS    1
    PLINT      n_labels     = NUM_LABELS;
    PLINT      label_opts[] = {
        PL_COLORBAR_LABEL_BOTTOM,
    };
    const char *labels[] = {
        "Magnitude",
    };

    // plplot initialization

    pls = new plstream();

    // Parse and process command line arguments.
    pls->MergeOpts( options, "x16c options", notes );
    pls->parseopts( &argc, argv, PL_PARSE_FULL );

    // Load colour palettes
    pls->spal0( "cmap0_black_on_white.pal" );
    pls->spal1( "cmap1_gray.pal", true );

    // Reduce colors in cmap 0 so that cmap 1 is useful on a
    //16-color display
    pls->scmap0n( 3 );


    // Initialize PLplot.
    finding_waldo();
    pls->init();

    pls->Alloc2dGrid( &z, nx, ny );
    pls->Alloc2dGrid( &w, nx, ny );

    // Set up data array

    for (int i = 0; i < nx; i++ )
    {
        x = (PLFLT) ( i - ( nx / 2 ) ) / (PLFLT) ( nx / 2 );
        for (int  j = 0; j < ny; j++ )
        {
            y = (PLFLT) ( j - ( ny / 2 ) ) / (PLFLT) ( ny / 2 ) - 1.0;

            z[i][j] = -sin( 7. * x ) * cos( 7. * y ) + x * x - y * y;
            w[i][j] = -cos( 7. * x ) * sin( 7. * y ) + 2 * x * y;
        }
    }

    pls->MinMax2dGrid( z, nx, ny, &zmax, &zmin );

    for (int  i = 0; i < ns; i++ )
        clevel[i] = zmin + ( zmax - zmin ) * ( i + 0.5 ) / (PLFLT) ns;

    for (int  i = 0; i < ns + 1; i++ )
        shedge[i] = zmin + ( zmax - zmin ) * (PLFLT) i / (PLFLT) ns;

// Set up coordinate grids

    cgrid1.xg = xg1;
    cgrid1.yg = yg1;
    cgrid1.nx = nx;
    cgrid1.ny = ny;

    pls->Alloc2dGrid( &cgrid2.xg, nx, ny );
    pls->Alloc2dGrid( &cgrid2.yg, nx, ny );
    cgrid2.nx = nx;
    cgrid2.ny = ny;

    for (int  i = 0; i < nx; i++ )
    {
        for (int  j = 0; j < ny; j++ )
        {
            mypltr( (PLFLT) i, (PLFLT) j, &x, &y, NULL );

            argx    = x * M_PI / 2;
            argy    = y * M_PI / 2;
            distort = 0.4;

            cgrid1.xg[i] = x + distort    * cos( argx );
            cgrid1.yg[j] = y - distort    * cos( argy );

            cgrid2.xg[i][j] = x + distort * cos( argx ) * cos( argy );
            cgrid2.yg[i][j] = y - distort * cos( argx ) * cos( argy );
        }
    }

    // Plot using identity transform

    pls->adv( 0 );
    pls->vpor( 0.1, 0.9, 0.1, 0.9 );
    pls->wind( -1.0, 1.0, -1.0, 1.0 );

    pls->psty( 0 );

    pls->shades( z, nx, ny, NULL, -1., 1., -1., 1.,
        shedge, ns + 1, fill_width,
        cont_color, cont_width,
        plstream::fill, true, NULL, NULL );

    if ( colorbar )
    {
        // Smaller text
        pls->schr( 0.0, 0.75 );
        // Small ticks on the vertical axis
        pls->smaj( 0.0, 0.5 );
        pls->smin( 0.0, 0.5 );

        num_values[0] = ns + 1;
        values[0]     = shedge;
        pls->colorbar( &colorbar_width, &colorbar_height,
            PL_COLORBAR_SHADE | PL_COLORBAR_SHADE_LABEL, 0,
            0.005, 0.0, 0.0375, 0.875, 0, 1, 1, 0.0, 0.0,
            cont_color, cont_width,
            n_labels, label_opts, labels,
            n_axis_opts, axis_opts,
            axis_ticks, axis_subticks,
            num_values, (const PLFLT * const *) values );

        // Reset text and tick sizes
        pls->schr( 0.0, 1.0 );
        pls->smaj( 0.0, 1.0 );
        pls->smin( 0.0, 1.0 );
    }

    pls->col0( 1 );
    pls->box( "bcnst", 0.0, 0, "bcnstv", 0.0, 0 );
    pls->col0( 2 );
    pls->lab( "distance", "altitude", "Bogon density" );


    pls->Free2dGrid( z, nx, ny );
    pls->Free2dGrid( w, nx, ny );
    pls->Free2dGrid( cgrid2.xg, nx, ny );
    pls->Free2dGrid( cgrid2.yg, nx, ny );

    delete[] px;
    delete[] py;
    delete[] xg1;
    delete[] yg1;
    delete[] clevel;
    delete[] shedge;
    delete pls;

}

#ifndef QT_INTERFACE_ON
int main( int argc, const char ** argv )
{
    contourf *x = new contourf( argc, argv );

    delete x;
}
#endif
