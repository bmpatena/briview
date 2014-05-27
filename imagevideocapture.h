#ifndef IMAGEVIDEOCAPTURE_H
#define IMAGEVIDEOCAPTURE_H

#include <QDialog>
#include <vector>
#include <briview_structs.h>
#include "scene_properties.h"

namespace Ui {
    class ImageVideoCapture_form;
}



class ImageVideoCapture : public QDialog
{
    Q_OBJECT

public:
    enum CompressionType { NOT_USED0, NONE, NOT_USED, NOT_USED2, NOT_USED3, LZW, NOT_USED4, JPEG_DCT };

    explicit ImageVideoCapture(QWidget *parent = 0);
    ~ImageVideoCapture();

    void setSceneWidget( briview::scene_properties* scene );
    std::vector< briview::animationParams > getAnimationList();
    unsigned int getFPS();

    void setImageExtents(const int & xsize, const int & ysize, const int & zsize);
    QString getMovieName();

public slots:
    void setFileName();
    void setMovieFileName();

    void changeCompressionType( int index );
    void changeNumberOfOrthoViews( int index );

    void getImageProperties(std::string & filename, CompressionType & c , int & numViews );
    void getMovieProperties(std::string & filename, unsigned int & fps, float & duration, float & tx, float & ty, float & tz, float & theta, float & phi );

    void prev_movie();
    void write_movie();
    void addStartPosition();
    void addCameraMotion();
    void addSliceMotion();
    void deleteAnimation();

    void changeXSlider1(int num);
    void changeYSlider1(int num);
    void changeZSlider1(int num);
    void changeXSlider2(int num);
    void changeYSlider2(int num);
    void changeZSlider2(int num);
    void changeX1Voxel(QString num);
    void changeY1Voxel(QString num);
    void changeZ1Voxel(QString num);
    void changeX2Voxel(QString num);
    void changeY2Voxel(QString num);
    void changeZ2Voxel(QString num);

protected:
    void changeEvent(QEvent *e);

signals:

    void sig_takeSnapShot();

    void sig_mov(bool write);

private:

    briview::scene_properties* scene_widget;

    std::vector< briview::animationParams > v_animation;
    Ui::ImageVideoCapture_form *ui;
    CompressionType comp_type;
    int n_orthoViews;
};

#endif // IMAGEVIDEOCAPTURE_H
