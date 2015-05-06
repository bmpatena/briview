#include "surfacecontainer.h"
#include <vector>
#include <QtOpenGL>
#include "fslvtkio/fslvtkio.h"
#include "struct_io.h"
#include <QFileDialog>


//#include <fslsurface/fslsurface_structs.h>
//#include <fslsurface/fslsurfaceio.h>
//#include <fslsurface/fslsurfacegl.h>

//#include <fslsurface/fslsurface_structs.h>

#include "GlyphGenerator.h"
extern "C" {
#include "giftiio/gifti_io.h"
}
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include "vectorFunctions.h"

//using namespace briview_structs;

using namespace fslvtkio;
using namespace fslsurface_name;
using namespace std;


namespace briview{



    SurfaceContainer::SurfaceContainer( )
    {
        surf_form=new SurfaceManipulator();
        preset_materials_names.push_back("Custom");
        loadColourTables((QApplication::applicationDirPath() + "/assets/colour_maps.txt").toStdString());

       connect(surf_form,SIGNAL(sig_changedGlyphProperties()), this , SLOT(updateGlyphProperties()));
        connect(surf_form,SIGNAL(sig_changedSurfaceProperties()), this , SLOT(updateSurfaceProperties()));


        connect(surf_form,SIGNAL(sig_changedColourTable()),this, SLOT(updateColourTable()));
        connect(surf_form,SIGNAL(sig_changedColourTableSc()),this, SLOT(updateColourTableSc()));

        //connect(surf_form,SIGNAL(sig_changedColourTable()), this , SLOT(changeColourTable()));
        connect(surf_form,SIGNAL(sig_changedColourTable(int)),this, SLOT(changeColourTable(int)));


        connect(surf_form,SIGNAL(sig_changeShaderProgram(bool)), this , SLOT(changeShaderProgram(bool)));
        connect(surf_form,SIGNAL(sig_changeSurfaceMaterial(int)), this, SLOT(changeMaterial(int)));
        connect(surf_form,SIGNAL(sig_changedCurrentSurface(int)), this , SLOT(updateSurfaceProperties(int)));
        connect(surf_form,SIGNAL(sig_changedColourBar()), this , SLOT(updateColourBar()));
        connect(surf_form,SIGNAL(sig_changedPolygonModeFront(int)), this , SLOT(updatePolygonModeFront(int)));
        connect(surf_form,SIGNAL(sig_changedPolygonModeBack(int)), this , SLOT(updatePolygonModeBack(int)));
        connect(surf_form,SIGNAL(sig_changedPolygonModeFrontGlyph(int)), this , SLOT(updatePolygonModeFrontGlyph(int)));
        connect(surf_form,SIGNAL(sig_changedPolygonModeBackGlyph(int)), this , SLOT(updatePolygonModeBackGlyph(int)));
    //pass on signal from manipulator
        connect(surf_form, SIGNAL(sig_appendSurfaceData()),this,SIGNAL(sig_appendSurfaceData()));
        connect(surf_form, SIGNAL(sig_changedScalarData()),this,SLOT(changeScalarData()));
        connect(surf_form, SIGNAL(sig_updateAppendedSurfaceInfo(int)),this,SLOT(updateAppendedInfo(int)));
        connect(surf_form, SIGNAL(sig_playSurfaceScalars()),this,SLOT(playScalarData()));
        connect(surf_form, SIGNAL(sig_prevSurfaceScalars()),this,SLOT(prevScalarData()));
        connect(surf_form, SIGNAL(sig_nextSurfaceScalars()),this,SLOT(nextScalarData()));



        connect(surf_form, SIGNAL(sig_displayGlyphs(bool)),this,SLOT(displayGlyphs(bool)));
        connect(surf_form, SIGNAL(sig_enableCullFace(bool)),this,SLOT(toggleCullFace(bool)));
        connect(surf_form, SIGNAL(sig_ff_ccw(bool)),this,SLOT(setFrontFaceCCW(bool)));

        connect(surf_form,SIGNAL(sig_changedBlendFunc(int) ),this,SLOT(changeBlendFunc(int)));
        connect(surf_form,SIGNAL(sig_changedOpacityMode(int)),this, SLOT(setOpacityMode(int)));

        c_bar=NULL;
        c_bar_onoff=false;
//        surf_form->setUseScalarMap(1);
        blendFunc=0;
        rel_opacity=0;

    }
    SurfaceContainer::~SurfaceContainer( )
    {
        for ( vector<GLuint*>::iterator i = vbos_vertices.begin(); i!=vbos_vertices.end();i++)
            glDeleteBuffers(2,*i);

        for ( vector< fslSurface<float,unsigned int>* >::iterator i_surf = v_surfaces.begin(); i_surf != v_surfaces.end();++i_surf)
            delete *i_surf;

        //for ( std::vector<fslsurface_name::vertexFloat*>::iterator i = v_vertices.begin(); i != v_vertices.begin() ; i++)
         //   delete *i;

     //   for ( vector<fslsurface_name::float3*>::iterator i=v_vectors.begin(); i!= v_vectors.end();i++)
       //     delete[] (*i);

        if (c_bar!=NULL)
        {
            delete c_bar;
        }

        delete surf_form;
    }
    void SurfaceContainer::writeSurface( ) const
    {
        QString fileName = QFileDialog::getSaveFileName(0, tr("Save Surface"),QDir::currentPath(),tr("Surfaces (*.gii)"));
       // cout<<"filename save "<<fileName.toStdString()<<endl;
        if (fileName=="")
            return;
        if (surf_form->getCurrentSurfaceIndex() == -1)
            return;
         writeGIFTI(*v_surfaces[surf_form->getCurrentSurfaceIndex()],fileName.toStdString());

    }
    void SurfaceContainer::writeSurfaces( ofstream *fout) const
    {
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
    }
    void SurfaceContainer::changeBlendFunc(int index)
    {
        blendFunc=index;
        emit sig_updateGL();
    }
    void SurfaceContainer::setOpacityMode(int mode)
    {
        rel_opacity=mode;
        emit sig_updateGL();

    }
    void SurfaceContainer::importPresetMaterials( const string & filename)
    {
        ifstream fin(filename.c_str());
        string mat_name;
        while ( fin>> mat_name)
        {
            material mat;
            fin>> mat.ambient[0]>> mat.ambient[1]>> mat.ambient[2]>> mat.ambient[3]>> mat.diffuse[0]>> mat.diffuse[1]>> mat.diffuse[2]>> mat.diffuse[3]>> mat.specular[0]>> mat.specular[1]>> mat.specular[2]>> mat.specular[3]>> mat.shininess;
            mat.opacity=1.0;
            preset_materials.push_back(mat);
            preset_materials_names.push_back(mat_name);

            surf_form->addPresetSetMaterial(mat_name);
        }
    }

    void SurfaceContainer::changeMaterial(int index)
    {
        if ( (vbos_vertices.size()>0))
        {
            vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
            for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
            {
                //cout<<"change material "<<*i_ind<<endl;
                if (index>0)
                {
                    v_materials.at(*i_ind)=preset_materials.at(index-1);
                }
                surf_form->setSurfaceMaterial(v_materials.at(*i_ind));
                surf_material_index.at(*i_ind)=index;

            }
            emit sig_updateGL();
        }else{
            surf_form->setSurfaceMaterial(preset_materials.at(index-1));
        }

    }

