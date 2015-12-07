#ifndef GLSL_EDITOR_H
#define GLSL_EDITOR_H

#include <QDialog>

namespace Ui {
class glsl_editor;
}


namespace briview{

class glsl_editor : public QDialog
{
    Q_OBJECT

public:
    explicit glsl_editor(QWidget *parent = 0);
    ~glsl_editor();
    void addShader( const QString & shader );
    void setProgram( const int  & program );
signals:
    void sig_setProgram(int);

private:
    Ui::glsl_editor *ui;
};
}

#endif // GLSL_EDITOR_H
