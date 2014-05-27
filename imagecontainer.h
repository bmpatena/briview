#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H
#include <QtOpenGL>
#include <QObject>

#include <fstream>
#include <colour_bar.h>

#include "imagemanipulator.h"
#include "imagevideocapture.h"

#include <briview_structs.h>

#include "fslsurface/fslsurface_structs.h"


namespace briview{

   

    class ImageContainer  : public QObject
    {
        Q_OBJECT
    public:


        ImageContainer(  );
        ~ImageContainer(  );

        fslsurface_name::image_dims getImageApplyBiasScale( std::vector<float>& image , const unsigned int & im_index );
        std::vector<int> getSelectedImageIndices();

        fslsurface_name::image_dims getCurrentImage( std::vector<float>& image );
        fslsurface_name::image_dims getCurrentImageDims();

        fslsurface_name::image_dims getCurrentImageApplyBiasScale( std::vector<float>& image );
        std::vector<double> getCurrentImageXFM();
        std::vector<double> getImageXFM( const int & index );

        QString getCurrentImageName();
        QString getImageName(const int & index  );

        float getCurrentImageBias();
        float getCurrentImageScale();
        void addImage( const std::string & filename, GLuint* verts);
        void setGLSLProgram(const GLuint & prog);
        void setGLSLProgramCbar(const GLuint & prog2 );

        void renderSlices();
        void renderColourBar();

        ImageManipulator* getImageManipulatorWidget() { return im_form; }
        void writeImages( std::ofstream *fout) const;
        void setImVidCaptureForm( ImageVideoCapture* vid_form_in );
    public slots:

        void setVisible( bool visible );

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


        void turnOnOffSliceX( bool on );
        void turnOnOffSliceY( bool on );
        void turnOnOffSliceZ( bool on );

        void changeSliceX( int slice );
        void changeSliceY( int slice );
        void changeSliceZ( int slice );
        void changeSliceX( float slice_mm );
        void changeSliceY( float slice_mm );
        void changeSliceZ( float slice_mm );

        void updateImageProperties(int index);

        void changeImageSpace( int space );

        void setOpacityMode(int mode);
        void changeBlendFunc(int index);

        void changedOpacity(QString op);
        void toggleImage(bool state);
     //   void changedMinIntensity(QString num);
    //    void changedMaxIntensity(QString num);
        void changeColourTable(int index);
        void updateColourBar();

//        void decreaseMinIntensity();
  //      void increaseMinIntensity();
    //    void decreaseMaxIntensity();
   //     void increaseMaxIntensity();
        void updateColourTable();
        void updateColourTableSc();
        void swapImageProperties( int index);
     void addColourBar(GLuint*  vbos);
     void setVertexAttribLocs(const GLint & vert_loc, const GLint & norm_loc, const GLint & sc_loc ) { vertexLoc=vert_loc; normalLoc=norm_loc; scalarLoc=sc_loc; }

    signals:
        void sig_updateGL();

    private:

        ColourBar* c_bar;
        bool c_bar_onoff;

        enum IMAGE_SPACE { SCALED_VOXEL , NIFTI };

        void updateColourTable( const int & index );

        void loadColourTables(const std::string & filename );
        void computeAndSetRelativeOpacities();

        ImageManipulator* im_form;
        ImageVideoCapture* vid_form;
        float contrast_step;


        std::vector< std::string > cmap_names;

        GLuint loc_th_bias,loc_th_scale, loc_op, loc_onoff, loc_min_cmap, loc_max_cmap;


//colour map glsl locations
        GLuint loc_r_lut, loc_g_lut, loc_b_lut,loc_a_lut, loc_sc_lut;
        GLuint loc_r_lut_last, loc_g_lut_last, loc_b_lut_last, loc_a_lut_last, loc_sc_lut_last;
        GLuint loc_low_clamp;
        GLuint loc_cbar_r_lut, loc_cbar_g_lut, loc_cbar_b_lut,loc_cbar_a_lut, loc_cbar_sc_lut;
        GLuint loc_cbar_r_lut_last, loc_cbar_g_lut_last, loc_cbar_b_lut_last, loc_cbar_a_lut_last, loc_cbar_sc_lut_last;
        GLuint loc_cbar_low_clamp;

        int number_of_images;
        int blendFunc;
        bool rel_opacity;

        std::vector<IMAGE_SPACE> v_im_space;

      //  std::vector<GLfloat> v_im_thresh_max;
   //     std::vector<GLfloat> v_im_thresh_bias;
     //   std::vector<GLfloat> v_im_thresh_scale;
        std::vector<GLfloat> v_im_opacity_rel, v_im_opacity_abs;
         std::vector<GLint> v_im_onoff;
    //    std::vector< GLfloat* > v_cmap;
         GLint vertexLoc, normalLoc, scalarLoc;

        std::vector< fslsurface_name::colour_table > v_im_ctables; //contains images specific colour table
        std::vector< fslsurface_name::colour_table > v_ctables; //contains base colour tables, loaded from file
       // std::vector< GLfloat* > v_max_cmap;
        std::vector<int> v_cmap_index;
        std::vector< GLuint > texNames;
        briview::float3 slice_position;
        std::vector< fslsurface_name::bool3 > slice_visibility;
        std::vector<fslsurface_name::image_dims> v_im_dims;
        std::vector<fslsurface_name::float2> v_im_i_range;

        std::vector<  std::vector<double> > v_xfm;

        GLuint shaderProgram, shaderProgram_cbar;

    };

}

#endif // IMAGECONTAINERSLICER_H
