#include "mainwindow.h"
#include "ui_mainwindow.h"

//#define GL_GLEXT_PROTOTYPES

#include <QtOpenGL>

#include "scenewriter.h"
#include <iostream>
#include <fslsurface.h>
#include <fslsurfacefns.h>

#include <sstream>
#include "newmatap.h"

#include <mp4v2/mp4v2.h>

using namespace NEWMAT;

using namespace fslsurface_name;
using namespace Ui;
using namespace briview;

//using namespace briview_structs;
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    QGLFormat fmt = ui->openGLwidget->format();
    fmt.setSwapInterval(1);
    ui->openGLwidget->setFormat(fmt);
    scene = new scene_properties();

    im_vid_widget = new ImageVideoCapture();
    im_vid_widget->setSceneWidget(scene);
    imageContainer = new ImageContainer();
    surfaceContainer = new SurfaceContainer();
    marchingCubes_widget = new MarchingCubes();

    //scene->setVisible(1);
    scene->setVisible(0);

  //  imageContainer->setVisible(1);
    imageContainer->setVisible(0);
    imageContainer->setImVidCaptureForm( im_vid_widget );

    //fixes wierd artifact, but of a hack but oh well
    surfaceContainer->setVisible(1);
    surfaceContainer->setVisible(0);

    surfaceContainer->importPresetMaterials(QApplication::applicationDirPath().toStdString() + "/assets/preset_materials.txt");

    //-----------------------SETUP DOCKS IN SIDE TAB-----------------------//
    this->addDockWidget(Qt::DockWidgetArea(0x2),imageContainer->getImageManipulatorWidget());
    tabifyDockWidget(imageContainer->getImageManipulatorWidget(),surfaceContainer->getSurfaceManipulatorWidget());
    tabifyDockWidget(imageContainer->getImageManipulatorWidget(),scene);

    //-----------------------SETUP DOCKS IN SIDE TAB-----------------------//

    //----------------------SETUP SCENE CONNECTIONS----------------------//

    connect(scene, SIGNAL(sig_changedMouseSensitivity(int)),this, SLOT(setMouseSensitivity(int)));
    connect(scene, SIGNAL(sig_changedMouseMode(int)),this, SLOT(setMouseMode(int)));
    connect(scene,SIGNAL(sig_updateGL()),ui->openGLwidget,SLOT(updateGL()));

    connect(im_vid_widget, SIGNAL(sig_takeSnapShot()),this,SLOT(takeSnapShot()));
    connect(im_vid_widget, SIGNAL(sig_mov(bool)),this,SLOT(createMovie(bool)));

    //----------------setup surface conntainer connections--------------//
    //----------------------END SETUP SCENE CONNECTIONS----------------------//
    connect(surfaceContainer,SIGNAL(sig_updateGL()),ui->openGLwidget,SLOT(updateGL()));

    connect(ui->actionAppend_Surface_Data, SIGNAL(triggered()),this,SLOT(doAppendSurfaceData()));
    connect(surfaceContainer,SIGNAL(sig_appendSurfaceData()),this,SLOT(doAppendSurfaceData()));
    //----------------end setup surface conntainer connections--------------//

    connect(imageContainer,SIGNAL(sig_updateGL()),ui->openGLwidget,SLOT(updateGL()));


    //----------------------SETUP ACTION CONNECTIONS (Toolbar)------------------//
    connect(ui->actionLoad_Surface, SIGNAL(triggered()),this,SLOT(doAddSurface()));
    connect(ui->actionSave_Surface, SIGNAL(triggered()),this,SLOT(writeSurface()));

    //-----scene actions--------------//
    connect(ui->actionScene_Manipulator_Sidebar,SIGNAL(toggled(bool)),scene,SLOT(setVisible(bool)));
    connect(ui->actionSceneNavigator,SIGNAL(triggered()),scene,SLOT(showDockMouse()));
    connect(ui->actionLighting_2,SIGNAL(triggered()),scene,SLOT(showDockLights()));
    connect(ui->actionCamera,SIGNAL(triggered()),scene,SLOT(showDockCamera()));
    connect(ui->actionBackground_Colour,SIGNAL(triggered()),scene,SLOT(showDockBackground()));
   // connect(ui->actionScreen_Capture,SIGNAL(triggered()),this,SLOT(screenCapture()));
    connect(ui->actionScreen_Capture,SIGNAL(triggered()),im_vid_widget,SLOT(show()));
    connect(ui->actionScreen_Capture,SIGNAL(triggered()),im_vid_widget,SLOT(raise()));

    //-----image actions--------------//
    connect(ui->actionImage_Manipulator_Sidebar,SIGNAL(toggled(bool)),imageContainer,SLOT(setVisible(bool)));
    connect(ui->actionImage_List,SIGNAL(triggered()),imageContainer,SLOT(showDockImList()));
    connect(ui->actionImageNavigator,SIGNAL(triggered()),imageContainer,SLOT(showDockImageNavigator()));
    connect(ui->actionImageColour_map,SIGNAL(triggered()),imageContainer,SLOT(showDockProperties()));
    connect(ui->actionCoordinate_System,SIGNAL(triggered()),imageContainer,SLOT(showDockCoordinateSystem()));
    connect(ui->actionBlending_2,SIGNAL(triggered()),imageContainer,SLOT(showDockBlending()));
    connect(ui->actionImageColour_Bar,SIGNAL(triggered()),imageContainer,SLOT(showDockColourBar()));



    //-----surface actions--------------//
    connect(ui->actionSurface_Manipulator_Sidebar,SIGNAL(toggled(bool)),surfaceContainer,SLOT(setVisible(bool)));
    connect(ui->actionSurface_Scalar_Selector,SIGNAL(triggered()),surfaceContainer,SLOT(showDockSurfaceSelector()));
    connect(ui->actionMaterial_Properties,SIGNAL(triggered()),surfaceContainer,SLOT(showDockMaterialProperties()));
    connect(ui->actionColour_Bar,SIGNAL(triggered()),surfaceContainer,SLOT(showDockColourBar()));
    connect(ui->actionGlyph_Options,SIGNAL(triggered()),surfaceContainer,SLOT(showDockGlyphs()));
    connect(ui->actionSurface_Colour_Mapping,SIGNAL(triggered()),surfaceContainer,SLOT(showDockColourMap()));
    connect(ui->actionPolygon_Rendering,SIGNAL(triggered()),surfaceContainer,SLOT(showDockPolygonMode()));

    //-----------------------------------//
    connect(ui->actionLoad_Image, SIGNAL(triggered()),this,SLOT(doAddImage()));
     connect(ui->actionSave_Scene, SIGNAL(triggered()),this,SLOT(writeSceneToFile()));
     connect(ui->actionLoad_Scene, SIGNAL(triggered()),this,SLOT(readSceneFromFile()));

     connect(ui->actionSlice_Surface, SIGNAL(triggered()),surfaceContainer,SLOT(sliceSurfaceY()));
     connect(ui->actionMarching_Cubes,SIGNAL(triggered()),this,SLOT(showMarchingCubes()));
     connect(marchingCubes_widget,SIGNAL(sig_apply()),this,SLOT(doMarchingCubes()));

    //----------------------END SETUP ACTION CONNECTIONS (Toolbar)------------------//

    ui->openGLwidget->setImageContainer(&imageContainer);
    ui->openGLwidget->setSurfaceContainer(&surfaceContainer);
    ui->openGLwidget->setSceneProperties(&scene);




}
MainWindow::~MainWindow()
{
    delete imageContainer;
    delete surfaceContainer;
    delete scene;
    delete marchingCubes_widget;
    delete ui;
}
void MainWindow::showMarchingCubes()
{
    marchingCubes_widget->show();
}

