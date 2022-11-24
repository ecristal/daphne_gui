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
private slots:
  void sourceInternalPressed();
  void sourceExternalPressed();
  void sourceSoftwarePressed();
  void buttonSetThresholdPressed();
  void spinBoxChannelValueChanged();
  void spinBoxLevelValueChanged();
private:
  Ui::TriggerMenuDialog *ui;
  QVector<int> triggerLevel;
  QVector<bool> triggerEnabled;
  void configTresholdSingleChannel(const uint32_t &channel, DaphneSocket *socket);
  void configTresholdAllChannels(DaphneSocket *socket);
  void configTriggerEnable(DaphneSocket *socket);
};

#endif // TRIGGERMENUDIALOG_H
