#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include "QVTKOpenGLWidget.h"

extern int qInitResources_resources();

int main(int argc, char *argv[])
{

    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());

    // QT Stuff
    QApplication app( argc, argv );

    QApplication::setStyle("fusion");

    qInitResources_resources();
    MainWindow w;
    w.show();

    return app.exec();
}
