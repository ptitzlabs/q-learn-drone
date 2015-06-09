#ifndef QT01_INTERFACE_H
#define QT01_INTERFACE_H

#define PLD_qtwidget

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

#include "qt.h"
#include "plstream.h"

class qt_interface : public QMainWindow {
    Q_OBJECT

   public:
    /// Constructor for the window, holding the widget parameters
    /// \param width Window width (independant of the plot size)
    /// \param height Window height
    /// \param saveWidth Plot actual width. Used to compute the aspect ratio on
    /// screen and the size in PS and SVG outputs \see resolution
    /// \param saveHeight Plot actual height.
    /// \param resolution Print resolution
    /// \param pageSize Print size
    /// \param orientation Print orientation
    /// \param parent Parent widget, if any.
    qt_interface(int argc = 0, char** argv = NULL, QWidget* parent = NULL);

    ~qt_interface();

   public slots:
    void test_cmac();
    void lineplot();
    void shadeplot();
    void contourf(PLFLT*,PLFLT*,PLFLT **, int, int);
    void test_contourf();

    void plotCurves();

    void interactive();

   protected:
    PLINT strm;
    QTextStream * out;

    QtExtWidget* plot;

   private:
    plstream *pls;
    int NSIZE;
};
#endif
