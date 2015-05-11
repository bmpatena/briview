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
blendFunc=0;
rel_opacity=0;
Nverts_per_node=0;
weightLinksByConnectivity=0;

loadColourTables((QApplication::applicationDirPath() + "/assets/colour_maps.txt").toStdString());

//   vbos_vertices = new GLuint[2];
connect(graph_form,SIGNAL(sig_copy_to_surfaces()),this,SIGNAL(sig_copy_to_surfaces()));
connect(graph_form,SIGNAL(sig_updateGraph()),this,SIGNAL(sig_updateGL()));
connect(graph_form,SIGNAL(sig_updateRadius(double)),this,SLOT(setRadius(double)));
connect(graph_form,SIGNAL(sig_updateLinkRadius(double)),this,SLOT(setLinkRadius(double)));


connect(graph_form,SIGNAL(sig_changedColourTableNode()),this, SLOT(updateColourTableNode()));
//connect(graph_form,SIGNAL(sig_changedColourTableNodeSc()),this, SLOT(updateColourTableNodeSc()));

connect(graph_form,SIGNAL(sig_changedColourTableLinks()),this, SLOT(updateColourTableEdges()));
//connect(graph_form,SIGNAL(sig_changedColourTableLinksSc()),this, SLOT(updateColourTableEdgesSc()));
connect(graph_form, SIGNAL(sig_setColourTableNodeSc(int)),this, SLOT(setNodeCmapRange(int)));
connect(graph_form, SIGNAL(sig_setColourTableEdgeSc(int)),this, SLOT(setEdgeCmapRange(int)));

connect(graph_form, SIGNAL(sig_changedColourTableNode(int)),this,SLOT(changeColourTableNode(int)));
connect(graph_form, SIGNAL(sig_changedColourTableEdge(int)),this,SLOT(changeColourTableEdge(int)));

connect(graph_form, SIGNAL(sig_addScalars(QString)),this,SLOT(addScalars(QString)));

connect(graph_form,SIGNAL(sig_changedBlendFunc(int) ),this,SLOT(changeBlendFunc(int)));
connect(graph_form,SIGNAL(sig_changedOpacityMode(int)),this, SLOT(setOpacityMode(int)));

connect(graph_form,SIGNAL(sig_wEdgeStateChanged(int)),this, SLOT(setWegihtByEdge(int)));

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

