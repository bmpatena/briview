#ifndef VECTORFUNCTIONS_H
#define VECTORFUNCTIONS_H

#include <QtOpenGL>
#include "fslsurface/fslsurface_structs.h"
#include <briview_structs.h>
namespace briview {

    void normalize(briview::float3 & v);

  briview::float3 subtract(const briview::float3 & a, const briview::float3 & b);
  briview::float3 normal(const briview::float3 & v1, const briview::float3 & v2 );
  void calculateNormals( fslsurface_name::vertex<float>* verts, GLuint* indices, const unsigned int & N_verts,  const unsigned int & N_indices);
  void calculateNormals( std::vector<fslsurface_name::vertex<float> > &  verts, const std::vector<GLuint> & indices);
    void rotateVertices( std::vector<fslsurface_name::vertex<float> > & vertices, const float & theta, const float & phi );

    void shiftVertices( std::vector<fslsurface_name::vertex<float> > & vertices, const float & dx, const float & dy , const float & dz );


    void shiftIndices( std::vector<GLuint> & indices, const unsigned int & index_shift);
    void setVertexScalars( std::vector<fslsurface_name::vertex<float> > & vertices, const float & scalars );

 }
#endif // VECTORFUNCTIONS_H
