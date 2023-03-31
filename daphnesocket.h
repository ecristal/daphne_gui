#ifndef DAPHNESOCKET_H
#define DAPHNESOCKET_H

#define READ_OPERATION 0x0
#define WRITE_OPERATION 0x1

#include <QString>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QTime>
#include <QEventLoop>
#include <QCoreApplication>
#include <QThread>
#include <chrono>

#include "udpthread.h"

class DaphneSocket : public QObject
{
    Q_OBJECT

public:
    DaphneSocket();
    DaphneSocket(const QString &ipAddr, const QString &targetIpAddr, const uint16_t &portNumber, const uint16_t &targetPortNumber);
    ~DaphneSocket();
    int read(const uint64_t &addr,const uint8_t &num);
    int write(const uint64_t &addr,const uint8_t &num, uint8_t *data);
    QString alignAFEsV1(const int &retry, QVector<bool> &isAfeAligned, QVector<QString> &isAfeAlignedStr);
    QString alignAFEsV2A(const int &retry, QVector<bool> &isAfeAligned, QVector<QString> &isAfeAlignedStr);
    int ping(int size);
    int pong(int size);
    QVector<QByteArray> *getReceivedData();
    void flushReceivedData();
    int sendSingleCommand(const uint64_t &addr, const uint64_t &data);
    int sendData(const uint64_t &addr, const QVector<uint64_t> &data);
    void waitForReadyRead();

    void delayMilli(int delay_milli);
    QString getBindedToStr(){return this->bindedToAddr;}
    void sendSoftwareTrigger();
    void sendSoftwareTriggerDeadTime();
    int getExpectedDatagrams();
    int setExpectedDatagrams(const int &value);
    void reserveDatagramSize(const int &value);
    int processDatagram(const int &expected_datagrams_);
    void flushBuffer();
    int receivedDataLength();
    void stopUdpThread();
    void startUdpThread();
    void setDataIs64bits2Thread(const bool &dataIs64bits_);
private slots:
    void readyRead_();
private:

    UdpThread receivedDataThread;
    uint16_t portNumber;
    uint16_t socketPortNumber;
    QString ipAddr;
    QString targetIpAddr;
    QUdpSocket *udpSocket;
    QHostAddress hostAddr;
    QHostAddress targetAddr;

    QVector<QByteArray> receivedData;
    int processDatagram(QByteArray &datagram);
    bool perform_aligment = true;
    bool receivedDatagramFlag = false;

    int expectedDatagrams;
    int datagramCounter = 0;

    QString bindedToAddr = "";
    int processDatagramReserved(QByteArray &datagram);
};

#endif // DAPHNESOCKET_H
