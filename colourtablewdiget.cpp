#include "colourtablewdiget.h"
#include "ui_colourtablewdiget.h"

ColourTableWdiget::ColourTableWdiget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColourTableWdiget)
{
    ui->setupUi(this);
}

ColourTableWdiget::~ColourTableWdiget()
{
    delete ui;
}

void ColourTableWdiget::changeEvent(QEvent *e)
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
