#include "graphmanipulator.h"
#include "ui_graphmanipulator.h"
#include <iostream>
#include <QVBoxLayout>
#include <QPushButton>

using namespace std;
using namespace fslsurface_name;
graphManipulator::graphManipulator(QWidget *parent) :
          QDockWidget(parent),
          ui(new Ui::graphManipulator_form)
{

    ui->setupUi(this);
    ui->sbox_rad->setValue(5);
    ui->sbox_rad_link->setValue(5);

    ui->sbox_rad->setSingleStep(0.1);
    ui->hrzSliders_rad->setSingleStep(1);
    ui->hrzSliders_rad->setRange(1,100);
    ui->sbox_rad_link->setSingleStep(0.1);
    ui->hrzSliders_rad_link->setSingleStep(1);
    ui->hrzSliders_rad_link->setRange(1,100);
    connect(ui->but_cp2surf,SIGNAL(pressed()),this,SIGNAL(sig_copy_to_surfaces()));
    connect(ui->but_renderGraph,SIGNAL(released()),this, SIGNAL(sig_updateGraph()));
    connect(ui->sbox_rad,SIGNAL(valueChanged(double)),this, SIGNAL(sig_updateRadius(double)) );
//    connect(ui->sbox_rad,SIGNAL(editingFinished()),this, SLOT(updateRadius()) );
    connect(ui->hrzSliders_rad,SIGNAL(sliderMoved(int)),this, SLOT(updateRadius(int)) );

    connect(ui->sbox_rad_link,SIGNAL(valueChanged(double)),this, SIGNAL(sig_updateLinkRadius(double)) );
    connect(ui->hrzSliders_rad_link,SIGNAL(sliderMoved(int)),this, SLOT(updateLinkRadius(int)) );


    //colour table

    connect(ui->ctab_sc1_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNodeSc()));
    connect(ui->ctab_sc2_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNodeSc()));
    connect(ui->ctab_sc3_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNodeSc()));
    connect(ui->ctab_sc4_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNodeSc()));
    connect(ui->ctab_sc5_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNodeSc()));
    connect(ui->ctab_sc6_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNodeSc()));

    connect(ui->ctab_r1_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_r2_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_r3_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_r4_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_r5_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_r6_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));

    connect(ui->ctab_g1_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_g2_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_g3_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_g4_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_g5_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_g6_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));

    connect(ui->ctab_b1_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_b2_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_b3_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_b4_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_b5_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_b6_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));

    connect(ui->ctab_a1_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_a2_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_a3_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_a4_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_a5_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_a6_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));

    connect(ui->ctab_r_lclamp_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_g_lclamp_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_b_lclamp_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));
    connect(ui->ctab_a_lclamp_node, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableNode()));

    //colour table

    connect(ui->ctab_sc1_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinksSc()));
    connect(ui->ctab_sc2_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinksSc()));
    connect(ui->ctab_sc3_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinksSc()));
    connect(ui->ctab_sc4_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinksSc()));
    connect(ui->ctab_sc5_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinksSc()));
    connect(ui->ctab_sc6_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinksSc()));

    connect(ui->ctab_r1_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_r2_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_r3_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_r4_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_r5_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_r6_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));

    connect(ui->ctab_g1_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_g2_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_g3_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_g4_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_g5_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_g6_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));

    connect(ui->ctab_b1_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_b2_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_b3_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_b4_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_b5_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_b6_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));

    connect(ui->ctab_a1_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_a2_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_a3_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_a4_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_a5_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_a6_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));

    connect(ui->ctab_r_lclamp_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_g_lclamp_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_b_lclamp_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));
    connect(ui->ctab_a_lclamp_links, SIGNAL(textEdited(QString)),this,SIGNAL(sig_changedColourTableLinks()));






}

graphManipulator::~graphManipulator()
{
    delete ui;
}
void graphManipulator::updateRadius( int slide_r )
{
    cout<<"update radius "<<ui->hrzSliders_rad->value()<<endl;
    ui->sbox_rad->setValue(static_cast<float>(slide_r)/10.0);
    emit sig_updateRadius(static_cast<float>(slide_r)/10.0);
}

