#include "glsl_editor.h"
#include "ui_glsl_editor.h"


namespace briview{


glsl_editor::glsl_editor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::glsl_editor)
{
    ui->setupUi(this);
    connect(ui->cbox_glsl_prog,SIGNAL(currentIndexChanged(int)), this, SIGNAL(sig_setProgram(int)));
}

glsl_editor::~glsl_editor()
{
    delete ui;
}



void glsl_editor::setProgram( const int & prog )
{
ui->lcd_prog->display(prog);
}


void glsl_editor::addShader( const QString & shader )
{
ui->cbox_glsl_prog->addItem(shader);
}
}