void MainWindow::takeSnapShot()
{
    cout<<"let see if we can do a screen capture"<<endl;
    ImageVideoCapture::CompressionType comp_type;
    string filename;
    int numViews;
    im_vid_widget->getImageProperties(filename,comp_type,numViews);
    //time in seconds
   // ui->openGLwidget->renderCapture(filename  , comp_type, 10, 60, 90,90,10,10,10);

   ui->openGLwidget->renderCapture(filename, comp_type, numViews);
cout<<filename<<" "<<endl;
cout<<comp_type<<" "<<numViews<<endl;
    cout<<"done render capture"<<endl;
}
void MainWindow::createMovie( bool write)
{

    cout<<"Create movie"<<endl;
    ImageVideoCapture::CompressionType comp_type;
    string filename;
    int numViews;
    im_vid_widget->getImageProperties(filename,comp_type,numViews);
    //time in seconds
    unsigned int fps =im_vid_widget->getFPS();
   // float duration,tx,ty,tz,theta,phi;
    filename = im_vid_widget->getMovieName().toStdString();
    //im_vid_widget->getMovieProperties(filename,fps,duration,tx,ty,tz,theta,phi);
    vector< animationParams > v_anims = im_vid_widget->getAnimationList();
 //   ui->openGLwidget->renderCapture(filename  , write, comp_type, fps,duration,theta,phi,tx,ty,tz);
 ui->openGLwidget->renderCaptureList(filename,write,comp_type, v_anims,fps);
//   ui->openGLwidget->renderCapture(filename, comp_type, numViews);
cout<<filename<<" "<<endl;
cout<<comp_type<<" "<<numViews<<endl;
    cout<<"done Create Movie"<<endl;
}





