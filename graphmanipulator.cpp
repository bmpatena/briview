#include "graphmanipulator.h"
#include "ui_graphmanipulator.h"
#include <iostream>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
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


    //connect(ui->but_AppendSurfData, SIGNAL(released()),this,SIGNAL(sig_appendSurfaceData()));
    connect(ui->but_viewScData, SIGNAL(released()),this,SIGNAL(sig_changedScalarData()) );


    connect(ui->but_cp2surf,SIGNAL(pressed()),this,SIGNAL(sig_copy_to_surfaces()));
    connect(ui->but_renderGraph,SIGNAL(released()),this, SIGNAL(sig_updateGraph()));
    connect(ui->sbox_rad,SIGNAL(valueChanged(double)),this, SIGNAL(sig_updateRadius(double)) );
//    connect(ui->sbox_rad,SIGNAL(editingFinished()),this, SLOT(updateRadius()) );
    connect(ui->hrzSliders_rad,SIGNAL(sliderMoved(int)),this, SLOT(updateRadius(int)) );

    connect(ui->sbox_rad_link,SIGNAL(valueChanged(double)),this, SIGNAL(sig_updateLinkRadius(double)) );
    connect(ui->hrzSliders_rad_link,SIGNAL(sliderMoved(int)),this, SLOT(updateLinkRadius(int)) );

connect(ui->check_wEdge,SIGNAL(	stateChanged(int) ) ,this, SIGNAL(sig_wEdgeStateChanged(int)));

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

    connect(ui->combo_cmap_node, SIGNAL(currentIndexChanged (int) ),this,SIGNAL(sig_changedColourTableNode(int)));
    connect(ui->combo_cmap_edge, SIGNAL(currentIndexChanged (int) ),this,SIGNAL(sig_changedColourTableEdge(int)));

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


    //scalars interface
    //add Scalars
    connect(ui->addNodeScalars,SIGNAL(pressed()),this,SLOT(addNodeScalars()));
    connect(ui->addEdgeScalars,SIGNAL(pressed()),this,SLOT(addEdgeScalars()));


connect(ui->but_node_setCRange,SIGNAL(pressed()),this,SLOT(setNodeCmapRange()) );
connect(ui->but_edge_setCRange,SIGNAL(pressed()),this,SLOT(setEdgeCmapRange()) );

connect(ui->combo_opacity, SIGNAL(activated (int) ),this,SIGNAL(sig_changedOpacityMode(int)));
connect(ui->combo_blend, SIGNAL(activated (int) ),this,SIGNAL(sig_changedBlendFunc(int)));



}

graphManipulator::~graphManipulator()
{
    delete ui;
}

void graphManipulator::setNodeCmapRange()
{
    cout<<"current row "<<ui->graphNodes_scalars_list->currentRow()<<endl;
    if (ui->graphNodes_scalars_list->currentRow() >= 0 )
    emit sig_setColourTableNodeSc(ui->graphNodes_scalars_list->currentRow());
}
void graphManipulator::setEdgeCmapRange()
{
    cout<<"current row "<<ui->graphNodes_scalars_list->currentRow()<<endl;
    if (ui->graphNodes_scalars_list->currentRow() >= 0 )
    emit sig_setColourTableEdgeSc(ui->graphEdge_scalars_list->currentRow());
}
void graphManipulator::setEdgeCMAPRange(const float & min, const float & max)
{
    cout<<"setNodeCMAPRange"<<endl;
    float step = (max-min)/5; //divide by (6-1)
    //there are six values
    QString snum;
    snum.setNum(min);
    ui->ctab_sc1_links->setText(snum);
    snum.setNum(min + step);
    ui->ctab_sc2_links->setText(snum);
    snum.setNum(min + 2*step);
    ui->ctab_sc3_links->setText(snum);
    snum.setNum(min + 3*step);
    ui->ctab_sc4_links->setText(snum);
    snum.setNum(min + 4*step);
    ui->ctab_sc5_links->setText(snum);
    snum.setNum(max);
    ui->ctab_sc6_links->setText(snum);


}

void graphManipulator::setNodeCMAPRange(const float & min, const float & max)
{
    cout<<"setNodeCMAPRange"<<endl;
    float step = (max-min)/5; //divide by (6-1)
    //there are six values
    QString snum;
    snum.setNum(min);
    ui->ctab_sc1_node->setText(snum);
    snum.setNum(min + step);
    ui->ctab_sc2_node->setText(snum);
    snum.setNum(min + 2*step);
    ui->ctab_sc3_node->setText(snum);
    snum.setNum(min + 3*step);
    ui->ctab_sc4_node->setText(snum);
    snum.setNum(min + 4*step);
    ui->ctab_sc5_node->setText(snum);
    snum.setNum(max);
    ui->ctab_sc6_node->setText(snum);


}

void graphManipulator::addNodeScalarsToList( const string & name)
{
        ui->graphNodes_scalars_list->addItem(QString(name.c_str()));
}

