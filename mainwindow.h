#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "marchingcubes_form.h"
#include "imagecontainer.h"
#include "surfacecontainer.h"
#include "scene_properties.h"
#include "imagevideocapture.h"
#include "my_structs.h"
#include "fslsurface/fslsurface.h"
#include <QMainWindow>

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:

    void doAddImage();
    void doAddSurface();
    void doAppendSurfaceData();
    void doMarchingCubes();
    void showMarchingCubes();



    void addImageToContainerSlicer(const QString & filename );
    void addSurfaceToContainer(const QString & filename );
    //void addSurfaceToContainer( fslsurface_name::fslSurface<float,unsigned int> * surface );
    void addSurfaceToContainer( fslsurface_name::fslSurface<float,unsigned int> * surface, const std::string & surf_name );


    void appendSurfaceData(const QString & filename);
    void addGlyphsToSurface(const unsigned int & index);
void addColourBarToSurface();
void addColourBarToImage();

    void setMouseSensitivity(int sens);
    void setMouseMode(int mode);

    void setCameraPositionToSurface( const int & index);

    void takeSnapShot();
    void createMovie( bool write );

    void writeSurface();
    void writeSceneToFile();
    void readSceneFromFile();

private:

    Ui::MainWindowClass *ui;

    briview::MarchingCubes* marchingCubes_widget;
    briview::ImageContainer* imageContainer;
    briview::SurfaceContainer* surfaceContainer;
    briview::scene_properties* scene;
    ImageVideoCapture* im_vid_widget;

};

#endif // MAINWINDOW_H
