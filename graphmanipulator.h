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

public slots:
    //    void updateRadius();
    void updateRadius( int slide_r );
    void updateLinkRadius( int slide_r );

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
private:
    Ui::graphManipulator_form *ui;
};

#endif // GRAPHCMANIPULATOR_H