void graphContainer::changeBlendFunc(int index)
{
    blendFunc=index;
    emit sig_updateGL();
}
void graphContainer::setOpacityMode(int mode)
{
    rel_opacity=mode;
    emit sig_updateGL();

}
void graphContainer::setWegihtByEdge(int state)
{
    weightLinksByConnectivity=state;
    generateLinks();
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
//edges_ctable = graph_form->getLinksColourTable();

//}


//void graphContainer::updateColourTableLinksSc()
//{
//    cout<<"graphContainer updateColourTableLinksSc "<<endl;

//}

//void graphContainer::importPresetMaterials( const string & filename)
//{
//    ifstream fin(filename.c_str());
//    string mat_name;
//    while ( fin>> mat_name)
//    {
//        material mat;
//        fin>> mat.ambient[0]>> mat.ambient[1]>> mat.ambient[2]>> mat.ambient[3]>> mat.diffuse[0]>> mat.diffuse[1]>> mat.diffuse[2]>> mat.diffuse[3]>> mat.specular[0]>> mat.specular[1]>> mat.specular[2]>> mat.specular[3]>> mat.shininess;
//        mat.opacity=1.0;
//        preset_materials.push_back(mat);
//        preset_materials_names.push_back(mat_name);

//        graph_form->addPresetSetMaterial(mat_name);
//    }
//}


void graphContainer::loadColourTables(const string & filename )
{
    ifstream fin(filename.c_str());
    string temp;

    if (fin.is_open())
    {
        while (fin>>temp)
        {
            //cout<<"load colour map: "<<temp<<endl;
            cmap_names.push_back(temp);
            graph_form->addColourTable(temp);

            float4 sclut,rlut,glut,blut,alut,clamp;
            float2 scl,rl,gl,bl,al;

            fin>>sclut.x>>sclut.y>>sclut.z>>sclut.w>>scl.x>>scl.y;
            fin>>rlut.x>>rlut.y>>rlut.z>>rlut.w>>rl.x>>rl.y;
            fin>>glut.x>>glut.y>>glut.z>>glut.w>>gl.x>>gl.y;
            fin>>blut.x>>blut.y>>blut.z>>blut.w>>bl.x>>bl.y;
            fin>>alut.x>>alut.y>>alut.z>>alut.w>>al.x>>al.y;
            fin>>clamp.x>>clamp.y>>clamp.z>>clamp.w;

            v_ctables.push_back(colour_table( rlut, rl, glut, gl, blut, bl, alut, al, sclut,scl,clamp.x, clamp.y, clamp.z, clamp.w));
        }
        if (cmap_names.size()>0)
        {

            graph_form->setColourTableNodeIndex(0);
            graph_form->setColourTableEdgeIndex(0);

        }
    }else{
        cerr<<"Colour map file, "<<filename<<", does not exist"<<endl;
        exit (EXIT_FAILURE);
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
void graphContainer::setNodeCmapRange( int curIndex )
{
    //int curIndex = graph_form->getCurrentNodeScalarIndex();
    vector<float>::const_iterator i = surf_graph_nodes_.const_scbegin(curIndex);
    float min,max;
    min = max = *i;

    ++i;
    for ( ; i != surf_graph_nodes_.const_scend(curIndex); ++i)
    {
        if (*i < min ) min = *i;
        if (*i > max ) max = *i;
    }
    graph_form->setNodeCMAPRange(min,max);
    updateColourTableNode();
}
void graphContainer::setEdgeCmapRange( int curIndex )
{
    //int curIndex = graph_form->getCurrentNodeScalarIndex();
    vector<float>::const_iterator i = surf_graph_links_.const_scbegin(curIndex);
    float min,max;
    min = max = *i;

    ++i;
    for ( ; i != surf_graph_links_.const_scend(curIndex); ++i)
    {
        if (*i < min ) min = *i;
        if (*i > max ) max = *i;
    }
    graph_form->setEdgeCMAPRange(min,max);
    updateColourTableNode();
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
          graph_form->setNodesScalarsName("node_index",0);

          if (count == 0 )
          {
              surf_graph_nodes_= surf_graph;
              Nverts_per_node = surf_graph.getNumberOfVertices();
          }else
           {
              surf_graph_nodes_.append(surf_graph);
      }
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

          float radius = radius_link_;
if (weightLinksByConnectivity>=0)
{
    radius += i->strength;
}
          fslSurface<float,unsigned int> surf_graph_conns;
//          cout<<"makeCylinder  "<<endl;
          makeCylinder( surf_graph_conns, radius_link_,radius_link_ , 20, 20, vec3<float>(v_cog_[i->src].x,v_cog_[i->src].y,v_cog_[i->src].z),vec3<float>(v_cog_[i->dest].x,v_cog_[i->dest].y,v_cog_[i->dest].z));
          surf_graph_conns.addScalars(i->strength,"edgeStrength");
          surf_graph_conns.setScalars(0);
          graph_form->setEdgeScalarsName("edgeStrength",0);
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
void graphContainer::addScalars( QString filename )
{
    ifstream fin(filename.toStdString().c_str());
    if (fin.is_open())
    {
        vector<float> scalars(surf_graph_nodes_.getNumberOfVertices(),0);
        vector<float>::iterator i_sc = scalars.begin();
        string name, line;//right now assumes a single column
        getline(fin,name);
                while (getline(fin,line))
        {
            stringstream ss;
            ss<<line;
            float val;
            ss>>val;
            cout<<"add scalar for node "<<Nverts_per_node<<" "<<val<<endl;
            for (int i = 0 ; i < Nverts_per_node; ++i,++i_sc )
            {
                *i_sc = val;
                cout<<"val "<<i<<" "<<(*i_sc)<<endl;
                }

        }
        fin.close();
        cout<<"scalar size "<<scalars.size()<<" "<<scalars[scalars.size()-1]<<endl;
        surf_graph_nodes_.addScalars(scalars,name);
        graph_form->addNodeScalarsToList(name);
    }
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
void graphContainer::changeColourTableNode(int index)
{
    cout<<"change colour table index GRAPH "<<index<<endl;

          if (index >0)
        {

               memcpy(nodes_ctable.r_lut , v_ctables[index-1].r_lut, sizeof(float)*6);
              memcpy(nodes_ctable.g_lut , v_ctables[index-1].g_lut, sizeof(float)*6);
              memcpy(nodes_ctable.b_lut , v_ctables[index-1].b_lut, sizeof(float)*6);
              memcpy(nodes_ctable.a_lut , v_ctables[index-1].a_lut, sizeof(float)*6);
//            graph_form->setColourTableNodeNoSc(v_ctables[index-1]);
            graph_form->setColourTableNodeNoSc(nodes_ctable);

            setColourTableNode();
                emit sig_updateGL();
          }

    //}
}
void graphContainer::changeColourTableEdge(int index)
{
    cout<<"change colour table index GRAPH "<<index<<endl;

          if (index >0)
        {

               memcpy(edges_ctable.r_lut , v_ctables[index-1].r_lut, sizeof(float)*6);
              memcpy(edges_ctable.g_lut , v_ctables[index-1].g_lut, sizeof(float)*6);
              memcpy(edges_ctable.b_lut , v_ctables[index-1].b_lut, sizeof(float)*6);
              memcpy(edges_ctable.a_lut , v_ctables[index-1].a_lut, sizeof(float)*6);
//            graph_form->setColourTableNodeNoSc(v_ctables[index-1]);
            graph_form->setColourTableEdgeNoSc(edges_ctable);

            setColourTableEdge();
                emit sig_updateGL();
          }

    //}
}
void graphContainer::setColourTableEdge(  )
{
//    nodes_ctable = graph_form->getNodeColourTable();
//            glUseProgram(glsl_programs[1]);
    //        int loc_test = glGetUniformLocation(glsl_programs[1],"r_lut");
           // v_ctables.at(*i_ind)=surf_form->getCurrentColourTable();
            glUniform4fv(loc_r_lut,1,edges_ctable.r_lut);
            glUniform4fv(loc_g_lut,1,edges_ctable.g_lut);
            glUniform4fv(loc_b_lut,1,edges_ctable.b_lut);
            glUniform4fv(loc_a_lut,1,edges_ctable.a_lut);
            glUniform4fv(loc_sc_lut,1,edges_ctable.sc_lut);
    //cout<<"gluniform "<<endl;
            glUniform2fv(loc_r_lut_last,1,edges_ctable.r_lut + 4);
            glUniform2fv(loc_g_lut_last,1,edges_ctable.g_lut + 4);
            glUniform2fv(loc_b_lut_last,1,edges_ctable.b_lut + 4);
            glUniform2fv(loc_a_lut_last,1,edges_ctable.a_lut + 4);
            glUniform2fv(loc_sc_lut_last,1,edges_ctable.sc_lut + 4);

            glUniform4fv(loc_low_clamp,1,edges_ctable.low_clamp);

}

void graphContainer::updateColourTableEdges(  )
{//this update colour table now updates the GLSL colour table not the form
    cout<<"update coour links table "<<endl;
    edges_ctable = graph_form->getLinksColourTable();

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
    switch (blendFunc)
    {

    case 0:
        glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
        break;
    case 1:
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
                break;
    case 2:
                glBlendFunc(GL_ONE_MINUS_DST_ALPHA , GL_ONE);
                break;
    case 3:
                glBlendFunc(GL_SRC_ALPHA , GL_DST_ALPHA);
                break;
    case 4:
        glBlendFunc(GL_ONE_MINUS_DST_ALPHA , GL_DST_ALPHA);
                break;
    default:
        break;
    }


    //    writeGIFTI(surf_graph_nodes_,"/Users/brian/git_repos/briview/nodes_render.gii");
        glUseProgram(glsl_programs[1]);
//        updateColourTableNode();
        setColourTableEdge();
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
