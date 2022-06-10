#ifndef SERIALEXCEPTION_H
#define SERIALEXCEPTION_H

#include "mainwindow.h"

class serialException
{

public:
  serialException(const int &code);
  ~serialException(){}

  void handleException(QWidget *mw);
private:
  int exceptionCode;
};

#endif // SERIALEXCEPTION_H
