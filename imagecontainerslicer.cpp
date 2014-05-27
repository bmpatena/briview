#include "imagecontainer.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
    using namespace NEWIMAGE;


namespace qt_opengl_gui_name{

 //   struct vertex
  //  {
   //     float x, y, z;        //Vertex
    //    float cr, cg, cb,ca;     //Normal
        //      float s0, t0;         //Texcoord0
        //    float s1, t1;         //Texcoord1
        //  float s2, t2;         //Texcoord2
        //float padding[4];
   // };




    ImageContainer::ImageContainer(QGLWidget* qwid )
    {
            currentOpenGLWidget=qwid;
            doRenderX=doRenderY=doRenderZ=false;
            renderSlice_index_x=renderSlice_index_y=renderSlice_index_z=0;

    }
    void ImageContainer::addImage( NEWIMAGE::volume<float> & image,GLuint* verts)
    {
//        glEnable(GL_TEXTURE_3D);

        v_float_ims.push_back(image);
        vbos_vertices.push_back(verts);
cout<<"add image "<<verts[0]<<" "<<vbos_vertices.size()<<endl;
        vertex*  vertices =  new vertex[4*image.xsize()*image.zsize()];
        level.push_back(125);
        window.push_back(255);
        image_dims dims;
        dims.xsize=image.xsize();
        dims.ysize=image.ysize();
        dims.zsize=image.zsize();
        dims.xdim=image.xdim();
        dims.ydim=image.ydim();
        dims.zdim=image.zdim();
       v_im_dims.push_back( dims );


float* intensities=new float[image.xsize()*image.ysize()*image.zsize()];
int count=0;

     for (int z=0; z<image.zsize();z++)
    for (int y=0; y<image.ysize();y++)
        for (int x=0; x<image.xsize();x++,count++)
        {
            intensities[count]=image.value(x,y,z);//count%3/2.0;//count/image.xsize()*image.ysize()*image.zsize();//image.value(x,y,z)/10.000000;

            //cout<<"intensity "<<x<<" "<<y<<" "<<z<<" "<<image.value(x,y,z)<<" "<<image.max()<<" "<<image.value(x,y,z)/image.max()<<endl;
        }


     float bias=image.min()/image.max();

cout<<"intensities loaded "<<1.0/image.max()<<" "<<bias<<endl;
//float range=image.max()-image.min();
glPixelTransferf(GL_RED_SCALE,1.0/image.max());
glPixelTransferf(GL_GREEN_SCALE,1.0/image.max());
glPixelTransferf(GL_BLUE_SCALE,1.0/image.max());
glPixelTransferf(GL_RED_BIAS,bias);
glPixelTransferf(GL_GREEN_BIAS,bias);
glPixelTransferf(GL_BLUE_BIAS,bias);



glGenTextures(1, &texName);
glBindTexture(GL_TEXTURE_3D, texName);
glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexImage3D(GL_TEXTURE_3D,0,GL_LUMINANCE16_ALPHA16, image.xsize(),image.ysize(),image.zsize(),0,GL_LUMINANCE,GL_FLOAT,intensities);

delete[] vertices;

    }