//function that use both surfaces and images fall into mainwindow
void MainWindow::doMarchingCubes()
{

    cout<<"run marching cubes"<<endl;

    vector<float> image;// = NULL;
int count=0;
    vector<int> indices = imageContainer->getSelectedImageIndices();
    for ( vector<int>::iterator i_ind = indices.begin(); i_ind != indices.end();++i_ind,++count)
    {
        fslSurface<float,unsigned int>* surface = new fslSurface<float,unsigned int>();

        cout<<"INMAGE INDEX "<<*i_ind<<endl;
        image_dims dims = imageContainer->getImageApplyBiasScale(image,*i_ind);
        cout<<"run marching cubes1 "<<dims.xdim<<" "<<dims.ydim<<" "<<dims.zdim<<endl;
        cout<<"run marching cubes1 "<<dims.xsize<<" "<<dims.ysize<<" "<<dims.zsize<<endl;

        cout<<"run marching cubes2 "<<image[0]<<endl;

        cout<<"run marching cubes "<<endl;
        //float scale = imageContainer->getCurrentImageScale();
        //float bias = imageContainer->getCurrentImageBias();

       // * v_im_thresh_scale[index] + v_im_thresh_bias[index] ;
    cout<<"image"<<endl;
    //for (int i = 0 ; i < dims.xsize*dims.ysize* dims.zsize; ++i)
    //    cout<<i<<" "<<image[i]<<endl;
        cout<<marchingCubes_widget->getThreshold()<<" "<< marchingCubes_widget->getLabel()<<" "<<marchingCubes_widget->getThresholdMode()<<endl;

        if (!marchingCubes_widget->getDoAllLabels())
        {
            marchingCubes<float,unsigned int,float>(*surface, &(image[0]), dims,marchingCubes_widget->getThreshold(), marchingCubes_widget->getLabel(),static_cast<MarchingCubesMode>( marchingCubes_widget->getThresholdMode()));

        }else{
            runMarchingCubesOnAllLabels<float,unsigned int,float>(*surface, &(image[0]), dims,marchingCubes_widget->getThreshold());
        }





        vector<double> xfm = imageContainer->getImageXFM(*i_ind);//getCurrentImageXFM();
        Matrix mxfm(4,4);
        mxfm=0;
    cout<<"Nverst "<<surface->getNumberOfVertices()<<endl;
        cout<<"vector "<<endl;
        cout<<"xfm "<<endl;
        for (int i =0 ; i < 4 ; ++i)
        {       for (int j = 0 ; j < 4; ++j)
            {
                mxfm.element(i,j) = xfm[j+i*4];
                cout<<xfm[j+i*4]<<" ";//<<endl;
                }
        cout<<endl;
    }
        //for (vector<double>::iterator i = xfm.begin(); i != xfm.end(); ++i)
       // {
        //    cout<<"xfm "<<*i<<endl;
         //   mxfm<<*i;
        //}
        cout<<"mxfm "<<mxfm.element(0,0)<<" "<<mxfm.element(1,1)<<" "<<mxfm.element(2,2)<<endl;
        mxfm=mxfm.t();
        cout<<"mxfm "<<endl;
        for (int i =0 ; i < 4 ; ++i)
        {   for (int j = 0 ; j < 4; ++j)
            {
                xfm[j+i*4] = mxfm.element(i,j) ;
                cout<<xfm[j+i*4]<<" ";
                }
            cout<<endl;
        }
        apply_xfm<float, unsigned int,double>( *surface, xfm);//imageContainer->getCurrentImageXFM() );

        vector< float > csys_eye(16,0);
        csys_eye[0]=1;
        csys_eye[5]=1;
        csys_eye[10]=1;
        csys_eye[15]=1;
        surface->addCoordSystem( csys_eye, "NIFTI_XFORM_SCANNER_ANAT");
        QString basename = imageContainer->getImageName(*i_ind);
        basename.replace(".nii.gz","");
        stringstream ss ;
        ss<<count;
       // string scount;
       // ss>>scount;
           addSurfaceToContainer(surface,basename.toStdString() + "_marchingcubes_" + marchingCubes_widget->getLabelAsString());//+"_"+scount);
      //  writeGIFTI(surface,"test.gii");
    //cout"dine write "<<endl;
        //surface->save("test.gii");
        //if (image != NULL)
        //    delete[] image;
          // delete surface;
    }
ui->openGLwidget->updateGL();
}

