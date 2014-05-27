#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H
#include <QMouseEvent>
#include <QWidget>
#include <QtOpenGL>

#include <iostream>
#include "fslsurface/fslsurface_structs.h"
#include <briview_structs.h>
#include "surfacecontainer.h"
#include "imagecontainer.h"
#include "scene_properties.h"
#include "tiffio.h"

class gluDockWidget;

using namespace std;

int writeTIFF(const unsigned int &  width, const unsigned int & height, const string & filename  , const unsigned int & compression);


class myOpenGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    myOpenGLWidget(QWidget *parent);

    ~myOpenGLWidget() ;

    void setCameraPositionToSurface(const int & index);
    void setSurfaceContainer(briview::SurfaceContainer** surf_in);
    void setImageContainer(briview::ImageContainer** images_in) { images=*images_in;}
    void setSceneProperties(briview::scene_properties** scene);
    void setGLSLColorTable(const fslsurface_name::colour_table & ctab);
    void setMouseSensitivity(int sens) { mouseSensitivity=sens/10.0; }
    void setMouseMode(int mode) { mouseMode=mode; }

    int animateCameraMotion( const bool & write, const unsigned int & compression, const briview::animationParams & par_anim , const float & fps, unsigned int & frame);
    int animateSliceMotion( const bool & write, const unsigned int & compression, const briview::animationParams & par_anim , const float & fps, unsigned int & frame);

    int animate( const bool & write,const unsigned int & compression, const briview::animationParams & par_anim, const float & fps,  unsigned int & i_frame );
    int renderCaptureList(const string & filename, const bool & write, const unsigned int & compression, const std::vector< briview::animationParams > & v_anim, const float & fps );

    int renderCapture( const std::string & filename  ,const bool & write, const unsigned int & compression, const unsigned int & fps,const float & duration, const float & h_rot_range,const float & v_rot_range, const float & tx, const float & ty, const float &tz);

    int renderCapture( const std::string & filename , const unsigned int & compression, const unsigned int & nviews );

signals:
    void changeCamera();
    void changePerspective();
    void renderSurfaces();
    void generateLights();


protected:

    void rotation(const float & theta, const float & phi );
    void translateAndRotate(const float & tx, const float & ty, const float & tz, const float & theta, const float & phi );

    void mousePressEvent( QMouseEvent * event );
    void mouseMoveEvent( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    void initializeGL();
    void resizeGL(int,int);
    void paintGL();
    int width,height;

    GLint InVertex_loc, InNormal_loc, InScalar_loc;

    float mouseSensitivity;
    int mouseMode;
    fslsurface_name::int2 prevLoc;
    briview::SurfaceContainer* surfaces;
    briview::ImageContainer* images;
    briview::scene_properties* scene_props;
    void setShaders();


private:
    enum { Color, Depth, NumRenderbuffers };

    GLuint fbo,rbo[NumRenderbuffers];


    GLfloat r_lut[4],g_lut[4],b_lut[4],a_lut[4],sc_lut[4];
        GLfloat r_lut_last[2],g_lut_last[2],b_lut_last[2],a_lut_last[2], sc_lut_last[2];

    GLint loc_r_lut,loc_g_lut,loc_b_lut,loc_a_lut, loc_sc_lut;
        GLint loc_r_lut_last,loc_g_lut_last,loc_b_lut_last,loc_a_lut_last, loc_sc_lut_last;
        GLint loc_low_clamp;
    GLuint v_light_dir,f_light_dir, p_light_dir;
    GLuint v_light_dir_map_scalars, p_light_dir_map_scalars,f_light_dir_map_scalars;//vert,frag,p;
//    GLuint v_cbar_map_scalars, p_cbar_map_scalars,f_cbar_map_scalars;//vert,frag,p;

    GLuint v_im_texture,f_im_texture,p_im_texture;//vert,frag,p;
    GLuint v_im_texture_ctr,f_im_texture_ctr,p_im_texture_ctr;//vert,frag,p;

    bool mouseInMotion;
    int mouseInMotionCount;
};

#endif // MYOPENGLWIDGET_H
