#ifndef DIALOGIVCURVE_H
#define DIALOGIVCURVE_H

#include <QDialog>
#include <QFileDialog>
#include <QFile>

namespace Ui {
class DialogIVcurve;
}

class DialogIVcurve : public QDialog
{
  Q_OBJECT

public:
  explicit DialogIVcurve(QWidget *parent = 0);
  ~DialogIVcurve();

private slots:
  void spinBoxBiasLowValueChanged();
  void spinBoxBiasUpperValueChanged();
  void pushButtonSaveDataPressed();
  void pushButtonStartPressed();
private:
  Ui::DialogIVcurve *ui;
  void initializeWindow();

  QVector<double> xValues;
  QVector<double> yValues;
};

#endif // DIALOGIVCURVE_H
