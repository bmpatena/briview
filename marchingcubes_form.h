#ifndef MARCHINGCUBES_FORM_H
#define MARCHINGCUBES_FORM_H

#include <QWidget>

namespace Ui {
    class MarchingCubes_Form;
}
namespace briview{

class MarchingCubes : public QWidget
{
    Q_OBJECT

public:
    explicit MarchingCubes(QWidget *parent = 0);
    ~MarchingCubes();
    void applyMarchingCubes();
    float getThreshold();
    float getLabel();
    std::string getLabelAsString();

    int getThresholdMode();
    bool getDoAllLabels();
protected:
    void changeEvent(QEvent *e);
signals:
    void sig_apply();
private:
    Ui::MarchingCubes_Form *ui;
};

}
#endif // MARCHINGCUBES_FORM_H
