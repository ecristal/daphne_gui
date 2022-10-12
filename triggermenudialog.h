#ifndef TRIGGERMENUDIALOG_H
#define TRIGGERMENUDIALOG_H

#include <QDialog>

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
  double getTriggerLevel();
  void setTriggerLevel(double &level);

private slots:
  void sourceInternalPressed();
  void sourceExternalPressed();
  void sourceSoftwarePressed();
private:
  Ui::TriggerMenuDialog *ui;
};

#endif // TRIGGERMENUDIALOG_H
