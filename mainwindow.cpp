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
#include <QAction>
#include <QMenu>

//using namespace briview_structs;
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    cout<<"setupUi"<<endl;
      ui->setupUi(this);
      cout<<"done setup ui"<<endl;
//    QAction *exitAction = new QAction(tr("Exit"), this);
//    QAction *aboutAct = new QAction(tr("About"), this);
//    QAction *aboutQtAct = new QAction(tr("About Qt"), this);

//    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
//    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
//    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
//cout<<"add menu "<<endl;
//    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
//    fileMenu->addAction(exitAction);
//cout<<"done fielmenu"<<endl;
//    QMenu* helpMenu = menuBar()->addMenu(tr("About"));
//    helpMenu->addAction(aboutAct);
//    helpMenu->addAction(aboutQtAct);
//    cout<<"done about"<<endl;

//    ui->menuBar->setVisible(true);
    //setup menu
//    QMenuBar *menuBar = new QMenuBar(0);
//    menuBar->addMenu("file");
//    setMenuBar(menuBar);

//                  QIcon(":/images/open.png"), tr("&Open..."), this);
//        openAct->setShortcuts(QKeySequence::Open);
//        openAct->setStatusTip(tr("Open an existing file"));
//        connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

//        fileMenu->addAction(openAct);


//          fileMenu->addAction(newAct);
      ///done menubar setup

    QGLFormat fmt = ui->openGLwidget->format();
    fmt.setSwapInterval(1);
    ui->openGLwidget->setFormat(fmt);
    scene = new scene_properties();

    im_vid_widget = new ImageVideoCapture();
    im_vid_widget->setSceneWidget(scene);
    imageContainer = new ImageContainer();
    surfaceContainer = new SurfaceContainer();
    marchingCubes_widget = new MarchingCubes();

    graphContainer_ = new graphContainer();
graphContainer_->setVisible(0);

//    scene->setVisible(1);
   scene->setVisible(0);

//   imageContainer->setVisible(1);
    imageContainer->setVisible(0);
    imageContainer->setImVidCaptureForm( im_vid_widget );

    //fixes wierd artifact, but of a hack but oh well
//    surfaceContainer->setVisible(1);
    surfaceContainer->setVisible(0);

    surfaceContainer->importPresetMaterials(QApplication::applicationDirPath().toStdString() + "/assets/preset_materials.txt");

    //-----------------------SETUP DOCKS IN SIDE TAB-----------------------//
    this->addDockWidget(Qt::DockWidgetArea(0x2),imageContainer->getImageManipulatorWidget());
    tabifyDockWidget(imageContainer->getImageManipulatorWidget(),surfaceContainer->getSurfaceManipulatorWidget());
    tabifyDockWidget(imageContainer->getImageManipulatorWidget(),scene);

    tabifyDockWidget(imageContainer->getImageManipulatorWidget(),graphContainer_->getGraphManipulatorWidgets());

    //setup menuBar
    setUpMenu();





    //-----------------------SETUP DOCKS IN SIDE TAB-----------------------//
    cout<<"run connections"<<endl;

    //----------------------SETUP SCENE CONNECTIONS----------------------//

    connect(scene, SIGNAL(sig_changedMouseSensitivity(int)),this, SLOT(setMouseSensitivity(int)));
    connect(scene, SIGNAL(sig_changedMouseMode(int)),this, SLOT(setMouseMode(int)));
    connect(scene,SIGNAL(sig_updateGL()),ui->openGLwidget,SLOT(updateGL()));

    connect(im_vid_widget, SIGNAL(sig_takeSnapShot()),this,SLOT(takeSnapShot()));
    connect(im_vid_widget, SIGNAL(sig_mov(bool)),this,SLOT(createMovie(bool)));

    //----------------setup surface conntainer connections--------------//
    //----------------------END SETUP SCENE CONNECTIONS----------------------//
    connect(surfaceContainer,SIGNAL(sig_updateGL()),ui->openGLwidget,SLOT(updateGL()));

    connect(surfaceContainer,SIGNAL(sig_appendSurfaceData()),this,SLOT(doAppendSurfaceData()));
    //----------------end setup surface conntainer connections--------------//

    connect(imageContainer,SIGNAL(sig_updateGL()),ui->openGLwidget,SLOT(updateGL()));
    connect(marchingCubes_widget,SIGNAL(sig_apply()),this,SLOT(doMarchingCubes()));

    connect(graphContainer_,SIGNAL(sig_copy_to_surfaces()),this, SLOT(copyGraphToSurfaces()));
    connect(graphContainer_,SIGNAL(sig_updateGL()),ui->openGLwidget,SLOT(updateGL()));

    //----------------------END SETUP ACTION CONNECTIONS (Toolbar)------------------//

    ui->openGLwidget->setImageContainer(&imageContainer);
    ui->openGLwidget->setSurfaceContainer(&surfaceContainer);
   cout<<"set graph conatiner"<<endl;
    ui->openGLwidget->setGraphContainer(&graphContainer_);
    cout<<"done set graph conatiner"<<endl;

    ui->openGLwidget->setSceneProperties(&scene);
    cout<<"done main window constructor "<<endl;
}
MainWindow::~MainWindow()
{
    delete imageContainer;
    delete surfaceContainer;
    delete scene;
    delete marchingCubes_widget;
    delete graphContainer_;
    delete ui;
}

