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
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>
#include <Eigen/QR>
#include <fstream>
#include <iostream>
#include <math.h>

#include "mainwindow.h"
#include "biasvoltageexception.h"

namespace Ui {
class DialogIVcurve;
}

class DialogIVcurve : public QDialog
{
  Q_OBJECT

public:
  explicit DialogIVcurve(QWidget *parent = 0);
  ~DialogIVcurve();

    void setWorkingFolder(QString workingFolder);
private slots:
  void spinBoxBiasLowValueChanged();
  void spinBoxBiasUpperValueChanged();
  void pushButtonSaveDataPressed();
  void pushButtonStartPressed();
  void pushButtonPausePressed();
  void pushButtonStopPressed();
  void openIVCurveFile();
  void plotSignalsAndCalculateBreakdown();
private:
  Ui::DialogIVcurve *ui;
  void initializeWindow();

  int initialPoistion = 0;

  bool pausePressedFLAG = false;
  bool runningFLAG = false;

  QVector<double> xValues;
  QVector<double> yValues;

  QString IVcurveFileName;
  QString multiple_waveforms_folder_address;

  QRandomGenerator64 rg;

  void closeEvent(QCloseEvent *event);


  Eigen::MatrixXd F;
  Eigen::MatrixXd G;
  Eigen::MatrixXd H;
  Eigen::MatrixXd D;

  QVector<double> calculateFilteredSignal(QVector<double> &inputData);
};

#endif // DIALOGIVCURVE_H
