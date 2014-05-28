#ifndef IMAGEMANIPULATOR_H
#define IMAGEMANIPULATOR_H

#include <QDockWidget>
#include <briview_structs.h>
#include "fslsurface/fslsurface_structs.h"
namespace Ui {
    class ImageManipulator_form;
}

class ImageManipulator : public QDockWidget {
    Q_OBJECT
    Q_DISABLE_COPY(ImageManipulator)
public:
            explicit ImageManipulator(QWidget *parent = 0);
    virtual ~ImageManipulator();
    void addItemToList(const std::string & filename);

    void setSlidersExtents(const int & xsize, const int & ysize, const int & zsize);
    void setSlidersPosition(const float & x, const float & y, const float & z,const float & xdim,const float & ydim,const float & zdim);
    void setSlicesVisibility(const fslsurface_name::bool3 & vis );
    void setImageOpacity(const float & op);
    void setImageOpacitySlider(const float & op);
    void setCoordinateSpace( const int & index );

    int getCurrentImageIndex();
    std::vector<int> getCurrentImageIndices();
    QString getImageName(const int & index );
    QString getCurrentImageName();

    int getNumberOfImages();
    void toggleImage(const bool & state);

  //  void updatePageText();
void setColourTableIndex( const int & index);
  //  void setImageMin(const float & min);
 //   void setImageMax(const float & max);
    void addColourTable(const std::string & filename );
//    void setColourTable(const float & rmin, const float & gmin, const float & bmin, const float & rmax, const float & gmax, const float & bmax );
  //  void setColourTable(const GLfloat *cmap);

    void getColourBarGeometry(briview::float3 & origin, float & length, float & width );
    bool getColourBarVisibility();
    std::string getColourBarTitle();

    fslsurface_name::colour_table getCurrentColourTable();
    void setColourTable( const fslsurface_name::colour_table & ctable );
    void setColourTableNoSc( const fslsurface_name::colour_table & ctable );

public slots:


    void showDockImageNavigator();
    void showDockProperties();
    void showDockColourBar();
    void showDockCoordinateSystem();
    void showDockBlending();
    void showDockImList();

    void doDockImageNavigator();
    void doDockProperties();
    void doDockColourBar();
    void doDockCoordinateSystem();
    void doDockBlending();
    void doDockImList();

void slot_currentImageChanged(int index );
    void slot_toggleImage(bool state);
  //  void slot_toggleImage();

    void setImageOpacitySlider(QString op);
    void setImageOpacityFromSlider(int op);
        void setImageOpacityFromSlider();


    void changeXVoxel(QString num);
    void changeYVoxel(QString num);
    void changeZVoxel(QString num);
   void changeXmm(QString num);
    void changeYmm(QString num);
    void changeZmm(QString num);


    void shiftImageUpList();
   void shiftImageDownList();
   // void nextPage();
 //   void prevPage();

   void decreaseContrast();
   void increaseContrast();
   void decreaseLevel();
   void increaseLevel();


protected:
    virtual void changeEvent(QEvent *e);
signals:
    void sig_changedSliceX(int);
    void sig_changedSliceY(int);
    void sig_changedSliceZ(int);
    void sig_changedSliceX(float);
    void sig_changedSliceY(float);
    void sig_changedSliceZ(float);


    void sig_toggledSliceX(bool);
    void sig_toggledSliceY(bool);
    void sig_toggledSliceZ(bool);

    void sig_changedImageSpace(int);

    void sig_changedCurrentImage(int);
    void sig_changedBlendFunc(int);
    void sig_changedOpacityMode(int);
    void sig_changedColourTable(int);
    void sig_changedColourTable();
    void sig_changedColourTableSc();
    void sig_swapImages(int);

    void sig_changedColourBar();
 //   void sig_incContrast();
 //   void sig_decContrast();
 //   void sig_incLevel();
 //   void sig_decLevel();


//    void sig_decreaseMinIntensity();
//    void sig_increaseMinIntensity();
//    void sig_decreaseMaxIntensity();
 //   void sig_increaseMaxIntensity();//
//    void sig_changedMaxIntensity(QString);
//    void sig_changedMinIntensity(QString);
    void sig_changedOpacity(QString);
    void sig_toggled_ImOnOff(bool);




private:

    Ui::ImageManipulator_form *ui;
};

#endif // IMAGEMANIPULATOR_H
