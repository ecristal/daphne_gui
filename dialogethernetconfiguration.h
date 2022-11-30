#ifndef DIALOGETHERNETCONFIGURATION_H
#define DIALOGETHERNETCONFIGURATION_H

#include <QDialog>

namespace Ui {
class DialogEthernetConfiguration;
}

class DialogEthernetConfiguration : public QDialog
{
  Q_OBJECT

public:
  explicit DialogEthernetConfiguration(QWidget *parent = 0);
  ~DialogEthernetConfiguration();

  void setDaphneIpAddress(const QString &addr);
  void setComputerIpAddress(const QString &addr);
  void setDaphnePortNumber(const int &portNumber);
  void setComputerPortNumber(const int &portNumber);
  QString getDaphneIpAddress();
  QString getComputerIpAddress();
  int getDaphnePortNumber();
  int getComputerPortNumber();

  int getDWT();
  void setDWT(const int &value);
private slots:
  void pushButtonDefaultPressed();
private:
  Ui::DialogEthernetConfiguration *ui;

};

#endif // DIALOGETHERNETCONFIGURATION_H
