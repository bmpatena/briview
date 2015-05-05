#ifndef GRAPHCONTAINER_H
#define GRAPHCONTAINER_H

#include <QObject>

#include <vector>
#include <string>
#include "graphmanipulator.h"
#include <briview_structs.h>

#include "fslsurface/fslsurface_structs.h"
#include "fslsurface/fslsurfacegl.h"
#include "fslsurface/fslsurface.h"
#include "fslsurface/fslsurfaceio.h"

namespace briview{

class graphContainer : public QObject
{
    Q_OBJECT
public:
    explicit graphContainer(QObject *parent = 0);
    ~graphContainer();

    void readFile( const std::string & filename );
    graphManipulator* getGraphManipulatorWidgets() {  return graph_form; }
    void generateNodes( );
    void generateLinks( );

    void render();
    bool doRender() { return graph_form->doRender();  }
    void setGLSLPrograms(const std::vector<GLuint> & progs);

    fslsurface_name::fslSurface<float,unsigned int> getGraphNodesAsSurface() { return surf_graph_nodes_;}
    fslsurface_name::fslSurface<float,unsigned int> getGraphLinksAsSurface() { return surf_graph_links_;}
    void setColourTableUniformLocations(const GLint & loc_r_lut_in,const GLint & loc_g_lut_in,const GLint & loc_b_lut_in, const GLint & loc_a_lut_in,const GLint & loc_sc_lut_in,const GLint & loc_r_lut_last_in,const GLint & loc_g_lut_last_in,const GLint & loc_b_lut_last_in,const GLint & loc_a_lut_last_in, const GLint & loc_sc_lut_last_in, const GLint & loc_low_clamp_in);
//    void importPresetMaterials( const string & filename);

signals:
void sig_copy_to_surfaces();
void sig_updateGL();

public slots:
    void setVisible( bool visible );
    void setRadius( );
    void setRadius(  double  r_in );
    void setLinkRadius( );
    void setLinkRadius(  double  r_in );
    void updateColourTableNode();
//    void updateColourTableNodeSc();
    void updateColourTableEdges();
//    void updateColourTableEdgesSc();
    void setNodeCmapRange( int curIndex );
    void setEdgeCmapRange( int curIndex );

    void loadColourTables(const std::string & filename );
    void changeColourTableNode(int index);
    void changeColourTableEdge(int index);
    void addScalars( QString filename );


private:

    void setColourTableNode();
    void setColourTableEdge();


    graphManipulator* graph_form;
    fslsurface_name::fslSurface<float,unsigned int> surf_graph_nodes_,surf_graph_links_;
 std::vector< GLuint > glsl_programs;
    float radius_, radius_link_;
    unsigned int Nnodes_;
    std::vector< conn > v_conn_;
    std::vector< float3 > v_cog_;
    std::vector< std::string > v_names_;
GLuint* vbos_nodes_;
    GLint vertexLoc, normalLoc, scalarLoc;
    fslsurface_name::colour_table nodes_ctable, edges_ctable;
    std::vector< fslsurface_name::colour_table > v_ctables;
    GLint loc_r_lut,loc_g_lut,loc_b_lut,loc_a_lut,loc_sc_lut;
    GLint loc_r_lut_last,loc_g_lut_last,loc_b_lut_last,loc_a_lut_last,loc_sc_lut_last;
    GLint loc_low_clamp;

    std::vector< std::string > cmap_names;


};

};
#endif // GRAPHCONTAINER_H