    void SurfaceContainer::toggleCullFace( bool cull )
    {

        //cout<<"cullface "<<cull<<endl;
      //  cullFace[ surf_form->getCurrentAppendedSurfaceIndex() ] = cull;
        vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
        for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
        {
            //cout<<"toggle cull face "<<*i_ind<<" "<<cull<<endl;
            cullFace[ *i_ind ] = cull;
       }
        emit sig_updateGL();

    }
    void SurfaceContainer::setFrontFaceCCW( bool CCW )
    {
        vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
        for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
        {
            frontFace[ *i_ind ] = CCW;
            //if (cullFace[ *i_ind ])
            //     frontFace[ surf_form->getCurrentAppendedSurfaceIndex() ] = CCW;
            //   if (cullFace[ surf_form->getCurrentAppendedSurfaceIndex() ])
        }

        emit sig_updateGL();

    }


    void SurfaceContainer::changeScalarData()
    {
//        /////cout<<"change scalar data "<<surf_form->getCurrentAppendedSurfaceIndex()<<" "<<scalar_indices[surf_form->getCurrentAppendedSurfaceIndex()]<<endl;
        //plus one because first load surface
        //cout<<"change scalar data "<<endl;
        if (surf_form->getCurrentAppendedSurfaceIndex() == -1 )
            return;

        vector<int> scalar_inds = v_surfaces[surf_form->getCurrentSurfaceIndex()]->getScalarIndices(surf_form->getCurrentAppendedSurfaceIndex()); //+1 to adjust for initial surf

        if (!scalar_inds.empty()){
                //cout<<"set scalars "<<scalar_inds[0]<<endl;
            v_surfaces[surf_form->getCurrentSurfaceIndex()]->setScalars(scalar_inds[0]);//account for original surface
           glBufferSubData_Vertices( *v_surfaces[surf_form->getCurrentSurfaceIndex()],vbos_vertices[surf_form->getCurrentSurfaceIndex()][0]);
             emit sig_updateGL();
            }
    }

    void SurfaceContainer::prevScalarData()
    {
//        /////cout<<"change scalar data "<<surf_form->getCurrentAppendedSurfaceIndex()<<" "<<scalar_indices[surf_form->getCurrentAppendedSurfaceIndex()]<<endl;
        //plus one because first load surface
        ////cout<<"prev scalar data "<<surf_form->getCurrentAppendedSurfaceIndex()+1<<endl;
        if (surf_form->getCurrentAppendedSurfaceIndex() == -1 )
            return;

        vector<int> scalar_inds = v_surfaces[surf_form->getCurrentSurfaceIndex()]->getScalarIndices(surf_form->getCurrentAppendedSurfaceIndex()); //+1 to adjust for initial surf

        int prevScalar = surf_form->getCurrentScalarIndex()-1;
        if (prevScalar<0) return;
        //cout<<"prev sc "<<prevScalar<<" "<<scalar_inds[prevScalar]<<" "<<surf_form->getCurrentSurfaceIndex()<<endl;

        surf_form->setScalarIndicesMinMax(prevScalar, scalar_inds.back());
        v_surfaces[surf_form->getCurrentSurfaceIndex()]->setScalars(scalar_inds[prevScalar]);//+1 account for original surface
        glBufferSubData_Vertices(*v_surfaces[surf_form->getCurrentSurfaceIndex()],vbos_vertices[surf_form->getCurrentSurfaceIndex()][0]);

        emit sig_updateGL();

    }
    void SurfaceContainer::nextScalarData()
    {
        //        /////cout<<"change scalar data "<<surf_form->getCurrentAppendedSurfaceIndex()<<" "<<scalar_indices[surf_form->getCurrentAppendedSurfaceIndex()]<<endl;
                //plus one because first load surface
                ////cout<<"next scalar data "<<surf_form->getCurrentAppendedSurfaceIndex()+1<<endl;
                if (surf_form->getCurrentAppendedSurfaceIndex() == -1 )
                    return;

                vector<int> scalar_inds = v_surfaces[surf_form->getCurrentSurfaceIndex()]->getScalarIndices(surf_form->getCurrentAppendedSurfaceIndex()); //+1 to adjust for initial surf

                int nextScalar = surf_form->getCurrentScalarIndex()+1;
                if (static_cast<unsigned int>(nextScalar)>=scalar_inds.size()) return;

                surf_form->setScalarIndicesMinMax(nextScalar, scalar_inds.back());
                //cout<<"next sc "<<nextScalar<<" "<<scalar_inds[nextScalar]<<" "<<surf_form->getCurrentSurfaceIndex()<<endl;
                v_surfaces[surf_form->getCurrentSurfaceIndex()]->setScalars(scalar_inds[nextScalar]);//+1 account for original surface
                glBufferSubData_Vertices(*v_surfaces[surf_form->getCurrentSurfaceIndex()],vbos_vertices[surf_form->getCurrentSurfaceIndex()][0]);

                emit sig_updateGL();

    }

    void SurfaceContainer::playScalarData()
    {
//        /////cout<<"change scalar data "<<surf_form->getCurrentAppendedSurfaceIndex()<<" "<<scalar_indices[surf_form->getCurrentAppendedSurfaceIndex()]<<endl;
        //plus one because first load surface
        ////cout<<"change scalar data "<<surf_form->getCurrentAppendedSurfaceIndex()+1<<endl;
        if (surf_form->getCurrentAppendedSurfaceIndex() == -1 )
            return;

        vector<int> scalar_inds = v_surfaces[surf_form->getCurrentSurfaceIndex()]->getScalarIndices(surf_form->getCurrentAppendedSurfaceIndex()); //+1 to adjust for initial surf

        int max = scalar_inds.back();

        for (vector<int>::iterator i = scalar_inds.begin(); i!=scalar_inds.end(); ++i)
        {
            ////cout<<"play: scalar index "<<surf_form->getCurrentSurfaceIndex()<<" "<<*i<<endl;
            surf_form->setScalarIndicesMinMax(*i, max);
           //surf_form->update();
           v_surfaces[surf_form->getCurrentSurfaceIndex()]->setScalars(*i);//+1 account for original surface
           glBufferSubData_Vertices( *v_surfaces[surf_form->getCurrentSurfaceIndex()], vbos_vertices[surf_form->getCurrentSurfaceIndex()][0]);
           emit sig_updateGL();

        }


    }



    void SurfaceContainer::updateAppendedInfo( int index )
    {
        ////cout<<"update Appended info "<<endl;
        if (index != -1)
        {
            vector<int> scalar_inds = v_surfaces[surf_form->getCurrentSurfaceIndex()]->getScalarIndices(index); //+1 to adjust for initial surf
            ////cout<<"got scalar inds "<<endl;
            if (!scalar_inds.empty())
                surf_form->setScalarIndicesMinMax(scalar_inds.front(), scalar_inds.back());
            else
                surf_form->setScalarIndicesMinMax(-1,-1);
        }
    }


    void SurfaceContainer::changeGlyph(int index)
    {
        vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
        for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
        {
              surf_form->setGlyphProperties(v_glyph_props.at(*i_ind));
            // surf_material_index.at(surf_form->getCurrentSurfaceIndex())=index;
          }
            emit sig_updateGL();

    }


    void SurfaceContainer::updateSurfaceProperties()//this gets from form
    {
        //cout<<"up surface props "<<endl;
        surf_form->setMaterialIndex(0);
        vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
        for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
        {
            v_materials.at(*i_ind)=surf_form->getCurrentSurfaceMaterial();
        }
        emit sig_updateGL();
    }
    void SurfaceContainer::displayGlyphs( bool disp )
    {
        ////cout<<"display glyphs "<<endl;
        vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
        for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
        {
            showGlyphs[*i_ind]=disp;
        }
        emit sig_updateGL();

    }