void graphManipulator::updateLinkRadius( int slide_r )
{
    cout<<"update  link radius "<<ui->hrzSliders_rad_link->value()<<endl;
    ui->sbox_rad_link->setValue(static_cast<float>(slide_r)/10.0);
    emit sig_updateLinkRadius(static_cast<float>(slide_r)/10.0);
}

bool graphManipulator::doRender()
{
    return ui->but_renderGraph->isChecked();
}
float graphManipulator::getRadius()
{
    return ui->sbox_rad->value();
}

float graphManipulator::getLinkRadius()
{
    return ui->sbox_rad_link->value();
}


colour_table graphManipulator::getLinksColourTable()
{
    return colour_table( ui->ctab_r1_links->text().toFloat(), ui->ctab_r2_links->text().toFloat(), ui->ctab_r3_links->text().toFloat(), ui->ctab_r4_links->text().toFloat(), ui->ctab_r5_links->text().toFloat(), ui->ctab_r6_links->text().toFloat(),\
                         ui->ctab_g1_links->text().toFloat(), ui->ctab_g2_links->text().toFloat(), ui->ctab_g3_links->text().toFloat(), ui->ctab_g4_links->text().toFloat(), ui->ctab_g5_links->text().toFloat(), ui->ctab_g6_links->text().toFloat(),\
                         ui->ctab_b1_links->text().toFloat(), ui->ctab_b2_links->text().toFloat(), ui->ctab_b3_links->text().toFloat(), ui->ctab_b4_links->text().toFloat(), ui->ctab_b5_links->text().toFloat(), ui->ctab_b6_links->text().toFloat(),\
                         ui->ctab_a1_links->text().toFloat(), ui->ctab_a2_links->text().toFloat(), ui->ctab_a3_links->text().toFloat(), ui->ctab_a4_links->text().toFloat(), ui->ctab_a5_links->text().toFloat(),  ui->ctab_a6_links->text().toFloat(),\
                         ui->ctab_sc1_links->text().toFloat(), ui->ctab_sc2_links->text().toFloat(), ui->ctab_sc3_links->text().toFloat(), ui->ctab_sc4_links->text().toFloat(), ui->ctab_sc5_links->text().toFloat(),ui->ctab_sc6_links->text().toFloat(), \
                         ui->ctab_r_lclamp_links->text().toFloat(),ui->ctab_g_lclamp_links->text().toFloat(),ui->ctab_b_lclamp_links->text().toFloat(),ui->ctab_a_lclamp_links->text().toFloat());
}


colour_table graphManipulator::getNodeColourTable()
{
    return colour_table( ui->ctab_r1_node->text().toFloat(), ui->ctab_r2_node->text().toFloat(), ui->ctab_r3_node->text().toFloat(), ui->ctab_r4_node->text().toFloat(), ui->ctab_r5_node->text().toFloat(), ui->ctab_r6_node->text().toFloat(),\
                         ui->ctab_g1_node->text().toFloat(), ui->ctab_g2_node->text().toFloat(), ui->ctab_g3_node->text().toFloat(), ui->ctab_g4_node->text().toFloat(), ui->ctab_g5_node->text().toFloat(), ui->ctab_g6_node->text().toFloat(),\
                         ui->ctab_b1_node->text().toFloat(), ui->ctab_b2_node->text().toFloat(), ui->ctab_b3_node->text().toFloat(), ui->ctab_b4_node->text().toFloat(), ui->ctab_b5_node->text().toFloat(), ui->ctab_b6_node->text().toFloat(),\
                         ui->ctab_a1_node->text().toFloat(), ui->ctab_a2_node->text().toFloat(), ui->ctab_a3_node->text().toFloat(), ui->ctab_a4_node->text().toFloat(), ui->ctab_a5_node->text().toFloat(),  ui->ctab_a6_node->text().toFloat(),\
                         ui->ctab_sc1_node->text().toFloat(), ui->ctab_sc2_node->text().toFloat(), ui->ctab_sc3_node->text().toFloat(), ui->ctab_sc4_node->text().toFloat(), ui->ctab_sc5_node->text().toFloat(),ui->ctab_sc6_node->text().toFloat(), \
                         ui->ctab_r_lclamp_node->text().toFloat(),ui->ctab_g_lclamp_node->text().toFloat(),ui->ctab_b_lclamp_node->text().toFloat(),ui->ctab_a_lclamp_node->text().toFloat());
}
