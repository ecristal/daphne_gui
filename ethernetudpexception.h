#ifndef ETHERNETUDPEXCEPTION_H
#define ETHERNETUDPEXCEPTION_H

#include <QString>
#include "mainwindow.h"

class ethernetUDPException
{
public:
  ethernetUDPException(const int &code);
  ethernetUDPException(const int &code, const QString &error_str);
  ~ethernetUDPException(){}

  void handleException(QWidget *mw);
private:
  int exceptionCode;
  QString error_str;
};

#endif // ETHERNETUDPEXCEPTION_H
