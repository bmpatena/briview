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

    void setRadius( const float & r_in ) { radius_ = r_in; }
    void readFile( const std::string & filename );
    graphManipulator* getGraphManipulatorWidgets() {  return graph_form; }
    void generateNodes( );
    void render();

signals:
void sig_copy_to_surfaces();
public slots:
    void setVisible( bool visible );

private:
    graphManipulator* graph_form;
    fslsurface_name::fslSurface<float,unsigned int> surf_graph_nodes_;

    float radius_;
    unsigned int Nnodes_;
    std::vector< float3 > v_cog_;
    std::vector< std::string > v_names_;
GLuint* vbos_nodes_;
    GLint vertexLoc, normalLoc, scalarLoc;


};

};
#endif // GRAPHCONTAINER_H
