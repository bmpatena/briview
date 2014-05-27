#ifndef COLOUR_BAR_H
#define COLOUR_BAR_H
#include <QtOpenGL>

#ifdef __linux

#include <GL/glut.h>

#else
#include <GLUT/glut.h>
#endif

#include <briview_structs.h>
#include <fslsurface/fslsurface_structs.h>
namespace briview{


    class ColourBar {
    public:
        ColourBar();
        ~ColourBar();

        void renderColourBar();
        void upadteColourBar(const unsigned int & index, const fslsurface_name::colour_table & c_table);
        void addColourBar(GLuint* vbos, const fslsurface_name::colour_table & c_table);
        void setColourTable(const unsigned int & index, const fslsurface_name::colour_table & c_table);

        void generateColourBar(const unsigned int & index, const fslsurface_name::colour_table & c_table);
        //   void setGLSLPrograms(const std::vector<GLuint> & progs);
        void setGeometry( const briview::float3 & origin, const float & length, const float & width );
        void setTitle( const std::string & title_in );

        void setMaterial( const fslsurface_name::material & mat) { surf_mat = mat; }
        void setVertexAttribLocs(const GLint & vert_loc, const GLint & norm_loc, const GLint & sc_loc ) { vertexLoc=vert_loc; normalLoc=norm_loc; scalarLoc=sc_loc; }
        void renderBitmapString(  float x, float y,float z,void *font,float & num);

        void renderBitmapString( float x, float y,float z, void *font, const std::string & str) ;
        void renderStrokeString(  float x, float y,float z,void *font,char *string);

        
        
    protected:

    private:

        briview::float3 origin;
        float length ;
        float width;
        std::string title;

        unsigned int N_triangles;
        std::vector< fslsurface_name::vertexFloat* > v_vertices;

        fslsurface_name::material surf_mat;

        std::vector<fslsurface_name::colour_table> v_c_table;
        std::vector<GLuint*> vbos_c_table;

        GLint vertexLoc, normalLoc, scalarLoc;
        // std::vector<GLuint> glsl_programs;
    };


}
#endif // COLOUR_BAR_H