    void SurfaceContainer::updateSurfaceProperties( int index )//this sets form
    {
        //cout<<"update surface properties"<<endl;
        surf_form->setSurfaceMaterial(v_materials.at(index));
        surf_form->setMaterialIndex( surf_material_index.at(index));
        surf_form->setShaderIndex(surf_glsl_programs.at(index));
        vector<string> app_names(1,"embedded_data");
        surf_form->setAppendedData(app_names);
        surf_form->setColourTable(v_surf_ctables[index]);
        //cout<<"update surface properties "<<cullFace[index]<<" "<<frontFace[index]<<endl;

        //poly renderers
        surf_form->setPolyMode(fill_type_f[index],fill_type_b[index] );
        surf_form->setPolyModeGlyph( fill_type_f_glyph[index],fill_type_b_glyph[index] );

        surf_form->setCullFace(  cullFace[index] );
         surf_form->setFrontFace( frontFace[index] );
    //    emit sig_updateGL();
    }

    void SurfaceContainer::updateGlyphProperties( )//this sets form
    {
        //cout<<"update glyhs "<<endl;
        //  surf_form->setGlyphProperties(surf_form->getCurrentSurfaceIndex());
        // surf_form->setMaterialIndex( surf_material_index.at(index));
        //    surf_form->setShaderIndex(surf_glsl_programs.at(index));
        vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
        for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
        {
            //cout<<"cur indices "<<*i_ind<<" "<<v_glyph_props.size()<<endl;
            v_glyph_props.at(*i_ind)=surf_form->getCurrentGlyph();
            if (v_glyph_props.at(*i_ind).N_faces>2)
                regenerateGlyphs(*i_ind);


        }
        emit sig_updateGL();
    }


