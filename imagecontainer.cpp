#include "imagecontainer.h"
#include <newimage/newimageall.h>
#include "newmatap.h"


using namespace NEWMAT;
using namespace NEWIMAGE;
//using namespace briview_structs;
using namespace fslsurface_name;
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace briview{

    ImageContainer::ImageContainer( )
    {
        im_form=new ImageManipulator();

        number_of_images=0;
        slice_position = float3(0,0,0);

       loadColourTables((QApplication::applicationDirPath() + "/assets/colour_maps.txt").toStdString());

        //state of gui variable
        blendFunc=0;
        rel_opacity=0;
        contrast_step=0.01;



        connect(im_form,SIGNAL(sig_changedSliceX(int)), this, SLOT(changeSliceX(int)));
        connect(im_form,SIGNAL(sig_changedSliceY(int)), this, SLOT(changeSliceY(int)));
        connect(im_form,SIGNAL(sig_changedSliceZ(int)), this, SLOT(changeSliceZ(int)));
        connect(im_form,SIGNAL(sig_changedSliceX(float)), this, SLOT(changeSliceX(float)));
        connect(im_form,SIGNAL(sig_changedSliceY(float)), this, SLOT(changeSliceY(float)));
        connect(im_form,SIGNAL(sig_changedSliceZ(float)), this, SLOT(changeSliceZ(float)));


        connect(im_form,SIGNAL(sig_toggledSliceX(bool)), this, SLOT(turnOnOffSliceX(bool)));
        connect(im_form,SIGNAL(sig_toggledSliceY(bool)), this, SLOT(turnOnOffSliceY(bool)));
        connect(im_form,SIGNAL(sig_toggledSliceZ(bool)), this, SLOT(turnOnOffSliceZ(bool)));


        connect(im_form,SIGNAL(sig_changedCurrentImage(int)), this, SLOT(updateImageProperties(int)));

        connect(im_form,SIGNAL(sig_changedOpacity(QString)), this, SLOT(changedOpacity(QString)));
        connect(im_form,SIGNAL(sig_toggled_ImOnOff(bool)),this,SLOT(toggleImage(bool)));


        connect(im_form,SIGNAL(sig_changedBlendFunc(int) ),this,SLOT(changeBlendFunc(int)));
         connect(im_form,SIGNAL(sig_changedOpacityMode(int)),this, SLOT(setOpacityMode(int)));

          connect(im_form,SIGNAL(sig_changedColourTable()),this, SLOT(updateColourTable()));
          connect(im_form,SIGNAL(sig_changedColourTableSc()),this, SLOT(updateColourTableSc()));

          connect(im_form,SIGNAL(sig_changedColourTable(int)),this, SLOT(changeColourTable(int)));
          connect(im_form,SIGNAL(sig_swapImages(int)),this, SLOT(swapImageProperties(int)));
          connect(im_form,SIGNAL(sig_changedColourBar()), this , SLOT(updateColourBar()));

          connect(im_form, SIGNAL(sig_changedImageSpace(int)), this, SLOT(changeImageSpace(int)) );


          c_bar=NULL;
          c_bar_onoff=false;



     }

    ImageContainer::~ImageContainer( )
    {
        if (texNames.size()>0)
            glDeleteTextures(texNames.size(),&texNames[0]);

        delete im_form;

        if (c_bar!=NULL)
        {
            delete c_bar;
        }


    }

    void ImageContainer::writeImages( ofstream *fout) const
    {
/*
        *fout<<"Surfaces "<<surface_filenames.size()<<endl;

        vector< int >::const_iterator i_glsl = surf_glsl_programs.begin();
        vector< int >::const_iterator i_imat = surf_material_index.begin();
        vector< material >::const_iterator i_mat = v_materials.begin();
        vector< colour_table >::const_iterator i_ctab = v_ctables.begin();
       // vector< bound >::const_iterator i_ctab = v_ctables.begin();

        for (vector<string>::const_iterator i =  surface_filenames.begin() ; i != surface_filenames.end(); i++, i_mat++, i_imat++, i_glsl++, i_ctab++)
        {
            *fout<<"filename "<<*i<<endl;
            *fout<<"GLSLprogram "<<*i_glsl<<endl;
            *fout<<"MaterialIndex "<<*i_imat<<endl;
            *fout<<"Material ";
            writeMaterial(*i_mat,fout);
            *fout<<"ColourTable ";
            writeColourTable(*i_ctab,fout);
        }
        */
    }


    void ImageContainer::setVisible( bool visible )
    {
            if (visible)
                im_form->show();
            else
                im_form->hide();
    }

    //-----------these connection pass up signal from the menu bar to the image manipulator---------//
    //these are just used to pass up to image manipulator
    void ImageContainer::showDockImageNavigator(){ im_form->showDockImageNavigator(); }
    void ImageContainer::showDockProperties(){ im_form->showDockProperties(); }
    void ImageContainer::showDockColourBar(){ im_form->showDockColourBar(); }
    void ImageContainer::showDockCoordinateSystem(){ im_form->showDockCoordinateSystem(); }
    void ImageContainer::showDockBlending(){ im_form->showDockBlending(); }
    void ImageContainer::showDockImList(){ im_form->showDockImList(); }

    void ImageContainer::doDockImageNavigator(){ im_form->doDockImageNavigator(); }
    void ImageContainer::doDockProperties(){ im_form->doDockProperties(); }
    void ImageContainer::doDockColourBar(){ im_form->doDockColourBar(); }
    void ImageContainer::doDockCoordinateSystem(){ im_form->doDockCoordinateSystem(); }
    void ImageContainer::doDockBlending(){ im_form->doDockBlending(); }
    void ImageContainer::doDockImList(){ im_form->doDockImList(); }

    //-----------these connection pass up signal from the menu bar to the image manipulator---------//


    void ImageContainer::loadColourTables(const string & filename )
    {
        ifstream fin(filename.c_str());
        string temp;

        if (fin.is_open())
        {
            while (fin>>temp)
            {
                //cout<<"load colour map: "<<temp<<endl;
                cmap_names.push_back(temp);
                im_form->addColourTable(temp);

                float4 sclut,rlut,glut,blut,alut,clamp;
                float2 scl,rl,gl,bl,al;

                fin>>sclut.x>>sclut.y>>sclut.z>>sclut.w>>scl.x>>scl.y;
                fin>>rlut.x>>rlut.y>>rlut.z>>rlut.w>>rl.x>>rl.y;
                fin>>glut.x>>glut.y>>glut.z>>glut.w>>gl.x>>gl.y;
                fin>>blut.x>>blut.y>>blut.z>>blut.w>>bl.x>>bl.y;
                fin>>alut.x>>alut.y>>alut.z>>alut.w>>al.x>>al.y;
                fin>>clamp.x>>clamp.y>>clamp.z>>clamp.w;

                v_ctables.push_back(colour_table( rlut, rl, glut, gl, blut, bl, alut, al, sclut,scl,clamp.x, clamp.y, clamp.z, clamp.w));
            }
            if (cmap_names.size()>0)
                im_form->setColourTableIndex(0);

        }else{
            cerr<<"Colour map file, "<<filename<<", does not exist"<<endl;
            exit (EXIT_FAILURE);
        }
    }


    void ImageContainer::computeAndSetRelativeOpacities()
    {
//        glUseProgram(shaderProgram);

        float total_alpha=0.0;
        unsigned int N = v_im_opacity_abs.size();
        for (unsigned int i=0;i<N;i++)
            if (v_im_onoff[i])
                total_alpha+=v_im_opacity_abs.at(i);

        for (unsigned int i=0;i<N;i++)
            if (v_im_onoff[i])
            {
                v_im_opacity_rel[i]=v_im_opacity_abs[i]/total_alpha;
                if (total_alpha == 0)
                   v_im_opacity_rel[i]=0;
            }else
            v_im_opacity_rel[i]=0.0;

    }

    void ImageContainer::changeColourTable(int index)
    {
        if (texNames.size()>0)
        {
              if (index >0)
            {
                  vector<int> cur_indices = im_form->getCurrentImageIndices();
                   for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
                   {//apply to all selected images
                         im_form->setColourTableNoSc(v_ctables.at(index-1));
                          memcpy(v_im_ctables[*i_ind].r_lut , v_ctables[index-1].r_lut, sizeof(float)*6);
                        memcpy(v_im_ctables[*i_ind].g_lut , v_ctables[index-1].g_lut, sizeof(float)*6);
                          memcpy(v_im_ctables[*i_ind].b_lut , v_ctables[index-1].b_lut, sizeof(float)*6);
                      memcpy(v_im_ctables[*i_ind].a_lut , v_ctables[index-1].a_lut, sizeof(float)*6);
                  //  cout<<"chCtableIndex "<<*i_ind<<" "<<index<<endl;
                      v_cmap_index.at(*i_ind)=index;

                  }
            }

            emit sig_updateGL();
        }
    }

    void ImageContainer::updateColourTable( )
    {
            if (im_form->getCurrentImageIndex() != -1)
            {
                vector<int> cur_indices = im_form->getCurrentImageIndices();
                 for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
                 {
                     //cout<<"update colour table index "<<*i_ind<<endl;
                 v_im_ctables[*i_ind]=im_form->getCurrentColourTable();
                 v_cmap_index.at(*i_ind)=0;
                im_form->setColourTableIndex(0);
               }
            emit sig_updateGL();
        }

    }
    void ImageContainer::updateColourTableSc( )
    {
        if (im_form->getCurrentImageIndex() != -1)
        {
            vector<int> cur_indices = im_form->getCurrentImageIndices();
             for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
             {
                 //cout<<"update colour tableSC index "<<*i_ind<<endl;

        v_im_ctables[*i_ind]=im_form->getCurrentColourTable();
    }
        emit sig_updateGL();
        }

    }
    void ImageContainer::updateColourTable( const int & index )
    {
        //cout<<"update colour table index "<<index<<endl;
     //   if (vbos_vertices.size()>0)
        {

            glUseProgram(shaderProgram);
            float* sc_4 = new float[4];
      //      sc_4[0] = v_im_ctables[index].sc_lut[0]* v_im_thresh_scale[index] + v_im_thresh_bias[index] ;
       //     sc_4[1] = v_im_ctables[index].sc_lut[1]* v_im_thresh_scale[index] + v_im_thresh_bias[index] ;
       //     sc_4[2] = v_im_ctables[index].sc_lut[2]* v_im_thresh_scale[index] + v_im_thresh_bias[index] ;
       //     sc_4[3] = v_im_ctables[index].sc_lut[3]* v_im_thresh_scale[index] + v_im_thresh_bias[index] ;
            sc_4[0] = v_im_ctables[index].sc_lut[0];//* v_im_thresh_scale[index] + v_im_thresh_bias[index] ;
            sc_4[1] = v_im_ctables[index].sc_lut[1];//* v_im_thresh_scale[index] + v_im_thresh_bias[index] ;
            sc_4[2] = v_im_ctables[index].sc_lut[2];//* v_im_thresh_scale[index] + v_im_thresh_bias[index] ;
            sc_4[3] = v_im_ctables[index].sc_lut[3];//* v_im_thresh_scale[index] + v_im_thresh_bias[index] ;

            glUniform4fv(loc_r_lut,1,v_im_ctables.at(index).r_lut);
            glUniform4fv(loc_g_lut,1,v_im_ctables.at(index).g_lut);
            glUniform4fv(loc_b_lut,1,v_im_ctables.at(index).b_lut);
            glUniform4fv(loc_a_lut,1,v_im_ctables.at(index).a_lut);
            glUniform4fv(loc_sc_lut,1,sc_4);//v_ctables.at(index).sc_lut);
            glUniform2fv(loc_r_lut_last,1,v_im_ctables.at(index).r_lut+4);
            glUniform2fv(loc_g_lut_last,1,v_im_ctables.at(index).g_lut+4);
            glUniform2fv(loc_b_lut_last,1,v_im_ctables.at(index).b_lut+4);
            glUniform2fv(loc_a_lut_last,1,v_im_ctables.at(index).a_lut+4);
            float* f = new float[2];

          //  f[0] = v_im_ctables.at(index).sc_lut[4]*v_im_thresh_scale[index]+v_im_thresh_bias[index];
            //f[1] = v_im_ctables.at(index).sc_lut[5]*v_im_thresh_scale[index]+v_im_thresh_bias[index];
            f[0] = v_im_ctables.at(index).sc_lut[4];//*v_im_thresh_scale[index]+v_im_thresh_bias[index];
            f[1] = v_im_ctables.at(index).sc_lut[5];//*v_im_thresh_scale[index]+v_im_thresh_bias[index];

            glUniform2fv(loc_sc_lut_last,1,f);

            glUniform4fv(loc_low_clamp,1,v_im_ctables.at(index).low_clamp);
            //cout<<"ctable "<<index<<endl;
            //cout<<sc_4[0]<<" "<<sc_4[1]<<" "<<sc_4[2]<<" "<<sc_4[3]<<" "<<f[0]<<" "<<f[1]<<endl;
            //cout<<v_im_ctables[index].r_lut[0]<<" "<<v_im_ctables[index].r_lut[1]<<" "<<v_im_ctables[index].r_lut[2]<<" "<<v_im_ctables[index].r_lut[3]<<" "<<v_im_ctables[index].r_lut[4]<<" "<<v_im_ctables[index].r_lut[5]<<endl;
            //cout<<v_im_ctables[index].g_lut[0]<<" "<<v_im_ctables[index].g_lut[1]<<" "<<v_im_ctables[index].g_lut[2]<<" "<<v_im_ctables[index].g_lut[3]<<" "<<v_im_ctables[index].g_lut[4]<<" "<<v_im_ctables[index].g_lut[5]<<endl;
            //cout<<v_im_ctables[index].b_lut[0]<<" "<<v_im_ctables[index].b_lut[1]<<" "<<v_im_ctables[index].b_lut[2]<<" "<<v_im_ctables[index].b_lut[3]<<" "<<v_im_ctables[index].b_lut[4]<<" "<<v_im_ctables[index].b_lut[5]<<endl;
            //cout<<v_im_ctables[index].a_lut[0]<<" "<<v_im_ctables[index].a_lut[1]<<" "<<v_im_ctables[index].a_lut[2]<<" "<<v_im_ctables[index].a_lut[3]<<" "<<v_im_ctables[index].a_lut[4]<<" "<<v_im_ctables[index].a_lut[5]<<endl;



            if (c_bar != NULL)
            {
                //cout<<"set bcar glsl vars fo rimages"<<endl;
              /*  glUseProgram(shaderProgram_cbar);
                c_bar->generateColourBar(0,v_im_ctables[im_form->getCurrentImageIndex()]);

                glUniform4fv(loc_cbar_r_lut,1,v_im_ctables.at(index).r_lut);
                glUniform4fv(loc_cbar_g_lut,1,v_im_ctables.at(index).g_lut);
                glUniform4fv(loc_cbar_b_lut,1,v_im_ctables.at(index).b_lut);
                glUniform4fv(loc_cbar_a_lut,1,v_im_ctables.at(index).a_lut);
                glUniform4fv(loc_cbar_sc_lut,1,sc_4);//v_ctables.at(index).sc_lut);
                glUniform2fv(loc_cbar_r_lut_last,1,v_im_ctables.at(index).r_lut+4);
                glUniform2fv(loc_cbar_g_lut_last,1,v_im_ctables.at(index).g_lut+4);
                glUniform2fv(loc_cbar_b_lut_last,1,v_im_ctables.at(index).b_lut+4);
                glUniform2fv(loc_cbar_a_lut_last,1,v_im_ctables.at(index).a_lut+4);

                glUniform2fv(loc_cbar_sc_lut_last,1,f);

                glUniform4fv(loc_cbar_low_clamp,1,v_im_ctables.at(index).low_clamp);
*/
            }


            delete[] f;
            delete [] sc_4;


        }
        glUseProgram(shaderProgram);

    }

    void ImageContainer::updateColourBar()
    {
        //cout<<"update image cbar"<<endl;
        if (c_bar != NULL)
        {
            //cout<<"get visibility "<<endl;
            c_bar_onoff = im_form->getColourBarVisibility();
                        //cout<<"get visibility "<<c_bar_onoff<<endl;

            float3 origin;
            float width,length;
            im_form->getColourBarGeometry(origin,width,length);
            string title = im_form->getColourBarTitle();
            c_bar->setGeometry(origin,length,width);
            c_bar->setTitle(title);
            c_bar->generateColourBar(0,v_im_ctables[im_form->getCurrentImageIndex()]);


        }
        emit sig_updateGL();

    }

    void ImageContainer::addColourBar(GLuint*  vbos)
    {
        //cout<<"generate image Colour bar "<<endl;
        if (c_bar==NULL)
        {
            c_bar = new ColourBar();
        }
        c_bar->setVertexAttribLocs(vertexLoc, normalLoc, scalarLoc);
      //  c_bar->setMaterial(v_materials.at(surf_form->getCurrentSurfaceIndex()));
        c_bar->addColourBar(vbos, v_im_ctables[im_form->getCurrentImageIndex()]);
        //c_bar->setGLSLPrograms(c_bar_progs);
    }
/*
    float ImageContainer::getCurrentImageBias()
    {
        return v_im_thresh_bias[im_form->getCurrentImageIndex()];
    }

    float ImageContainer::getCurrentImageScale()
    {
        return v_im_thresh_scale[im_form->getCurrentImageIndex()];
    }
*/

    std::vector<int> ImageContainer::getSelectedImageIndices()
    {

        return  im_form->getCurrentImageIndices();


    }

    fslsurface_name::image_dims ImageContainer::getImageApplyBiasScale( std::vector<float>& image , const unsigned int & im_index )
    {
        if (im_index == -1)
        {
       //     image = NULL;
            return image_dims(0,0,0,0,0,0);

        }
    cout<<"im_index "<<im_index<<endl;
        image_dims dim_cur =  v_im_dims[im_index];

        dim_cur.xsize/=dim_cur.xdim;
        dim_cur.ysize/=dim_cur.ydim;
        dim_cur.zsize/=dim_cur.zdim;

        unsigned int nvoxels = static_cast<unsigned int>(dim_cur.xsize * dim_cur.ysize * dim_cur.zsize);
        image.resize(nvoxels);
        //image = new float[static_cast<unsigned int>(dim_cur.xsize * dim_cur.ysize * dim_cur.zsize)];
        //cout<<"get image "<<static_cast<unsigned int>(dim_cur.xsize * dim_cur.ysize * dim_cur.zsize)<<" "<<dim_cur.xsize * dim_cur.ysize * dim_cur.zsize<<endl;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, texNames[im_index]);

/*
        float im_thresh_bias=0;
        float im_thresh_scale=1;

        glPixelTransferf(GL_RED_SCALE,im_thresh_scale);
        glPixelTransferf(GL_GREEN_SCALE,im_thresh_scale);
        glPixelTransferf(GL_BLUE_SCALE,im_thresh_scale);
        glPixelTransferf(GL_RED_BIAS,im_thresh_bias);
        glPixelTransferf(GL_GREEN_BIAS,im_thresh_bias);
        glPixelTransferf(GL_BLUE_BIAS,im_thresh_bias);
*/
        //cout<<"bind texture "<<image[0]<<endl;
        glGetTexImage(	GL_TEXTURE_3D,0,GL_RED,GL_FLOAT,&(image[0]));

       // float bias = v_im_thresh_bias[im_index];
        //float scale = v_im_thresh_scale[im_index];

     //   for (unsigned int i =0 ; i < nvoxels; ++i)
       // {
         //   if (image[i]>0)
           // cout<<"im "<<i<<" "<<image[i]<<endl;//" "<<(image[i] -bias)/ scale<<" "<<bias<<" "<<scale<<endl;

          //  image[i]= (image[i] -bias)/ scale ;
        //}
        //cout<<"image got "<<image[0]<<endl;
        glBindTexture(GL_TEXTURE_3D, 0);

        return dim_cur;

    }


    fslsurface_name::image_dims ImageContainer::getCurrentImageApplyBiasScale( std::vector<float>& image )
    {
        return getImageApplyBiasScale( image, im_form->getCurrentImageIndex() );
    }

    fslsurface_name::image_dims ImageContainer::getCurrentImageDims(){
        return  v_im_dims[im_form->getCurrentImageIndex()];
    }

    image_dims ImageContainer::getCurrentImage( vector<float>& image)
    {
        if (im_form->getCurrentImageIndex() == -1)
        {
       //     image = NULL;
            return image_dims(0,0,0,0,0,0);

        }
        image_dims dim_cur =  v_im_dims[im_form->getCurrentImageIndex()];

        dim_cur.xsize/=dim_cur.xdim;
        dim_cur.ysize/=dim_cur.ydim;
        dim_cur.zsize/=dim_cur.zdim;


    image.resize(static_cast<unsigned int>(dim_cur.xsize * dim_cur.ysize * dim_cur.zsize));
        //image = new float[static_cast<unsigned int>(dim_cur.xsize * dim_cur.ysize * dim_cur.zsize)];
        //cout<<"get image "<<static_cast<unsigned int>(dim_cur.xsize * dim_cur.ysize * dim_cur.zsize)<<" "<<dim_cur.xsize * dim_cur.ysize * dim_cur.zsize<<endl;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, texNames[im_form->getCurrentImageIndex()]);



        //cout<<"bind texture "<<image[0]<<endl;
        glGetTexImage(	GL_TEXTURE_3D,0,GL_RED,GL_FLOAT,&(image[0]));
        //cout<<"image got "<<image[0]<<endl;
        glBindTexture(GL_TEXTURE_3D, 0);
        return dim_cur;

    }
    std::vector<double> ImageContainer::getCurrentImageXFM()
    {
        return v_xfm[im_form->getCurrentImageIndex()];

    }
    std::vector<double> ImageContainer::getImageXFM( const int & index )
    {
        return v_xfm[index];

    }
    QString ImageContainer::getImageName( const int & index  )
    {
        return im_form->getImageName( index );

    }

    QString ImageContainer::getCurrentImageName()
    {
        return im_form->getCurrentImageName();

    }

    void ImageContainer::addImage( const string & filename ,GLuint* verts)
    {
        volume<float>* image = new volume<float>();
        read_volume(*image,filename);
        Matrix m = image->newimagevox2mm_mat();
        Matrix m_mm2vox(4,4);
        m_mm2vox=0;
        m_mm2vox.element(0,0)= 1.0/image->xdim();
        m_mm2vox.element(1,1)= 1.0/image->ydim();
        m_mm2vox.element(2,2)= 1.0/image->zdim();
        m_mm2vox.element(3,3)= 1.0;
        m = m * m_mm2vox;
      //  m=m.t();
       // m=m.i();
        //cout<<"addimage "<<m<<endl;
        {
            vector<double> xfm(16,0);
            int count=0;
            for (int i =0 ; i < 4; ++i)
                for (int j =0 ; j < 4 ; ++j,++count)
                    xfm[count]=m.element(j,i);
            v_xfm.push_back(xfm);
        }
        v_im_space.push_back( NIFTI );

        image_dims dims = image_dims( image->xsize()*image->xdim(), image->ysize()*image->ydim(), image->zsize()*image->zdim(), \
                                      image->xdim(), image->ydim(), image->zdim() );
        v_im_dims.push_back( dims );
        v_im_i_range.push_back(float2(image->min(),image->max()));
        slice_position = float3( dims.xsize/2.0 , dims.ysize/2.0 , dims.zsize/2.0 );
        slice_visibility.push_back( bool3( false, true, false) );



        float* intensities=new float[image->xsize()*image->ysize()*image->zsize()];
        {
            int count=0;

            for (int z=0; z<image->zsize();z++)
                for (int y=0; y<image->ysize();y++)
                    for (int x=0; x<image->xsize();x++,count++)
                    {
                intensities[count]=image->value(x,y,z);
               // cout<<"addImage "<<x<<" "<<y<<" "<<z<<" "<<intensities[count]<<endl;
            }
        }

      //  float bias=image->min()/image->max();



/*
        GLfloat im_thresh_bias=image->min()/image->max();
        GLfloat im_thresh_scale=1.0/image->max();
cout<<"imthresh1 "<<im_thresh_scale<<endl;
        im_thresh_bias=0.0f;
        //im_thresh_scale=0.2f;
        im_thresh_scale=1.0f;

        glPixelTransferf(GL_RED_SCALE,im_thresh_scale);
        glPixelTransferf(GL_GREEN_SCALE,im_thresh_scale);
        glPixelTransferf(GL_BLUE_SCALE,im_thresh_scale);
        glPixelTransferf(GL_RED_BIAS,im_thresh_bias);
        glPixelTransferf(GL_GREEN_BIAS,im_thresh_bias);
        glPixelTransferf(GL_BLUE_BIAS,im_thresh_bias);
*/

        glActiveTexture(GL_TEXTURE0);//+number_of_images);
        GLuint texName;

        glGenTextures(1, &texName);
        GLuint texLoc;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, texName);
        texLoc= glGetUniformLocation(shaderProgram, "tex0");
        glUniform1i(texLoc, 0);

        /*
        switch  (number_of_images)
        {
        case 0:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_3D, texName);
            texLoc= glGetUniformLocation(shaderProgram, "tex0");
            glUniform1i(texLoc, 0);
            break;

        case 1:
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_3D, texName);
            texLoc= glGetUniformLocation(shaderProgram, "tex1");
            glUniform1i(texLoc, 1);
            break;
        case 2:
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_3D, texName);
            texLoc= glGetUniformLocation(shaderProgram, "tex2");
            glUniform1i(texLoc, 2);
            break;
        case 3:
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_3D, texName);
            texLoc= glGetUniformLocation(shaderProgram, "tex3");
            glUniform1i(texLoc, 3);
            break;
        default:
            cerr<<"No more than 4 images currently supported"<<endl;
            break;

        }
*/
        //enable image
        v_im_onoff.push_back(1);
        v_cmap_index.push_back(1);
        v_im_ctables.push_back(v_ctables[0]);

        //float dispmin = image->getDisplayMinimum();
        //float dispmax = image->getDisplayMaximum();
        float dispmin = image->min();
        float dispmax = image->max();


        float slope=(dispmax-dispmin)/5.0;
        v_im_ctables.back().sc_lut[0]= dispmin;
        v_im_ctables.back().sc_lut[1]= dispmin + slope;
        v_im_ctables.back().sc_lut[2]= dispmin + slope*2;
        v_im_ctables.back().sc_lut[3]= dispmin + slope*3;
        v_im_ctables.back().sc_lut[4]= dispmin + slope*4;
        v_im_ctables.back().sc_lut[5]= dispmax;

       // cout<<"colour table in addimage "<<endl;
       // cout<<v_im_ctables.back().sc_lut[0]<<" "<<v_im_ctables.back().sc_lut[1]<<" "<<v_im_ctables.back().sc_lut[2]<<" "<<v_im_ctables.back().sc_lut[3]<<" "<<v_im_ctables.back().sc_lut[4]<<" "<<v_im_ctables.back().sc_lut[5]<<" "<<endl;
        im_form->setColourTable(v_im_ctables.back());

     //   GLfloat im_thresh_max=1.0f;
        GLfloat im_opacity_rel=0.0f;
        GLfloat im_opacity_abs=1.0f;

      //  v_im_thresh_bias.push_back(im_thresh_bias);
      //  v_im_thresh_scale.push_back(im_thresh_scale);
        v_im_opacity_rel.push_back(im_opacity_rel);
        v_im_opacity_abs.push_back(im_opacity_abs);

        //compute relative opacities (i.e. alpha sum to 1 for enable images)
        //set up image appropriate GLSL
        computeAndSetRelativeOpacities();

        texNames.push_back(texName);

        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

//        glTexImage3D(GL_TEXTURE_3D,0,1, image->xsize(),image->ysize(),image->zsize(),0,GL_LUMINANCE,GL_FLOAT,intensities);

        glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA32F_ARB, image->xsize(),image->ysize(),image->zsize(),0,GL_RED,GL_FLOAT,intensities);


