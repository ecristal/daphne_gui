#ifndef DAPHNESOCKET_H
#define DAPHNESOCKET_H

#define READ_OPERATION 0x0
#define WRITE_OPERATION 0x1

#include <QString>
#include <QUdpSocket>

class DaphneSocket
{
public:
    DaphneSocket();
    DaphneSocket(const QString &ipAddr,const QString &targetIpAddr, const uint16_t &portNumber);
    ~DaphneSocket();
    int read(const uint64_t &addr,const uint8_t &num, uint8_t *data);
    int write(const uint64_t &addr,const uint8_t &num, uint8_t *data);

private:
    uint16_t portNumber;
    QString ipAddr;
    QString targetIpAddr;
    QUdpSocket *udpSocket;
    QHostAddress hostAddr;
    QHostAddress targetAddr;
};

#endif // DAPHNESOCKET_H
