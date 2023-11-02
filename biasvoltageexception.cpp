#include "biasvoltageexception.h"

biasVoltageException::biasVoltageException(int code)
{
    this->exceptionCode = code;
}

void biasVoltageException::handleException(QWidget *mw){
  MainWindow *mymainwindow = reinterpret_cast<MainWindow*>(mw);
  switch(this->exceptionCode){
    case 1:
      mymainwindow->displayErrorMessageBox("Trim Voltage is larger that Bias Voltage.");
      break;
    case 2:
      mymainwindow->displayErrorMessageBox("Bias Control Value is lower that Bias Voltage.\nPlease set the Bias Control Value larger than the desired Bias Voltage at:\nConfiguration->Bias");
      break;
    default:
      mymainwindow->displayErrorMessageBox("Internal GUI Error:\nUnknown Error");
      break;
  }
}