void graphManipulator::addNodeScalars( )
{
    QString filename = QFileDialog::getOpenFileName(this,"Load Node Scalars From File (*.csv *.txt)","./","Scalar File (*.csv *.txt)");

    if (!filename.isNull())
    {
        emit sig_addScalars(filename);
        cout<<"loadEdgeScalaras"<<endl;
//        ifstream fin(filename.toStdString().c_str());
//        if (fin.is_open())
//        {
//            string name, line;//right now assumes a single column
//            getline(fin,name)
//            while (getline(fin,line))
//            {
//                stringstream ss;
//                ss<<line;
//                float val;
//                ss>>val;
//            }
//            fin.close();
//        }
    }
    ui->addNodeScalars->setChecked(false);
}
void graphManipulator::addEdgeScalars( )
{
    QString filename = QFileDialog::getOpenFileName(this,"Load Edge Scalars From File (*.csv,*.txt)","./","*.csv,.txt");

    if (!filename.isNull())
    {
        cout<<"loadEdgeScalaras"<<endl;
    }
    ui->addEdgeScalars->setChecked(false);

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

int graphManipulator::getCurrentNodeScalarIndex(){

    return  ui->graphNodes_scalars_list->currentRow();
}

void graphManipulator::setCurrentNodeScalarIndex( int row ){

     ui->graphNodes_scalars_list->setCurrentRow(row);
}

void graphManipulator::setNodesScalarsName(const std::string name, const int & index)
{
    if (( ui->graphNodes_scalars_list->count() == 0 ) && ( index == 0 ))
    {
        ui->graphNodes_scalars_list->addItem(QString(name.c_str()));
        ui->graphNodes_scalars_list->setCurrentRow(index);
    }else if (ui->graphNodes_scalars_list->count() > index )
    {
        ui->graphNodes_scalars_list->item(index)->setText(QString(name.c_str()));
        ui->graphNodes_scalars_list->setCurrentRow(index);
    }

}

void graphManipulator::setEdgeScalarsName(const std::string name, const int & index)
{

    if (ui->graphEdge_scalars_list->count() > index )
    {
        ui->graphEdge_scalars_list->item(index)->setText(QString(name.c_str()));
        ui->graphNodes_scalars_list->setCurrentRow(index);
    }else if (index ==0)
    {
        ui->graphEdge_scalars_list->addItem(QString(name.c_str()));
        ui->graphNodes_scalars_list->setCurrentRow(index);

    }
}
//void graphManipulator::addPresetSetMaterial(const string & name)
//{
//ui->combo_preset_materials->addItem(QString(name.c_str()));

//}
void graphManipulator::setColourTableNodeIndex( const int & index)
{
    ui->combo_cmap_node->setCurrentIndex(index);
}
void graphManipulator::setColourTableEdgeIndex( const int & index)
{
    ui->combo_cmap_node->setCurrentIndex(index);
}

void graphManipulator::setColourTableNodeNoSc( const fslsurface_name::colour_table & ctable ){
    QString temp;
    temp.setNum( ctable.r_lut[0],'g',4);
    ui->ctab_r1_node->setText(temp);
    temp.setNum( ctable.r_lut[1],'g',4);
    ui->ctab_r2_node->setText(temp);
    temp.setNum( ctable.r_lut[2],'g',4);
    ui->ctab_r3_node->setText(temp);
    temp.setNum( ctable.r_lut[3],'g',4);
    ui->ctab_r4_node->setText(temp);
    temp.setNum( ctable.r_lut[4],'g',4);
    ui->ctab_r5_node->setText(temp);
    temp.setNum( ctable.r_lut[5],'g',4);
    ui->ctab_r6_node->setText(temp);

    temp.setNum( ctable.g_lut[0],'g',4);
    ui->ctab_g1_node->setText(temp);
    temp.setNum( ctable.g_lut[1],'g',4);
    ui->ctab_g2_node->setText(temp);
    temp.setNum( ctable.g_lut[2],'g',4);
    ui->ctab_g3_node->setText(temp);
    temp.setNum( ctable.g_lut[3],'g',4);
    ui->ctab_g4_node->setText(temp);
    temp.setNum( ctable.g_lut[4],'g',4);
    ui->ctab_g5_node->setText(temp);
    temp.setNum( ctable.g_lut[5],'g',4);
    ui->ctab_g6_node->setText(temp);

    temp.setNum( ctable.b_lut[0],'g',4);
    ui->ctab_b1_node->setText(temp);
    temp.setNum( ctable.b_lut[1],'g',4);
    ui->ctab_b2_node->setText(temp);
    temp.setNum( ctable.b_lut[2],'g',4);
    ui->ctab_b3_node->setText(temp);
    temp.setNum( ctable.b_lut[3],'g',4);
    ui->ctab_b4_node->setText(temp);
    temp.setNum( ctable.b_lut[4],'g',4);
    ui->ctab_b5_node->setText(temp);
    temp.setNum( ctable.b_lut[5],'g',4);
    ui->ctab_b6_node->setText(temp);

//cout<<"alpha "<< ctable.a_lut[0]<<endl;
    temp.setNum( ctable.a_lut[0],'g',4);
    ui->ctab_a1_node->setText(temp);
    temp.setNum( ctable.a_lut[1],'g',4);
    ui->ctab_a2_node->setText(temp);
    temp.setNum( ctable.a_lut[2],'g',4);
    ui->ctab_a3_node->setText(temp);
    temp.setNum( ctable.a_lut[3],'g',4);
    ui->ctab_a4_node->setText(temp);
    temp.setNum( ctable.a_lut[4],'g',4);
    ui->ctab_a5_node->setText(temp);
    temp.setNum( ctable.a_lut[5],'g',4);
    ui->ctab_a6_node->setText(temp);

//cout<<"set alpha 5 "<<ctable.sc_lut[5]<<endl;
    temp.setNum( ctable.low_clamp[0],'g',4);
    ui->ctab_r_lclamp_node->setText(temp);
    temp.setNum( ctable.low_clamp[1],'g',4);
    ui->ctab_g_lclamp_node->setText(temp);
    temp.setNum( ctable.low_clamp[2],'g',4);
    ui->ctab_b_lclamp_node->setText(temp);
    temp.setNum( ctable.low_clamp[3],'g',4);
    ui->ctab_a_lclamp_node->setText(temp);
}

void graphManipulator::setColourTableEdgeNoSc( const fslsurface_name::colour_table & ctable ){
    QString temp;
    temp.setNum( ctable.r_lut[0],'g',4);
    ui->ctab_r1_links->setText(temp);
    temp.setNum( ctable.r_lut[1],'g',4);
    ui->ctab_r2_links->setText(temp);
    temp.setNum( ctable.r_lut[2],'g',4);
    ui->ctab_r3_links->setText(temp);
    temp.setNum( ctable.r_lut[3],'g',4);
    ui->ctab_r4_links->setText(temp);
    temp.setNum( ctable.r_lut[4],'g',4);
    ui->ctab_r5_links->setText(temp);
    temp.setNum( ctable.r_lut[5],'g',4);
    ui->ctab_r6_links->setText(temp);

    temp.setNum( ctable.g_lut[0],'g',4);
    ui->ctab_g1_links->setText(temp);
    temp.setNum( ctable.g_lut[1],'g',4);
    ui->ctab_g2_links->setText(temp);
    temp.setNum( ctable.g_lut[2],'g',4);
    ui->ctab_g3_links->setText(temp);
    temp.setNum( ctable.g_lut[3],'g',4);
    ui->ctab_g4_links->setText(temp);
    temp.setNum( ctable.g_lut[4],'g',4);
    ui->ctab_g5_links->setText(temp);
    temp.setNum( ctable.g_lut[5],'g',4);
    ui->ctab_g6_links->setText(temp);

    temp.setNum( ctable.b_lut[0],'g',4);
    ui->ctab_b1_links->setText(temp);
    temp.setNum( ctable.b_lut[1],'g',4);
    ui->ctab_b2_links->setText(temp);
    temp.setNum( ctable.b_lut[2],'g',4);
    ui->ctab_b3_links->setText(temp);
    temp.setNum( ctable.b_lut[3],'g',4);
    ui->ctab_b4_links->setText(temp);
    temp.setNum( ctable.b_lut[4],'g',4);
    ui->ctab_b5_links->setText(temp);
    temp.setNum( ctable.b_lut[5],'g',4);
    ui->ctab_b6_links->setText(temp);

//cout<<"alpha "<< ctable.a_lut[0]<<endl;
    temp.setNum( ctable.a_lut[0],'g',4);
    ui->ctab_a1_links->setText(temp);
    temp.setNum( ctable.a_lut[1],'g',4);
    ui->ctab_a2_links->setText(temp);
    temp.setNum( ctable.a_lut[2],'g',4);
    ui->ctab_a3_links->setText(temp);
    temp.setNum( ctable.a_lut[3],'g',4);
    ui->ctab_a4_links->setText(temp);
    temp.setNum( ctable.a_lut[4],'g',4);
    ui->ctab_a5_links->setText(temp);
    temp.setNum( ctable.a_lut[5],'g',4);
    ui->ctab_a6_links->setText(temp);

//cout<<"set alpha 5 "<<ctable.sc_lut[5]<<endl;
    temp.setNum( ctable.low_clamp[0],'g',4);
    ui->ctab_r_lclamp_links->setText(temp);
    temp.setNum( ctable.low_clamp[1],'g',4);
    ui->ctab_g_lclamp_links->setText(temp);
    temp.setNum( ctable.low_clamp[2],'g',4);
    ui->ctab_b_lclamp_links->setText(temp);
    temp.setNum( ctable.low_clamp[3],'g',4);
    ui->ctab_a_lclamp_links->setText(temp);
}
void graphManipulator::addColourTable(const string & filename )
{
    ui->combo_cmap_node->addItem(QString(filename.c_str()));
    ui->combo_cmap_edge->addItem(QString(filename.c_str()));

    //ui->image_list->setCurrentRow(ui->image_list->count()-1);
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
