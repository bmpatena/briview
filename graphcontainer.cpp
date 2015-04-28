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
    radius_ = graph_form->getRadius();
    radius_link_ = graph_form->getLinkRadius();
    Nnodes_ = 0;
    vertexLoc=0;
    normalLoc=1;
    scalarLoc=2;
vbos_nodes_=NULL;
//   vbos_vertices = new GLuint[2];
connect(graph_form,SIGNAL(sig_copy_to_surfaces()),this,SIGNAL(sig_copy_to_surfaces()));
connect(graph_form,SIGNAL(sig_updateGraph()),this,SIGNAL(sig_updateGL()));
connect(graph_form,SIGNAL(sig_updateRadius(double)),this,SLOT(setRadius(double)));
connect(graph_form,SIGNAL(sig_updateLinkRadius(double)),this,SLOT(setLinkRadius(double)));


connect(graph_form,SIGNAL(sig_changedColourTableNode()),this, SLOT(updateColourTableNode()));
//connect(graph_form,SIGNAL(sig_changedColourTableNodeSc()),this, SLOT(updateColourTableNodeSc()));

connect(graph_form,SIGNAL(sig_changedColourTableLinks()),this, SLOT(updateColourTableLinks()));
//connect(graph_form,SIGNAL(sig_changedColourTableLinksSc()),this, SLOT(updateColourTableLinksSc()));


}

graphContainer::~graphContainer()
{
    delete graph_form;

    if (vbos_nodes_ != NULL )
    {
        glDeleteBuffers(4,vbos_nodes_);
        delete[]   vbos_nodes_;
    }


}

//void graphContainer::updateColourTableNode()
//{
//cout<<"graphContainer updateColourTableNode "<<endl;
//nodes_ctable = graph_form->getNodeColourTable();
//}


//void graphContainer::updateColourTableNodeSc()
//{
//    cout<<"graphContainer updateColourTableNodeSc "<<endl;

//}
//void graphContainer::updateColourTableLinks()
//{
//cout<<"graphContainer updateColourTableLinks "<<endl;
//links_ctable = graph_form->getLinksColourTable();

//}


//void graphContainer::updateColourTableLinksSc()
//{
//    cout<<"graphContainer updateColourTableLinksSc "<<endl;

//}


void graphContainer::setVisible( bool visible )
{
    cout<<"set visible "<<endl;
        if (visible)
            graph_form->show();
        else
            graph_form->hide();
}
void graphContainer::setRadius(  double r_in )
{
    cout<<"setRadius "<<endl;
    radius_ = r_in;
    generateNodes();
}
void graphContainer::setLinkRadius(  double r_in )
{
    cout<<"setRadius "<<endl;
    radius_link_ = r_in;
    generateLinks();
}
void graphContainer::setRadius(  )
{
    radius_ = graph_form->getRadius();
    cout<<"setRadiusFromForm "<<radius_<<endl;

    generateNodes();
}

void graphContainer::setLinkRadius(  )
{
    radius_link_ = graph_form->getLinkRadius();
    cout<<"setRadiusFromForm "<<radius_link_<<endl;

    generateLinks();
}


void graphContainer::readFile( const string & filename )
{
    ifstream fin(filename.c_str());

    if (fin.is_open())
    {
        v_conn_.clear();
        v_cog_.clear();
        v_names_.clear();
        string line;
        while (getline(fin,line))
        {
            stringstream ss;
            ss<<line;
            string name;
            float3 cog;
            ss>>name;
            if (name == "conn")
            {
                conn link;
                ss>>link.src>>link.dest>>link.strength;
                v_conn_.push_back(link);
            }else{
                ss>>cog.x>>cog.y>>cog.z;
                v_cog_.push_back(cog);
                v_names_.push_back(name);
            }
        }
        fin.close();
    }
generateNodes();
generateLinks();
}

