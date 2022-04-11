#ifndef DAPHNESOCKET_H
#define DAPHNESOCKET_H

#define READ_OPERATION 0x0
#define WRITE_OPERATION 0x1

#include <QString>
#include <QUdpSocket>
#include <QNetworkDatagram>

class DaphneSocket : public QObject
{
    Q_OBJECT

public:
    DaphneSocket();
    DaphneSocket(const QString &ipAddr,const QString &targetIpAddr, const uint16_t &portNumber);
    ~DaphneSocket();
    int read(const uint64_t &addr,const uint8_t &num);
    int write(const uint64_t &addr,const uint8_t &num, uint8_t *data);
    int ping(int size);
    int pong(int size);
    QVector<QByteArray> getReceivedData();
    void flushReceivedData();
private slots:
    void readyRead_();
private:
    uint16_t portNumber;
    uint16_t socketPortNumber;
    QString ipAddr;
    QString targetIpAddr;
    QUdpSocket *udpSocket;
    QHostAddress hostAddr;
    QHostAddress targetAddr;

    QVector<QByteArray> receivedData;
    int processDatagram(QByteArray &datagram);
};

#endif // DAPHNESOCKET_H
