#include "imagevideocapture.h"
#include "ui_imagevideocapture.h"
#include <QFileDialog>
#include <iostream>
#include <mp4v2/mp4v2.h>
using namespace fslsurface_name;
using namespace briview;
using namespace std;
ImageVideoCapture::ImageVideoCapture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageVideoCapture_form)
{
    ui->setupUi(this);

    connect(ui->filename_but,SIGNAL(pressed()),this,SLOT(setFileName()));
    connect(ui->filename_but_2,SIGNAL(pressed()),this,SLOT(setMovieFileName()));

    connect(ui->comp_combo,SIGNAL(activated(int)),this,SLOT(changeCompressionType(int)));
    connect(ui->nviews_combo,SIGNAL(activated(int)),this,SLOT(changeNumberOfOrthoViews(int)));

    connect(ui->snapshot_but,SIGNAL(pressed()),this,SIGNAL(sig_takeSnapShot()));

    connect(ui->mov_prev,SIGNAL(pressed()),this,SLOT(prev_movie()));
    connect(ui->mov_write,SIGNAL(pressed()),this,SLOT(write_movie()));

    connect(ui->add_start,SIGNAL(pressed()),this,SLOT(addStartPosition()));
    connect(ui->add_camera_motion,SIGNAL(pressed()),this,SLOT(addCameraMotion()));
    connect(ui->add_slice_motion,SIGNAL(pressed()),this,SLOT(addSliceMotion()));
    connect(ui->del_anim  ,SIGNAL(pressed()),this,SLOT(deleteAnimation()));

  connect(ui->x_slider,SIGNAL(sliderMoved(int)),this, SLOT(changeXSlider1(int)));
  connect(ui->x_slider_2,SIGNAL(sliderMoved(int)),this, SLOT(changeXSlider2(int)));
  connect(ui->y_slider,SIGNAL(sliderMoved(int)),this, SLOT(changeYSlider1(int)));
  connect(ui->y_slider_2,SIGNAL(sliderMoved(int)),this, SLOT(changeYSlider2(int)));
  connect(ui->z_slider,SIGNAL(sliderMoved(int)),this, SLOT(changeZSlider1(int)));
  connect(ui->z_slider_2,SIGNAL(sliderMoved(int)),this, SLOT(changeZSlider2(int)));



    comp_type=NONE;
    n_orthoViews=1;
    //filename=ui->filename_edit->text()->toStdString();
}

ImageVideoCapture::~ImageVideoCapture()
{
    delete ui;
}
void ImageVideoCapture::setImageExtents(const int & xsize, const int & ysize, const int & zsize)
{
    ui->x_slider->setRange(0,xsize-1);
    ui->y_slider->setRange(0,ysize-1);
    ui->z_slider->setRange(0,zsize-1);
    ui->x_slider_2->setRange(0,xsize-1);
    ui->y_slider_2->setRange(0,ysize-1);
    ui->z_slider_2->setRange(0,zsize-1);
}


std::vector< briview::animationParams > ImageVideoCapture::getAnimationList()
{
    return v_animation;
}

void ImageVideoCapture::prev_movie()
{

    emit sig_mov(false);
}
void ImageVideoCapture::write_movie()
{
    emit sig_mov(true);
}
void ImageVideoCapture::setSceneWidget( briview::scene_properties* scene )
{
    scene_widget = scene;

}

void ImageVideoCapture::addStartPosition()
{
    std::cout<<"add start"<<std::endl;
    //v_animation
    ui->anim_list->addItem("Set Camera Orientation");
    briview::float3 eye(0,0,0),centre(0,0,0),vup(0,0,0);
     scene_widget->getCameraParameters(eye,centre,vup);
    std::cout<<eye.x<<" "<<eye.y<<" "<<eye.z<<std::endl;
    std::cout<<centre.x<<" "<<centre.y<<" "<<centre.z<<std::endl;
    std::cout<<vup.x<<" "<<vup.y<<" "<<vup.z<<std::endl;
    float duration = ui->duration->text().toFloat();
    v_animation.push_back(animationParams(eye,centre,vup,duration));
}

void ImageVideoCapture::addCameraMotion()
{
    std::cout<<"add camera motion"<<std::endl;
    //v_animation
    ui->anim_list->addItem("Camera Motion");
    float  duration;
    float  tx,ty, tz, theta,phi;
    string filename;
    unsigned int fps;
    getMovieProperties( filename, fps, duration, tx,ty,  tz,  theta,  phi );
    cout<< duration<<" "<<tx<<" "<<ty<<" "<<tz<<" "<<theta<<" "<<phi <<endl;

    v_animation.push_back(animationParams( tx,ty,tz,theta,phi,duration));
}
void ImageVideoCapture::deleteAnimation()
{
    int index = ui->anim_list->currentRow();
    v_animation.erase(v_animation.begin()+index);
    ui->anim_list->takeItem( index );

}

unsigned int ImageVideoCapture::getFPS(){
    return static_cast<unsigned int>(ui->fps->text().toInt());
}