void graphContainer::generateNodes( )
{
    if (vbos_nodes_ == NULL )
    {
        vbos_nodes_ = new GLuint[4];
        glGenBuffers(4,vbos_nodes_);
    }

      vec3<float> cog(0,0,0);
      int count =0 ;
      for ( typename vector< float3 >::iterator i_c = v_cog_.begin(); i_c != v_cog_.end(); ++i_c,++count )
      {
          fslSurface<float,unsigned int> surf_graph;
cout<<"count  "<<count<<endl;
          makeSphere( surf_graph, radius_, 20 , 20 , vec3<float>(i_c->x,i_c->y,i_c->z) );
          surf_graph.addScalars(count,"node_index");
          surf_graph.setScalars(0);
          if (count == 0 )
              surf_graph_nodes_= surf_graph;
          else
              surf_graph_nodes_.append(surf_graph);
      }

//      count = 0 ;
//      for ( vector<conn>::iterator i = v_conn_.begin(); i != v_conn_.end(); ++i,++count)
//      {

//          fslSurface<float,unsigned int> surf_graph_conns;
//          cout<<"makeCylinder  "<<endl;
//          makeCylinder( surf_graph_conns, radius_link_,radius_link_ , 20, 20, vec3<float>(v_cog_[i->src].x,v_cog_[i->src].y,v_cog_[i->src].z),vec3<float>(v_cog_[i->dest].x,v_cog_[i->dest].y,v_cog_[i->dest].z));
//          if (count==0)
//          {
//              surf_graph_links_ = surf_graph_conns;
//          }else{
//              surf_graph_links_.append(surf_graph_conns);
//          }

//          //      writeGIFTI(surf_graph_nodes_,"/Users/brian/git_repos/briview/nodes.gii");
//          cout<<"write cyl"<<endl;
//      }
// writeGIFTI(surf_graph_links_,"/Users/brian/git_repos/briview/conns.gii");
surf_graph_nodes_.calculateNormals();
//surf_graph_links_.calculateNormals();

 //      GLuint* vbos_nodes_;
//          GLint vertexLoc, normalLoc, scalarLoc;
      cout<<"render "<<vbos_nodes_[0]<<" "<<vbos_nodes_[1]<<endl;
      glBufferData_Vertices(surf_graph_nodes_,vbos_nodes_[0]);
glBufferData_Faces(surf_graph_nodes_,vbos_nodes_[1]);
//glBufferData_Vertices(surf_graph_links_,vbos_nodes_[2]);
//glBufferData_Faces(surf_graph_links_,vbos_nodes_[3]);*/


//      fslsurface_name::render(surf_graph_nodes_, vertexLoc, normalLoc, scalarLoc,vbos_nodes_[0] ,vbos_nodes_[1] );
cout<<"done render "<<endl;



emit sig_updateGL();
}

void graphContainer::generateLinks( )
{
    if (vbos_nodes_ == NULL )
    {
        vbos_nodes_ = new GLuint[4];
        glGenBuffers(4,vbos_nodes_);
    }


      int count = 0 ;
      for ( vector<conn>::iterator i = v_conn_.begin(); i != v_conn_.end(); ++i,++count)
      {

          fslSurface<float,unsigned int> surf_graph_conns;
//          cout<<"makeCylinder  "<<endl;
          makeCylinder( surf_graph_conns, radius_link_,radius_link_ , 20, 20, vec3<float>(v_cog_[i->src].x,v_cog_[i->src].y,v_cog_[i->src].z),vec3<float>(v_cog_[i->dest].x,v_cog_[i->dest].y,v_cog_[i->dest].z));
          surf_graph_conns.addScalars(i->strength,"edgeStrength");
          surf_graph_conns.setScalars(0);
          if (count==0)
          {
              surf_graph_links_ = surf_graph_conns;
          }else{
              surf_graph_links_.append(surf_graph_conns);
          }

          //      writeGIFTI(surf_graph_nodes_,"/Users/brian/git_repos/briview/nodes.gii");
//          cout<<"write cyl"<<endl;
      }
// writeGIFTI(surf_graph_links_,"/Users/brian/git_repos/briview/conns.gii");
surf_graph_links_.calculateNormals();

 //      GLuint* vbos_nodes_;
//          GLint vertexLoc, normalLoc, scalarLoc;
      cout<<"render "<<vbos_nodes_[0]<<" "<<vbos_nodes_[1]<<endl;
//      glBufferData_Vertices(surf_graph_nodes_,vbos_nodes_[0]);
//glBufferData_Faces(surf_graph_nodes_,vbos_nodes_[1]);
glBufferData_Vertices(surf_graph_links_,vbos_nodes_[2]);
glBufferData_Faces(surf_graph_links_,vbos_nodes_[3]);


//      fslsurface_name::render(surf_graph_nodes_, vertexLoc, normalLoc, scalarLoc,vbos_nodes_[0] ,vbos_nodes_[1] );
cout<<"done render "<<endl;



emit sig_updateGL();
}

void graphContainer::setColourTableUniformLocations(const GLint & loc_r_lut_in,const GLint & loc_g_lut_in,const GLint & loc_b_lut_in, const GLint & loc_a_lut_in,const GLint & loc_sc_lut_in,const GLint & loc_r_lut_last_in,const GLint & loc_g_lut_last_in,const GLint & loc_b_lut_last_in,const GLint & loc_a_lut_last_in, const GLint & loc_sc_lut_last_in, const GLint & loc_low_clamp_in)
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