void MainWindow::doAddImage()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,"Load NIFIT Image File (*.nii.gz)","./","*.nii.gz");

    //if (!filename.isNull())
    for (QStringList::iterator i = filenames.begin() ; i != filenames.end(); ++i)
        addImageToContainerSlicer(*i);
    //    addImageToContainerSlicer(filename);


}
void MainWindow::doAddSurface()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,"Load Surface File (*.vtk,*.ply,*.gii,*.gii.gz)","./","*.vtk;*.ply;*.gii;*.gii.gz");

    for (QStringList::iterator i = filenames.begin() ; i != filenames.end(); ++i)
        addSurfaceToContainer(*i);
                              //filename);

    //if (!filename.isNull())

}
void MainWindow::doAppendSurfaceData()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,"Append Surface Data File (*.vtk,*.ply,*.gii,*.gii.gz)","./","*.vtk;*.ply;*.gii;*.gii.gz");

    for (QStringList::iterator i = filenames.begin() ; i != filenames.end(); ++i)
         appendSurfaceData(*i);
 //   if (!filename.isNull())
   //     appendSurfaceData(filename);

}
void MainWindow::addImageToContainerSlicer(const QString & filename )
{

    GLuint* vbo = new GLuint[2];
    glGenBuffers(2,vbo);
    imageContainer->addImage(filename.toStdString(),vbo);
   addColourBarToImage();

}
void MainWindow::addSurfaceToContainer(const QString & filename )
{
    //need to valid vbos from this current context, 0:vertices, 1:incdices
    GLuint* vbos = new GLuint[2];

    glGenBuffersARB(2,vbos);
    surfaceContainer->addSurface(filename.toStdString(),vbos);
    addGlyphsToSurface(surfaceContainer->getNumberOfSurfaces()-1);
    addColourBarToSurface();
}

void MainWindow::addSurfaceToContainer( fslsurface_name::fslSurface<float,unsigned int> * surface , const string & surf_name)
{
    //need to valid vbos from this current context, 0:vertices, 1:incdices
    GLuint* vbos = new GLuint[2];

    glGenBuffersARB(2,vbos);
     surfaceContainer->addSurface(surface,vbos,surf_name);
  //  surfaceContainer->addSurface(filename.toStdString(),vbos);
   addGlyphsToSurface(surfaceContainer->getNumberOfSurfaces()-1);
   addColourBarToSurface();
}

void MainWindow::appendSurfaceData(const QString & filename )
{
    //need to valid vbos from this current context, 0:vertices, 1:incdices
    //GLuint* vbos = new GLuint[2];

   // glGenBuffers(2,vbos);
    surfaceContainer->appendSurfaceData(filename.toStdString());
   addGlyphsToSurface(surfaceContainer->getNumberOfSurfaces()-1);
   // addColourBarToSurface();
}

void MainWindow::addGlyphsToSurface(const unsigned int & index)
{
   // cout<<"add gkyphs mainwindow "<<index<<endl;
    //need to valid vbos from this current context, 0:vertices, 1:incdices
    GLuint* vbos = new GLuint[2];

    glGenBuffersARB(2,vbos);
    //cout<<"add gkyphs mainwindow2"<<endl;

    surfaceContainer->addGlyphs(index,vbos);
    //cout<<"done add glyphs"<<endl;
}

void MainWindow::addColourBarToSurface()
{
    GLuint* vbos = new GLuint[2];
    glGenBuffersARB(4,vbos);
    surfaceContainer->addColourBar(vbos);

}
void MainWindow::addColourBarToImage()
{
    GLuint* vbos = new GLuint[2];
    glGenBuffersARB(4,vbos);
    imageContainer->addColourBar(vbos);

}


void MainWindow::setCameraPositionToSurface(const int & index)
{
    ui->openGLwidget->setCameraPositionToSurface(index);

}



void MainWindow::setMouseSensitivity(int sens) {
    ui->openGLwidget->setMouseSensitivity(sens);
}
void MainWindow::setMouseMode(int mode) {
    ui->openGLwidget->setMouseMode(mode);
}
void MainWindow::writeSurface(){
    surfaceContainer->writeSurface();
}

void MainWindow::writeSceneToFile()
{
    QString filename = QFileDialog::getSaveFileName(this,"Save scene to file...","Unititled",tr("Scenes (*.scene)"));

    if (!filename.isNull())
    {
        sceneWriter* f_scene = new sceneWriter(filename.toStdString());
        f_scene->writeSceneProperties(scene);
        f_scene->writeSceneSurfaces(surfaceContainer);

        delete f_scene;
    }
}
void MainWindow::readSceneFromFile()
{
    QString filename = QFileDialog::getOpenFileName(this,"Load scene from file...","Unititled",tr("Scenes (*.scene)"));

    if (!filename.isNull())
    {
        sceneReader* f_scene = new sceneReader(filename.toStdString());
        f_scene->readSceneProperties(scene);
        f_scene->readSceneSurfaces(surfaceContainer);

        ui->openGLwidget->updateGL();

        delete f_scene;

    }
}