    void ImageContainer::renderSlices()
    {
      //  cout<<"render sliceBLAH "<<endl;
       // int y=renderSlice_index;
    //    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
  //        glEnable ( GL_COLOR_MATERIAL ) ;
   //   glEnable ( GL_COLOR_MATERIAL ) ;
       // glDepthMask(GL_TRUE);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        glEnable ( GL_TEXTURE_3D ) ;
//cout<<"texName "<<texName<<endl;
        glBindTexture(GL_TEXTURE_3D, texName);

        glBegin(GL_QUADS);
//cout<<"slice "<<renderSlice_index<<" "<<renderSlice_index/v_im_dims.at(0).ysize<<endl;

        //x
        if (doRenderX){
        glTexCoord3f( renderSlice_index_x/v_im_dims.at(0).xsize,0.0,0.0); glVertex3f( renderSlice_index_x*v_im_dims.at(0).xdim,0.0, 0.0); glNormal3f(0,1.0,0);
        glTexCoord3f( renderSlice_index_x/v_im_dims.at(0).xsize,0.0,1.0); glVertex3f( renderSlice_index_x*v_im_dims.at(0).xdim,0.0, v_im_dims.at(0).zsize*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);
        glTexCoord3f( renderSlice_index_x/v_im_dims.at(0).xsize,1.0,1.0); glVertex3f( renderSlice_index_x*v_im_dims.at(0).xdim, v_im_dims.at(0).ysize*v_im_dims.at(0).ydim,v_im_dims.at(0).zsize*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);
        glTexCoord3f( renderSlice_index_x/v_im_dims.at(0).xsize,1.0,0.0); glVertex3f( renderSlice_index_x*v_im_dims.at(0).xdim, v_im_dims.at(0).ysize*v_im_dims.at(0).ydim, 0.0);glNormal3f(0,1.0,0);


        glTexCoord3f( renderSlice_index_x/v_im_dims.at(0).xsize,0.0,0.0); glVertex3f( renderSlice_index_x*v_im_dims.at(0).xdim, 0.0,0.0);glNormal3f(0,1.0,0);
        glTexCoord3f(renderSlice_index_x/v_im_dims.at(0).xsize,1.0, 0.0); glVertex3f( renderSlice_index_x*v_im_dims.at(0).xdim,v_im_dims.at(0).ysize*v_im_dims.at(0).ydim, 0.0);glNormal3f(0,1.0,0);
        glTexCoord3f( renderSlice_index_x/v_im_dims.at(0).xsize,1.0,1.0); glVertex3f(renderSlice_index_x*v_im_dims.at(0).xdim,v_im_dims.at(0).ysize*v_im_dims.at(0).ydim,  v_im_dims.at(0).zsize*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);
        glTexCoord3f( renderSlice_index_x/v_im_dims.at(0).xsize,0.0,1.0); glVertex3f(renderSlice_index_x*v_im_dims.at(0).xdim,0.0 , v_im_dims.at(0).zsize*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);
    }

        //y
        if (doRenderY){

        glTexCoord3f(0.0, renderSlice_index_y/v_im_dims.at(0).ysize,0.0); glVertex3f(0.0, renderSlice_index_y*v_im_dims.at(0).ydim, 0.0); glNormal3f(0,1.0,0);
        glTexCoord3f(0.0, renderSlice_index_y/v_im_dims.at(0).ysize,1.0); glVertex3f(0.0, renderSlice_index_y*v_im_dims.at(0).ydim, v_im_dims.at(0).zsize*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);
        glTexCoord3f(1.0, renderSlice_index_y/v_im_dims.at(0).ysize,1.0); glVertex3f(v_im_dims.at(0).xsize*v_im_dims.at(0).xdim, renderSlice_index_y*v_im_dims.at(0).ydim, v_im_dims.at(0).zsize*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);
        glTexCoord3f(1.0, renderSlice_index_y/v_im_dims.at(0).ysize,0.0); glVertex3f(v_im_dims.at(0).xsize*v_im_dims.at(0).xdim, renderSlice_index_y*v_im_dims.at(0).ydim, 0.0);glNormal3f(0,1.0,0);

cout<<"rennnnnnn "<<renderSlice_index_y<<" "<<renderSlice_index_y<<" "<<renderSlice_index_y*v_im_dims.at(0).ydim<<endl;
        glTexCoord3f(0.0, renderSlice_index_y/v_im_dims.at(0).ysize,0.0); glVertex3f(0.0, renderSlice_index_y*v_im_dims.at(0).ydim, 0.0);glNormal3f(0,1.0,0);
        glTexCoord3f(1.0, renderSlice_index_y/v_im_dims.at(0).ysize,0.0); glVertex3f(v_im_dims.at(0).xsize*v_im_dims.at(0).xdim, renderSlice_index_y*v_im_dims.at(0).ydim, 0.0);glNormal3f(0,1.0,0);
        glTexCoord3f(1.0, renderSlice_index_y/v_im_dims.at(0).ysize,1.0); glVertex3f(v_im_dims.at(0).xsize*v_im_dims.at(0).xdim, renderSlice_index_y*v_im_dims.at(0).ydim, v_im_dims.at(0).zsize*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);
        glTexCoord3f(0.0, renderSlice_index_y/v_im_dims.at(0).ysize,1.0); glVertex3f(0.0, renderSlice_index_y*v_im_dims.at(0).ydim, v_im_dims.at(0).zsize*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);
    }


        if (doRenderZ){
        //z
        glTexCoord3f(0.0,0.0, renderSlice_index_z/v_im_dims.at(0).zsize); glVertex3f(0.0, 0.0,renderSlice_index_z*v_im_dims.at(0).zdim); glNormal3f(0,1.0,0);
        glTexCoord3f(0.0,1.0, renderSlice_index_z/v_im_dims.at(0).zsize); glVertex3f(0.0, v_im_dims.at(0).ysize*v_im_dims.at(0).ydim, renderSlice_index_z*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);
        glTexCoord3f(1.0,1.0, renderSlice_index_z/v_im_dims.at(0).zsize); glVertex3f(v_im_dims.at(0).xsize*v_im_dims.at(0).xdim, v_im_dims.at(0).ysize*v_im_dims.at(0).ydim, renderSlice_index_z*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);
        glTexCoord3f(1.0,0.0, renderSlice_index_z/v_im_dims.at(0).zsize); glVertex3f(v_im_dims.at(0).xsize*v_im_dims.at(0).xdim,  0.0,renderSlice_index_z*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);


        glTexCoord3f(0.0,0.0, renderSlice_index_z/v_im_dims.at(0).zsize); glVertex3f(0.0, 0.0, renderSlice_index_z*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);
        glTexCoord3f(1.0,0.0, renderSlice_index_z/v_im_dims.at(0).zsize); glVertex3f(v_im_dims.at(0).xsize*v_im_dims.at(0).xdim, 0.0, renderSlice_index_z*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);
        glTexCoord3f(1.0,1.0, renderSlice_index_z/v_im_dims.at(0).zsize); glVertex3f(v_im_dims.at(0).xsize*v_im_dims.at(0).xdim, v_im_dims.at(0).ysize*v_im_dims.at(0).ydim, renderSlice_index_z*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);
        glTexCoord3f(0.0,1.0, renderSlice_index_z/v_im_dims.at(0).zsize); glVertex3f(0.0,  v_im_dims.at(0).ysize*v_im_dims.at(0).ydim,renderSlice_index_z*v_im_dims.at(0).zdim);glNormal3f(0,1.0,0);

    }






        glEnd();
     //   glDepthMask(GL_FALSE);



    }


}
