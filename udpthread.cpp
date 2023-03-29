#include "udpthread.h"

UdpThread::UdpThread()
{}

UdpThread::~UdpThread()
{}

void UdpThread::setSocket(QUdpSocket *socket_){
    this->socket = socket_;
}

void UdpThread::setReceivedData(QVector<QByteArray> *receivedData_){
    this->receivedData = receivedData_;
}

void UdpThread::setHostAddr(QHostAddress *hostAddr_){
    this->hostAddr = hostAddr_;
}

void UdpThread::setSocketPortNumber(uint16_t *socketPortNumber_){
    this->socketPortNumber = socketPortNumber_;
}

void UdpThread::exitRunFunction(){
    this->keepRunning = false;
}

void UdpThread::enterRunFunction(){
    this->keepRunning = true;
}

void UdpThread::run(){
    while(this->keepRunning){
        while(this->socket->hasPendingDatagrams()){
            QByteArray datagram;
            datagram.resize(this->socket->pendingDatagramSize());
            this->socket->readDatagram(datagram.data(), datagram.size(), this->hostAddr, this->socketPortNumber);
            if(datagram.length() != 0){
              this->receivedData->append(datagram);
            }
            if(this->receivedData->length() > 15000){
                this->receivedData->pop_back();
                qDebug()<<"received poped back";
            }
        }
    }
}

