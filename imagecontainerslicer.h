#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H
#include <newimage/newimageall.h>
 #include <QtOpenGL>
//#include "my_structs.h"

namespace qt_opengl_gui_name{

    struct image_dims{
        int xsize,ysize,zsize;
        float xdim,ydim,zdim;
    };

    class ImageContainerSlicer
    {
    public:


        ImageContainerSlicer( QGLWidget* qwid );

        void addImage( NEWIMAGE::volume<float> & image, GLuint* verts);
     //   void generateSliceCallList(const int & slice, const int & list_id);
       // void generateSlice_Y_CallLists();

        void setDoRenderSlices(const bool & renderX, const bool & renderY, const bool & renderZ) { doRenderX=renderX; doRenderY=renderY; doRenderZ=renderZ; }
   //     void renderSliceY(const bool & renderY, const int & slice);
      void renderSlices();
        void setRenderSlices(const float & ind_x, const float & ind_y,const float & ind_z ) {  renderSlice_index_x=ind_x; renderSlice_index_y=ind_y;  renderSlice_index_z=ind_z;}

        void setQGLWidget(QGLWidget* qwid ) { currentOpenGLWidget=qwid; }
    private:
        vector<unsigned int> callListIndices_x, callListIndices_y, callListIndices_z;
        vector< NEWIMAGE::volume<float> > v_float_ims;

        //   bool renderX,renderY,renderZ;
        QGLWidget* currentOpenGLWidget;
        GLuint texName;
        float  renderSlice_index_x, renderSlice_index_y, renderSlice_index_z;
        bool doRenderX,doRenderY,doRenderZ;
        vector<GLuint*> vbos_vertices;
        vector<image_dims> v_im_dims;
        vector<float> level;
        vector<float> window;



};

}

#endif // IMAGECONTAINERSLICER_H
