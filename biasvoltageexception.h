#ifndef BIASVOLTAGEEXCEPTION_H
#define BIASVOLTAGEEXCEPTION_H

#include "mainwindow.h"

class biasVoltageException
{
public:
    biasVoltageException(int code);
    ~biasVoltageException(){}

    void handleException(QWidget *mw);
private:
    int exceptionCode;
};

#endif // BIASVOLTAGEEXCEPTION_H
