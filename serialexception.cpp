#include "serialexception.h"


serialException::serialException(const int &code)
  {
  this->exceptionCode = code;
}

void serialException::handleException(QWidget *mw){
  MainWindow *mymainwindow = reinterpret_cast<MainWindow*>(mw);
  switch(this->exceptionCode){
    case 1:
      mymainwindow->displayMessageBox("Serial Port is closed.");
      //QMessageBox::critical(this, QObject::tr("Error"), "Serial Port is closed.");
      break;
    case 2:
      mymainwindow->displayMessageBox("Internal GUI Error:\nTimeout waiting for DAPHNE serial response");
      //QMessageBox::critical(this, QObject::tr("Error"), "Internal GUI Error:\nTimeout waiting for DAPHNE serial response");
      break;
    case 3:
      mymainwindow->Message("Aborting sending further commands to DAPHNE",2);
      break;
    case 4:
      mymainwindow->Message("Reached maximun command retries.\nAborting sending further commands to DAPHNE",2);
      break;
    default:
      mymainwindow->displayMessageBox("Internal GUI Error:\nUnknown Error");
      //QMessageBox::critical(this, QObject::tr("Error"), "Internal GUI Error:\nUnknown Error");
      break;
  }
}