    void SurfaceContainer::loadColourTables(const string & filename )
    {
        ifstream fin(filename.c_str());
        string temp;

        if (fin.is_open())
        {
            while (fin>>temp)
            {
                //cout<<"load colour map: "<<temp<<endl;
                cmap_names.push_back(temp);
                surf_form->addColourTable(temp);

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
                surf_form->setColourTableIndex(0);

        }else{
            cerr<<"Colour map file, "<<filename<<", does not exist"<<endl;
            exit (EXIT_FAILURE);
        }
    }
    void SurfaceContainer::changeColourTable(int index)
    {
        cout<<"change colour table"<<endl;
     //   if (texNames.size()>0)
      //  {
              if (index >0)
            {
              surf_form->setColourTableNoSc(v_ctables.at(index-1));
              vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
               for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
               {
               //    memcpy(v_surf_ctables[*i_ind].sc_lut , v_ctables[index-1].sc_lut, sizeof(float)*6);

                   memcpy(v_surf_ctables[*i_ind].r_lut , v_ctables[index-1].r_lut, sizeof(float)*6);
                  memcpy(v_surf_ctables[*i_ind].g_lut , v_ctables[index-1].g_lut, sizeof(float)*6);
                  memcpy(v_surf_ctables[*i_ind].b_lut , v_ctables[index-1].b_lut, sizeof(float)*6);
                  memcpy(v_surf_ctables[*i_ind].a_lut , v_ctables[index-1].a_lut, sizeof(float)*6);

             //     memcpy(v_surf_ctables[surf_form->getCurrentSurfaceIndex()].r_lut , v_ctables[index-1].r_lut, sizeof(float)*6);
              //   memcpy(v_surf_ctables[surf_form->getCurrentSurfaceIndex()].g_lut , v_ctables[index-1].g_lut, sizeof(float)*6);
               //  memcpy(v_surf_ctables[surf_form->getCurrentSurfaceIndex()].b_lut , v_ctables[index-1].b_lut, sizeof(float)*6);
                // memcpy(v_surf_ctables[surf_form->getCurrentSurfaceIndex()].a_lut , v_ctables[index-1].a_lut, sizeof(float)*6);
            }
           }
            v_cmap_index.at(surf_form->getCurrentSurfaceIndex())=index;
            emit sig_updateGL();
        //}
    }
    void SurfaceContainer::updateColourTable( )
    {
        //cout<<"update colourtable nosc"<<endl;

        if (surf_form->getCurrentSurfaceIndex() != -1)
        {
            vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
             for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
             {
              // v_surf_ctables[surf_form->getCurrentSurfaceIndex()]=surf_form->getCurrentColourTable();
                //  v_cmap_index.at(surf_form->getCurrentSurfaceIndex())=0;
                 v_surf_ctables[*i_ind]=surf_form->getCurrentColourTable();
                          v_cmap_index.at(*i_ind)=0;
                }
          surf_form->setColourTableIndex(0);

        emit sig_updateGL();
    }

    }

    void SurfaceContainer::updateColourTableSc( )
    {
        //cout<<"update colourtable sc"<<endl;
        if (surf_form->getCurrentSurfaceIndex() != -1)
        {
            vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
             for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
             {
              v_surf_ctables[*i_ind]=surf_form->getCurrentColourTable();
         }
         }
        emit sig_updateGL();
    }

        void SurfaceContainer::updateColourTable( const int & index )
        {//this update colour table now updates the GLSL colour table not the form
            cout<<"update coour table "<<index<<endl;
            if (vbos_vertices.size()>0)
            {
            //    vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
             //   for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
              //  {

                glUseProgram(glsl_programs.at(surf_glsl_programs.at(index)));
                int loc_test = glGetUniformLocation(glsl_programs.at(surf_glsl_programs.at(index)),"r_lut");

                cout<<"sc "<<v_surf_ctables.at(index).sc_lut[0]<<" "<<v_surf_ctables.at(index).sc_lut[1]<<" "<<v_surf_ctables.at(index).sc_lut[2]<<" "<<v_surf_ctables.at(index).sc_lut[3]<<" "<<endl;
//        /cout<<"temp1 "<<index<<" "<<v_surf_ctables.size()<<endl;
               // v_ctables.at(*i_ind)=surf_form->getCurrentColourTable();
                glUniform4fv(loc_r_lut,1,v_surf_ctables.at(index).r_lut);
                glUniform4fv(loc_g_lut,1,v_surf_ctables.at(index).g_lut);
                glUniform4fv(loc_b_lut,1,v_surf_ctables.at(index).b_lut);
                glUniform4fv(loc_a_lut,1,v_surf_ctables.at(index).a_lut);
                glUniform4fv(loc_sc_lut,1,v_surf_ctables.at(index).sc_lut);
//cout<<"gluniform "<<endl;
                glUniform2fv(loc_r_lut_last,1,v_surf_ctables.at(index).r_lut + 4);
                glUniform2fv(loc_g_lut_last,1,v_surf_ctables.at(index).g_lut + 4);
                glUniform2fv(loc_b_lut_last,1,v_surf_ctables.at(index).b_lut + 4);
                glUniform2fv(loc_a_lut_last,1,v_surf_ctables.at(index).a_lut + 4);
                glUniform2fv(loc_sc_lut_last,1,v_surf_ctables.at(index).sc_lut + 4);

                glUniform4fv(loc_low_clamp,1,v_surf_ctables.at(index).low_clamp);
                /*
                glUniform4fv(loc_r_lut,1,v_ctables.at(*i_ind).r_lut);
                glUniform4fv(loc_g_lut,1,v_ctables.at(*i_ind).g_lut);
                glUniform4fv(loc_b_lut,1,v_ctables.at(*i_ind).b_lut);
                glUniform4fv(loc_a_lut,1,v_ctables.at(*i_ind).a_lut);
                glUniform4fv(loc_sc_lut,1,v_ctables.at(*i_ind).sc_lut);

                glUniform2fv(loc_r_lut_last,1,v_ctables.at(*i_ind).r_lut + 4);
                glUniform2fv(loc_g_lut_last,1,v_ctables.at(*i_ind).g_lut + 4);
                glUniform2fv(loc_b_lut_last,1,v_ctables.at(*i_ind).b_lut + 4);
                glUniform2fv(loc_a_lut_last,1,v_ctables.at(*i_ind).a_lut + 4);
                glUniform2fv(loc_sc_lut_last,1,v_ctables.at(*i_ind).sc_lut + 4);

                glUniform4fv(loc_low_clamp,1,v_ctables.at(*i_ind).low_clamp);
*/
//   }

                if (c_bar != NULL)
                {
                 //   glUseProgram(1);
                    //            glUniform4fv(cbar_loc_r_lut,1,v_ctables.at(surf_form->getCurrentSurfaceIndex()).r_lut);
                    //    glUniform4fv(cbar_loc_g_lut,1,v_ctables.at(surf_form->getCurrentSurfaceIndex()).g_lut);
                    //  glUniform4fv(cbar_loc_b_lut,1,v_ctables.at(surf_form->getCurrentSurfaceIndex()).b_lut);
                    //  glUniform4fv(cbar_loc_sc_lut,1,v_ctables.at(surf_form->getCurrentSurfaceIndex()).sc_lut);


                    //c_bar->setColourTable(0,v_ctables.at(surf_form->getCurrentSurfaceIndex()));
                   // c_bar->generateColourBar(0,v_ctables.at(surf_form->getCurrentSurfaceIndex()));
                }
               // emit sig_updateGL();
            }
           cout<<"done update colour table"<<endl;
        }

    void SurfaceContainer::updateColourBar()
    {
        //cout<<"update cbar"<<endl;
        if (c_bar != NULL)
        {
            ////cout<<"get visibility "<<endl;
            c_bar_onoff = surf_form->getColourBarVisibility();
                        ////cout<<"get visibility "<<c_bar_onoff<<endl;

            float3 origin;
            float width,length;
            surf_form->getColourBarGeometry(origin,width,length);
            string title = surf_form->getColourBarTitle();

            c_bar->setGeometry(origin,length,width);
            c_bar->setTitle(title);
            c_bar->generateColourBar(0,v_surf_ctables.at(surf_form->getCurrentSurfaceIndex()));
        }
        //cout<<"done update cbar"<<endl;

        emit sig_updateGL();

    }
    void SurfaceContainer::updatePolygonModeBack( int gl_enum )
    {
        ////cout<<"update polygon mode back in "<<gl_enum<<endl;

        switch (gl_enum)
        {
        case 0 :
            gl_enum = GL_POINT;
            break;
        case 1:
            //cout<<"set 1"<<endl;

            gl_enum = GL_LINE;
            break;
        case 2:
            gl_enum = GL_FILL;
            break;
        }
        //cout<<"update polygon mode back "<<gl_enum<<endl;
        vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
        for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
        {
        fill_type_b[*i_ind] = gl_enum;
    }
        emit sig_updateGL();

    }
    void SurfaceContainer::updatePolygonModeFront( int gl_enum )
    {
        //cout<<"update polygon mode front"<<endl;
        switch (gl_enum)
        {
        case 0 :
            gl_enum = GL_POINT;
            break;
        case 1:
            //cout<<"set 1"<<endl;
            gl_enum = GL_LINE;
            break;
        case 2:
            gl_enum = GL_FILL;
            break;
        }
        vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
        for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
        {
        fill_type_f[*i_ind] = gl_enum;
    }
        emit sig_updateGL();

    }
    void SurfaceContainer::updatePolygonModeBackGlyph( int gl_enum )
    {
        //cout<<"update polygon mode glyph back "<<endl;
        switch (gl_enum)
        {
        case 0 :
            gl_enum = GL_POINT;
            break;
        case 1:
            gl_enum = GL_LINE;
            break;
        case 2:
            gl_enum = GL_FILL;
            break;
        }
        vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
        for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
        {
        fill_type_b_glyph[*i_ind] = gl_enum;
    }
        emit sig_updateGL();

    }
    void SurfaceContainer::updatePolygonModeFrontGlyph( int gl_enum )
    {
        //cout<<"update polygon mode glyph front"<<endl;
        switch (gl_enum)
        {
        case 0 :
            gl_enum = GL_POINT;
            break;
        case 1:
            gl_enum = GL_LINE;
            break;
        case 2:
            gl_enum = GL_FILL;
            break;
        }
        vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
        for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
        {
        fill_type_f_glyph[*i_ind] = gl_enum;
    }
        emit sig_updateGL();

    }
    void SurfaceContainer::setColourTableUniformLocations(const GLint & loc_r_lut_in,const GLint & loc_g_lut_in,const GLint & loc_b_lut_in, const GLint & loc_a_lut_in,const GLint & loc_sc_lut_in,const GLint & loc_r_lut_last_in,const GLint & loc_g_lut_last_in,const GLint & loc_b_lut_last_in,const GLint & loc_a_lut_last_in, const GLint & loc_sc_lut_last_in, const GLint & loc_low_clamp_in)
    {
        loc_r_lut = loc_r_lut_in;
        loc_g_lut = loc_g_lut_in;
        loc_b_lut = loc_b_lut_in;
        loc_a_lut = loc_a_lut_in;
        loc_sc_lut = loc_sc_lut_in;

                loc_r_lut_last = loc_r_lut_last_in;
        loc_g_lut_last = loc_g_lut_last_in;
        loc_b_lut_last = loc_b_lut_last_in;
        loc_a_lut_last = loc_a_lut_last_in;
        loc_sc_lut_last = loc_sc_lut_last_in;
        loc_low_clamp = loc_low_clamp_in ;

    }
  /*      void SurfaceContainer::setColourBarUniformLocations(const GLint & loc_r_lut_in,const GLint & loc_g_lut_in,const GLint & loc_b_lut_in, const GLint & loc_sc_lut_in)
    {
        cbar_loc_r_lut = loc_r_lut_in;
        cbar_loc_g_lut = loc_g_lut_in;
        cbar_loc_b_lut = loc_b_lut_in;
        cbar_loc_sc_lut = loc_sc_lut_in;
    }

*/

    void SurfaceContainer::changeShaderProgram( bool use_scalars )
    {
         if (vbos_vertices.size()>0)
        {
             //cout<<"get curerent index << "<<surf_form->getCurrentSurfaceIndex()<<endl;
             vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
             for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
             {
             surf_glsl_programs.at(*i_ind)=use_scalars;
         }
             emit sig_updateGL();
    }
    }

    void SurfaceContainer::appendSurfaceData( const std::string & filename )
    {
        //cout<<"append surfdata "<<index<<" "<<endl;//vbos_vertices[index][0]<<" "<<vbos_vertices[index][1]<<endl;
        unsigned int index = surf_form->getCurrentSurfaceIndex();
        unsigned int Nverts = v_surfaces[index]->getNumberOfVertices();
        unsigned int Nfaces = v_surfaces[index]->getNumberOfFaces();


        unsigned int Nscalars = v_surfaces[index]->getNumberOfScalarData();

        read_surface<float,unsigned int>( *v_surfaces[index],filename);


        if ( (Nverts != v_surfaces[index]->getNumberOfVertices()) || (Nfaces != v_surfaces[index]->getNumberOfFaces()) )
        {
            //cout<<"calc number of normals "<<endl;
            v_surfaces[index]->calculateNormals(true,false);
        }

        //cout<<"buffer number of normals "<<endl;
        if ( (Nverts != v_surfaces[index]->getNumberOfVertices()) && (Nverts==0))
            glBufferData_Vertices( *v_surfaces[index],vbos_vertices[index][0]);
        else
            glBufferSubData_Vertices(*v_surfaces[index],vbos_vertices[index][0]);

        if ( (Nfaces != v_surfaces[index]->getNumberOfFaces()) && (Nverts==0))
            glBufferData_Faces(*v_surfaces[index],vbos_vertices[index][1]);
        else
            glBufferSubData_Faces(*v_surfaces[index],vbos_vertices[index][1]);


        surf_form->addItemToAppendedList(filename);
        surface_appended_names[index].push_back(filename);
        emit sig_updateGL();

    }
    void SurfaceContainer::addSurface( fslsurface_name::fslSurface<float,unsigned int> * surf, GLuint*  vbos, const string & surf_name)
    {
        cout<<"add surgface2 "<<endl;
        //  gifti_read_image("aghahgs",1);
        cout<<"add surgface2 "<<surf_name<<endl;

          surface_filenames.push_back(surf_name);
          frontFace.push_back(0);
          fill_type_b.push_back(GL_FILL);
          fill_type_f.push_back(GL_FILL);
          fill_type_b_glyph.push_back(GL_FILL);
          fill_type_f_glyph.push_back(GL_FILL);
          v_glyph_exists.push_back(0);
          cout<<"add surgface2.1 "<<surf_name<<endl;

          v_glyph_props.push_back(surf_form->getCurrentGlyph());
          cout<<"add surgface2.2 "<<surf_name<<endl;

          surf_glsl_programs.push_back(0);


            showGlyphs.push_back(0);
            cullFace.push_back(0);

         v_surfaces.push_back(surf);
    //surf->printScalars(0);
          surf->calculateNormals();
  cout<<"done surface normals"<<endl;
          glBufferData_Vertices(*surf, vbos[0]);
          glBufferData_Faces(*surf, vbos[1]);
  cout<<"done buffering data"<<endl;
      //  N_verts.push_back(surf->getNumberOfVertices() );


           vbos_vertices.push_back(vbos);


           float mag=1/sqrt(3);
           material surf_m = material(0.2,0.2,0.2,1.0, \
                                      mag,mag,mag,1.0, \
                                      0.35,0.35,0.35,1.0, \
                                      60.0, 1.0 );
          colour_table ctable;

           if ( surf->getNumberOfDataTableEntries() <= 5 )
           {
               map<int,float4> dataTable = surf->getDataTable();
               unsigned int Nentries = surf->getNumberOfDataTableEntries();


               float4 r(0,0,0,0),g(0,0,0,0),b(0,0,0,0),a(1,1,1,1),sc(0,1,2,3);
               float2 r5(0,0);
               float2 g5(0,0);
               float2 b5(0,0);
               float2 a5(1.0,1.0);
               float2 sc5(4.0,5.0);
               int count=0;
               for ( map<int,float4>::iterator i = dataTable.begin(); i != dataTable.end(); ++i,++count)
               {
                   //cout<<"entry "<<endl;
                   //cout<<i->first<<" "<<i->second.x<<" "<<i->second.y<<" "<<i->second.z<<" "<<i->second.w<<endl;
                   switch (count)
                   {
                   case 0:
                       sc.x = i->first;
                       r.x = i->second.x;
                       g.x = i->second.y;
                       b.x = i->second.z;
                       a.x = i->second.w;
                       break;
                   case 1:
                       sc.y = i->first;
                       r.y = i->second.x;
                       g.y = i->second.y;
                       b.y = i->second.z;
                       a.y = i->second.w;
                       break;
                   case 2:
                       sc.z = i->first;
                        r.z = i->second.x;
                        g.z = i->second.y;
                        b.z = i->second.z;
                        a.z = i->second.w;
                        break;
                   case 3:
                        sc.w = i->first;
                       r.w = i->second.x;
                       g.w = i->second.y;
                       b.w = i->second.z;
                       a.w = i->second.w;
                       break;
                   case 4:
                       sc5.x = i->first;
                       r5.x = i->second.x;
                       g5.x = i->second.y;
                       b5.x = i->second.z;
                       a5.x = i->second.w;
                       break;
                   }
                   //cout<<"change shader "<<endl;
  //                 changeShaderProgram(1);
                   surf_glsl_programs.back()=1;
                   //cout<<"set scalar2"<<endl;

                   surf_form->setUseScalarMap(1);
                  //cout<<"set scalar"<<endl;
               }


               ctable = colour_table( r, r5, g, g5, b, b5, a, a5, sc, sc5, 0,0,0,0);

            }else{

           ctable = colour_table( 1.0, 1.0  , 0.0  , 0.0, 0.0, 0.0, \
                                               0.0, 1.0  , 1.0  , 1.0, 0.0, 0.0, \
                                               0.0, 0.0  , 0.0  , 1.0, 1.0, 0.0, \
                                               0.0, 1.0/4, 2.0/4, 0.75, 1.0, 0.0, \
                                               1.0,1.0,1.0,1.0,1.0,1.0,\
                                               1.0,0.0,0.0,0.0);
           }
                   //cout<<"data buffered 2"<<endl;

           v_materials.push_back(surf_m);
           //cout<<"puch_back ctable "<<endl;
           v_surf_ctables.push_back(ctable);
                   //cout<<"data buffered 4"<<endl;
           v_cmap_index.push_back(1);

           surf_form->setSurfaceMaterial(surf_m);
           surf_form->setColourTable(ctable);
                   //cout<<"data buffered 5"<<endl;

           surf_form->setMaterialIndex(0);
           surf_material_index.push_back(0);
                  //         //cout<<"data buffered 6 "<<filename<<endl;

           surf_form->addItemToList( surf_name);
           vector<string> append_names(1,"embedded_data");
           surface_appended_names.push_back(append_names);//append_names);

                   //cout<<"data buffered 3"<<endl;
           vec3<float> cog=surf->getCOG();
          v_cog.push_back(float3(cog.x,cog.y,cog.z));
          //cout<<"cog "<<surf->getCOG().x<<" "<<surf->getCOG().y<<" "<<surf->getCOG().z<<endl;

  /*
          count=0;
          for (vector< vector<unsigned int> >::iterator v_i = polys.begin() ; v_i != polys.end();v_i++)
              for (vector<unsigned int>::iterator v_j = v_i->begin();v_j!=v_i->end();v_j++,count++)
              {
              indices[count]=*v_j;
          }


          calculateNormals(vertices, indices,verts.size()/3, index_count );
  */

  cout<<"done add surface "<<endl;

    }

    void SurfaceContainer::addSurface( const string & filename , GLuint*  vbos)//fslvtkio::fslvtkIO * surface, GLuint*  vbos)
    {
      //  gifti_read_image("aghahgs",1);
      //  surface_filenames.push_back(filename);

        fslSurface<float,unsigned int> *surf = new fslSurface<float,unsigned int>();
        read_surface<float,unsigned int>(*surf,filename);

        addSurface(surf,vbos,filename);

    }

        unsigned int SurfaceContainer::getNumberOfSurfaces()
        {
            return v_surfaces.size();
        }

    void SurfaceContainer::addGlyphs( const unsigned int & surf_index ,  GLuint*  vbos)
    {
     //   cout<<"regen2 glyphs "<<v_glyph_props.size()<<" "<<v_vertices.size()<<" "<<v_vectors.size()<<" "<<N_verts.size()<<endl;

        cout<<"add glyphs2 "<<vbos_vertices.size()<<" "<<surf_index<<endl;
    //    glBindBuffer(GL_ARRAY_BUFFER, vbos_vertices[surf_index][0]);
      //  vector< vertexFloat > surf_verts( N_verts[surf_index]);
      //  glGetBufferSubData(GL_ARRAY_BUFFER,0,N_verts.at(surf_index)*sizeof(fslsurface_name::vertexFloat),&surf_verts[0]);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);


        //cout<<"add glyphs "<<v_glyph_exists.size()<<" "<<surf_index<<" "<<v_surfaces[surf_index]->getNumberOfVectorData()<<endl;
         //      float height = 100;
       vector< fslsurface_name::vertex<float> >* vertices = new vector<fslsurface_name::vertex<float> >();
        vector<GLuint>* indices = new vector<GLuint>();
        v_glyph_exists.at(surf_index)=1;
   unsigned int vec_index=0;
        generateArrowGlyphs( *vertices, *indices, v_surfaces[surf_index], vec_index, \
                             v_glyph_props[surf_index].scale_factor * v_glyph_props[surf_index].tip[0], v_glyph_props[surf_index].scale_factor * v_glyph_props[surf_index].base_radius,  v_glyph_props[surf_index].scale_factor, \
                             v_glyph_props[surf_index].tip[1], v_glyph_props[surf_index].N_faces );
       // generateArrowGlyphs( v_surfaces[surf_index]. , *indices, v_vertices.at(surf_index), v_vectors.at(surf_index),N_verts.at(surf_index)/3, \
         //                       v_glyph_props[surf_index].scale_factor * v_glyph_props[surf_index].tip[0], v_glyph_props[surf_index].scale_factor * v_glyph_props[surf_index].base_radius,  v_glyph_props[surf_index].scale_factor, \
           //                      v_glyph_props[surf_index].tip[1], v_glyph_props[surf_index].N_faces );
       // generateArrowGlyphs( *vertices, *indices, v_vertices.at(surf_index), v_vectors.at(surf_index),N_verts.at(surf_index)/3, \
         //                    v_glyph_props[surf_index].scale_factor * v_glyph_props[surf_index].tip[0], v_glyph_props[surf_index].scale_factor * v_glyph_props[surf_index].base_radius,  v_glyph_props[surf_index].scale_factor, \
           //                  v_glyph_props[surf_index].tip[1], v_glyph_props[surf_index].N_faces );
        vbos_glyphs.push_back(vbos);
        cout<<"done gen arrows"<<endl;
//for (int i=0; i< vertices.size();i++)
//    //cout<<vertices.at(i).x<<" "<<vertices.at(i).y<<" "<<vertices.at(i).z<<" "<<endl;

////cout<<"indices "<<indices.size()<<endl;
//for (int i=0; i< indices.size();i+=3)
  //  //cout<<indices.at(i)<<" "<<indices.at(i+1)<<" "<<indices.at(i+2)<<endl;



//       float head_radius = 10;
 //       float shaft_radius = 4;
 //       float head_height = 0.4; //a percentage
 //       unsigned int N_faces = 10;


//--------end create shaft-----------------//
       // unsigned int N_verts = vertices.size();
if (! indices->empty())
        {
      unsigned int N_triangles = indices->size() / 3 ;
//unsigned int N_triangles = 10;
      // unsigned int index_count = N_verts_head *3 ;


////cout<<"calculate normals "<<N_verts<<" "<<N_triangles<<endl;
       // calculateNormals(vertices, indices,N_verts_head+1, index_count );

      //   ulateNormals(&vertices.at(0),&indices.at(0) ,3*N_verts+2, indices.size()  );
     //  calculateNormals(&vertices.at(0),&indices.at(0) ,3*N_verts+2, N_triangles*3 );        //cout<<"calculate normals2"<<endl;
        N_triangles_Glyphs.push_back( N_triangles );
        /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
      //  glBufferData(GL_ARRAY_BUFFER, ( 3*N_verts + 2) * sizeof(vertex), &(vertices.at(0)), GL_DYNAMIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(fslsurface_name::vertex<float>), &(vertices->at(0)), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, N_triangles*3 * sizeof(unsigned int), &(indices->at(0)), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


    }else{
        cerr<<"Alert: Tried to render glpyhs but there were no points"<<endl;

    }

    delete vertices;
    delete indices;
//cout<<"done glyphs"<<endl;
    }

            void SurfaceContainer::addColourBar(GLuint*  vbos)
            {
               // cout<<"generate Colour bar "<<surf_form->getCurrentSurfaceIndex()<<" "<<endl;
                if (c_bar==NULL)
                {
                    c_bar = new ColourBar();
                }
                c_bar->setVertexAttribLocs(vertexLoc, normalLoc, scalarLoc);
                c_bar->setMaterial(v_materials.at(surf_form->getCurrentSurfaceIndex()));
                c_bar->addColourBar(vbos, v_surf_ctables.at(surf_form->getCurrentSurfaceIndex()));
                //cout<<"done generate Colour bar "<<surf_form->getCurrentSurfaceIndex()<<" "<<endl;

                //c_bar->setGLSLPrograms(c_bar_progs);
            }




     void SurfaceContainer::regenerateGlyphs( const unsigned int & surf_index)
    {
         //cout<<"regen glyphs "<<v_glyph_props.size()<<" "<<endl;//v_vertices.size()<<" "<<v_vectors.size()<<" "<<N_verts.size()<<endl;
         //      float height = 100;
        vector< vertex<float> >* vertices = new vector< vertex<float> >();
        vector<GLuint>* indices = new vector<GLuint>();
        v_glyph_exists.at(surf_index)=1;
//cout<<"gen arrow"<<endl;


//A WAY TO COPY VERTICES FROM GRAPHICS BUFFER
//glBindBuffer(GL_ARRAY_BUFFER, vbos_vertices[surf_index][0]);
//vector< vertexFloat > surf_verts( N_verts[surf_index]);
//glGetBufferSubData(GL_ARRAY_BUFFER,0,N_verts.at(surf_index)*sizeof(fslsurface_name::vertexFloat),&surf_verts[0]);
//glBindBuffer(GL_ARRAY_BUFFER, 0);
unsigned int vec_index=0;
     generateArrowGlyphs( *vertices, *indices,v_surfaces[surf_index], vec_index, \
                            v_glyph_props[surf_index].scale_factor * v_glyph_props[surf_index].tip[0], v_glyph_props[surf_index].scale_factor * v_glyph_props[surf_index].base_radius,  v_glyph_props[surf_index].scale_factor, \
                            v_glyph_props[surf_index].tip[1], v_glyph_props[surf_index].N_faces );

//cout<<"done generating arrows"<<endl;
if (! indices->empty())
        {
      unsigned int N_triangles = indices->size() / 3 ;
      N_triangles_Glyphs.at(surf_index)= N_triangles ;
        /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
        glBindBuffer(GL_ARRAY_BUFFER, vbos_glyphs[surf_index][0]);
      //  glBufferData(GL_ARRAY_BUFFER, ( 3*N_verts + 2) * sizeof(vertex), &(vertices.at(0)), GL_DYNAMIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(vertex<float>), &(vertices->at(0)), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbos_glyphs[surf_index][1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, N_triangles*3 * sizeof(unsigned int), &(indices->at(0)), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    }
        delete vertices;
        delete indices;
//cout<<"done glyphs"<<endl;
    }



    void SurfaceContainer::setGLSLPrograms(const std::vector<GLuint> & progs)
    {
        glsl_programs=progs;
    }

    void SurfaceContainer::setGLSLProgramsColourBar(const std::vector<GLuint> & progs)
    {
       // if (c_bar != NULL)
          //  c_bar->setGLSLPrograms(c_bar_progs);
            c_bar_progs=progs;
    }

    float3 SurfaceContainer::getSurfaceCOG(const int & index)
    {
        float3 cog;
        if (v_cog.size()>0)
            cog=v_cog.at(index);
        else
            cog=float3(0,0,0);
        return cog;

    }



    void SurfaceContainer::setAmbient(const unsigned int & surf_ind, const float4 & rgba )
    {
        v_materials.at(surf_ind).ambient[0]=rgba.x;
        v_materials.at(surf_ind).ambient[1]=rgba.y;
        v_materials.at(surf_ind).ambient[2]=rgba.z;
        v_materials.at(surf_ind).ambient[3]=rgba.w;
    }

    void SurfaceContainer::setDiffuse(const unsigned int & surf_ind, const float4 & rgba )
    {
        v_materials.at(surf_ind).diffuse[0]=rgba.x;
        v_materials.at(surf_ind).diffuse[1]=rgba.y;
        v_materials.at(surf_ind).diffuse[2]=rgba.z;
        v_materials.at(surf_ind).diffuse[3]=rgba.w;
    }

    void SurfaceContainer::setSpecular(const unsigned int & surf_ind, const float4 & rgba )
    {
        v_materials.at(surf_ind).specular[0]=rgba.x;
        v_materials.at(surf_ind).specular[1]=rgba.y;
        v_materials.at(surf_ind).specular[2]=rgba.z;
        v_materials.at(surf_ind).specular[3]=rgba.w;
    }

    void SurfaceContainer::setShininess(const unsigned int & surf_ind, const float & sh )
    {
        v_materials.at(surf_ind).shininess=sh;
    }
    void SurfaceContainer::setSurfaceProperties(const unsigned int & surf_ind, const material & mat_in )
    {
        v_materials.at(surf_ind) = mat_in;
    }

    material SurfaceContainer::getSurfaceProperties(const unsigned int & surf_ind )
    {
        return v_materials.at(surf_ind);
    }


    void SurfaceContainer::renderOpaqueSurfaces()
    {
    //cout<<"render opaque"<<endl;
    double* m = new double[16];

    glGetDoublev(GL_MODELVIEW_MATRIX, m);
    //cout<<"m222 "<<m[0]<<" "<<m[1]<<" "<<m[2]<<" "<<m[3]<<endl;
    //cout<<"m22 "<<m[4]<<" "<<m[5]<<" "<<m[6]<<" "<<m[7]<<endl;
    //cout<<"m22 "<<m[8]<<" "<<m[9]<<" "<<m[10]<<" "<<m[11]<<endl;
    //cout<<"m22 "<<m[12]<<" "<<m[13]<<" "<<m[14]<<" "<<m[15]<<endl;
delete[] m;


//        glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    cout<<"surface-blendfunc "<<blendFunc<<endl;
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
        for (unsigned int surf_ind = 0;surf_ind<vbos_vertices.size();surf_ind++)
        {
            if (cullFace[surf_ind])
            {
                //cout<<"enable cull face"<<endl;
                glEnable(GL_CULL_FACE);
            }

            if (frontFace[surf_ind])
               glFrontFace(GL_CCW);
            else
               glFrontFace(GL_CW);


            if ((v_materials.at(surf_ind).ambient[3]>=1.0) && (v_materials.at(surf_ind).diffuse[3]>=1.0))
            {
                 updateColourTable(surf_ind);
                if (fill_type_f[surf_ind] ==fill_type_b[surf_ind] )
                {
                    //cout<<"fill1 type "<< fill_type_f[surf_ind] <<" "<< fill_type_b[surf_ind]<<endl;

                    glPolygonMode(GL_FRONT_AND_BACK, fill_type_f[surf_ind] );
                }else
                {
                    //cout<<"fille type "<< fill_type_f[surf_ind] <<" "<< fill_type_b[surf_ind]<<endl;
                    glPolygonMode(GL_FRONT, fill_type_f[surf_ind] );
                    glPolygonMode(GL_BACK , fill_type_b[surf_ind] );
                }

                glUseProgram(glsl_programs.at(surf_glsl_programs.at(surf_ind)));

                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, v_materials.at(surf_ind).ambient );
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  v_materials.at(surf_ind).diffuse );
                glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,v_materials.at(surf_ind).shininess);
                glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, v_materials.at(surf_ind).specular);
                ////cout<<"render "<<v_surfaces.size()<<" "<<surf_ind<<" "<<vertexLoc<<" "<<normalLoc<<" "<<scalarLoc<<endl;
                render<float,unsigned int>(*v_surfaces[surf_ind], vertexLoc,normalLoc,scalarLoc,vbos_vertices.at(surf_ind)[0] ,vbos_vertices.at(surf_ind)[1] );

            }