//        unsigned int nvoxels = static_cast<unsigned int>(image->xsize() * image->ysize() * image->zsize());
//        vector<float> image2(nvoxels);//
//cout<<"image back out"<<endl;
  //      glGetTexImage(	GL_TEXTURE_3D,0,GL_RED,GL_FLOAT,&(image2[0]));
    //    for (vector<float>::iterator i = image2.begin(); i!= image2.end();++i)
       //     if (*i >0)
         //       cout<<*i<<endl;


        number_of_images++;

        im_form->addItemToList(filename);
        glBindTexture(GL_TEXTURE_3D, 0);

        delete[] intensities;
        delete image;
        emit sig_updateGL();

    }
    void ImageContainer::setGLSLProgram(const GLuint & prog)
    {
        shaderProgram=prog;
        glUseProgram(shaderProgram);
        loc_th_bias  = glGetUniformLocation(shaderProgram,"im_th_bias");
        loc_th_scale = glGetUniformLocation(shaderProgram,"im_th_scale");
        loc_op      = glGetUniformLocation(shaderProgram,"opacity");
      //  loc_onoff   = glGetUniformLocation(shaderProgram,"imageOnOff");
        loc_r_lut = glGetUniformLocation(shaderProgram,"r_lut");
        loc_g_lut = glGetUniformLocation(shaderProgram,"g_lut");
        loc_b_lut = glGetUniformLocation(shaderProgram,"b_lut");
        loc_a_lut = glGetUniformLocation(shaderProgram,"a_lut");
        loc_sc_lut = glGetUniformLocation(shaderProgram,"sc_lut");

        loc_r_lut_last = glGetUniformLocation(shaderProgram,"r_lut_last");
        loc_g_lut_last = glGetUniformLocation(shaderProgram,"g_lut_last");
        loc_b_lut_last = glGetUniformLocation(shaderProgram,"b_lut_last");
        loc_a_lut_last = glGetUniformLocation(shaderProgram,"a_lut_last");
        loc_sc_lut_last = glGetUniformLocation(shaderProgram,"sc_lut_last");
        loc_low_clamp =  glGetUniformLocation(shaderProgram,"lower_clamp");

    }
    void ImageContainer::setGLSLProgramCbar(const GLuint & prog2 )
    {
        shaderProgram_cbar=prog2;
        glUseProgram(shaderProgram_cbar);

        loc_cbar_r_lut = glGetUniformLocation(shaderProgram,"r_lut");
        loc_cbar_g_lut = glGetUniformLocation(shaderProgram,"g_lut");
        loc_cbar_b_lut = glGetUniformLocation(shaderProgram,"b_lut");
        loc_cbar_a_lut = glGetUniformLocation(shaderProgram,"a_lut");
        loc_cbar_sc_lut = glGetUniformLocation(shaderProgram,"sc_lut");

        loc_cbar_r_lut_last = glGetUniformLocation(shaderProgram,"r_lut_last");
        loc_cbar_g_lut_last = glGetUniformLocation(shaderProgram,"g_lut_last");
        loc_cbar_b_lut_last = glGetUniformLocation(shaderProgram,"b_lut_last");
        loc_cbar_a_lut_last = glGetUniformLocation(shaderProgram,"a_lut_last");
        loc_cbar_sc_lut_last = glGetUniformLocation(shaderProgram,"sc_lut_last");
        loc_cbar_low_clamp =  glGetUniformLocation(shaderProgram,"lower_clamp");

    }


    void ImageContainer::turnOnOffSliceX( bool on )
    {
        vector<int> cur_indices = im_form->getCurrentImageIndices();
         for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
         {
        slice_visibility[*i_ind].x=on;
        }
        emit sig_updateGL();
    }
    void ImageContainer::turnOnOffSliceY( bool on )
    {
        vector<int> cur_indices = im_form->getCurrentImageIndices();
         for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
         {
        slice_visibility[*i_ind].y=on;
    }
         emit sig_updateGL();

    }
    void ImageContainer::turnOnOffSliceZ( bool on )
    {
        vector<int> cur_indices = im_form->getCurrentImageIndices();
         for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
         {
        slice_visibility[*i_ind].z=on;
    }
         emit sig_updateGL();

    }

    void ImageContainer::changeSliceX( int slice )
    {
        if (im_form->getCurrentImageIndex() != -1)
        {
        slice_position.x=slice*v_im_dims.at(im_form->getCurrentImageIndex()).xdim;
        im_form->setSlidersPosition( slice_position.x/v_im_dims.at(im_form->getCurrentImageIndex()).xdim, slice_position.y/v_im_dims.at(im_form->getCurrentImageIndex()).ydim, slice_position.z/v_im_dims.at(im_form->getCurrentImageIndex()).zdim, \
                                     v_im_dims.at(im_form->getCurrentImageIndex()).xdim, v_im_dims.at(im_form->getCurrentImageIndex()).ydim, v_im_dims.at(im_form->getCurrentImageIndex()).zdim );
        emit sig_updateGL();
        }
    }

    void ImageContainer::changeSliceY( int slice )
    {
        if (im_form->getCurrentImageIndex() != -1)
        {
        slice_position.y=slice*v_im_dims.at(im_form->getCurrentImageIndex()).ydim;
        im_form->setSlidersPosition( slice_position.x/v_im_dims.at(im_form->getCurrentImageIndex()).xdim, slice_position.y/v_im_dims.at(im_form->getCurrentImageIndex()).ydim, slice_position.z/v_im_dims.at(im_form->getCurrentImageIndex()).zdim, \
                                     v_im_dims.at(im_form->getCurrentImageIndex()).xdim, v_im_dims.at(im_form->getCurrentImageIndex()).ydim, v_im_dims.at(im_form->getCurrentImageIndex()).zdim );
        emit sig_updateGL();
    }
    }

    void ImageContainer::changeSliceZ( int slice )
    {
        if (im_form->getCurrentImageIndex() != -1)
        {
            slice_position.z=slice*v_im_dims.at(im_form->getCurrentImageIndex()).zdim;
            im_form->setSlidersPosition( slice_position.x/v_im_dims.at(im_form->getCurrentImageIndex()).xdim, slice_position.y/v_im_dims.at(im_form->getCurrentImageIndex()).ydim, slice_position.z/v_im_dims.at(im_form->getCurrentImageIndex()).zdim, \
                                     v_im_dims.at(im_form->getCurrentImageIndex()).xdim, v_im_dims.at(im_form->getCurrentImageIndex()).ydim, v_im_dims.at(im_form->getCurrentImageIndex()).zdim );
            emit sig_updateGL();
         }
    }

        void ImageContainer::changeSliceX( float slice_mm )
    {
            if (im_form->getCurrentImageIndex() != -1)
            {
        slice_position.x=slice_mm;
        im_form->setSlidersPosition( slice_position.x/v_im_dims.at(im_form->getCurrentImageIndex()).xdim, slice_position.y/v_im_dims.at(im_form->getCurrentImageIndex()).ydim, slice_position.z/v_im_dims.at(im_form->getCurrentImageIndex()).zdim, \
                                     v_im_dims.at(im_form->getCurrentImageIndex()).xdim, v_im_dims.at(im_form->getCurrentImageIndex()).ydim, v_im_dims.at(im_form->getCurrentImageIndex()).zdim );
        emit sig_updateGL();
    }
    }

    void ImageContainer::changeSliceY( float slice_mm )
    {
        if (im_form->getCurrentImageIndex() != -1)
        {
        slice_position.y=slice_mm;
        im_form->setSlidersPosition( slice_position.x/v_im_dims.at(im_form->getCurrentImageIndex()).xdim, slice_position.y/v_im_dims.at(im_form->getCurrentImageIndex()).ydim, slice_position.z/v_im_dims.at(im_form->getCurrentImageIndex()).zdim, \
        v_im_dims.at(im_form->getCurrentImageIndex()).xdim, v_im_dims.at(im_form->getCurrentImageIndex()).ydim, v_im_dims.at(im_form->getCurrentImageIndex()).zdim );
        emit sig_updateGL();
    }
    }

    void ImageContainer::changeSliceZ( float slice_mm )
    {
        if (im_form->getCurrentImageIndex() != -1)
        {
        slice_position.z=slice_mm;
        im_form->setSlidersPosition( slice_position.x/v_im_dims.at(im_form->getCurrentImageIndex()).xdim, slice_position.y/v_im_dims.at(im_form->getCurrentImageIndex()).ydim, slice_position.z/v_im_dims.at(im_form->getCurrentImageIndex()).zdim, \
                                     v_im_dims.at(im_form->getCurrentImageIndex()).xdim, v_im_dims.at(im_form->getCurrentImageIndex()).ydim, v_im_dims.at(im_form->getCurrentImageIndex()).zdim );
        emit sig_updateGL();
    }
    }

    void ImageContainer::changeImageSpace(int space){
        vector<int> cur_indices = im_form->getCurrentImageIndices();
         for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
         {
             cout<<"change image space "<<*i_ind<<" "<<space<<endl;
           v_im_space[*i_ind] = static_cast<IMAGE_SPACE>(space);
    }
       // cout<<"change image space"<<endl;
        emit sig_updateGL();

    }
    void ImageContainer::setImVidCaptureForm( ImageVideoCapture* vid_form_in )
    {
        vid_form = vid_form_in;
    }

    void ImageContainer::updateImageProperties(int index)
    {
//        cout<<"update image properties"<<endl;
        vid_form->setImageExtents(static_cast<int>(v_im_dims.at(index).xsize/v_im_dims.at(index).xdim),\
                                   static_cast<int>(v_im_dims.at(index).ysize/v_im_dims.at(index).ydim),\
                                   static_cast<int>(v_im_dims.at(index).zsize/v_im_dims.at(index).zdim));
        //cout<<"update image properties "<<done<<endl;

            im_form->setSlidersExtents(static_cast<int>(v_im_dims.at(index).xsize/v_im_dims.at(index).xdim),\
                                       static_cast<int>(v_im_dims.at(index).ysize/v_im_dims.at(index).ydim),\
                                       static_cast<int>(v_im_dims.at(index).zsize/v_im_dims.at(index).zdim));
            im_form->setSlidersPosition( static_cast<int>(slice_position.x/v_im_dims.at(index).xdim), static_cast<int>(slice_position.y/v_im_dims.at(index).ydim), static_cast<int>(slice_position.z/v_im_dims.at(index).zdim), \
                                         v_im_dims.at(index).xdim, v_im_dims.at(index).ydim, v_im_dims.at(index).zdim );
            im_form->setSlicesVisibility( slice_visibility[index] );

        //    im_form->setImageMin(v_im_i_range.at(index).x);
         //   im_form->setImageMax(v_im_i_range.at(index).y);

            im_form->setImageOpacity( v_im_opacity_abs[index] );
            im_form->setImageOpacitySlider( v_im_opacity_abs[index] );
//            cout<<"update image properties2 "<<index<<" "<<v_im_onoff[0]<<" "<<v_im_onoff[1]<<" "<<v_im_onoff[2]<<endl;
            im_form->toggleImage(v_im_onoff[index]);

          //  int cmap_index=v_cmap_index.at(index);
             im_form->setColourTable(v_im_ctables.at(index));//im_form->getCurrentImageIndex()));
//cout<<"ctable index "<<index<<" "<<v_cmap_index[index]<<endl;
             im_form->setColourTableIndex( v_cmap_index[index] );
             //cout<<"end image properties"<<endl;
            im_form->setCoordinateSpace(v_im_space[index]);

    }

    void ImageContainer::setOpacityMode(int mode)
    {
        rel_opacity=mode;
        emit sig_updateGL();

    }

    void ImageContainer::changedOpacity(QString op)
    {

        if (im_form->getCurrentImageIndex() != -1)
        {
            glUseProgram(shaderProgram);
            vector<int> cur_indices = im_form->getCurrentImageIndices();
             for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
             {
               v_im_opacity_abs[*i_ind]=op.toFloat();
                }
        computeAndSetRelativeOpacities();
        emit sig_updateGL();
    }
    }
    void ImageContainer::toggleImage(bool state)
    {
        glUseProgram(shaderProgram);
        vector<int> cur_indices = im_form->getCurrentImageIndices();
        cout<<"ImageContainer::toggleImage " <<cur_indices.size()<<endl;
        for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
         {
             cout<<"set image state "<<*i_ind<<" "<<state<<endl;
          v_im_onoff[*i_ind]=state;
        }
         computeAndSetRelativeOpacities();

        emit sig_updateGL();
    }


    void ImageContainer::changeBlendFunc(int index)
    {
        blendFunc=index;
        emit sig_updateGL();
    }

    void ImageContainer::swapImageProperties( int index)
    {

        //cout<<"Swap image propereties "<<im_form->getCurrentImageIndex()<<" "<<index<<endl;
        //number of images should always match number in form
        //max index is size -1
    //    int cur_index = static_cast<int>(v_im_onoff.size() -1 - im_form->getCurrentImageIndex());
        int cur_index = im_form->getCurrentImageIndex();

       // index= static_cast<int>(v_im_onoff.size() -1 - index);
     //   if (cur_index >=0)
      //  {
        {
          //  cout<<"swap image space1 "<<index<<" "<<cur_index<<" "<<v_im_space[index]<<" "<<v_im_space[cur_index]<<endl;
            IMAGE_SPACE temp = v_im_space[index];
            v_im_space[index] = v_im_space[cur_index];
            v_im_space[cur_index] = temp;
            cout<<"swap image space2 "<<index<<" "<<cur_index<<" "<<v_im_space[index]<<" "<<v_im_space[cur_index]<<endl;


        }

        {//colour map index
            //cout<<"cmap swap1 "<<index<<" "<< v_cmap_index[index]<<endl;
            //cout<<"cmap swap2 "<<cur_index<<" "<< v_cmap_index[cur_index]<<endl;

            int temp = v_cmap_index[index];
            v_cmap_index[index] = v_cmap_index[cur_index];
            v_cmap_index[cur_index] = temp;
        }

        {//texture Names
            GLuint temp = texNames[index];
            texNames[index] = texNames[cur_index];
            texNames[cur_index] = temp;

        }

        {//image dimensions
            image_dims temp=v_im_dims[index];
            v_im_dims[index] = v_im_dims[cur_index];
            v_im_dims[cur_index] = temp;
        }

        {//image intensity range
            float2 temp=v_im_i_range[index];
            v_im_i_range[index] = v_im_i_range[cur_index];
            v_im_i_range[cur_index] = temp;
        }

 //      {//image intensity bias
   //         GLfloat temp=v_im_thresh_bias[index];
     //       v_im_thresh_bias[index] = v_im_thresh_bias[cur_index];
       //     v_im_thresh_bias[cur_index] = temp;
       // }

    //    {//image intensity scale
      //      GLfloat temp=v_im_thresh_scale[index];
        //    v_im_thresh_scale[index] = v_im_thresh_scale[cur_index];
          //  v_im_thresh_scale[cur_index] = temp;
        //}
        {//image intensity bias
            bool3 temp=slice_visibility[index];
            slice_visibility[index] = slice_visibility[cur_index];
            slice_visibility[cur_index] = temp;
        }

        {//image intensity bias
            float2 temp=v_im_i_range[index];
            v_im_i_range[index] = v_im_i_range[cur_index];
            v_im_i_range[cur_index] = temp;
        }

        {//image intensity op_rel
            GLfloat temp=v_im_opacity_rel[index];
            v_im_opacity_rel[index] = v_im_opacity_rel[cur_index];
            v_im_opacity_rel[cur_index] = temp;
        }
        {//image intensity op_abs
            GLfloat temp=v_im_opacity_abs[index];
            v_im_opacity_abs[index] = v_im_opacity_abs[cur_index];
            v_im_opacity_abs[cur_index] = temp;
        }

        {//image intensity op_abs
            GLint temp=v_im_onoff[index];
            v_im_onoff[index] = v_im_onoff[cur_index];
            v_im_onoff[cur_index] = temp;
        }

        {//image intensity colour table
            fslsurface_name::colour_table temp=v_im_ctables[index];
            v_im_ctables[index] = v_im_ctables[cur_index];
            v_im_ctables[cur_index] = temp;
        }
        updateImageProperties(cur_index);
        emit sig_updateGL();

    }
 //   }

    void ImageContainer::renderColourBar(){
      //cout<<"redner image colour bar"<<endl;
        if (c_bar_onoff)
        {
        //cout<<"RenderColour "<<shaderProgram_cbar<<endl;
        glUseProgram(shaderProgram_cbar);//glsl_programs.at(2));

        if (c_bar != NULL)
        {
            updateColourTable(im_form->getCurrentImageIndex());

             //           c_bar->generateColourBar(0,v_ctables.at(surf_form->getCurrentSurfaceIndex()));
            cout<<"rimage not null "<<"render color bar "<<endl;
            c_bar->renderColourBar();
        }
    }
    }

    void ImageContainer::renderSlices()
    {
        switch (blendFunc)
        {

        case 0:
            glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
            break;
        case 1:
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);
                    break;
        case 2:
                    glBlendFunc(GL_ONE_MINUS_DST_ALPHA , GL_ONE);
                    break;
        case 3:
                    glBlendFunc(GL_SRC_ALPHA , GL_DST_ALPHA);
                    break;
        case 4:
            glBlendFunc(GL_ONE_MINUS_DST_ALPHA , GL_DST_ALPHA);
                    break;
        default:
            break;
        }
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );


        GLuint imageN=0;
      //  GLuint imageN=texNames.size()-1;
        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glDepthMask(GL_TRUE);
        glEnable ( GL_TEXTURE_3D ) ;
        //cout<<"shader program "<<shaderProgram<<endl;
        vector<IMAGE_SPACE>::iterator i_im_space = v_im_space.begin();
        for (vector<GLuint>::iterator i_tex=texNames.begin();i_tex!=texNames.end();++i_tex,++imageN, ++i_im_space)
        {
//glPolygonOffset(1.0, 1.0);


            if (v_im_onoff[imageN])
            {
                glMatrixMode(GL_MODELVIEW);
                //handle XFM matrices
                double* m = new double[16];
//this is where image space is used
                //currently only allows to spaces
                //the other defaults space doesn't used any xfm
                if (*i_im_space == NIFTI)
                {
                    glPushMatrix();
                    glGetDoublev(GL_MODELVIEW_MATRIX, m);
                    glLoadIdentity();
                    glMultMatrixd(m);
                    //cout<<"vxfm  "<<v_xfm.size()<<" "<<imageN<<endl;
                  //  cout<<"image space "<<*i_im_space<<endl;
                    glMultMatrixd(&(v_xfm[imageN][0]));

                    // glMultTransposeMatrixd(&(v_xfm[imageN][0]));
                }


                if (rel_opacity)
                {
                    glUniform1f(loc_op,v_im_opacity_rel[imageN]);
                }else
                {
                    glUniform1f(loc_op,v_im_opacity_abs[imageN]);
                }

             //   glUniform1f(loc_th_bias,v_im_thresh_bias[imageN]);
              //  glUniform1f(loc_th_scale,v_im_thresh_scale[imageN]);

                updateColourTable(imageN);

                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                //for (vector<GLuint>::iterator i=texNames.begin(); i!=texNames.end();i++,texN++)
                //{
                glBindTexture(GL_TEXTURE_3D, *i_tex);
                //}
                glBegin(GL_QUADS);

                //x
                float half_vox_x = v_im_dims[imageN].xdim/2.0;
                float half_vox_y = v_im_dims[imageN].ydim /2.0 ;
                float half_vox_z = v_im_dims[imageN].zdim /2.0 ;
                float xdim =  v_im_dims[imageN].xdim;
                float ydim =  v_im_dims[imageN].ydim;
                float zdim =  v_im_dims[imageN].zdim;


                if (slice_visibility[imageN].x){


                //    glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,0.0,0.0); glVertex3f( slice_position.x , -half_vox_y                      , -half_vox_z); glNormal3f(0,1.0,0);
                  //  glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,0.0,1.0); glVertex3f( slice_position.x , -half_vox_y                       , v_im_dims[imageN].zsize+half_vox_z);glNormal3f(0,1.0,0);
                  //  glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,1.0,1.0); glVertex3f( slice_position.x , v_im_dims[imageN].ysize+half_vox_y, v_im_dims[imageN].zsize+half_vox_z);glNormal3f(0,1.0,0);
                  //  glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,1.0,0.0); glVertex3f( slice_position.x , v_im_dims[imageN].ysize+half_vox_y, -half_vox_z);glNormal3f(0,1.0,0);

              //      glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,0.0,0.0); glVertex3f( slice_position.x / xdim , -half_vox_y       / ydim                , -half_vox_z/zdim); glNormal3f(0,1.0,0);
                //    glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,0.0,1.0); glVertex3f( slice_position.x / xdim , -half_vox_y        / ydim               , (v_im_dims[imageN].zsize+half_vox_z)/zdim);glNormal3f(0,1.0,0);
                  //  glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,1.0,1.0); glVertex3f( slice_position.x / xdim , (v_im_dims[imageN].ysize+half_vox_y)/ ydim, (v_im_dims[imageN].zsize+half_vox_z)/zdim);glNormal3f(0,1.0,0);
                    //glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,1.0,0.0); glVertex3f( slice_position.x / xdim , (v_im_dims[imageN].ysize+half_vox_y)/ ydim, -half_vox_z/zdim);glNormal3f(0,1.0,0);

                    glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,half_vox_y/v_im_dims[imageN].ysize,half_vox_z/v_im_dims[imageN].zsize);
                   // glVertex3f( slice_position.x / xdim ,0 , 0); glNormal3f(0,1.0,0);
                    glVertex3f( slice_position.x ,0 , 0); glNormal3f(0,1.0,0);

                    glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,half_vox_y/v_im_dims[imageN].ysize,(v_im_dims[imageN].zsize-half_vox_z) / v_im_dims[imageN].zsize);
                    glVertex3f( slice_position.x , 0, (v_im_dims[imageN].zsize) - zdim );glNormal3f(0,1.0,0);

                    glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,(v_im_dims[imageN].ysize-half_vox_y) / v_im_dims[imageN].ysize,(v_im_dims[imageN].zsize-half_vox_z) / v_im_dims[imageN].zsize);
                    glVertex3f( slice_position.x  , (v_im_dims[imageN].ysize) - ydim, (v_im_dims[imageN].zsize) - zdim );glNormal3f(0,1.0,0);

                    glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,(v_im_dims[imageN].ysize-half_vox_y) / v_im_dims[imageN].ysize,half_vox_z/v_im_dims[imageN].zsize);
                    glVertex3f( slice_position.x  , (v_im_dims[imageN].ysize) - ydim  , 0);glNormal3f(0,1.0,0);


                    /*
                glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,0.0,0.0); glVertex3f( slice_position.x , -half_vox_y                       , -half_vox_z);glNormal3f(0,1.0,0);
                glTexCoord3f((slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,1.0, 0.0); glVertex3f( slice_position.x , v_im_dims[imageN].ysize+half_vox_y, -half_vox_z);glNormal3f(0,1.0,0);
                glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,1.0,1.0); glVertex3f( slice_position.x , v_im_dims[imageN].ysize+half_vox_y,  v_im_dims[imageN].zsize+half_vox_z);glNormal3f(0,1.0,0);
                glTexCoord3f( (slice_position.x + half_vox_x)/v_im_dims[imageN].xsize,0.0,1.0); glVertex3f( slice_position.x , -half_vox_y                        , v_im_dims[imageN].zsize+half_vox_z);glNormal3f(0,1.0,0);
*/
                }

                //y
                if (slice_visibility[imageN].y){
//                    glTexCoord3f(0.0, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,0.0); glVertex3f(-half_vox_x, slice_position.y, -half_vox_z); glNormal3f(0,1.0,0);
  //                  glTexCoord3f(0.0, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,1.0); glVertex3f(-half_vox_x, slice_position.y, v_im_dims[imageN].zsize+half_vox_z);glNormal3f(0,1.0,0);
    //                glTexCoord3f(1.0, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,1.0); glVertex3f(v_im_dims[imageN].xsize+half_vox_x, slice_position.y, v_im_dims[imageN].zsize+half_vox_z);glNormal3f(0,1.0,0);
      //              glTexCoord3f(1.0, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,0.0); glVertex3f(v_im_dims[imageN].xsize+half_vox_x, slice_position.y, -half_vox_z);glNormal3f(0,1.0,0);
                //cout<<"render y "<<slice_position.y <<" "<< half_vox_y<<" "<<v_im_dims[imageN].ysize<<" "<< (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize<<endl;
      //              glTexCoord3f(0.0, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,0.0); glVertex3f(-half_vox_x / xdim, slice_position.y/ ydim, -half_vox_z/zdim); glNormal3f(0,1.0,0);
        //            glTexCoord3f(0.0, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,1.0); glVertex3f(-half_vox_x / xdim, slice_position.y/ ydim, (v_im_dims[imageN].zsize+half_vox_z)/zdim);glNormal3f(0,1.0,0);
          //          glTexCoord3f(1.0, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,1.0); glVertex3f((v_im_dims[imageN].xsize+half_vox_x) / xdim, slice_position.y/ ydim, (v_im_dims[imageN].zsize+half_vox_z)/zdim);glNormal3f(0,1.0,0);
            //        glTexCoord3f(1.0, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,0.0); glVertex3f((v_im_dims[imageN].xsize+half_vox_x) / xdim, slice_position.y/ ydim, -half_vox_z/zdim);glNormal3f(0,1.0,0);
      //              glTexCoord3f(half_vox_x/v_im_dims[imageN].xsize, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,half_vox_z/v_im_dims[imageN].zsize); glVertex3f(-half_vox_x / xdim, slice_position.y/ ydim, -half_vox_z/zdim); glNormal3f(0,1.0,0);
    //                glTexCoord3f(half_vox_x/v_im_dims[imageN].xsize, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,(v_im_dims[imageN].zsize-half_vox_z) / v_im_dims[imageN].zsize); glVertex3f(-half_vox_x / xdim, slice_position.y/ ydim, (v_im_dims[imageN].zsize+half_vox_z)/zdim);glNormal3f(0,1.0,0);
  //                  glTexCoord3f((v_im_dims[imageN].xsize-half_vox_x) / v_im_dims[imageN].xsize, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,(v_im_dims[imageN].zsize-half_vox_z) / v_im_dims[imageN].zsize); glVertex3f((v_im_dims[imageN].xsize+half_vox_x) / xdim, slice_position.y/ ydim, (v_im_dims[imageN].zsize+half_vox_z)/zdim);glNormal3f(0,1.0,0);
//                    glTexCoord3f((v_im_dims[imageN].xsize-half_vox_x) / v_im_dims[imageN].xsize, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,half_vox_z/v_im_dims[imageN].zsize); glVertex3f((v_im_dims[imageN].xsize+half_vox_x) / xdim, slice_position.y/ ydim, -half_vox_z/zdim);glNormal3f(0,1.0,0);

glTexCoord3f(half_vox_x/v_im_dims[imageN].xsize, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,half_vox_z/v_im_dims[imageN].zsize);
//glTexCoord3f(half_vox_x/v_im_dims[imageN].xsize, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,0);
//cout<<"slicey "<<imageN<<" "<<slice_position.y<<endl;
glVertex3f(0, slice_position.y, 0); glNormal3f(0,1.0,0);

glTexCoord3f(half_vox_x/v_im_dims[imageN].xsize, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,(v_im_dims[imageN].zsize-half_vox_z) / v_im_dims[imageN].zsize);
glVertex3f(0, slice_position.y, v_im_dims[imageN].zsize - zdim  );glNormal3f(0,1.0,0);

glTexCoord3f((v_im_dims[imageN].xsize-half_vox_x) / v_im_dims[imageN].xsize, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,(v_im_dims[imageN].zsize-half_vox_z) / v_im_dims[imageN].zsize);
glVertex3f( v_im_dims[imageN].xsize - xdim , slice_position.y, (v_im_dims[imageN].zsize) - zdim );glNormal3f(0,1.0,0);

glTexCoord3f((v_im_dims[imageN].xsize-half_vox_x) / v_im_dims[imageN].xsize, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,half_vox_z/v_im_dims[imageN].zsize);
glVertex3f( v_im_dims[imageN].xsize - xdim , slice_position.y, 0);glNormal3f(0,1.0,0);


/*
                glTexCoord3f(0.0, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,0.0); glVertex3f(-half_vox_x, slice_position.y, -half_vox_z);glNormal3f(0,1.0,0);
                glTexCoord3f(1.0, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,0.0); glVertex3f(v_im_dims[imageN].xsize+half_vox_x, slice_position.y, -half_vox_z);glNormal3f(0,1.0,0);
                glTexCoord3f(1.0, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,1.0); glVertex3f(v_im_dims[imageN].xsize+half_vox_x, slice_position.y, v_im_dims[imageN].zsize+half_vox_z);glNormal3f(0,1.0,0);
                glTexCoord3f(0.0, (slice_position.y + half_vox_y)/v_im_dims[imageN].ysize,1.0); glVertex3f(-half_vox_x, slice_position.y, v_im_dims[imageN].zsize+half_vox_z);glNormal3f(0,1.0,0);
*/
                }


                if (slice_visibility[imageN].z){
                    //z
              //      glTexCoord3f(0.0,0.0, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize); glVertex3f(0.0, 0.0,slice_position.z); glNormal3f(0,1.0,0);
              //      glTexCoord3f(0.0,1.0, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize); glVertex3f(0.0, v_im_dims[imageN].ysize, slice_position.z);glNormal3f(0,1.0,0);
                //    glTexCoord3f(1.0,1.0, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize); glVertex3f(v_im_dims[imageN].xsize, v_im_dims[imageN].ysize, slice_position.z);glNormal3f(0,1.0,0);
                //    glTexCoord3f(1.0,0.0, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize); glVertex3f(v_im_dims[imageN].xsize,  0.0,slice_position.z);glNormal3f(0,1.0,0);
                //    glTexCoord3f(0.0,0.0, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize); glVertex3f(-half_vox_x / xdim, -half_vox_y / ydim,slice_position.z/zdim); glNormal3f(0,1.0,0);
                  //  glTexCoord3f(0.0,1.0, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize); glVertex3f(-half_vox_x / xdim, (v_im_dims[imageN].ysize+half_vox_y)/ ydim, slice_position.z/zdim);glNormal3f(0,1.0,0);
                 //   glTexCoord3f(1.0,1.0, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize); glVertex3f((v_im_dims[imageN].xsize+half_vox_x) / xdim, (v_im_dims[imageN].ysize+half_vox_y)/ ydim, slice_position.z/zdim);glNormal3f(0,1.0,0);
                 //   glTexCoord3f(1.0,0.0, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize); glVertex3f((v_im_dims[imageN].xsize+half_vox_x) / xdim,  -half_vox_y       / ydim,slice_position.z/zdim);glNormal3f(0,1.0,0);
                    glTexCoord3f(half_vox_x/v_im_dims[imageN].xsize,half_vox_y/v_im_dims[imageN].ysize, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize);
                    glVertex3f(0, 0,slice_position.z); glNormal3f(0,1.0,0);

                    glTexCoord3f(half_vox_x/v_im_dims[imageN].xsize,(v_im_dims[imageN].ysize-half_vox_y) / v_im_dims[imageN].ysize, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize);
                    glVertex3f(0, (v_im_dims[imageN].ysize) -ydim, slice_position.z);
                    glNormal3f(0,1.0,0);

                    glTexCoord3f((v_im_dims[imageN].xsize-half_vox_x) / v_im_dims[imageN].xsize,(v_im_dims[imageN].ysize-half_vox_y) / v_im_dims[imageN].ysize, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize);
                    glVertex3f((v_im_dims[imageN].xsize) -xdim, (v_im_dims[imageN].ysize) -ydim , slice_position.z);glNormal3f(0,1.0,0);

                    glTexCoord3f((v_im_dims[imageN].xsize-half_vox_x) / v_im_dims[imageN].xsize,half_vox_y/v_im_dims[imageN].ysize, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize);
                    glVertex3f((v_im_dims[imageN].xsize) -xdim, 0 ,slice_position.z);glNormal3f(0,1.0,0);

                    /*
                glTexCoord3f(0.0,0.0, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize); glVertex3f(0.0, 0.0, slice_position.z);glNormal3f(0,1.0,0);
                glTexCoord3f(1.0,0.0, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize); glVertex3f(v_im_dims[imageN].xsize, 0.0, slice_position.z);glNormal3f(0,1.0,0);
                glTexCoord3f(1.0,1.0, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize); glVertex3f(v_im_dims[imageN].xsize, v_im_dims[imageN].ysize, slice_position.z);glNormal3f(0,1.0,0);
                glTexCoord3f(0.0,1.0, (slice_position.z + half_vox_z)/v_im_dims[imageN].zsize); glVertex3f(0.0,  v_im_dims[imageN].ysize,slice_position.z);glNormal3f(0,1.0,0);
*/
                }

                glEnd();
                //  texN=0;
                //   for (vector<GLuint>::iterator i=texNames.begin(); i!=texNames.end();i++,texN++)
                // {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_3D, 0);
                //}

                if ( *i_im_space == NIFTI )
                {
                    glPopMatrix();

                    glGetDoublev(GL_MODELVIEW_MATRIX, m);
                }
                //cout<<"m2 "<<m[0]<<" "<<m[1]<<" "<<m[2]<<" "<<m[3]<<endl;
                //cout<<"m2 "<<m[4]<<" "<<m[5]<<" "<<m[6]<<" "<<m[7]<<endl;
                //cout<<"m2 "<<m[8]<<" "<<m[9]<<" "<<m[10]<<" "<<m[11]<<endl;
                //cout<<"m2 "<<m[12]<<" "<<m[13]<<" "<<m[14]<<" "<<m[15]<<endl;


                delete[] m;

            }

        }

    }





}
