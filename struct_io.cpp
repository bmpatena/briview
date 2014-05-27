#include "struct_io.h"

using namespace std;
using namespace fslsurface_name;
namespace briview {


 void writeMaterial( const material & mat, ofstream *fout )
    {
        *fout<<mat.ambient[0]<<" "<<mat.ambient[1]<<" "<<mat.ambient[2]<<" "<<mat.ambient[3]<<" ";
        *fout<<mat.diffuse[0]<<" "<<mat.diffuse[1]<<" "<<mat.diffuse[2]<<" "<<mat.diffuse[3]<<" ";
        *fout<<mat.specular[0]<<" "<<mat.specular[1]<<" "<<mat.specular[2]<<" "<<mat.specular[3]<<" ";
        *fout<<mat.shininess<<" "<<mat.opacity<<endl;
    }


  void writeColourTable( const colour_table & ctable, ofstream *fout )
{
    *fout<<ctable.r_lut[0]<<" "<<ctable.r_lut[1]<<" "<<ctable.r_lut[2]<<" "<<ctable.r_lut[3]<<" ";
    *fout<<ctable.g_lut[0]<<" "<<ctable.g_lut[1]<<" "<<ctable.g_lut[2]<<" "<<ctable.g_lut[3]<<" ";
    *fout<<ctable.b_lut[0]<<" "<<ctable.b_lut[1]<<" "<<ctable.b_lut[2]<<" "<<ctable.b_lut[3]<<" ";
    *fout<<ctable.sc_lut[0]<<" "<<ctable.sc_lut[1]<<" "<<ctable.sc_lut[2]<<" "<<ctable.sc_lut[3]<<endl;
}

}
