#ifndef GRAPHCMANIPULATOR_H
#define GRAPHCMANIPULATOR_H

#include <QDockWidget>

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
signals:
    void sig_copy_to_surfaces();

private:
    Ui::graphManipulator_form *ui;
};

#endif // GRAPHCMANIPULATOR_H
