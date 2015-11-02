#ifndef STRUCT_IO_H
#define STRUCT_IO_H

#include <fstream>
#include <sstream>
#include "fslsurface/fslsurface_structs.h"

namespace briview {

    void writeMaterial( const fslsurface_name::material & mat, std::ofstream *fout );
    void writeColourTable( const fslsurface_name::colour_table & ctable, std::ofstream *fout );
    void readColourTable( fslsurface_name::colour_table & ctable, std::stringstream & ss );


}
#endif // STRUCT_IO_H
