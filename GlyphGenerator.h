#ifndef GLYPHGENERATOR_H
#define GLYPHGENERATOR_H
#include "fslsurface/fslsurface.h"
#include "fslsurface/fslsurface_structs.h"
#include <vector>
#include <QtOpenGL>

namespace briview{

    void calculateNormals( std::vector<fslsurface_name::vertex<float> > &  verts, const std::vector<GLuint> & indices);
    void  generateArrowGlyph( std::vector<fslsurface_name::vertex<float> > & vertices, std::vector<GLuint> & indices,  const float & head_radius, const float & shaft_radius, const float & height, const float & head_height, const unsigned int & N_faces );
    //void  generateArrowGlyphs( std::vector<fslsurface_name::vertex<float> > & v_vertices, std::vector<GLuint> & v_indices,  fslsurface_name::vertexFloat* vertices, fslsurface_name::float3* vecs, const unsigned int & N_verts, const float & head_radius, const float & shaft_radius, const float & height, const float & head_height, const unsigned int & N_faces );

    void  generateArrowGlyphs( std::vector<fslsurface_name::vertex<float> > & v_vertices , std::vector<GLuint> & v_indices,  const fslsurface_name::fslSurface< float, unsigned int > * surf, const unsigned int & vec_index, const float & head_radius, const float & shaft_radius, const float & height, const float & head_height, const unsigned int & N_faces );

}
#endif // GLYPHGENERATOR_H