void graphContainer::setColourTableNode(  )
{
//    nodes_ctable = graph_form->getNodeColourTable();
//            glUseProgram(glsl_programs[1]);

            glUniform4fv(loc_r_lut,1,nodes_ctable.r_lut);
            glUniform4fv(loc_g_lut,1,nodes_ctable.g_lut);
            glUniform4fv(loc_b_lut,1,nodes_ctable.b_lut);
            glUniform4fv(loc_a_lut,1,nodes_ctable.a_lut);
            glUniform4fv(loc_sc_lut,1,nodes_ctable.sc_lut);

            glUniform2fv(loc_r_lut_last,1,nodes_ctable.r_lut + 4);
            glUniform2fv(loc_g_lut_last,1,nodes_ctable.g_lut + 4);
            glUniform2fv(loc_b_lut_last,1,nodes_ctable.b_lut + 4);
            glUniform2fv(loc_a_lut_last,1,nodes_ctable.a_lut + 4);
            glUniform2fv(loc_sc_lut_last,1,nodes_ctable.sc_lut + 4);

            glUniform4fv(loc_low_clamp,1,nodes_ctable.low_clamp);
}


void graphContainer::updateColourTableNode(  )
{//this update colour table now updates the GLSL colour table not the form
    cout<<"update coour node table "<<endl;

    //    vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
     //   for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
      //  {
//setColourTableNode();
    nodes_ctable = graph_form->getNodeColourTable();


        emit sig_updateGL();

   cout<<"done update colour table"<<endl;
}
void graphContainer::setColourTableLinks(  )
{
//    nodes_ctable = graph_form->getNodeColourTable();
//            glUseProgram(glsl_programs[1]);
    //        int loc_test = glGetUniformLocation(glsl_programs[1],"r_lut");
           // v_ctables.at(*i_ind)=surf_form->getCurrentColourTable();
            glUniform4fv(loc_r_lut,1,links_ctable.r_lut);
            glUniform4fv(loc_g_lut,1,links_ctable.g_lut);
            glUniform4fv(loc_b_lut,1,links_ctable.b_lut);
            glUniform4fv(loc_a_lut,1,links_ctable.a_lut);
            glUniform4fv(loc_sc_lut,1,links_ctable.sc_lut);
    //cout<<"gluniform "<<endl;
            glUniform2fv(loc_r_lut_last,1,links_ctable.r_lut + 4);
            glUniform2fv(loc_g_lut_last,1,links_ctable.g_lut + 4);
            glUniform2fv(loc_b_lut_last,1,links_ctable.b_lut + 4);
            glUniform2fv(loc_a_lut_last,1,links_ctable.a_lut + 4);
            glUniform2fv(loc_sc_lut_last,1,links_ctable.sc_lut + 4);

            glUniform4fv(loc_low_clamp,1,links_ctable.low_clamp);

}

void graphContainer::updateColourTableLinks(  )
{//this update colour table now updates the GLSL colour table not the form
    cout<<"update coour links table "<<endl;
    links_ctable = graph_form->getLinksColourTable();

    //    vector<int> cur_indices = surf_form->getCurrentSurfaceIndices();
     //   for (vector<int>::iterator i_ind = cur_indices.begin(); i_ind != cur_indices.end(); ++i_ind )
      //  {
//        setColourTableLinks();

        emit sig_updateGL();

   cout<<"done update colour table"<<endl;
}

void graphContainer::render()
{
    if (vbos_nodes_ != NULL )
    {
    cout<<"graph render "<<glsl_programs[0]<<" "<<glsl_programs.size()<<endl;
//    writeGIFTI(surf_graph_nodes_,"/Users/brian/git_repos/briview/nodes_render.gii");
        glUseProgram(glsl_programs[1]);
//        updateColourTableNode();
        setColourTableLinks();
       fslsurface_name::render( surf_graph_links_, vertexLoc, normalLoc, scalarLoc,vbos_nodes_[2] ,vbos_nodes_[3] );
       setColourTableNode();
       fslsurface_name::render(surf_graph_nodes_, vertexLoc, normalLoc, scalarLoc,vbos_nodes_[0] ,vbos_nodes_[1] );

}
}

void graphContainer::setGLSLPrograms(const std::vector<GLuint> & progs)
{
    glsl_programs=progs;
}


};