            glDisable(GL_CULL_FACE);
        }


     //   //cout<<"render glyophs2"<<endl;

////cout<<"done render opaque"<<endl;

    }
    void SurfaceContainer::renderTranslucentSurfaces()
    {
       // cout<<"render translucent "<<endl;
        glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glBlendFunc (GL_SRC_ALPHA,GL_ONE);
        glBlendFunc (GL_ONE_MINUS_DST_COLOR,GL_DST_COLOR);

glBlendEquation(GL_FUNC_ADD);
        for (unsigned int surf_ind = 0;surf_ind<vbos_vertices.size();surf_ind++)
        {

            if (cullFace[surf_ind])
            {
                //cout<<"enable cull face"<<endl;
                glEnable(GL_CULL_FACE);
            }

            if (frontFace[surf_ind])
               glFrontFace(GL_CCW);
            else
               glFrontFace(GL_CW);


            if ((v_materials.at(surf_ind).ambient[3]<1.0) || (v_materials.at(surf_ind).diffuse[3]<1.0))
            {
              //  depthSortTriangles(v_surfaces[surf_ind],vbos_vertices.at(surf_ind)[0],vbos_vertices.at(surf_ind)[1]);
                updateColourTable(surf_ind);


                if (fill_type_f[surf_ind] ==fill_type_b[surf_ind] )
                    glPolygonMode(GL_FRONT_AND_BACK, fill_type_f[surf_ind] );
                else
                {
                    glPolygonMode(GL_FRONT, fill_type_f[surf_ind] );
                    glPolygonMode(GL_BACK , fill_type_b[surf_ind] );
                }


                glUseProgram(glsl_programs.at(surf_glsl_programs.at(surf_ind)));

                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, v_materials.at(surf_ind).ambient );
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  v_materials.at(surf_ind).diffuse );
                glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,v_materials.at(surf_ind).shininess);
                glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, v_materials.at(surf_ind).specular);

               render<float,unsigned int>( *v_surfaces[surf_ind], vertexLoc,normalLoc,scalarLoc,vbos_vertices.at(surf_ind)[0] ,vbos_vertices.at(surf_ind)[1] );

            }




            glDisable(GL_CULL_FACE);
        }
      //  cout<<"done render translucent "<<endl;

    }

    void SurfaceContainer::renderOpaqueGlyphs()
    {
        //glPolygonMode(, GL_LINE);

        // glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//cout<<"render opaque glyphs "<<endl;

        for (unsigned int surf_ind = 0;surf_ind<vbos_vertices.size();surf_ind++)
        {
            if (cullFace[surf_ind])
                glEnable(GL_CULL_FACE);

            if (frontFace[surf_ind])
               glFrontFace(GL_CCW);
            else
               glFrontFace(GL_CW);


            if (showGlyphs[surf_ind])
            {
                //cout<<"show opaque glyphs "<<endl;

                if  ((v_glyph_props.at(surf_ind).opacity>=1.0) && (v_glyph_exists.at(surf_ind)))
                {
                    if (fill_type_f_glyph[surf_ind] ==fill_type_b_glyph[surf_ind] )
                    {
                        // //cout<<"fill1 type "<< fill_type_f[surf_ind] <<" "<< fill_type_b[surf_ind]<<endl;

                        glPolygonMode(GL_FRONT_AND_BACK, fill_type_f_glyph[surf_ind] );
                    }else
                    {
                        // //cout<<"fille type "<< fill_type_f[surf_ind] <<" "<< fill_type_b[surf_ind]<<endl;
                        glPolygonMode(GL_FRONT, fill_type_f_glyph[surf_ind] );
                        glPolygonMode(GL_BACK , fill_type_b_glyph[surf_ind] );
                    }
//                    /cout<<"render glyphs2"<<endl;
                    glUseProgram(glsl_programs.at(surf_glsl_programs.at(surf_ind)));

                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, v_materials.at(surf_ind).ambient );
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  v_materials.at(surf_ind).diffuse );
                    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,v_materials.at(surf_ind).shininess);
                    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, v_materials.at(surf_ind).specular);

                    glBindBuffer(GL_ARRAY_BUFFER, vbos_glyphs.at(surf_ind)[0] );         // for vertex coordinates
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos_glyphs.at(surf_ind)[1] );

                    glEnableVertexAttribArray(vertexLoc);
                    glEnableVertexAttribArray(normalLoc);
                    glEnableVertexAttribArray(scalarLoc);

                    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(vertexFloat), 0);
                    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(vertexFloat), BUFFER_OFFSET(3*sizeof(float)));
                    glVertexAttribPointer(scalarLoc, 1, GL_FLOAT, GL_FALSE, sizeof(vertexFloat), BUFFER_OFFSET(6*sizeof(float)));
                    //cout<<"draw "<<N_triangles_Glyphs.at(surf_ind)<<endl;
                    glDrawElements(GL_TRIANGLES,N_triangles_Glyphs.at(surf_ind)*3,GL_UNSIGNED_INT,0);

                    glDisableVertexAttribArray(vertexLoc);
                    glDisableVertexAttribArray(normalLoc);
                    glDisableVertexAttribArray(scalarLoc);

                    glBindBuffer(GL_ARRAY_BUFFER, 0 );         // for vertex coordinates
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );
                }

            }
            glDisable(GL_CULL_FACE);
        }
    }
    void SurfaceContainer::renderTranslucentGlyphs()
    {
        glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

        for (unsigned int surf_ind = 0;surf_ind<vbos_vertices.size();surf_ind++)
        {
            if (cullFace[surf_ind])
                glEnable(GL_CULL_FACE);

            if (frontFace[surf_ind])
               glFrontFace(GL_CCW);
            else
               glFrontFace(GL_CW);


            if (showGlyphs[surf_ind])
            {
                if  ((v_glyph_props.at(surf_ind).opacity<1.0) && (v_glyph_exists.at(surf_ind)))
                {
                    //cout<<"render blended glyphs "<<endl;
                    if (fill_type_f_glyph[surf_ind] ==fill_type_b_glyph[surf_ind] )
                    {
                        // //cout<<"fill1 type "<< fill_type_f[surf_ind] <<" "<< fill_type_b[surf_ind]<<endl;

                        glPolygonMode(GL_FRONT_AND_BACK, fill_type_f_glyph[surf_ind] );
                    }else
                    {
                        // //cout<<"fille type "<< fill_type_f[surf_ind] <<" "<< fill_type_b[surf_ind]<<endl;
                        glPolygonMode(GL_FRONT, fill_type_f_glyph[surf_ind] );
                        glPolygonMode(GL_BACK , fill_type_b_glyph[surf_ind] );
                    }

                    glUseProgram(glsl_programs.at(surf_glsl_programs.at(surf_ind)));

                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, v_materials.at(surf_ind).ambient );
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  v_materials.at(surf_ind).diffuse );
                    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,v_materials.at(surf_ind).shininess);
                    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, v_materials.at(surf_ind).specular);


                    glBindBuffer(GL_ARRAY_BUFFER, vbos_glyphs.at(surf_ind)[0] );         // for vertex coordinates
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos_glyphs.at(surf_ind)[1] );

                    glEnableVertexAttribArray(vertexLoc);
                    glEnableVertexAttribArray(normalLoc);
                    glEnableVertexAttribArray(scalarLoc);

                    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(vertexFloat), 0);
                    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(vertexFloat), BUFFER_OFFSET(3*sizeof(float)));
                    glVertexAttribPointer(scalarLoc, 1, GL_FLOAT, GL_FALSE, sizeof(vertexFloat), BUFFER_OFFSET(6*sizeof(float)));
                    //cout<<"draw "<<N_triangles_Glyphs.at(surf_ind)<<endl;
                    glDrawElements(GL_TRIANGLES,N_triangles_Glyphs.at(surf_ind)*3,GL_UNSIGNED_INT,0);

                    glDisableVertexAttribArray(vertexLoc);
                    glDisableVertexAttribArray(normalLoc);
                    glDisableVertexAttribArray(scalarLoc);

                    glBindBuffer(GL_ARRAY_BUFFER, 0 );         // for vertex coordinates
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );
                }
            }
            glDisable(GL_CULL_FACE);
        }
    }
    void SurfaceContainer::renderColourBar(){
      //cout<<"redner colour bar"<<endl;
        if (c_bar_onoff)
        {
       //cout<<"RenderColour "<<glsl_programs.size()<<endl;
//        glUseProgram(1);//glsl_programs.at(2));
        glUseProgram(glsl_programs.at(surf_glsl_programs.at(surf_form->getCurrentSurfaceIndex())));

        if (c_bar != NULL)
        {
             //           c_bar->generateColourBar(0,v_ctables.at(surf_form->getCurrentSurfaceIndex()));

            c_bar->renderColourBar();
        }
    }
    }
