#include "myopenglwidget.h"
#include <sstream>
using namespace std;
//using namespace briview_structs;
using namespace briview;
using namespace fslsurface_name;
#include<QDir>
#include "misc.h"
#include <cmath>
#define PI 3.14159265


myOpenGLWidget::myOpenGLWidget(QWidget *parent) : QGLWidget(parent) {
    mouseSensitivity=1;
    mouseMode=0;   
    mouseInMotion=false;
    mouseInMotionCount=0;

}

myOpenGLWidget::~myOpenGLWidget() {

    glDetachShader(p_light_dir,v_light_dir);
    glDetachShader(p_light_dir,f_light_dir);
    glDetachShader(p_light_dir_map_scalars,v_light_dir_map_scalars);
    glDetachShader(p_light_dir_map_scalars,f_light_dir_map_scalars);
    glDetachShader(p_im_texture,v_im_texture);
    glDetachShader(p_im_texture,f_im_texture);
    glDeleteProgram(p_light_dir);
    glDeleteProgram(p_light_dir_map_scalars);
    glDeleteProgram(p_im_texture);

#ifdef GL_GLEXT_PROTOTYPES 
	glDeleteFramebuffersEXT(1,&fbo);
    glDeleteRenderbuffersEXT(2, rbo);
#else
    glDeleteFramebuffers(1,&fbo);
    glDeleteRenderbuffers(2, rbo);
#endif

}


