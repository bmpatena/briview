#include <fstream>
#include <sstream>

#include "graphcontainer.h"
#include "fslsurface/fslsurfacegl.h"

#include "fslsurface/fslsurface.h"
#include "fslsurface/fslsurfacefns.h"

#include "fslsurface/fslsurfaceio.h"


using namespace std;
using namespace fslsurface_name;
namespace briview{

graphContainer::graphContainer(QObject *parent) : QObject(parent)
{
    graph_form = new graphManipulator();
    radius_ = 5;
    Nnodes_ = 0;
    vertexLoc=0;
    normalLoc=1;
    scalarLoc=2;
vbos_nodes_=NULL;
//   vbos_vertices = new GLuint[2];
connect(ui,SIGNAL(sig_copy_to_surfaces()),this,SIGNAL(sig_copy_to_surfaces()));

}

graphContainer::~graphContainer()
{
    delete graph_form;

    if (vbos_nodes_ != NULL )
    {
        glDeleteBuffers(2,vbos_nodes_);
        delete[]   vbos_nodes_;
    }


}


void graphContainer::setVisible( bool visible )
{
    cout<<"set visible "<<endl;
        if (visible)
            graph_form->show();
        else
            graph_form->hide();
}
void graphContainer::readFile( const string & filename )
{
    ifstream fin(filename.c_str());

    if (fin.is_open())
    {
        string line;
        while (getline(fin,line))
        {
            stringstream ss;
            ss<<line;
            string name;
            float3 cog;
            ss>>name>>cog.x>>cog.y>>cog.z;
            v_cog_.push_back(cog);
            v_names_.push_back(name);
        }
        fin.close();
    }
generateNodes();
}

void graphContainer::generateNodes( )
{
    if (vbos_nodes_ == NULL )
    {
        vbos_nodes_ = new GLuint[2];
        glGenBuffers(2,vbos_nodes_);
    }

      vec3<float> cog(0,0,0);
      int count =0 ;
      for ( typename vector< float3 >::iterator i_c = v_cog_.begin(); i_c != v_cog_.end(); ++i_c,++count )
      {
          fslSurface<float,unsigned int> surf_graph;

          makeSphere( surf_graph, radius_, 20 , 20 , vec3<float>(i_c->x,i_c->y,i_c->z) );
          surf_graph.addScalars(count+1,"graph_index");
          if (count == 0 )
              surf_graph_nodes_= surf_graph;
          else
              surf_graph_nodes_.append(surf_graph);
      }


      writeGIFTI(surf_graph_nodes_,"/Users/brian/git_repos/briview/nodes.gii");
//      GLuint* vbos_nodes_;
//          GLint vertexLoc, normalLoc, scalarLoc;
      cout<<"render "<<vbos_nodes_[0]<<" "<<vbos_nodes_[1]<<endl;
      glBufferData_Vertices(surf_graph_nodes_,vbos_nodes_[0]);
glBufferData_Faces(surf_graph_nodes_,vbos_nodes_[1]);
      fslsurface_name::render(surf_graph_nodes_, vertexLoc, normalLoc, scalarLoc,vbos_nodes_[0] ,vbos_nodes_[1] );
cout<<"done render "<<endl;
}
void graphContainer::render()
{
    if (vbos_nodes_ != NULL )
    {
    cout<<"graph render "<<endl;
//    writeGIFTI(surf_graph_nodes_,"/Users/brian/git_repos/briview/nodes_render.gii");

        fslsurface_name::render(surf_graph_nodes_, vertexLoc, normalLoc, scalarLoc,vbos_nodes_[0] ,vbos_nodes_[1] );
}
}



};
