#ifndef SURFACECONTAINER_H
#define SURFACECONTAINER_H

#include <colour_bar.h>
#include <QtOpenGL>
//#include "my_structs.h"
#include <QObject>
#include <briview_structs.h>
#include "fslsurface/fslsurface_structs.h"
#include "surfacemanipulator.h"
#include "fslsurface/fslsurfacegl.h"

#include "fslsurface/fslsurface.h"
#include "fslsurface/fslsurfaceio.h"

namespace briview{
   // struct vertex;

    class SurfaceContainer : public QObject{
        Q_OBJECT

    public:
        SurfaceContainer( );
        ~SurfaceContainer( );
        void writeSurfaces( std::ofstream *fout) const ;
        void writeSurface( ) const ;

        void addSurface( const std::string & filename, GLuint*  vbos);//fslvtkio::fslvtkIO* mesh , GLuint* verts);
        void addSurface( fslsurface_name::fslSurface<float,unsigned int> * surface, GLuint*  vbos, const std::string & surf_name);//fslvtkio::fslvtkIO* mesh , GLuint* verts);

        void appendSurfaceData( const std::string & filename );
        void addGlyphs( const unsigned int & surf_index ,  GLuint*  vbos);
     void regenerateGlyphs( const unsigned int & surf_index);

        void addColourBar(GLuint*  vbos);

        void renderOpaqueSurfaces();
        void renderTranslucentSurfaces();
         void renderOpaqueGlyphs();
        void renderTranslucentGlyphs();
        void renderColourBar();

        void setVertexAttribLocs(const GLint & vert_loc, const GLint & norm_loc, const GLint & sc_loc ) { vertexLoc=vert_loc; normalLoc=norm_loc; scalarLoc=sc_loc; }
        void setVertexAttribVertexLoc(const GLint & vert_loc ) { vertexLoc=vert_loc; }
        void setVertexAttribNormalLoc(const GLint & normal_loc ) { normalLoc=normal_loc; }
        void setVertexAttribScalarLoc(const GLint & scalar_loc ) { scalarLoc=scalar_loc; }

        briview::float3 normal(const  briview::float3 & v1, const briview::float3 & v2 );


        void setAmbient(const unsigned int & surf_ind, const fslsurface_name::float4 & rgba );
        void setDiffuse(const unsigned int & surf_ind, const fslsurface_name::float4 & rgba );
        void setSpecular(const unsigned int & surf_ind, const fslsurface_name::float4 & rgba );
        void setShininess(const unsigned int & surf_ind, const float & sh );
        void setSurfaceProperties(const unsigned int & surf_ind, const fslsurface_name::material & mat_in );

        unsigned int getNumberOfSurfaces();
        int getCurrentSurfaceIndex();
        int getCurrentAppendedSurfaceIndex();
        fslsurface_name::material getSurfaceProperties(const unsigned int & surf_ind );
        briview::float3 getSurfaceCOG(const int & index);
        void importPresetMaterials( const std::string & filename);

        SurfaceManipulator* getSurfaceManipulatorWidget() { return surf_form; }
 //       void setColourTableUniformLocations(const GLint & loc_r_lut,const GLint & loc_g_lut,const GLint & loc_b_lut, const GLint & loc_sc_lut, const GLint & loc_r_lut_last,const GLint & loc_g_lut_last,const GLint & loc_b_lut_last, const GLint & loc_sc_lut_last, const GLint & loc_low_clamp);
      //  void setColourBarUniformLocations(const GLint & loc_r_lut,const GLint & loc_g_lut,const GLint & loc_b_lut, const GLint & loc_sc_lut);
    void setColourTableUniformLocations(const GLint & loc_r_lut_in,const GLint & loc_g_lut_in,const GLint & loc_b_lut_in, const GLint & loc_a_lut_in,const GLint & loc_sc_lut_in,const GLint & loc_r_lut_last_in,const GLint & loc_g_lut_last_in,const GLint & loc_b_lut_last_in,const GLint & loc_a_lut_last_in, const GLint & loc_sc_lut_last_in, const GLint & loc_low_clamp_in);

        void setGLSLPrograms(const std::vector<GLuint> & progs);
        void setGLSLProgramsColourBar(const std::vector<GLuint> & progs);

    public slots:


        void changeMaterial(int index);
        void changeScalarData();
        void playScalarData();
        void prevScalarData();
        void nextScalarData();

        void displayGlyphs( bool disp );

        void updateAppendedInfo( int index );
        void updateSurfaceProperties( );//gets from form
        void updateGlyphProperties();//sets form

    void changeGlyph(int index);

        void updateSurfaceProperties( int index);//sets form
        void updateColourTable();
        void updateColourTableSc();
        void updateColourTable( const int & index );
        void changeColourTable(int index);
        void updateColourBar();
            void updatePolygonModeBack( int gl_enum );
            void updatePolygonModeFront( int gl_enum );
            void updatePolygonModeBackGlyph( int gl_enum );
            void updatePolygonModeFrontGlyph( int gl_enum );
        void changeShaderProgram( bool use_scalars );
        void toggleCullFace( bool cull );
        void setFrontFaceCCW( bool CCW );

        void sliceSurfaceY();
        void setVisible( bool visible );

        //-----------DOCKING-------------//
        void showDockSurfaceSelector();
        void showDockMaterialProperties();
        void showDockColourBar();
        void showDockGlyphs();
        void showDockColourMap();
        void showDockPolygonMode();

        void doDockSurfaceSelector();
        void doDockMaterialProperties();
        void doDockColourBar();
        void doDockGlyphs();
        void doDockColourMap();
        void doDockPolygonMode();


        //--------------------------------//




    signals:
        void sig_updateGL();
        void sig_appendSurfaceData();
    protected:

    private:
        void loadColourTables(const std::string & filename );

        std::vector<GLenum> fill_type_b, fill_type_f;
        std::vector<GLenum> fill_type_b_glyph, fill_type_f_glyph;
        std::vector< std::string > cmap_names;


        SurfaceManipulator* surf_form;
        std::vector<briview::float3> v_cog;
        std::vector<std::string> surface_filenames;
        std::vector< std::vector<std::string> > surface_appended_names;

    //    std::vector< std::vector<int> > scalar_indices;
     //   std::vector< int > scalar_indices;
//
        std::vector<GLuint*> vbos_vertices;
    //    std::vector<fslsurface_name::vertexFloat*> v_vertices;
     //   std::vector<fslsurface_name::float3*> v_vectors;

        std::vector< fslsurface_name::fslSurface<float,unsigned int>* > v_surfaces;

        std::vector<GLsizei> N_triangles;//, N_verts, N_vecs;
        std::vector<fslsurface_name::bounds> v_bounds;

        ///GLYPH STUFF
        std::vector<GLsizei> N_triangles_Glyphs;
        std::vector<GLuint*> vbos_glyphs;

       //  std::vector<GLuint*> vbos_colour_bars;
        ColourBar* c_bar;
        bool c_bar_onoff;
        GLint vertexLoc, normalLoc, scalarLoc;
     //   std::vector<unsigned int> callListIndices;

        GLint loc_r_lut,loc_g_lut,loc_b_lut,loc_a_lut,loc_sc_lut;
        GLint loc_r_lut_last,loc_g_lut_last,loc_b_lut_last,loc_a_lut_last,loc_sc_lut_last;
        GLint loc_low_clamp;
//GLint cbar_loc_r_lut, cbar_loc_g_lut, cbar_loc_b_lut, cbar_loc_sc_lut;
        std::vector< fslsurface_name::material > preset_materials;
        std::vector< std::string > preset_materials_names;

        std::vector< fslsurface_name::material > v_materials;
        std::vector< fslsurface_name::glyph > v_glyph_props;
        std::vector< int > v_glyph_exists;
        std::vector< int > showGlyphs;
        std::vector< int > cullFace;
        std::vector< int > frontFace;

        std::vector<int> v_cmap_index;
        std::vector< fslsurface_name::colour_table > v_surf_ctables; //contains images specific colour table
        std::vector< fslsurface_name::colour_table > v_ctables; //contains base colour tables, loaded from file

        std::vector< GLuint > glsl_programs;//compile GLSL program
        std::vector< GLuint > c_bar_progs;
        std::vector< int > surf_glsl_programs;//which program the surface uses
        std::vector< int > surf_material_index;




    };

}
#endif // SURFACECONTAINER_H