//----------------DOCKING STUFF-------------------//


    void SurfaceContainer::setVisible( bool visible )
    {
            if (visible)
                surf_form->show();
            else
                surf_form->hide();
    }

    void SurfaceContainer::showDockSurfaceSelector(){ surf_form->showDockSurfaceSelector(); }
    void SurfaceContainer::showDockMaterialProperties(){ surf_form->showDockMaterialProperties(); }
    void SurfaceContainer::showDockColourBar(){ surf_form->showDockColourBar(); }
    void SurfaceContainer::showDockGlyphs(){ surf_form->showDockGlyphs(); }
    void SurfaceContainer::showDockColourMap(){ surf_form->showDockColourMap(); }
    void SurfaceContainer::showDockPolygonMode(){ surf_form->showDockPolygonMode(); }
    void SurfaceContainer::doDockSurfaceSelector(){ surf_form->doDockSurfaceSelector(); }
    void SurfaceContainer::doDockMaterialProperties(){ surf_form->doDockMaterialProperties(); }
    void SurfaceContainer::doDockColourBar(){ surf_form->doDockColourBar(); }
    void SurfaceContainer::doDockGlyphs(){ surf_form->doDockGlyphs(); }
    void SurfaceContainer::doDockColourMap(){ surf_form->doDockColourMap(); }
    void SurfaceContainer::doDockPolygonMode(){ surf_form->doDockPolygonMode(); }
    //---------------------------------------------//
    void SurfaceContainer::sliceSurfaceY()
    {
        //cout<<"surf container slice surface y "<<endl;
    }


}
