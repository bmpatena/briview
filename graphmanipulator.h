#ifndef GRAPHCMANIPULATOR_H
#define GRAPHCMANIPULATOR_H

#include <QDockWidget>
#include "fslsurface/fslsurface_structs.h"

namespace Ui {
class graphManipulator_form;
}


class graphManipulator : public QDockWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(graphManipulator)

public:
    explicit graphManipulator(QWidget *parent = 0);
    ~graphManipulator();
    bool doRender();
    float getRadius();
    float getLinkRadius();
    fslsurface_name::colour_table getNodeColourTable();
    fslsurface_name::colour_table getLinksColourTable();
    void setNodesScalarsName(const std::string name, const int & index);
    void setEdgeScalarsName(const std::string name, const int & index);
    int getCurrentNodeScalarIndex();
    void setNodeCMAPRange(const float & min, const float & max);
    void setEdgeCMAPRange(const float & min, const float & max);

//    void addPresetSetMaterial(const string & name);
    void setColourTableNodeIndex( const int & index);
    void setColourTableEdgeIndex( const int & index);

    void addColourTable(const std::string & filename );
    void setColourTableNodeNoSc( const fslsurface_name::colour_table & ctable );
    void setColourTableEdgeNoSc( const fslsurface_name::colour_table & ctable );
    void addNodeScalarsToList( const std::string & name);



public slots:
    //    void updateRadius();
    void updateRadius( int slide_r );
    void updateLinkRadius( int slide_r );
    void addNodeScalars( );
    void addEdgeScalars( );
    void setNodeCmapRange();
    void setEdgeCmapRange();

signals:
    void sig_copy_to_surfaces();
    void sig_updateGraph();
    void sig_updateRadius(double d);
    void sig_updateRadius();
    void sig_updateLinkRadius(double d);
    void sig_updateLinkRadius();
    void sig_changedColourTableNode();
    void sig_changedColourTableNodeSc();
    void sig_changedColourTableLinks();
    void sig_changedColourTableLinksSc();
    void sig_setColourTableNodeSc(int index );
    void sig_setColourTableEdgeSc(int index);
    void sig_changedColourTableNode(int index);
    void sig_changedColourTableEdge(int index);
    void sig_addScalars( QString filename );
    void sig_changedBlendFunc(int);
    void sig_changedOpacityMode(int);
private:
    Ui::graphManipulator_form *ui;
};

#endif // GRAPHCMANIPULATOR_H
