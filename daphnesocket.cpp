#include "daphnesocket.h"

DaphneSocket::DaphneSocket()
{
    this->ipAddr = "192.168.133.1";
    this->targetIpAddr = "192.168.133.12";
    this->portNumber = 2001;
    this->hostAddr.setAddress(this->ipAddr);
    this->targetAddr.setAddress(this->targetIpAddr);
    this->udpSocket = new QUdpSocket();
    this->udpSocket->bind(this->hostAddr,this->portNumber);
}

DaphneSocket::DaphneSocket(const QString &ipAddr, const QString &targetIpAddr, const uint16_t &portNumber)
{
    this->ipAddr = ipAddr;
    this->targetIpAddr = targetIpAddr;
    this->portNumber = portNumber;
    this->hostAddr.setAddress(this->ipAddr);
    this->targetAddr.setAddress(this->targetIpAddr);
    this->udpSocket = new QUdpSocket();
    this->udpSocket->bind(this->hostAddr,this->portNumber);
}

DaphneSocket::~DaphneSocket(){
    delete this->udpSocket;
}

int DaphneSocket::write(const uint64_t &addr,const uint8_t &num, uint8_t *data){
    int message_length = 2; //Considering the byte operation
    message_length = message_length + sizeof(addr);
    message_length = message_length + sizeof(uint8_t)*num;

    uint8_t *message = new uint8_t[message_length];
    message[0] = WRITE_OPERATION;
    message[1] = num;

    uint64_t *addr_ = reinterpret_cast<uint64_t*>(message + 2*sizeof(uint8_t));
    addr_[0] = addr;

    uint8_t *data_ = reinterpret_cast<uint8_t*>(addr_+1);
    for(int i =0; i<num; i++){
        data_[i] = data[i];
    }

    char* message_ = reinterpret_cast<char*>(message);
    this->udpSocket->writeDatagram(message_,message_length,this->targetAddr,this->portNumber);

    delete[] data_;
    delete[] addr_;
    delete[] message_;
    delete[] message;
    return 0; // por ahora se retorna 0
}

int DaphneSocket::read(const uint64_t &addr,const uint8_t &num, uint8_t *data){
    int message_length = 2; //Considering the byte operation
    message_length = message_length + sizeof(addr);

    uint8_t *message = new uint8_t[message_length];
    message[0] = READ_OPERATION;
    message[1] = num;

    uint64_t *addr_ = reinterpret_cast<uint64_t*>(message + 2*sizeof(uint8_t));
    addr_[0] = addr;

    char* message_ = reinterpret_cast<char*>(message);
    this->udpSocket->writeDatagram(message_,message_length,this->targetAddr,this->portNumber);
    char *data_ = reinterpret_cast<char*>(data);

    this->udpSocket->readDatagram(data_,num);

    delete[] data_;
    delete[] addr_;
    delete[] message_;
    delete[] message;
    return 0; // por ahora se retorna 0
}


