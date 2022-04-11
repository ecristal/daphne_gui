#include "daphnesocket.h"

DaphneSocket::DaphneSocket()
{
    this->ipAddr = "192.168.133.1";
    this->targetIpAddr = "192.168.133.12";
    this->portNumber = 2001;
    this->socketPortNumber = 58789;
    this->hostAddr.setAddress(this->ipAddr);
    this->targetAddr.setAddress(this->targetIpAddr);
    this->udpSocket = new QUdpSocket(this);


    if(!this->udpSocket->bind(this->hostAddr,this->socketPortNumber))
    {
       qDebug() << "Failed to bind UDP socket:" << this->udpSocket->errorString();
    }else{
       qDebug() << "UDP socket binded to: " << this->hostAddr.toString() << "::" << this->socketPortNumber;
       connect(this->udpSocket, SIGNAL(readyRead()), this, SLOT(readyRead_()), Qt::QueuedConnection);
    }
}

DaphneSocket::DaphneSocket(const QString &ipAddr, const QString &targetIpAddr, const uint16_t &portNumber)
{
    this->ipAddr = ipAddr;
    this->targetIpAddr = targetIpAddr;
    this->portNumber = portNumber;
    this->hostAddr.setAddress(this->ipAddr);
    this->targetAddr.setAddress(this->targetIpAddr);
    this->udpSocket = new QUdpSocket();
    this->udpSocket->bind(this->hostAddr,this->socketPortNumber);

    //connect(this->udpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

DaphneSocket::~DaphneSocket(){
    disconnect(this->udpSocket, SIGNAL(readyRead()), this, SLOT(readyRead_()));
    delete this->udpSocket;
}

int DaphneSocket::write(const uint64_t &addr,const uint8_t &num, uint8_t *data){
    int message_length = 2; //Considering the byte operation
    message_length = message_length + sizeof(addr);
    message_length = message_length + sizeof(uint64_t)*num;

    uint8_t *message = new uint8_t[message_length];
    message[0] = WRITE_OPERATION;
    message[1] = num;

    uint64_t *addr_ = reinterpret_cast<uint64_t*>(message + 2*sizeof(uint8_t));
    addr_[0] = addr;

    uint8_t *data_ = reinterpret_cast<uint8_t*>(addr_+1);
    for(uint32_t i =0; i<sizeof(uint64_t)*num; i++){
        data_[i] = data[i];
    }
//    qDebug() << "--------------start packet-----------------";
    char* message_ = reinterpret_cast<char*>(message);
//    for(int i = 0; i<message_length; i++){
//        qDebug() << "message["<<i<<"]: " <<(uint8_t)message_[i];
//    }
//     qDebug() << "--------------end packet-----------------";
    int bytes_sent = this->udpSocket->writeDatagram(message_,message_length,this->targetAddr,this->portNumber);
    //delete[] data_;
    //delete[] addr_;
    //delete[] message_;
    delete[] message;
    return bytes_sent; // por ahora se retorna 0
}

int DaphneSocket::read(const uint64_t &addr,const uint8_t &num){
    int message_length = 2; //Considering the byte operation
    message_length = message_length + sizeof(addr);

    uint8_t *message = new uint8_t[message_length];
    message[0] = READ_OPERATION;
    message[1] = num;

    uint64_t *addr_ = reinterpret_cast<uint64_t*>(message + 2);
    addr_[0] = addr;

    char* message_ = reinterpret_cast<char*>(message);
//    for(int i = 0; i<message_length; i++){
//        qDebug() << "message["<<i<<"]: " <<(uint8_t)message_[i];
//    }
    int bytes_written = this->udpSocket->writeDatagram(message_,message_length,this->targetAddr,this->portNumber);
//    qDebug() << "bytes_written: " << bytes_witten;
    //delete[] data_;
    //delete[] addr_;
    //delete[] message_;
    delete[] message;
    return bytes_written; // por ahora se retorna 0
}

int DaphneSocket::ping(int size){
    char *data_to_send = new char[size];
    int bytes_sent = this->udpSocket->writeDatagram(data_to_send,size,this->hostAddr,this->portNumber);
    return bytes_sent;
}

int DaphneSocket::pong(int size){
    char *data_to_receive = new char[size];
    int received_bytes = this->udpSocket->readDatagram(data_to_receive,size);
    return received_bytes;
}

void DaphneSocket::readyRead_()
{
    int number_of_bytes_received = 0;
    //while(this->udpSocket->hasPendingDatagrams()){
    QByteArray datagram;
    datagram.resize(this->udpSocket->pendingDatagramSize());
    this->udpSocket->readDatagram(datagram.data(), datagram.size(), &this->hostAddr, &this->socketPortNumber);
    number_of_bytes_received = number_of_bytes_received + this->processDatagram(datagram);
    //}
    qDebug() << "Message from: ";
}

int DaphneSocket::processDatagram(QByteArray &datagram){
    this->receivedData.append(datagram);
    return datagram.length();
}

QVector<QByteArray> DaphneSocket::getReceivedData(){
    return this->receivedData;
}

void DaphneSocket::flushReceivedData(){
    this->receivedData.clear();
}

