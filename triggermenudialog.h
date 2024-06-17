#ifndef TRIGGERMENUDIALOG_H
#define TRIGGERMENUDIALOG_H

#include <QDialog>

#include "mainwindow.h"
#include "daphnesocket.h"
#include "ethernetudpexception.h"

namespace Ui {
class TriggerMenuDialog;
}

class TriggerMenuDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TriggerMenuDialog(QWidget *parent = 0);
  ~TriggerMenuDialog();

  QVector<bool> getTriggerSource();
  void setTriggerSource(QVector<bool> &triggerSource);
  int getTriggerChannel();
  void setTriggerChannel(int &channel);
  QVector<int> getTriggerLevel();
  void setTriggerLevel(QVector<int> &level);
  void setTriggerEnableChannel(QVector<bool> &triggerEnabled);
  void setMultiplierValue(uint16_t &multiplier);
  uint16_t getMultiplierValue();
  void setCheckBoxStates(const QVector<bool> &states);
  QVector<bool> getCheckBoxStates();
  uint64_t getTriggerEnabledConfig();
private slots:
  void sourceInternalPressed();
  void sourceExternalPressed();
  void sourceSoftwarePressed();
  void buttonSetThresholdPressed();
  void spinBoxChannelValueChanged();
  void spinBoxLevelValueChanged();
  void pushButtonUnselectAllPressed();
  void pushButtonSelectAllPressed();
private:
  Ui::TriggerMenuDialog *ui;
  QVector<int> triggerLevel;
  QVector<bool> triggerEnabled;
  QVector<QCheckBox*> ptr_ch_enabled;
  void configTresholdSingleChannel(const uint32_t &channel, DaphneSocket *socket);
  void configTresholdAllChannels(DaphneSocket *socket);
  void configTriggerEnable(DaphneSocket *socket);
  void configureCheckboxPointers();
};

#endif // TRIGGERMENUDIALOG_H
