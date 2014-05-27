#ifndef COLOURTABLEWDIGET_H
#define COLOURTABLEWDIGET_H

#include <QWidget>

namespace Ui {
    class ColourTableWdiget;
}

class ColourTableWdiget : public QWidget
{
    Q_OBJECT

public:
    explicit ColourTableWdiget(QWidget *parent = 0);
    ~ColourTableWdiget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ColourTableWdiget *ui;
};

#endif // COLOURTABLEWDIGET_H
