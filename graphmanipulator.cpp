#include "graphmanipulator.h"
#include "ui_graphmanipulator.h"
#include <iostream>
#include <QVBoxLayout>
#include <QPushButton>

using namespace std;
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
