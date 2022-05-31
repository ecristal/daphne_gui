#ifndef DIALOGACQUISITIONCONFIGURATION_H
#define DIALOGACQUISITIONCONFIGURATION_H

#include <QDialog>
#include <QCheckBox>
#include <QDebug>

namespace Ui {
class DialogAcquisitionConfiguration;
}

class DialogAcquisitionConfiguration : public QDialog
{
  Q_OBJECT

public:
  explicit DialogAcquisitionConfiguration(QWidget *parent = 0);
  ~DialogAcquisitionConfiguration();

  QVector<bool> getCheckBoxStates();
  void setCheckBoxStates(const QVector<bool> &states);
  int getRecordLength();
  void setRecordLength(const int &record_length);
private slots:
  void pushButtonSelectAllPressed();
  void pushButtonUnselectAllPressed();
private:
  Ui::DialogAcquisitionConfiguration *ui;

  QVector<QCheckBox*> ptr_ch_enabled;
  void configureCheckboxPointers();
};

#endif // DIALOGACQUISITIONCONFIGURATION_H