int writeTIFF(const unsigned int &  width, const unsigned int & height, const string & filename  , const unsigned int & compression)
{
    TIFF *file;
    GLfloat *image, *p;
    int i;
//height=2;
    file = TIFFOpen(filename.c_str(), "w");
    if (file == NULL) {
      return 1;
    }
    image = (GLfloat *) malloc(width * height * sizeof(GLfloat) * 3);

    /* OpenGL's default 4 byte pack alignment would leave extra bytes at the
       end of each image row so that each full row contained a number of bytes
       divisible by 4.  Ie, an RGB row with 3 pixels and 8-bit componets would
       be laid out like "RGBRGBRGBxxx" where the last three "xxx" bytes exist
       just to pad the row out to 12 bytes (12 is divisible by 4). To make sure
       the rows are packed as tight as possible (no row padding), set the pack
       alignment to 1. */
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, image);

 //   for (int i = 0 ; i < 10; ++i)
   //        cout<<"image "<<sizeof(GL_FLOAT)<<" "<<image[i]<<endl;


    TIFFSetField(file, TIFFTAG_IMAGEWIDTH, (uint32) width);
    TIFFSetField(file, TIFFTAG_IMAGELENGTH, (uint32) height);
    TIFFSetField(file, TIFFTAG_BITSPERSAMPLE, sizeof(GL_FLOAT)*8);
    TIFFSetField(file, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
//TIFFSetField(file, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
   TIFFSetField(file, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(file, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(file, TIFFTAG_ROWSPERSTRIP, 1);
    TIFFSetField(file, TIFFTAG_IMAGEDESCRIPTION, "OpenGL-rendered using brview");
    p = image;
    for (i = height - 1; i >= 0; i--) {
          if (TIFFWriteScanline(file, p, i, 0) < 0) {
              free(image);
            TIFFClose(file);
            return 1;
          }
      p += width*3;// * sizeof(GLfloat) * 3;
    }
    TIFFClose(file);
}

int myOpenGLWidget::animateCameraMotion( const bool & write,const unsigned int & compression, const animationParams & par_anim , const float & fps, unsigned int & i_frame )
{
    //bool write = false;
    QDir mdir("/tmp/brimovie");

    unsigned int nframes = static_cast<unsigned int>( par_anim.duration* fps + 0.5);
    float tx_step = par_anim.tx /nframes;
    float ty_step = par_anim.ty /nframes;
    float tz_step = par_anim.tz /nframes;
    float theta_step = par_anim.theta/180.0*PI /nframes;
    float phi_step = par_anim.phi/180.0*PI /nframes;
cout<<"par anim "<<par_anim.theta<<" "<<par_anim.phi<<" "<<par_anim.duration<<" "<<fps<<" "<<nframes<<endl;
    for (unsigned int i = 0 ; i < nframes; ++i,++i_frame)
    {
     //   if (progressD.wasCanceled())
     //       break;
       // progressD.setValue(i);

           cout<<"frame : "<<i_frame<<" "<<tx_step<<" "<<ty_step<<" "<<tz_step<<" "<<theta_step<<" "<<phi_step<<endl;
           cout<<"frame : "<<i<<" "<<i*tx_step<<" "<<i*ty_step<<" "<<i*tz_step<<" "<<i*theta_step<<" "<<i*phi_step<<endl;

        translateAndRotate(tx_step,ty_step,tz_step,theta_step,phi_step);


        if (write)
        {


        stringstream ss;
        ss<<i_frame;
        string snum;
        ss>>snum;

        if (i_frame<10)
            snum="00000"+snum;
        else if (i_frame<100)
             snum="0000"+snum;
        else if (i_frame<1000)
             snum="000"+snum;
        else if (i_frame<10000)
             snum="00"+snum;
        else if (i_frame<100000)
             snum="0"+snum;


        QString imname(("/frame_" + snum + ".tiff").c_str() );
        imname.prepend(mdir.absolutePath());
        cout<<"fnamenew "<<imname.toStdString()<<endl;
        writeTIFF(width, height,imname.toStdString(), compression);

        QString imname_jpg(("/frame_" + snum + ".jpg").c_str() );
        imname_jpg.prepend(mdir.absolutePath());
        QString program = "convert";
          QStringList arguments;
            arguments <<"-quality"<<"100"<<imname<< imname_jpg;
cout<<imname_jpg.toStdString()<<endl;
          QProcess *myProcess = new QProcess();
          myProcess->setStandardOutputFile("/tmp/brimovie/stdout.txt");
          myProcess->setStandardErrorFile("/tmp/brimovie/err.log");

            myProcess->start(program, arguments);
          myProcess->waitForFinished(-1);
          // myProcess->close();
          cout<<"done convert "<<imname_jpg.toStdString()<<endl;
           program.clear();
           program.append("rm");
           arguments.clear();
           arguments<<imname;
           myProcess->start(program, arguments);
         myProcess->waitForFinished(-1);

         delete myProcess;
          cout<<"done remove_file"<<endl;
        }
        //animate(animationParams);

    }

}
int myOpenGLWidget::animateSliceMotion( const bool & write,const unsigned int & compression, const animationParams & par_anim , const float & fps, unsigned int & i_frame)
{
    cout<<"animate slice "<<par_anim.duration<<" "<<fps<<endl;
  //  bool write = false;
    unsigned int nframes = static_cast<unsigned int>( par_anim.duration* fps + 0.5);
    cout<<"animate slice2 "<<nframes<<endl;
    QDir mdir("/tmp/brimovie");

    unsigned int x_1_vox,y_1_vox,z_1_vox;
     unsigned int x_2_vox,y_2_vox,z_2_vox;
     bool x_1_on, y_1_on,z_1_on;
     bool x_2_on, y_2_on,z_2_on;

cout<<"turn on "<<par_anim.x_1_on<<" "<<par_anim.y_1_on<<" "<<par_anim.z_1_on<<endl;
     images->turnOnOffSliceX( par_anim.x_1_on );
     images->turnOnOffSliceY( par_anim.y_1_on );
     images->turnOnOffSliceZ( par_anim.z_1_on );
     vec3<float> start(par_anim.x_1_vox,par_anim.y_1_vox,par_anim.z_1_vox);
     vec3<float> end(par_anim.x_2_vox,par_anim.y_2_vox,par_anim.z_2_vox);
     image_dims dims =  images->getCurrentImageDims();
cout<<"stend "<<start.x<<" "<<start.y<<" "<<start.z<<"stend "<<end.x<<" "<<end.y<<" "<<end.z<<endl;
     vec3<float> step = vec3<float>((end.x-start.x)/nframes,(end.y-start.y)/nframes,(end.z-start.z)/nframes);
     for (unsigned int i = 0 ; i < nframes; ++i,++i_frame)
     {
         cout<<"frame "<<i_frame<<" "<< (start.y+i*step.y)*dims.ydim<<endl;
         images->changeSliceX( (start.x+i*step.x)*dims.xdim );
        images->changeSliceY(  (start.y+i*step.y)*dims.ydim );
        images->changeSliceZ(  (start.z+i*step.z)*dims.zdim  );


        if (write)
        {


        stringstream ss;
        ss<<i_frame;
        string snum;
        ss>>snum;

        if (i_frame<10)
            snum="00000"+snum;
        else if (i_frame<100)
             snum="0000"+snum;
        else if (i_frame<1000)
             snum="000"+snum;
        else if (i_frame<10000)
             snum="00"+snum;
        else if (i_frame<100000)
             snum="0"+snum;


        QString imname(("/frame_" + snum + ".tiff").c_str() );
        imname.prepend(mdir.absolutePath());
        cout<<"fnamenew "<<imname.toStdString()<<endl;
        writeTIFF(width, height,imname.toStdString(), compression);

        QString imname_jpg(("/frame_" + snum + ".jpg").c_str() );
        imname_jpg.prepend(mdir.absolutePath());
        QString program = "convert";
          QStringList arguments;
            arguments <<"-quality"<<"100"<<imname<< imname_jpg;
cout<<imname_jpg.toStdString()<<endl;
          QProcess *myProcess = new QProcess();
          myProcess->setStandardOutputFile("/tmp/brimovie/stdout.txt");
          myProcess->setStandardErrorFile("/tmp/brimovie/err.log");

            myProcess->start(program, arguments);
          myProcess->waitForFinished(-1);
          // myProcess->close();
          cout<<"done convert "<<imname_jpg.toStdString()<<endl;
           program.clear();
           program.append("rm");
           arguments.clear();
           arguments<<imname;
           myProcess->start(program, arguments);
         myProcess->waitForFinished(-1);

         delete myProcess;
          cout<<"done remove_file"<<endl;
        }




    }
     images->turnOnOffSliceX( par_anim.x_2_on );
     images->turnOnOffSliceY( par_anim.y_2_on );
     images->turnOnOffSliceZ( par_anim.z_2_on );
     cout<<"turn on "<<par_anim.x_2_on<<" "<<par_anim.y_2_on<<" "<<par_anim.z_2_on<<endl;



}


int myOpenGLWidget::animate( const bool & write,const unsigned int & compression, const animationParams & par_anim , const float & fps, unsigned int & i_frame)
{
   // enum AnimationType { Start, CameraMotion, SliceMoving };
        AnimationType type = par_anim.type;
        switch (type){
            case Start:
                cout<<"set start"<<endl;
                scene_props->setCameraParameters(par_anim.eye,par_anim.centre,par_anim.vup);
                updateGL();

              break;
        case CameraMotion:
            cout<<"do camera motion "<<fps<<endl;
            animateCameraMotion(write,compression,par_anim, fps,i_frame);
            break;
        case SliceMoving:
            animateSliceMotion(write, compression,par_anim, fps,i_frame);

            break;
           default:
             break;
        }

}
int myOpenGLWidget::renderCaptureList(const string & filename, const bool & write, const unsigned int & compression, const vector< animationParams > & v_animation , const float & fps)
{
    QDir mdir("/tmp/brimovie");
    cout<<"movie directory "<<" "<<mdir.dirName().toStdString()<<endl;

    if (write)
    {
    cout<<"does exist"<<endl;
    if ( ! mdir.mkdir(mdir.absolutePath()) )
        {
            //remove files.
        cout<<"remove old file"<<endl;
            QStringList files = mdir.entryList();
            for ( QList<QString>::iterator file = files.begin(); file != files.end();file=file+1)
            {
                mdir.remove(*file);
            }
        }
    }
    cout<<"render Capture List"<<endl;
   // QProgressDialog progressD("Rendering Frames of Movie...", "Abort Render", 0, nframes, this);
     // progressD.setWindowModality(Qt::WindowModal);
    unsigned int frame=0;

    for ( vector< animationParams >::const_iterator i_a = v_animation.begin(); i_a != v_animation.end();++i_a )
    {
        cout<<"animate "<<fps<<" fps"<<endl;
        animate(write,compression,*i_a, fps, frame);
    }

    if (write)
    {
        QProgressDialog progressD2("Merging Images into Movie...", "Abort Render", 0, 10, this);
           progressD2.setWindowModality(Qt::WindowModal);
    //create movie
           if (QFile::exists(QString(filename.c_str())))
               QFile::remove(QString(filename.c_str()));

cout<<"Creating movie from Images..."<<endl;
progressD2.setValue(1);

    QString program = "ffmpeg";
      QStringList arguments;
      QString stemp;
      stemp.setNum(fps);
      arguments<<"-i"<<"/tmp/brimovie/frame_%06d.jpg"<<"-qmin"<<"1"<<"-qmax"<<"1"<<"-r"<<stemp<<filename.c_str();
    QProcess *myProcess = new QProcess();
   myProcess->setStandardOutputFile("/tmp/brimovie/ffmpeg.txt");
    myProcess->setStandardErrorFile("/tmp/brimovie/ffmpeg.log");
    myProcess->start(program, arguments);
     myProcess->waitForFinished(-1);
     cout<<"ffmpeg -i /tmp/brimovie/frame_%06d.jpg -qmin 1 -qmax 1 -r "<<stemp.toStdString()<<" "<<filename.c_str()<<endl;
/*
    cout<<"Images have been merged..."<<endl;
    program.clear();
    program.append("rm");
    arguments.clear();
    for (unsigned int i = 0 ; i < frame; ++i)
    {

        stringstream ss;
        ss<<i;
        string snum;
        ss>>snum;

        if (i<10)
            snum="00000"+snum;
        else if (i<100)
             snum="0000"+snum;
        else if (i<1000)
             snum="000"+snum;
        else if (i<10000)
             snum="00"+snum;
        else if (i<100000)
             snum="0"+snum;

        QString imname_jpg(("/frame_" + snum + ".jpg").c_str() );
        imname_jpg.prepend(mdir.absolutePath());
        arguments<<imname_jpg;

    }
    myProcess->start(program, arguments);
     myProcess->waitForFinished(-1);
*/
    delete myProcess;

    progressD2.setValue(10);
}


}

int myOpenGLWidget::renderCapture( const string & filename  , const bool & write, const unsigned int & compression, const unsigned int & fps, const float & duration, const float & h_rot_range,const float & v_rot_range, const float & tx, const float & ty, const float &tz)
{
    cout<<"renderCapture "<<filename<<" "<<fps<<" "<<duration<<" "<<h_rot_range<<" "<<v_rot_range<<endl;
    float theta=v_rot_range/180.0*PI;
    float phi=h_rot_range/180.0*PI;
    unsigned int nframes = static_cast<unsigned int>( duration* fps + 0.5);
    float tx_step = tx /nframes;
    float ty_step = ty /nframes;
    float tz_step = tz /nframes;
    float theta_step = theta /nframes;
    float phi_step = phi /nframes;
    //QString tmppath();
    cout<<"nframes "<<nframes<<endl;

    QDir mdir("/tmp/brimovie");
    cout<<"movie directory "<<" "<<mdir.dirName().toStdString()<<endl;

    briview::float3 eye_camera,center_camera,vup_camera;
    scene_props->getCameraParameters(eye_camera,center_camera,vup_camera);

    if (write)
    {
    cout<<"does exist"<<endl;
    if ( ! mdir.mkdir(mdir.absolutePath()) )
        {
            //remove files.
        cout<<"remove old file"<<endl;
            QStringList files = mdir.entryList();
            for ( QList<QString>::iterator file = files.begin(); file != files.end();file=file+1)
            {
                mdir.remove(*file);
            }
        }
    }
    QProgressDialog progressD("Rendering Frames of Movie...", "Abort Render", 0, nframes, this);
       progressD.setWindowModality(Qt::WindowModal);

    for (unsigned int i = 0 ; i < nframes; ++i)
    {
        if (progressD.wasCanceled())
            break;
        progressD.setValue(i);

           cout<<"frame : "<<i<<" "<<tx_step<<" "<<ty_step<<" "<<tz_step<<" "<<theta_step<<" "<<phi_step<<endl;
           cout<<"frame : "<<i<<" "<<i*tx_step<<" "<<i*ty_step<<" "<<i*tz_step<<" "<<i*theta_step<<" "<<i*phi_step<<endl;

        translateAndRotate(tx_step,ty_step,tz_step,theta_step,phi_step);
        //animate(animationParams);

        if (write)
        {


        stringstream ss;
        ss<<i;
        string snum;
        ss>>snum;

        if (i<10)
            snum="00000"+snum;
        else if (i<100)
             snum="0000"+snum;
        else if (i<1000)
             snum="000"+snum;
        else if (i<10000)
             snum="00"+snum;
        else if (i<100000)
             snum="0"+snum;


        QString imname(("/frame_" + snum + ".tiff").c_str() );
        imname.prepend(mdir.absolutePath());
        cout<<"fnamenew "<<imname.toStdString()<<endl;
        writeTIFF(width, height,imname.toStdString(), compression);

        QString imname_jpg(("/frame_" + snum + ".jpg").c_str() );
        imname_jpg.prepend(mdir.absolutePath());
        QString program = "convert";
          QStringList arguments;
            arguments <<"-quality"<<"100"<<imname<< imname_jpg;
cout<<imname_jpg.toStdString()<<endl;
          QProcess *myProcess = new QProcess();
          myProcess->setStandardOutputFile("/tmp/brimovie/stdout.txt");
          myProcess->setStandardErrorFile("/tmp/brimovie/err.log");

            myProcess->start(program, arguments);
          myProcess->waitForFinished(-1);
          // myProcess->close();
          cout<<"done convert "<<imname_jpg.toStdString()<<endl;
           program.clear();
           program.append("rm");
           arguments.clear();
           arguments<<imname;
           myProcess->start(program, arguments);
         myProcess->waitForFinished(-1);

         delete myProcess;
          cout<<"done remove_file"<<endl;
        }
    }
    progressD.setValue(nframes);


    if (write)
    {
        QProgressDialog progressD2("Merging Images into Movie...", "Abort Render", 0, 10, this);
           progressD2.setWindowModality(Qt::WindowModal);
    //create movie
           if (QFile::exists(QString(filename.c_str())))
               QFile::remove(QString(filename.c_str()));

cout<<"Creating movie from Images..."<<endl;
progressD2.setValue(1);

    QString program = "ffmpeg";
      QStringList arguments;
      QString stemp;
      stemp.setNum(fps);
      arguments<<"-i"<<"/tmp/brimovie/frame_%06d.jpg"<<"-qmin"<<"1"<<"-qmax"<<"1"<<"-r"<<stemp<<filename.c_str();
    QProcess *myProcess = new QProcess();
   myProcess->setStandardOutputFile("/tmp/brimovie/ffmpeg.txt");
    myProcess->setStandardErrorFile("/tmp/brimovie/ffmpeg.log");
    myProcess->start(program, arguments);
     myProcess->waitForFinished(-1);

    cout<<"Images have been merged..."<<endl;
    program.clear();
    program.append("rm");
    arguments.clear();

    for (unsigned int i = 0 ; i < nframes; ++i)
    {

        stringstream ss;
        ss<<i;
        string snum;
        ss>>snum;

        if (i<10)
            snum="00000"+snum;
        else if (i<100)
             snum="0000"+snum;
        else if (i<1000)
             snum="000"+snum;
        else if (i<10000)
             snum="00"+snum;
        else if (i<100000)
             snum="0"+snum;

        QString imname_jpg(("/frame_" + snum + ".jpg").c_str() );
        imname_jpg.prepend(mdir.absolutePath());
        arguments<<imname_jpg;

    }
    myProcess->start(program, arguments);
     myProcess->waitForFinished(-1);

    delete myProcess;

    progressD2.setValue(10);
}
    scene_props->setCameraParameters(eye_camera,center_camera,vup_camera);
    updateGL();


}

int myOpenGLWidget::renderCapture( const string & filename  , const unsigned int & compression, const unsigned int & nviews )
{
    cout<<"get render data "<<filename<<endl;
  //  float* image = new float[width*height*3];
   // TIFF* tif = TIFFOpen("foo.tif", "w");

   // glReadPixels(0,0,width,height,GL_RGB,GL_FLOAT, image);
    //for (int i = 0 ; i < 10; ++i)
      //  cout<<"image "<<image[i]<<endl;
//COMPRESSION_NONE
//        COMPRESSION_LZW
    int str_max=filename.size()-1;
    bool istif=false;
    bool istiff=false;
     string fname_new=filename;
     cout<<"filename base "<<filename.substr(str_max-3,4)<<endl;
     //if dont check if string is long enough, it will crash when too short
     if (str_max>=3)
    if (! filename.compare(str_max-3,4,".tif"))
    {
        cout<<"file end with .tif"<<endl;
        istif=true;
    }

    if (str_max>=4)
    if (! filename.compare(str_max-4,5,".tiff"))
    {
        cout<<"file end with .tiff"<<endl;
        istiff=true;
    }
    if (nviews == 1)
    {
        //add tif extension if none found
        if (!((istif)||(istiff)))
            fname_new=filename+".tif";


        writeTIFF(width, height, fname_new, compression);

    }else if ( nviews == 4 )
    {
        for ( int i = 0 ; i<4; ++i)
        {

            stringstream ss;
            ss<<i;
            string snum;
            ss>>snum;
            if (istif)
                fname_new=filename.substr(0, str_max-3)+"-"+snum+".tif";
            else if (istiff)
                fname_new=filename.substr(0, str_max-4)+"-"+snum+".tiff";
            else
                fname_new = filename +"-"+snum+ ".tif";

            cout<<"fnamenew "<<fname_new<<endl;
            writeTIFF(width, height,fname_new, compression);
            rotation(0,PI/2.0);

        }
       // rotation(0,PI/2.0);

    }else if ( nviews == 6 )
    {
        for ( int i = 0 ; i<4; ++i)
        {
            stringstream ss;
            ss<<i;
            string snum;
            ss>>snum;
            if (istif)
                fname_new=filename.substr(0, str_max-3)+"-"+snum+".tif";
            else if (istiff)
                fname_new=filename.substr(0, str_max-4)+"-"+snum+".tiff";
            else
                fname_new = filename +"-"+snum+ ".tif";

            writeTIFF(width, height,fname_new, compression);
            rotation(0,PI/2.0);

        }
       // rotation(0,PI/2.0);
 //-------NEXT STEP
        {
        stringstream ss;
        ss<<4;
        string snum;
        ss>>snum;
        if (istif)
            fname_new=filename.substr(0, str_max-3)+"-"+snum+".tif";
        else if (istiff)
            fname_new=filename.substr(0, str_max-4)+"-"+snum+".tiff";
        else
            fname_new = filename +"-"+snum+ ".tif";

        rotation(PI/2.0,0);
        writeTIFF(width, height,fname_new, compression);
    }
//-------NEXT STEP
        {
        stringstream ss;
        ss<<5;
        string snum;
        ss>>snum;
        if (istif)
            fname_new=filename.substr(0, str_max-3)+"-"+snum+".tif";
        else if (istiff)
            fname_new=filename.substr(0, str_max-4)+"-"+snum+".tiff";
        else
            fname_new = filename +"-"+snum+ ".tif";

        rotation(PI,0);
        writeTIFF(width, height,fname_new, compression);
    }
          rotation(PI/2.0,0);
    }

}

void  myOpenGLWidget::setShaders() {

    char *vs,*fs;

    //**********************Directional Light Shader with mapped colours********************//

    v_light_dir_map_scalars = glCreateShader(GL_VERTEX_SHADER);
    f_light_dir_map_scalars = glCreateShader(GL_FRAGMENT_SHADER);
    //prt pixel shaders
    //cout<<"set shaders "<<(QApplication::applicationDirPath() + "/glsl_shaders/surface_dir_light_map_scalars.vert").toStdString()<<endl;
    vs = textFileRead((QApplication::applicationDirPath() + "/glsl_shaders/surface_dir_light_map_scalars_ctrthresh.vert").toUtf8().data());
    ///vs = textFileRead((QApplication::applicationDirPath() + "/glsl_shaders/surface_dir_light_map_scalars.vert").toAscii());

    fs = textFileRead((QApplication::applicationDirPath() + "/glsl_shaders/surface_dir_light_map_scalars.frag").toUtf8().data());

    const char * vv_light_dir = vs;
    const char * ff_light_dir = fs;

    glShaderSource(v_light_dir_map_scalars, 1, &vv_light_dir,NULL);
    glShaderSource(f_light_dir_map_scalars, 1, &ff_light_dir,NULL);

    glCompileShader(v_light_dir_map_scalars);
    glCompileShader(f_light_dir_map_scalars);

    p_light_dir_map_scalars = glCreateProgram();

    glAttachShader(p_light_dir_map_scalars,v_light_dir_map_scalars);
    glAttachShader(p_light_dir_map_scalars,f_light_dir_map_scalars);

    glBindAttribLocation(p_light_dir_map_scalars,0,"InVertex");
    glBindAttribLocation(p_light_dir_map_scalars,1,"InNormal");
    glBindAttribLocation(p_light_dir_map_scalars,2,"InScalar");


    glLinkProgram(p_light_dir_map_scalars);
    glUseProgram(p_light_dir_map_scalars);
    loc_r_lut = glGetUniformLocation(p_light_dir_map_scalars,"r_lut");
    loc_g_lut = glGetUniformLocation(p_light_dir_map_scalars,"g_lut");
    loc_b_lut = glGetUniformLocation(p_light_dir_map_scalars,"b_lut");
    loc_a_lut = glGetUniformLocation(p_light_dir_map_scalars,"a_lut");

    loc_sc_lut = glGetUniformLocation(p_light_dir_map_scalars,"sc_lut");
     loc_r_lut_last = glGetUniformLocation(p_light_dir_map_scalars,"r_lut_last");
    loc_g_lut_last = glGetUniformLocation(p_light_dir_map_scalars,"g_lut_last");
    loc_b_lut_last = glGetUniformLocation(p_light_dir_map_scalars,"b_lut_last");
    loc_a_lut_last = glGetUniformLocation(p_light_dir_map_scalars,"a_lut_last");

    loc_sc_lut_last = glGetUniformLocation(p_light_dir_map_scalars,"sc_lut_last");
    loc_low_clamp = glGetUniformLocation(p_light_dir_map_scalars,"lower_clamp");

    r_lut[0]=1.0;
    r_lut[1]=1.0;
    r_lut[2]=0.0;
    r_lut[3]=0.0;
    r_lut_last[0]=0.0;
    r_lut_last[1]=0.0;

    g_lut[0]=0.0;
    g_lut[1]=1.0;
    g_lut[2]=1.0;
    g_lut[3]=1.0;
    g_lut_last[0]=0.0;
    g_lut_last[1]=0.0;

    b_lut[0]=0.0;
    b_lut[1]=0.0;
    b_lut[2]=0.0;
    b_lut[3]=1.0;
    b_lut_last[0]=1.0;
    b_lut_last[1]=1.0;

    a_lut[0]=1.0;
    a_lut[1]=1.0;
    a_lut[2]=1.0;
    a_lut[3]=1.0;
    a_lut_last[0]=1.0;
    a_lut_last[1]=1.0;

    sc_lut[0]=0.0;
    sc_lut[1]=1.0/5;
    sc_lut[2]=2.0/5;
    sc_lut[3]=3.0/5;
    sc_lut_last[0]=4.0/5;
    sc_lut_last[1]=5.0/5;

    glUniform4fv(loc_r_lut,1,r_lut);
    glUniform4fv(loc_g_lut,1,g_lut);
    glUniform4fv(loc_b_lut,1,b_lut);
    glUniform4fv(loc_a_lut,1,a_lut);
    glUniform4fv(loc_sc_lut,1,sc_lut);

    glUniform2fv(loc_r_lut_last,1,r_lut_last);
    glUniform2fv(loc_g_lut_last,1,g_lut_last);
    glUniform2fv(loc_b_lut_last,1,b_lut_last);
    glUniform2fv(loc_a_lut_last,1,a_lut_last);
    glUniform2fv(loc_sc_lut_last,1,sc_lut_last);

    glUniform4f(loc_low_clamp,r_lut[0],g_lut[0],b_lut[0],a_lut[0]);



    InVertex_loc = glGetAttribLocation(p_light_dir_map_scalars, "InVertex");
    InNormal_loc = glGetAttribLocation(p_light_dir_map_scalars, "InNormal");
    InScalar_loc = glGetAttribLocation(p_light_dir_map_scalars, "InScalar");
    //cout<<"locations "<<InVertex_loc<<" "<<InNormal_loc<<" "<<InScalar_loc<<endl;
    surfaces->setVertexAttribLocs(InVertex_loc,InNormal_loc, InScalar_loc);
    images->setVertexAttribLocs(InVertex_loc,InNormal_loc, InScalar_loc);
    surfaces->setColourTableUniformLocations(loc_r_lut,loc_g_lut,loc_b_lut,loc_a_lut, loc_sc_lut,loc_r_lut_last,loc_g_lut_last,loc_b_lut_last,loc_a_lut_last,loc_sc_lut_last,loc_low_clamp);


  //  surfaces->setVertexAttribLocs(0,1, 2);
  //**********************Directional Light Shader for Colour Bar********************//
/*
    v_cbar_map_scalars = glCreateShader(GL_VERTEX_SHADER);
    f_cbar_map_scalars = glCreateShader(GL_FRAGMENT_SHADER);
    //prt pixel shaders
    vs = textFileRead((QApplication::applicationDirPath() + "/glsl_shaders/colour_bar_map_scalars.vert").toAscii());
    fs = textFileRead((QApplication::applicationDirPath() + "/glsl_shaders/colour_bar_map_scalars.frag").toAscii());

    const char * vv_cbar = vs;
    const char * ff_cbar = fs;

    glShaderSource(v_cbar_map_scalars, 1, &vv_cbar,NULL);
    glShaderSource(f_cbar_map_scalars, 1, &ff_cbar,NULL);

    glCompileShader(v_cbar_map_scalars);
    glCompileShader(f_cbar_map_scalars);

    p_cbar_map_scalars = glCreateProgram();

    glAttachShader(p_cbar_map_scalars,v_cbar_map_scalars);
    glAttachShader(p_cbar_map_scalars,f_cbar_map_scalars);

    glBindAttribLocation(p_cbar_map_scalars,0,"InVertex");
    glBindAttribLocation(p_cbar_map_scalars,1,"InNormal");
    glBindAttribLocation(p_cbar_map_scalars,2,"InScalar");


    glLinkProgram(p_cbar_map_scalars);
    glUseProgram(p_cbar_map_scalars);

loc_r_lut = glGetUniformLocation(p_cbar_map_scalars,"r_lut");
    loc_g_lut = glGetUniformLocation(p_cbar_map_scalars,"g_lut");
    loc_b_lut = glGetUniformLocation(p_cbar_map_scalars,"b_lut");
    loc_sc_lut = glGetUniformLocation(p_cbar_map_scalars,"sc_lut");

    r_lut[0]=1.0;
    r_lut[1]=1.0;
    r_lut[2]=0.0;
    r_lut[3]=0.0;

    g_lut[0]=0.0;
    g_lut[1]=1.0;
    g_lut[2]=1.0;
    g_lut[3]=0.0;

    b_lut[0]=0.0;
    b_lut[1]=0.0;
    b_lut[2]=1.0;
    b_lut[3]=1.0;

    sc_lut[0]=0.0;
    sc_lut[1]=1.0/3;
    sc_lut[2]=2.0/3;
    sc_lut[3]=1.0;

    glUniform4fv(loc_r_lut,1,r_lut);
    glUniform4fv(loc_g_lut,1,g_lut);
  glUniform4fv(loc_b_lut,1,b_lut);
  glUniform4fv(loc_sc_lut,1,sc_lut);

  //  InVertex_loc = glGetAttribLocation(p_light_dir_map_scalars, "InVertex");
  //  InNormal_loc = glGetAttribLocation(p_light_dir_map_scalars, "InNormal");
   // InScalar_loc = glGetAttribLocation(p_light_dir_map_scalars, "InScalar");

    //surfaces->setVertexAttribLocs(0,1, 2);
    surfaces->setColourBarUniformLocations(loc_r_lut,loc_g_lut,loc_b_lut,loc_sc_lut);
*/
    //-------------------------shader for constant colour---------------//

    v_light_dir = glCreateShader(GL_VERTEX_SHADER);
    f_light_dir = glCreateShader(GL_FRAGMENT_SHADER);

    vs = textFileRead((QApplication::applicationDirPath() + "/glsl_shaders/surface_dir_light.vert").toUtf8().data());
    fs = textFileRead((QApplication::applicationDirPath() + "/glsl_shaders/surface_dir_light.frag").toUtf8().data());

    vv_light_dir = vs;
    ff_light_dir = fs;

    glShaderSource(v_light_dir, 1, &vv_light_dir,NULL);
    glShaderSource(f_light_dir, 1, &ff_light_dir,NULL);

    glCompileShader(v_light_dir);
    glCompileShader(f_light_dir);

    p_light_dir = glCreateProgram();

    glAttachShader(p_light_dir,v_light_dir);
    glAttachShader(p_light_dir,f_light_dir);

    glBindAttribLocation(p_light_dir,0,"InVertex");
    glBindAttribLocation(p_light_dir,1,"InNormal");

    glLinkProgram(p_light_dir);
    glUseProgram(p_light_dir);


    //**********************Texture Image Shader********************//

    vs = textFileRead((QApplication::applicationDirPath() + "/glsl_shaders/image_plane_texture.vert").toUtf8().data());
    fs = textFileRead((QApplication::applicationDirPath() + "/glsl_shaders/image_plane_texture.frag").toUtf8().data());

   // vs = textFileRead((QApplication::applicationDirPath() + "/glsl_shaders/image_plane_texture_with_vertex.vert").toAscii());
    //fs = textFileRead((QApplication::applicationDirPath() + "/glsl_shaders/image_plane_texture_with_vertex.frag").toAscii());

    v_im_texture = glCreateShader(GL_VERTEX_SHADER);
    f_im_texture = glCreateShader(GL_FRAGMENT_SHADER);
    const char * vv_im_texture = vs;
    const char * ff_im_texture = fs;
    glShaderSource(v_im_texture, 1, &vv_im_texture,NULL);
    glShaderSource(f_im_texture, 1, &ff_im_texture,NULL);
    glCompileShader(v_im_texture);
    glCompileShader(f_im_texture);

    p_im_texture = glCreateProgram();

    glAttachShader(p_im_texture,v_im_texture);
    glAttachShader(p_im_texture,f_im_texture);

    glLinkProgram(p_im_texture);
    glUseProgram(p_im_texture);

    //**********************Texture Image Shader ctr thresh********************//

    vs = textFileRead((QApplication::applicationDirPath() + "/glsl_shaders/image_plane_texture.vert").toUtf8().data());
    fs = textFileRead((QApplication::applicationDirPath() + "/glsl_shaders/image_plane_texture_ctrthresh.frag").toUtf8().data());

    v_im_texture_ctr = glCreateShader(GL_VERTEX_SHADER);
    f_im_texture_ctr = glCreateShader(GL_FRAGMENT_SHADER);
    const char * vv_im_texture_ctr = vs;
    const char * ff_im_texture_ctr = fs;
    glShaderSource(v_im_texture_ctr, 1, &vv_im_texture_ctr,NULL);
    glShaderSource(f_im_texture_ctr, 1, &ff_im_texture_ctr,NULL);
    glCompileShader(v_im_texture_ctr);
    glCompileShader(f_im_texture_ctr);

    p_im_texture_ctr = glCreateProgram();

    glAttachShader(p_im_texture_ctr,v_im_texture_ctr);
    glAttachShader(p_im_texture_ctr,f_im_texture_ctr);

    glLinkProgram(p_im_texture_ctr);
    glUseProgram(p_im_texture_ctr);




}

float norm( briview::float3  & v ) {
    float mag=std::sqrt(v.x*v.x +v.y*v.y + v.z*v.z) ;
    if (mag==0)
    {
        v.x=0;v.y=0;v.z=0;
    } else
    {
        float rmag=1/mag;
        v.x*=rmag; v.y*=rmag;v.z*=rmag;
    }
    return mag;
}

briview::float3 operator -(briview::float3& a , briview::float3 & b )
{
    return briview::float3(a.x-b.x, a.y-b.y, a.z-b.z);
}
briview::float3 operator +(briview::float3& a , briview::float3 & b )
{
    return briview::float3(a.x+b.x, a.y+b.y, a.z+b.z);
}

float operator *(briview::float3& a , briview::float3 & b )
{
    return a.x*b.x+a.y*b.y+a.z*b.z;
}
briview::float3 operator *(briview::float3& a , float & b )
{
    return briview::float3(a.x*b, a.y*b, a.z*b);
}

briview::float3 crossProd( const briview::float3  & v1, const briview::float3 & v2 ) {

    briview::float3 v3;
    v3.x = v1.y*v2.z - v1.z*v2.y;
    v3.y = v1.z*v2.x - v1.x*v2.z;
    v3.z = v1.x*v2.y - v1.y*v2.x;

    return v3;
}

void myOpenGLWidget::rotation(const float & theta, const float & phi )
{
    briview::float3 eye_camera,center_camera,vup_camera,viewSide;

    scene_props->getCameraParameters(eye_camera,center_camera,vup_camera);
    briview::float3 dir_proj = center_camera - eye_camera;
    float mag = norm(dir_proj);
    viewSide = crossProd(dir_proj,vup_camera);
    norm(viewSide);

    mouseInMotion=true;

  float phi_step=0.02,phi_rem=0;
  float theta_step=0.02,theta_rem=0;

  int nsteps=0;
//  int nsteps_phi=0;
 // int nsteps_theta=0;
    if (phi> theta)
  {
      nsteps = static_cast<int>(phi/abs(phi_step));
      theta_step = theta/nsteps;
      phi_rem = phi - nsteps*abs(phi_step);
        theta_rem = 0 ;
  } else{
       nsteps = static_cast<int>(theta/abs(theta_step));
        phi_step = phi/nsteps;

        theta_rem = theta - nsteps*abs(theta_step);
        phi_rem = 0;
   }

    //nsteps_theta = static_cast<int>(theta/abs(theta_step));


/*  if (abs(dif.x)>abs(dif.y))
  {
        if (dif.x<0)
            phi=-0.01;
        else
            phi=0.01;
        nsteps = static_cast<int>(PI/2.0/abs(phi));
        phi_rem = PI/2.0 - nsteps*abs(phi);
        if (dif.x<0) phi_rem*=-1;

    }else{
        if (dif.y<0)
            theta=-0.01;
        else
            theta=0.01;
        nsteps = static_cast<int>(PI/2.0/abs(theta));
        theta_rem = PI/2.0 - nsteps*abs(theta);
        if (dif.y<0) theta_rem*=-1;

    }
    */
    for (int i = 0 ; i< nsteps; i++)
    {
        // mag=1.0;
     //   //cout<<"step "<<i<<"/"<<nsteps<<endl;
      //  //cout<<"thetaphi "<<theta<<" "<<phi<<endl;
        scene_props->getCameraParameters(eye_camera,center_camera,vup_camera);
        dir_proj = center_camera - eye_camera;
        mag = norm(dir_proj);
        viewSide = crossProd(dir_proj,vup_camera);
        norm(viewSide);

        //------------------Elevation--------------------//
        float x = -mag*cosf(theta_step);
        float y = mag*sinf(theta_step);
        briview::float3 xp = (dir_proj*x);
        briview::float3 yp = (vup_camera*y);
        eye_camera = center_camera + xp;
        eye_camera = eye_camera +yp;

        //------------------Azimuth--------------------//
        y =  mag*sinf(phi_step);
        yp = (viewSide*y);

        eye_camera = eye_camera + yp;

        //------------------------------------------------//

        dir_proj=center_camera-eye_camera;
        viewSide=crossProd(dir_proj, vup_camera);
        norm(viewSide);
        vup_camera=crossProd(viewSide,dir_proj);
        norm(vup_camera);


        scene_props->setCameraParameters(eye_camera,center_camera,vup_camera);

        updateGL();
        // break;
    }

    scene_props->getCameraParameters(eye_camera,center_camera,vup_camera);
    dir_proj = center_camera - eye_camera;
    mag = norm(dir_proj);
    viewSide = crossProd(dir_proj,vup_camera);
    norm(viewSide);

    //------------------Elevation--------------------//
    float x = -mag*cosf(theta_rem);
    float y = mag*sinf(theta_rem);
    briview::float3 xp = (dir_proj*x);
    briview::float3 yp = (vup_camera*y);
    eye_camera = center_camera + xp;
    eye_camera = eye_camera +yp;

    //------------------Azimuth--------------------//
    y =  mag*sinf(phi_rem);
    yp = (viewSide*y);

    eye_camera = eye_camera + yp;

    //------------------------------------------------//

    dir_proj=center_camera-eye_camera;
    viewSide=crossProd(dir_proj, vup_camera);
    norm(viewSide);
    vup_camera=crossProd(viewSide,dir_proj);
    norm(vup_camera);


    scene_props->setCameraParameters(eye_camera,center_camera,vup_camera);

    updateGL();

}

void myOpenGLWidget::translateAndRotate(const float & tx, const float & ty, const float & tz, const float & theta, const float & phi )
{
//no steps
    briview::float3 eye_camera,center_camera,vup_camera,viewSide;

    scene_props->getCameraParameters(eye_camera,center_camera,vup_camera);
    briview::float3 dir_proj = center_camera - eye_camera;
    float mag = norm(dir_proj);
    viewSide = crossProd(dir_proj,vup_camera);
    norm(viewSide);

    scene_props->getCameraParameters(eye_camera,center_camera,vup_camera);

    //-----Translations----------//
    //do translation first
    eye_camera.x+=tx;
    eye_camera.y+=ty;
    eye_camera.z+=tz;
    //---------------------------//


    dir_proj = center_camera - eye_camera;
    mag = norm(dir_proj);
    viewSide = crossProd(dir_proj,vup_camera);
    norm(viewSide);



cout<<"thetaphi "<<theta<<" "<<phi<<endl;
    //------------------Elevation--------------------//
    float x = -mag*cosf(theta);
    float y = mag*sinf(theta);
    briview::float3 xp = (dir_proj*x);
    briview::float3 yp = (vup_camera*y);
    eye_camera = center_camera + xp;
    eye_camera = eye_camera +yp;

    //------------------Azimuth--------------------//
    y =  mag*sinf(phi);
    yp = (viewSide*y);

    eye_camera = eye_camera + yp;

    //------------------------------------------------//

    dir_proj=center_camera-eye_camera;
    viewSide=crossProd(dir_proj, vup_camera);
    norm(viewSide);
    vup_camera=crossProd(viewSide,dir_proj);
    norm(vup_camera);


    scene_props->setCameraParameters(eye_camera,center_camera,vup_camera);

    updateGL();

}

void myOpenGLWidget::mouseMoveEvent ( QMouseEvent * event )
{
    if ((!mouseInMotion)|| (mouseMode!=5))
    {
        //mouseInMotion=true;
    //    //cout<<"mouse in motion count "<<mouseInMotionCount<<endl;

    float2 dif;
    briview::float3 eye_camera,center_camera,vup_camera,viewSide;

    dif.x=(event->x()-prevLoc.x);
    dif.y=-(event->y()-prevLoc.y);

    if ((mouseMode!=5) || (mouseInMotionCount==0))
    {
        prevLoc.x=event->x();
        prevLoc.y=event->y();
    }
    mouseInMotionCount++;

    float l=sqrt(dif.x*dif.x+dif.y*dif.y);

    if (l!=0)
    {
    dif.x*=mouseSensitivity/l;
    dif.y*=mouseSensitivity/l;



    scene_props->getCameraParameters(eye_camera,center_camera,vup_camera);
    briview::float3 dir_proj = center_camera - eye_camera;
    float mag = norm(dir_proj);
    viewSide = crossProd(dir_proj,vup_camera);
    norm(viewSide);

    if (mouseMode==0)
    {
        //translation
        briview::float3 v_tr = viewSide * dif.x;
        briview::float3 dy=vup_camera* dif.y;
        v_tr = v_tr + dy;
        eye_camera=eye_camera+v_tr;
        center_camera=center_camera+v_tr;
    }else if (mouseMode==1)
    {//zoom
        //cout<<"dify "<<dif.y<<" "<<mouseSensitivity<<" "<<mag<<endl;
        if ((dif.y<0)&&(mouseSensitivity<mag))
        {
            eye_camera.x+=dir_proj.x*mouseSensitivity;
            eye_camera.y+=dir_proj.y*mouseSensitivity;
            eye_camera.z+=dir_proj.z*mouseSensitivity;

        }else if (dif.y>0)
        {
            eye_camera.x-=dir_proj.x*mouseSensitivity;
            eye_camera.y-=dir_proj.y*mouseSensitivity;
            eye_camera.z-=dir_proj.z*mouseSensitivity;
        }

    }else if (mouseMode==2)
    {
        //rotation

        float theta=0.01*mouseSensitivity;

        if (event->x()<width/2.0f){
            if (dif.y>0)
                theta*=-1;
        }else{
            if (dif.y<0)
                theta*=-1;
        }

        float y=cosf(theta);
        float x=sinf(theta);
        briview::float3 xp=(viewSide*x);
        briview::float3 yp=(vup_camera*y);
        vup_camera = xp+yp ;


    }else if (mouseMode==3)
    {
//rotation
           float theta=dif.y*0.001*mouseSensitivity;
           float phi=dif.x*0.001*mouseSensitivity;

        //------------------Elevation--------------------//
        float x = -mag*cosf(theta);
        float y = mag*sinf(theta);
        briview::float3 xp = (dir_proj*x);
        briview::float3 yp = (vup_camera*y);
        eye_camera = center_camera + xp;
        eye_camera = eye_camera +yp;

        //------------------Azimuth--------------------//
        y =  mag*sinf(phi);
        yp = (viewSide*y);

        eye_camera = eye_camera + yp;

        //------------------------------------------------//

        dir_proj=center_camera-eye_camera;
        viewSide=crossProd(dir_proj, vup_camera);
        norm(viewSide);
        vup_camera=crossProd(viewSide,dir_proj);
        norm(vup_camera);

    }else if (mouseMode==4)
    {
//rotation
        float theta=dif.y*0.001*mouseSensitivity;
        float phi=dif.x*0.001*mouseSensitivity;
        //------------------Pitch--------------------//
        float x = mag*cosf(theta);
        float y = -mag*sinf(theta);
        briview::float3 xp = (dir_proj*x);
        briview::float3 yp = (vup_camera*y);
        center_camera = eye_camera + xp;
        center_camera = center_camera +yp;
        //------------------Azimuth--------------------//
        y =  -mag*sinf(phi);
        yp = (viewSide*y);

        center_camera = center_camera + yp;

        //------------------------------------------------//

        dir_proj=center_camera-eye_camera;
        viewSide=crossProd(dir_proj, vup_camera);
        norm(viewSide);
        vup_camera=crossProd(viewSide,dir_proj);
        norm(vup_camera);
    }else if ( (mouseMode==5) && (mouseInMotionCount == 10) )
    {
        mouseInMotion=true;

      float phi=0,phi_rem=0;
      float theta=0,theta_rem=0;
      int nsteps=0;
      if (abs(dif.x)>abs(dif.y))
      {
            if (dif.x<0)
                phi=-0.01;
            else
                phi=0.01;
            nsteps = static_cast<int>(PI/2.0/abs(phi));
            phi_rem = PI/2.0 - nsteps*abs(phi);
            if (dif.x<0) phi_rem*=-1;

        }else{
            if (dif.y<0)
                theta=-0.01;
            else
                theta=0.01;
            nsteps = static_cast<int>(PI/2.0/abs(theta));
            theta_rem = PI/2.0 - nsteps*abs(theta);
            if (dif.y<0) theta_rem*=-1;

        }
        for (int i = 0 ; i< nsteps; i++)
        {
            // mag=1.0;
         //   //cout<<"step "<<i<<"/"<<nsteps<<endl;
          //  //cout<<"thetaphi "<<theta<<" "<<phi<<endl;
            scene_props->getCameraParameters(eye_camera,center_camera,vup_camera);
            dir_proj = center_camera - eye_camera;
            mag = norm(dir_proj);
            viewSide = crossProd(dir_proj,vup_camera);
            norm(viewSide);

            //------------------Elevation--------------------//
            float x = -mag*cosf(theta);
            float y = mag*sinf(theta);
            briview::float3 xp = (dir_proj*x);
            briview::float3 yp = (vup_camera*y);
            eye_camera = center_camera + xp;
            eye_camera = eye_camera +yp;

            //------------------Azimuth--------------------//
            y =  mag*sinf(phi);
            yp = (viewSide*y);

            eye_camera = eye_camera + yp;

            //------------------------------------------------//

            dir_proj=center_camera-eye_camera;
            viewSide=crossProd(dir_proj, vup_camera);
            norm(viewSide);
            vup_camera=crossProd(viewSide,dir_proj);
            norm(vup_camera);


            scene_props->setCameraParameters(eye_camera,center_camera,vup_camera);

            updateGL();
            // break;
        }

        scene_props->getCameraParameters(eye_camera,center_camera,vup_camera);
        dir_proj = center_camera - eye_camera;
        mag = norm(dir_proj);
        viewSide = crossProd(dir_proj,vup_camera);
        norm(viewSide);

        //------------------Elevation--------------------//
        float x = -mag*cosf(theta_rem);
        float y = mag*sinf(theta_rem);
        briview::float3 xp = (dir_proj*x);
        briview::float3 yp = (vup_camera*y);
        eye_camera = center_camera + xp;
        eye_camera = eye_camera +yp;

        //------------------Azimuth--------------------//
        y =  mag*sinf(phi_rem);
        yp = (viewSide*y);

        eye_camera = eye_camera + yp;

        //------------------------------------------------//

        dir_proj=center_camera-eye_camera;
        viewSide=crossProd(dir_proj, vup_camera);
        norm(viewSide);
        vup_camera=crossProd(viewSide,dir_proj);
        norm(vup_camera);

  }
    scene_props->setCameraParameters(eye_camera,center_camera,vup_camera);

    updateGL();
}
  //  mouseInMotion=false;

}
}

void myOpenGLWidget::setSceneProperties(scene_properties** scene)
{
    scene_props=*scene;
}

void myOpenGLWidget::setSurfaceContainer(SurfaceContainer** surf_in)
{
    surfaces=*surf_in;
}

void  myOpenGLWidget::setCameraPositionToSurface(const int & index)
{
    briview::float3 cog=surfaces->getSurfaceCOG(index);
    briview::float3 eye= briview::float3(cog.x,cog.y-300,cog.z);
    scene_props->setCameraCenter(cog);
    scene_props->setCameraEye(eye);

}



void myOpenGLWidget::mouseReleaseEvent ( QMouseEvent * event )
{
  //  //cout<<"mouse release "<<hasMouseTracking()<<endl;
    mouseInMotion=false;
    mouseInMotionCount=0;
}


void myOpenGLWidget::mousePressEvent ( QMouseEvent * event )
{
  //  //cout<<"mouse press "<<hasMouseTracking()<<endl;
  //  prevLoc.x=event->x();
  //  prevLoc.y=event->y();

}


void myOpenGLWidget::setGLSLColorTable(const colour_table & ctab)
{
    glUniform4fv(loc_r_lut,1,ctab.r_lut);
    glUniform4fv(loc_g_lut,1,ctab.g_lut);
    glUniform4fv(loc_b_lut,1,ctab.b_lut);
    glUniform4fv(loc_sc_lut,1,ctab.sc_lut);

}


void myOpenGLWidget::initializeGL()
{
    if (QGLFormat::OpenGL_Version_3_0)
        cout<<"openGL 3.0 is supported"<<endl;
    else if (QGLFormat::OpenGL_Version_2_1)
        cout<<"openGL 2.1 is supported"<<endl;
    else if (QGLFormat::OpenGL_Version_2_0)
        cout<<"openGL 2.0 is supported"<<endl;
    else
    {
        cerr<<"Software has not been tested below openGL 3.0"<<endl;
        exit (EXIT_FAILURE);
    }
#ifdef GL_GLEXT_PROTOTYPES

    glGenFramebuffersEXT(1,&fbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

    glGenRenderbuffersEXT(NumRenderbuffers, rbo);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,rbo[Color]);
    //glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT,256,256);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_RGBA,256,256);

    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

    if  (status!=GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        cerr<<"Incomplete Framebuffer Object1"<<endl;
        const char *err_str = 0;
        char buf[80];
        //cout<<"status "<<status<<endl;
        switch ( status )
        {
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            err_str = "UNSUPPORTED";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
            err_str = "INCOMPLETE ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            err_str = "INCOMPLETE DRAW BUFFER";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            err_str = "INCOMPLETE READ BUFFER";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            err_str = "INCOMPLETE MISSING ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT:
            err_str = "INCOMPLETE MULTISAMPLE";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT:
            err_str = "INCOMPLETE LAYER TARGETS";
            break;

            // Removed in version #117 of the EXT_framebuffer_object spec
            //case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT:

        default:
            sprintf( buf, "0x%x", status );
            err_str = buf ;
            break;
        }
        //  exit (EXIT_FAILURE);
    }


    //cout<<"color "<<rbo[Color]<<endl;
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_RENDERBUFFER_EXT ,rbo[Color]);
    glBindRenderbufferEXT(GL_FRAMEBUFFER_EXT,rbo[Depth]);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_RGBA,256,256);

    //	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT24_ARB,256,256);
    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    //cout<<"status "<<status<<endl;

    if  (status!=GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        //cout<<"status "<<status<<endl;

        cerr<<"Incomplete Framebuffer Object2"<<endl;
        exit (EXIT_FAILURE);
    }


    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT ,rbo[Depth]);

    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

    if  (status!=GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        const char *err_str = 0;
        char buf[80];
        switch ( status )
        {
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            err_str = "UNSUPPORTED";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
            err_str = "INCOMPLETE ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            err_str = "INCOMPLETE DRAW BUFFER";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            err_str = "INCOMPLETE READ BUFFER";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            err_str = "INCOMPLETE MISSING ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT:
            err_str = "INCOMPLETE MULTISAMPLE";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT:
            err_str = "INCOMPLETE LAYER TARGETS";
            break;

            // Removed in version #117 of the EXT_framebuffer_object spec
            //case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT:

        default:
            sprintf( buf, "0x%x", status );
            err_str = buf ;
            break;
        }

        cerr<<"Incomplete Framebuffer Object4 "<<status<<endl;
        exit (EXIT_FAILURE);
    }
