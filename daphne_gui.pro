QT += core gui
QT += serialport
QT += printsupport
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    daphnesocket.cpp \
    dialogaligment.cpp \
    dialogreadoutchannel.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    dialogethernetconfiguration.cpp \
    dialogacquisitionconfiguration.cpp \
    serialexception.cpp \
    dialogafeconfiguration.cpp

HEADERS += \
    daphnesocket.h \
    dialogaligment.h \
    dialogreadoutchannel.h \
    mainwindow.h \
    qcustomplot.h \
    dialogethernetconfiguration.h \
    dialogacquisitionconfiguration.h \
    serialexception.h \
    dialogafeconfiguration.h

FORMS += \
    dialogaligment.ui \
    dialogreadoutchannel.ui \
    mainwindow.ui \
    dialogethernetconfiguration.ui \
    dialogacquisitionconfiguration.ui \
    dialogafeconfiguration.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Para crear un ejecutable standalone, copiar solo el ejecutable (.exe) de la carpeta build en otra carpeta. Abrir una terminal
# y ejecutar el comando windeployqt
