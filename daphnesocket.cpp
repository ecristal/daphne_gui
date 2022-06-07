#include "daphnesocket.h"

#include <exception>

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

DaphneSocket::DaphneSocket(const QString &ipAddr, const QString &targetIpAddr, const uint16_t &portNumber, const uint16_t &targetPortNumber)
{
    this->ipAddr = ipAddr;
    this->targetIpAddr = targetIpAddr;
    this->portNumber = targetPortNumber;
    this->socketPortNumber = portNumber;
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
    //qDebug() << "Received data: ";
}

int DaphneSocket::processDatagram(QByteArray &datagram){
    if(datagram.length() != 0)
      this->receivedData.append(datagram);
    if(this->receivedData.length() > 15000)
        this->receivedData.pop_back();
    return datagram.length();
}

QVector<QByteArray> DaphneSocket::getReceivedData(){
    return this->receivedData;
}

void DaphneSocket::flushReceivedData(){
    this->receivedData.clear();
}

void DaphneSocket::waitForReadyRead(){
    this->udpSocket->waitForReadyRead();
}

QString DaphneSocket::alignAFEs(const int &retry, QVector<bool> &isAfeAligned, QVector<QString> &isAfeAlignedStr){
   int MINWIDTH = 13;
   qDebug() << "Resetting IDELAY y ISERCES...";
   QString output = "Resetting IDELAY y ISERCES...\n";
   this->sendSingleCommand(0x2001,0xdeadbeef);
   for(int afe = 0; afe<5; afe++){
       qDebug() << "AFE"<<afe;
       output = output + "AFE" + QString::number(afe) + "\n";
       for(int retry_ = 0; retry_ < retry; retry_++){
           uint64_t x[32] = {0};
           int w = 0;
           int c = 0;
           QString es = "";
           isAfeAlignedStr[afe] = "";
           for(int dv = 0; dv < 32; dv++){
               this->sendSingleCommand(0x4000+afe,dv);
               this->sendSingleCommand(0x2000,0x1234);
               this->read((0x40000000+(0x100000*afe)+(0x80000)),5);
               this->waitForReadyRead();
               this->delayMilli(5);
               QByteArray rec_data = this->getReceivedData().at(0);
               this->flushReceivedData();
               uint64_t *data_ = reinterpret_cast<uint64_t*>(rec_data.begin()+2);
               x[dv] = data_[1];
               this->flushReceivedData();
           }
           for(int i = 0; i < 32; i++){
               if(x[i] == 0x3F80){
                   w = w + 1;
                   c = c + i;
                   es = es + "*";
                   isAfeAlignedStr[afe] = isAfeAlignedStr[afe] + "*";
               }
               else{
                   es = es + ".";
                   isAfeAlignedStr[afe] = isAfeAlignedStr[afe] + ".";
               }
           }
           if(x[0]==0x3F80 and x[31]==0x3F80)
               continue;
           if(w >= MINWIDTH){
               qDebug() << es << " ALIGNED OK! bit width " << w << ", using IDALEY tap " << (int)(c/w);
               output = output + es + "ALIGNED OK! bit width " + QString::number(w) + ", using IDALEY tap " + QString::number((int)(c/w)) + "\n";
               this->sendSingleCommand(0x4000+afe,(int)(c/w));
               isAfeAligned[afe] = 1;
               isAfeAlignedStr[afe] = isAfeAlignedStr[afe] + " ALIGNED OK!";
               break;
           }else{
               QVector<uint64_t> v({0,1,2,3,4,5,6,7,8});
               this->sendData((0x3000+(0x10*afe)),v);
           }
           if(retry == retry-1)
               qDebug() << "FAILED!";
               output = output + "FAILED!\n";
               isAfeAlignedStr[afe] = isAfeAlignedStr[afe] + " FAILED!";
       }
   }
   return output;
}

int DaphneSocket::sendSingleCommand(const uint64_t &addr, const uint64_t &data){

    uint8_t *data_to_send = new uint8_t[8];
    uint64_t *data_ = reinterpret_cast<uint64_t*>(data_to_send);
    data_[0] = data;
    return this->write(addr,1,data_to_send);
}

int DaphneSocket::sendData(const uint64_t &addr, const QVector<uint64_t> &data){
    uint8_t number_data_to_send = data.length();
    uint8_t* data_ = new uint8_t[number_data_to_send*sizeof(uint64_t)];
    uint64_t *data_64 = reinterpret_cast<uint64_t*>(data_);
    for(int i = 0; i < number_data_to_send; i++){
        data_64[i] = data[i];
    }
    int bytes_send = this->write(addr,number_data_to_send,data_);
    delete[] data_;
    return bytes_send;
}

void DaphneSocket::delayMilli(int delay_milli){
    QTime dieTime = QTime::currentTime().addMSecs(delay_milli);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

