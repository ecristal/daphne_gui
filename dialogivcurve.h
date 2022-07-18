#ifndef DIALOGIVCURVE_H
#define DIALOGIVCURVE_H

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QRandomGenerator64>
#include <algorithm>
#include <QTextStream>
#include <QString>
#include <qmath.h>

#include "mainwindow.h"

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
  void pushButtonPausePressed();
  void pushButtonStopPressed();
private:
  Ui::DialogIVcurve *ui;
  void initializeWindow();

  int initialPoistion = 0;

  bool pausePressedFLAG = false;


  QVector<double> xValues;
  QVector<double> yValues;

  QRandomGenerator64 rg;
};

#endif // DIALOGIVCURVE_H
