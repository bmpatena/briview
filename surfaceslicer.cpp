#include "surfaceslicer.h"
#include <iostream>
using namespace std;
using namespace fslsurface_name;
namespace briview{

SurfaceSlicer::SurfaceSlicer()
{
}

void SurfaceSlicer::slicey( vertexFloat* verts, vertexFloat* tris , const float & slicey )
{
    cout<<"run slice y "<<slicey<<endl;
}

}