#else
//cout<<"in the else "<<endl;
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenRenderbuffers(NumRenderbuffers, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER,rbo[Color]);

    glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA,256,256);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER ,rbo[Color]);
    glBindRenderbuffer(GL_RENDERBUFFER,rbo[Depth]);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,256,256);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER ,rbo[Depth]);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if  (status!=GL_FRAMEBUFFER_COMPLETE)
    {
        cerr<<"Incomplete Framebuffer Object"<<endl;
        exit (EXIT_FAILURE);
    }

#endif




    glEnable(GL_TEXTURE_3D);

    glClearColor(0.0,0.0,0.0,0.0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0f);
    glDepthMask(GL_TRUE);

 //   glEnable(GL_CULL_FACE);
  //  glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);


    glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glAlphaFunc ( GL_GREATER, 0.01 ) ;
    glEnable ( GL_ALPHA_TEST ) ;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT );
    //glFrontFace(GL_CW);
    glFrontFace(GL_CCW);

    setShaders();

    vector<GLuint> progs;
    progs.push_back(p_light_dir);
    progs.push_back(p_light_dir_map_scalars);
    //  progs.push_back(p_cbar_map_scalars);

    surfaces->setGLSLPrograms(progs);
    //surfaces->setGLSLProgramsColourBar(progs);
    graph_->setGLSLPrograms(progs);
    images->setGLSLProgram(p_im_texture);
    images->setGLSLProgramCbar(p_light_dir_map_scalars);





