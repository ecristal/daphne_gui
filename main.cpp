#include "mainwindow.h"

#include <QApplication>
#include <TApplication.h>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    TApplication rootapp("rootapp", &argc, argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