void ImageVideoCapture::addSliceMotion()
{
    std::cout<<"add slice motion "<<ui->x_voxel->text().toFloat()<<" "<< ui->y_voxel->text().toFloat()<<" "<<ui->z_voxel->text().toFloat()<<std::endl;
    //v_animation
    ui->anim_list->addItem("Slice Motion");

    float duration = ui->duration->text().toFloat();
    v_animation.push_back(animationParams( ui->x_voxel->text().toFloat(),  ui->y_voxel->text().toFloat(),  ui->z_voxel->text().toFloat(),  ui->x_onoff->isChecked(),  ui->y_onoff->isChecked(),  ui->z_onoff->isChecked(), \
                                           ui->x_voxel_2->text().toFloat(),  ui->y_voxel_2->text().toFloat(),  ui->z_voxel_2->text().toFloat(),  ui->x_onoff_2->text().toFloat(),  ui->y_onoff_2->isChecked(),  ui->z_onoff_2->isChecked(), duration));
    std::cout<<"end slice motion"<<std::endl;

}
void ImageVideoCapture::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ImageVideoCapture::getImageProperties(std::string & filename, CompressionType & c , int & numViews )
{
    filename = ui->filename_edit->text().toStdString();
    c=comp_type;
    numViews = n_orthoViews;
    std::cout<<"get image params "<<filename<<" "<<numViews<<" "<<c<<std::endl;

}
void ImageVideoCapture::getMovieProperties(std::string & filename, unsigned int & fps, float & duration, float & tx, float & ty, float & tz, float & theta, float & phi )
{
    filename = ui->movie_name->text().toStdString();
    fps = ui->fps->value();
    QString stemp= ui->duration->text();
    duration=stemp.toFloat();

    stemp= ui->fps->text();
    fps=stemp.toFloat();

    stemp= ui->tx->text();
    tx=stemp.toFloat();

    stemp= ui->ty->text();
    ty=stemp.toFloat();

    stemp= ui->tz->text();
    tz=stemp.toFloat();

    stemp= ui->theta->text();
    theta=stemp.toFloat();

    stemp= ui->phi->text();
    phi=stemp.toFloat();

}

void ImageVideoCapture::changeNumberOfOrthoViews( int index )
{
    switch (index){

    case 0:
        n_orthoViews = 1;
        break;
    case 1:
        n_orthoViews = 4;
        break;
    case 2:
        n_orthoViews = 6;
        break;
    default:
        break;

    }

}

void ImageVideoCapture::changeCompressionType( int index )
{
    //cout<<"change compression type"<<endl;
    switch (index){

    case 0:
        comp_type = NONE;
        break;
    case 1:
        comp_type = LZW;
        break;
    case 2:
        comp_type = JPEG_DCT;
        break;
    default:
        break;

    }

}

QString ImageVideoCapture::getMovieName()
{
    return ui->movie_name->text();
}

void ImageVideoCapture::setMovieFileName()
{
    QString filename = QFileDialog::getSaveFileName(this,"Choose Movie Name...","brimovie",tr("Scenes (*.mp4)"));

    if (!filename.isNull())
    {
        ui->movie_name->setText(filename);
    }
    ui->filename_but_2->setDown(false);
}
void ImageVideoCapture::setFileName()
{
    QString filename = QFileDialog::getSaveFileName(this,"Choose Image Name...","image",tr("Scenes (*.tif)"));

    if (!filename.isNull())
    {
        ui->filename_edit->setText(filename);
    }
    ui->filename_but_2->setDown(false);
}

void ImageVideoCapture::changeXSlider1(int num)
{
    QString stemp;
    stemp.setNum(num);
    ui->x_voxel->setText(stemp);
}

void ImageVideoCapture::changeYSlider1(int num)
{
    QString stemp;
    stemp.setNum(num);
    ui->y_voxel->setText(stemp);
}

void ImageVideoCapture::changeZSlider1(int num)
{
    QString stemp;
    stemp.setNum(num);
    ui->z_voxel->setText(stemp);
}
void ImageVideoCapture::changeXSlider2(int num)
{
    QString stemp;
    stemp.setNum(num);
    ui->x_voxel_2->setText(stemp);
}

void ImageVideoCapture::changeYSlider2(int num)
{
    QString stemp;
    stemp.setNum(num);
    ui->y_voxel_2->setText(stemp);
}

void ImageVideoCapture::changeZSlider2(int num)
{
    QString stemp;
    stemp.setNum(num);
    ui->z_voxel_2->setText(stemp);
}
void ImageVideoCapture::changeX1Voxel(QString num)
{
    ui->x_slider->setValue(num.toInt());
}
void ImageVideoCapture::changeY1Voxel(QString num)
{
    ui->y_slider->setValue(num.toInt());

}
void ImageVideoCapture::changeZ1Voxel(QString num)
{
    ui->z_slider->setValue(num.toInt());

}
void ImageVideoCapture::changeX2Voxel(QString num)
{
    ui->x_slider_2->setValue(num.toInt());

}
void ImageVideoCapture::changeY2Voxel(QString num)
{
    ui->y_slider_2->setValue(num.toInt());

}
void ImageVideoCapture::changeZ2Voxel(QString num)
{
    ui->z_slider_2->setValue(num.toInt());

}
