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
  bool getCheckBoxSaveTextState();
  bool getCheckBoxSaveBinaryState();
  void setCheckBoxSaveTextState(const bool &state);
  void setCheckBoxSaveBinaryState(const bool &state);
private slots:
  void pushButtonSelectAllPressed();
  void pushButtonUnselectAllPressed();
  void checkBoxSaveBinaryClicked();
  void checkBoxSaveTextClicked();
private:
  Ui::DialogAcquisitionConfiguration *ui;

  QVector<QCheckBox*> ptr_ch_enabled;
  void configureCheckboxPointers();
};

#endif // DIALOGACQUISITIONCONFIGURATION_H
