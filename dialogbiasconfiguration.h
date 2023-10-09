#ifndef DIALOGBIASCONFIGURATION_H
#define DIALOGBIASCONFIGURATION_H

#include <QDialog>
#include <QVector>

namespace Ui {
class DialogBiasConfiguration;
}

class DialogBiasConfiguration : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBiasConfiguration(QWidget *parent = nullptr);
    ~DialogBiasConfiguration();

    double getBiasControlValue();
    void setBiasControlValue(double biasControlValue);
    void setBiasOffsetValues(QVector<double> biasOFFSETvalues);
    QVector<double> getBiasOffsetValues();
private:
    Ui::DialogBiasConfiguration *ui;
};

#endif // DIALOGBIASCONFIGURATION_H
