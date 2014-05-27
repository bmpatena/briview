#include "marchingcubes_form.h"
#include "ui_marchingcubes_form.h"
#include <string>
namespace briview{

MarchingCubes::MarchingCubes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarchingCubes_Form)
{
    ui->setupUi(this);

    connect(ui->but_apply,SIGNAL(pressed()),this, SIGNAL(sig_apply()) );
}

MarchingCubes::~MarchingCubes()
{
    delete ui;
}
float MarchingCubes::getThreshold()
{
    return ui->thresh->text().toFloat();
}

float MarchingCubes::getLabel()
{
    return ui->label->text().toFloat();

}
std::string MarchingCubes::getLabelAsString()
{
    return ui->label->text().toStdString();

}
bool MarchingCubes::getDoAllLabels()
{
    return ui->radio_doAllLabels->isChecked();
}

int MarchingCubes::getThresholdMode()
{
    return ui->thresh_mode->currentIndex();
}

void MarchingCubes::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
}
