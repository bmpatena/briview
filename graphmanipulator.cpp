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

    connect(ui->but_cp2surf,SIGNAL(pressed()),this,SIGNAL());
}

graphManipulator::~graphManipulator()
{
    delete ui;
}