void MainWindow::setUpMenu()
{
    ui->menuBar->clear();

    QMenu* menu_file =  ui->menuBar->addMenu(tr("&File"));
    QAction* actionLoad_Image = new QAction(tr("&Load Image..."),0);
    QAction* actionLoad_Surface = new QAction(tr("&Load Surface..."),0);
    QAction* actionAppend_Surface_Data = new QAction(tr("&Append Surface Data..."),0);
    QAction* actionSave_Surface = new QAction(tr("&Save Surface..."),0);
    QAction* actionSave_Scene = new QAction(tr("&Save Scene..."),0);
    QAction* actionLoad_Scene = new QAction(tr("&Load Scene..."),0);
    QAction* actionLoad_Graph = new QAction(tr("&Load Graph..."),0);

    //----------------------SETUP ACTION CONNECTIONS (Toolbar)------------------//
    connect(actionLoad_Image, SIGNAL(triggered()),this,SLOT(doAddImage()));
    connect(actionLoad_Surface, SIGNAL(triggered()),this,SLOT(doAddSurface()));
    connect(actionAppend_Surface_Data, SIGNAL(triggered()),this,SLOT(doAppendSurfaceData()));
    connect(actionSave_Surface, SIGNAL(triggered()),this,SLOT(writeSurface()));
    connect(actionSave_Scene, SIGNAL(triggered()),this,SLOT(writeSceneToFile()));
    connect(actionLoad_Scene, SIGNAL(triggered()),this,SLOT(readSceneFromFile()));
    connect(actionLoad_Graph, SIGNAL(triggered()),this,SLOT(readGraphFromFile()));

    //---------
    menu_file->addAction(actionLoad_Image);
    menu_file->addSeparator();
    menu_file->addAction(actionLoad_Surface);
    menu_file->addAction(actionAppend_Surface_Data);
    menu_file->addAction(actionSave_Surface);
    menu_file->addSeparator();
    menu_file->addAction(actionSave_Scene);
    menu_file->addAction(actionLoad_Scene);
    menu_file->addSeparator();
    menu_file->addAction(actionLoad_Graph);


    /////--------SCENE MENU
    ///

    QMenu* menu_scene =  ui->menuBar->addMenu(tr("&Scene"));

    QAction* actionScene_Manipulator_Sidebar = new QAction(tr("&Scene Manipulator Sidebar..."),0);
    actionScene_Manipulator_Sidebar->setCheckable(true);
    QAction* actionSceneNavigator = new QAction(tr("&Navigator (Mouse UI)..."),0);
    QAction* actionLighting = new QAction(tr("&Lighting..."),0);
    QAction* actionCamera = new QAction(tr("&Camera..."),0);
    QAction* actionBackground_Colour = new QAction(tr("&Background Colour..."),0);
    QAction* actionScreen_Capture = new QAction(tr("&Screen Capture..."),0);

//    //-----scene actions--------------//
    connect(actionScene_Manipulator_Sidebar,SIGNAL(toggled(bool)),scene,SLOT(setVisible(bool)));
    connect(actionSceneNavigator,SIGNAL(triggered()),scene,SLOT(showDockMouse()));
    connect(actionLighting,SIGNAL(triggered()),scene,SLOT(showDockLights()));
    connect(actionCamera,SIGNAL(triggered()),scene,SLOT(showDockCamera()));
    connect(actionBackground_Colour,SIGNAL(triggered()),scene,SLOT(showDockBackground()));
    connect(actionScreen_Capture,SIGNAL(triggered()),im_vid_widget,SLOT(show()));
    connect(actionScreen_Capture,SIGNAL(triggered()),im_vid_widget,SLOT(raise()));


    menu_scene->addAction(actionScene_Manipulator_Sidebar);
    menu_scene->addSeparator();
    menu_scene->addAction(actionSceneNavigator);
    menu_scene->addAction(actionLighting);
    menu_scene->addAction(actionCamera);
    menu_scene->addAction(actionBackground_Colour);
    menu_scene->addAction(actionScreen_Capture);


    /// -------
    /////--------Image MENU
    ///

    QMenu* menu_image =  ui->menuBar->addMenu(tr("&Image Tools"));

    QAction* actionImage_Manipulator_Sidebar = new QAction(tr("&Image Manipulator Sidebar..."),0);
    actionImage_Manipulator_Sidebar->setCheckable(true);
    QAction* actionImage_List = new QAction(tr("&Image List..."),0);
    QAction* actionImageNavigator = new QAction(tr("&Slice Plane Navigator..."),0);
    QAction* actionImageColour_map = new QAction(tr("&Image Material Properties..."),0);
    QAction* actionCoordinate_System = new QAction(tr("&Coordinate System..."),0);
    QAction* actionBlending = new QAction(tr("&Blending..."),0);
    QAction* actionImageColour_Bar = new QAction(tr("&Colour Bar..."),0);

//    //-----scene actions--------------//

    //-----image actions--------------//
    connect(actionImage_Manipulator_Sidebar,SIGNAL(toggled(bool)),imageContainer,SLOT(setVisible(bool)));
    connect(actionImage_List,SIGNAL(triggered()),imageContainer,SLOT(showDockImList()));
    connect(actionImageNavigator,SIGNAL(triggered()),imageContainer,SLOT(showDockImageNavigator()));
    connect(actionImageColour_map,SIGNAL(triggered()),imageContainer,SLOT(showDockProperties()));
    connect(actionCoordinate_System,SIGNAL(triggered()),imageContainer,SLOT(showDockCoordinateSystem()));
    connect(actionBlending,SIGNAL(triggered()),imageContainer,SLOT(showDockBlending()));
    connect(actionImageColour_Bar,SIGNAL(triggered()),imageContainer,SLOT(showDockColourBar()));

    menu_image->addAction(actionImage_Manipulator_Sidebar);
    menu_image->addSeparator();
    menu_image->addAction(actionImage_List);
    menu_image->addAction(actionImageNavigator);
    menu_image->addAction(actionImageColour_map);
    menu_image->addAction(actionCoordinate_System);
    menu_image->addAction(actionBlending);
    menu_image->addAction(actionImageColour_Bar);


    /// -------
    ///

    /// -------
    /////--------Surface MENU
    ///

    QMenu* menu_surface =  ui->menuBar->addMenu(tr("&Surface Tools"));

    QAction* actionSurface_Manipulator_Sidebar = new QAction(tr("&Surface Manipulator Sidebar..."),0);
    actionSurface_Manipulator_Sidebar->setCheckable(true);
    QAction* actionSurface_Scalar_Selector = new QAction(tr("&Surface/Scalar Selector..."),0);
    QAction* actionSurface_Colour_Mapping = new QAction(tr("&Surface Colour Mapping..."),0);
    QAction* actionMaterial_Properties = new QAction(tr("&Material Properties..."),0);
    QAction* actionGlyph_Options = new QAction(tr("&Glyph Options..."),0);
    QAction* actionColour_Bar = new QAction(tr("&Colour Bar..."),0);
    QAction* actionPolygon_Rendering = new QAction(tr("&Polygon Rendering..."),0);
    QAction* actionSlice_Surface = new QAction(tr("&Slice Surface..."),0);

    //-----surface actions--------------//
    connect(actionSurface_Manipulator_Sidebar,SIGNAL(toggled(bool)),surfaceContainer,SLOT(setVisible(bool)));
    connect(actionSurface_Scalar_Selector,SIGNAL(triggered()),surfaceContainer,SLOT(showDockSurfaceSelector()));
    connect(actionMaterial_Properties,SIGNAL(triggered()),surfaceContainer,SLOT(showDockMaterialProperties()));
    connect(actionColour_Bar,SIGNAL(triggered()),surfaceContainer,SLOT(showDockColourBar()));
    connect(actionGlyph_Options,SIGNAL(triggered()),surfaceContainer,SLOT(showDockGlyphs()));
    connect(actionSurface_Colour_Mapping,SIGNAL(triggered()),surfaceContainer,SLOT(showDockColourMap()));
    connect(actionPolygon_Rendering,SIGNAL(triggered()),surfaceContainer,SLOT(showDockPolygonMode()));
    connect(actionSlice_Surface, SIGNAL(triggered()),surfaceContainer,SLOT(sliceSurfaceY()));


    menu_surface->addAction(actionSurface_Manipulator_Sidebar);
    menu_surface->addSeparator();
    menu_surface->addAction(actionSurface_Scalar_Selector);
    menu_surface->addAction(actionSurface_Colour_Mapping);
    menu_surface->addAction(actionMaterial_Properties);
    menu_surface->addAction(actionGlyph_Options);
    menu_surface->addAction(actionColour_Bar);
    menu_surface->addAction(actionPolygon_Rendering);
    menu_surface->addAction(actionSlice_Surface);


    /// -------
    /////--------Other Menu MENU
    ///

    QMenu* menu_other =  ui->menuBar->addMenu(tr("&Other Tools"));
    QAction* actionMarching_Cubes = new QAction(tr("&Marchine Cubes..."),0);
    connect(actionMarching_Cubes,SIGNAL(triggered()),this,SLOT(showMarchingCubes()));
    menu_surface->addAction(actionMarching_Cubes);


    /// -------


    QMenu* menu_graph =  ui->menuBar->addMenu(tr("&Graphs"));
    QAction* openGraphAct = new QAction(tr("&Graph Explorer Sidebar..."),0);
    openGraphAct->setCheckable(true);
    menu_graph->addAction(openGraphAct);
    connect(openGraphAct,SIGNAL(toggled(bool)),graphContainer_,SLOT(setVisible(bool)));


}
void MainWindow::copyGraphToSurfaces()
{
    cout<<"copyGraphToSurfaces "<<endl;
    GLuint* vbos = new GLuint[2];

    glGenBuffersARB(2,vbos);
  //  surfaceContainer->addSurface(filename.toStdString(),vbos);
//   addColourBarToSurface();
     fslSurface<float,unsigned int> *graph_surf_nodes = new fslSurface<float,unsigned int>();
   *graph_surf_nodes = graphContainer_->getGraphNodesAsSurface();
    surfaceContainer->addSurface(graph_surf_nodes,vbos,"graph_nodes_to_surface");

    GLuint* vbos_links = new GLuint[2];

    glGenBuffersARB(2,vbos_links);
    fslSurface<float,unsigned int> *graph_surf_links = new fslSurface<float,unsigned int>();
  *graph_surf_links = graphContainer_->getGraphLinksAsSurface();
   surfaceContainer->addSurface(graph_surf_links,vbos_links,"graph_links_to_surface");


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
void MainWindow::addGraphToContainer(const QString & filename )
{
    graphContainer_->readFile(filename.toStdString());
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

void MainWindow::readGraphFromFile(){
    QString filename = QFileDialog::getOpenFileName(this,"Load graph from file...","Unititled",tr("Graph (*.txt)"));
    if (!filename.isNull())
    {
        cout<<"read graoh "<<filename.toStdString()<<endl;
        graphContainer_->readFile(filename.toStdString());
    }


}




