#ifndef UDPTHREAD_H
#define UDPTHREAD_H

#include <QString>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QEventLoop>
#include <QThread>
#include <chrono>


class UdpThread : public QThread
{
protected:
    void run() override;
public:
    UdpThread();
    ~UdpThread();
    void setSocket(QUdpSocket* socket_);
    void setReceivedData(QVector<QByteArray>* receivedData_);
    void setHostAddr(QHostAddress *hostAddr_);
    void setSocketPortNumber(uint16_t *socketPortNumber_);
    void exitRunFunction();
    void enterRunFunction();
    void setDataIs64bits(const bool &dataIs64bits_);
private:
    uint16_t *socketPortNumber;
    QHostAddress *hostAddr;
    bool keepRunning = true;
    bool dataIs64bits = false;
    QUdpSocket* socket;
    QVector<QByteArray>* receivedData;
};

#endif // UDPTHREAD_H
