#include "ethernetudpexception.h"

ethernetUDPException::ethernetUDPException(const int &code)
{
  this->exceptionCode = code;
}

ethernetUDPException::ethernetUDPException(const int &code, const QString &error_str)
{
  this->exceptionCode = code;
  this->error_str = error_str;
}

void ethernetUDPException::handleException(QWidget *mw){
  MainWindow *mymainwindow = reinterpret_cast<MainWindow*>(mw);
  switch(this->exceptionCode){
    case 1:
      mymainwindow->displayErrorMessageBox("Requested received data.\nReceived data is empty. Check the connection.");
      //QMessageBox::critical(this, QObject::tr("Error"), "Serial Port is closed.");
      break;
    case 2:
      mymainwindow->displayErrorMessageBox(this->error_str);
      break;
    case 3:
      mymainwindow->displayErrorMessageBox("Ethernet checkbox is not enabled.\n");
      break;
    default:
      mymainwindow->displayErrorMessageBox("Internal GUI Error:\nUnknown Error");
      //QMessageBox::critical(this, QObject::tr("Error"), "Internal GUI Error:\nUnknown Error");
      break;
  }
}
