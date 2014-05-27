#ifndef SCENE_PROPERTIES_H
#define SCENE_PROPERTIES_H
#include <vector>
#include <QDialog>
#include <QtOpenGL>
#include <QDockWidget>
#include <briview_structs.h>
#include "fslsurface/fslsurface_structs.h"
#include <fstream>

namespace Ui {
    class scene_properties_form;
}
namespace briview{
    class scene_properties : public QDockWidget {
        Q_OBJECT
    public:
        scene_properties(QWidget *parent = 0);
        ~scene_properties();

        void writeSceneProperties(   std::ofstream * fou);
        void readSceneProperties( std::ifstream * fin);

        //camera and projection
        void getCameraParameters(briview::float3 & eye, briview::float3 & centre, briview::float3 & vup);
        void setCameraParameters(const briview::float3 & eye, const briview::float3 & centre, const briview::float3 & vup);

        void get_gluPersp_params(fslsurface_name::float4 & gluPersp_params);
        void set_gluPersp_params(const fslsurface_name::float4 & gluPersp_params);
      //  void updatePageText();

        void setCameraCenter(const briview::float3 & cog);
        void setCameraEye(const briview::float3 & eye_in);
        void setWindowSize(const int & w, const int & h);
        void setProjectionProperties();

    public slots:
        //Common
        void showDialog() { show(); }
      //  void nextPage();
    //    void prevPage();

        //Lighting
        void setCameraOrientation();
        void setLights();
        void changeCurrentLight(int index);
        void showDockMouse();
        void showDockLights();
        void showDockCamera();
        void showDockBackground();

        void doDockMouse();
        void doDockCamera();
        void doDockLights();
         void doDockBackground();
        //void removeLightingFromLayout(bool floating);

     //   GLuint getDisplayList_ID() { return displayList_ID; }

        void set_LightPosition();
        void set_LightAmbient();
        void set_GlobalLightAmbient();
        void set_GlobalLightAmbientMag(int);
        void set_LightDiffuse();
        void set_LightSpecular();

        void set_ClearColor();
        void set_ClearColorMag( int mag );
        void updatePerspectiveFOVY();
        void updatePerspectiveAspect();
        void updatePerspectiveNear();
        void updatePerspectiveFar();

        void setAspectRatio();

        void en_disableLight(bool state );

        //camera and projection
        void setCameraParameters();
        void addLight();
        void emitMouseMode();

    signals:
        void sig_updateGL();
        void sig_changedMouseSensitivity(int sens);
        void sig_changedMouseMode(int mode);
    protected:
        void changeEvent(QEvent *e);
        void set_GlobalLightAmbientText();
        void set_LightAmbientText( const int & index );
        void set_LightDiffuseText( const int & index );
        void set_LightSpecularText( const int & index );
        void set_LightPositionText( const int & index );
    private:
      //  GLuint displayList_ID;
        GLfloat* global_ambient;
        std::vector<GLfloat*> ambient,diffuse,specular;
        std::vector<GLfloat*> position;
        briview::float3 eye,center,vup;
        fslsurface_name::float4 gluPersp_params;
        GLfloat* background_color;
        std::vector<bool> enabled;
        fslsurface_name::int2 window_size;
        Ui::scene_properties_form *ui;
        int Nlights;

        enum PROJECTION_METHOD { PERSPECTIVE, ORTHO };
        PROJECTION_METHOD proj_mode;
      //  int dock_mouse_ui_page,dock_lighting_ui_page,dock_camera_ui_page;
    };
}
#endif // SCENE_PROPERTIES_H
