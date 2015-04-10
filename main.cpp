#include <QApplication>

#include <QtOpenGL>

#include "mainwindow.h"
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.desktop()->screenGeometry();
    MainWindow w;
    w.setMaximumSize ( a.desktop()->screenGeometry().size() );
    w.showMaximized();

    int i=1;
    bool loadIm=false,loadMesh=false;
    while (i<argc)
    {
        if (!strcmp(argv[i],"-im") )
        {
            i++;
            w.addImageToContainerSlicer(argv[i]);
            loadIm=true;
            loadMesh=false;
        }else if (!strcmp(argv[i],"-m") )
        {
            i++;
            w.addSurfaceToContainer(argv[i]);
            loadMesh=true;
            loadIm=false;
            w.setCameraPositionToSurface(0);

        }else if ( loadIm )
        {
            w.addImageToContainerSlicer(argv[i]);
        }else if (loadMesh )
        {
            w.addSurfaceToContainer(argv[i]);
        }
        i++;
    }


    return a.exec();
}