#ifdef GL_GLEXT_PROTOTYPES
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
#else
    glBindFramebufferEXT(GL_FRAMEBUFFER, fbo);

#endif
}

void myOpenGLWidget::resizeGL(int w, int h)
{
    // setup viewport, projection etc.:
    width=w;
    height=h;

#ifdef GL_GLEXT_PROTOTYPES 

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,rbo[Depth]);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT,w,h);

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,rbo[Color]);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_RGBA,w,h);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
#else
	glBindRenderbuffer(GL_RENDERBUFFER,rbo[Depth]);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,w,h);
	
    glBindRenderbuffer(GL_RENDERBUFFER,rbo[Color]);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA,w,h);
	
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
#endif
	
	
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    scene_props->setWindowSize(w,h);
    scene_props->setAspectRatio();
    scene_props->setProjectionProperties();

#ifdef GL_GLEXT_PROTOTYPES 
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#endif
    cout<<"done iniatlize GL"<<endl;
}



void myOpenGLWidget::paintGL()
{
    cout<<"paintGL"<<endl;
#ifdef GL_GLEXT_PROTOTYPES 
    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, fbo);
#else
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

#endif
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    scene_props->setLights();

    scene_props->setCameraOrientation();
    // scene_props->setLights();
    //-----RENDER Opaque OBJECTS------------//

    //glDepthMask(GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
     images->renderSlices();

   // glUseProgram(p_light_dir_map_scalars);
    //images->renderColourBar();
//cout<<"p_light_dir "<<p_light_dir_map_scalars<<endl;
   // glDisable(GL_DEPTH_TEST);

    //glUseProgram(p_light_dir_map_scalars);
images->renderColourBar();
    //render command handles the progam setting

//  HERE  surfaces->renderOpaqueSurfaces();
    surfaces->renderOpaqueSurfaces();
cout<<"render surfa es"<<endl;
    surfaces->renderOpaqueGlyphs();
    glUseProgram(p_light_dir_map_scalars);
    cout<<"render colour bar"<<endl;
    surfaces->renderColourBar();
    //cout<<"done rende cbar"<<endl;

    //-----RENDER TRANSLUCENT OBJECTS------------//
    // glDisable(GL_DEPTH_TEST);
    glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glDepthMask(GL_FALSE);
    surfaces->renderTranslucentSurfaces();
    surfaces->renderTranslucentGlyphs();
cout<<"render graph"<<endl;
    if (graph_->doRender())
       graph_->render();
    cout<<"done render graph"<<endl;


    glDepthMask(GL_TRUE);


#ifdef GL_GLEXT_PROTOTYPES 
    glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, fbo);
    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
    glBlitFramebufferEXT(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
#else
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
#endif


}
