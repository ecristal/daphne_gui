#ifndef DIALOGCALIBRATION_H
#define DIALOGCALIBRATION_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class dialogCalibration;
}

class dialogCalibration : public QDialog
{
    Q_OBJECT

public:
    explicit dialogCalibration(QWidget *parent = nullptr);
    ~dialogCalibration();

public slots:
    void PlotSineWave();
private slots:
    void processROOTEvents();
private:
    Ui::dialogCalibration *ui;

    QTimer rootProcessTimer;
};

#endif // DIALOGCALIBRATION_H
