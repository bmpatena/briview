#ifndef SURFACESLICER_H
#define SURFACESLICER_H

#include "fslsurface/fslsurface_structs.h"
namespace briview{


class SurfaceSlicer
{
public:
    SurfaceSlicer();
    static void slicey( fslsurface_name::vertexFloat* verts, fslsurface_name::vertexFloat* tris ,  const float & slicey );

};

}
#endif // SURFACESLICER_H
