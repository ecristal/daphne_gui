#include "savedatathread.h"

saveDataThread::saveDataThread()
{}

void saveDataThread::setChannelsDataQueuePointer(QQueue<QVector<QVector<double> > > *channelsData_queue_){
    this->channelsData_queue = channelsData_queue_;
}

void saveDataThread::exitRunFunction(){
    this->keepRunning = false;
}

void saveDataThread::enterRunFunction(){
    this->keepRunning = true;
}

void saveDataThread::setFormatPointer(bool *saveFormat_){
    this->saveFormat = saveFormat_;
}

void saveDataThread::setMutexPointer(QMutex *saveMutex_){
    this->saveMutex = saveMutex_;
}

void saveDataThread::saveMultiChannel(const QVector<QVector<double>> &data, const bool &format){

  for(int i = 0; i < this->saveFiles.length(); i++){
    QFile file(this->saveFiles.at(i));
    file.open(QIODevice::Append);
    //QVector<double> channel_data = data.at(this->enabledChannelsNumbers.at(i));
    //******saving in binary part ******************////
    if(format){
        QDataStream writeToFile_binary(&file);
        writeToFile_binary.setByteOrder(QDataStream::LittleEndian);
        for(double data2write : data.at(this->enabledChannelsNumbers.at(i))){
          writeToFile_binary << (u_int16_t)data2write;
        }
    }else{
    //*******saving in txt part****************/////
        QTextStream writeToFile_txt(&file);
        for(double data2write : data.at(this->enabledChannelsNumbers.at(i))){
          writeToFile_txt << QString::number(data2write)<<"\n";
        }
    }
    file.close();
  }
}

void saveDataThread::createFileNames(const QString &address, const QVector<bool> &enabledChannels){

    QVector<QString> files;
    QVector<int> enabledChannelsNumbers_;
    int k = 0;
    for(int i = 0; i < enabledChannels.length(); i++){
      if(enabledChannels.at(i)){
        QString fileName = address + "/channel_" + QString::number(k) + ".dat";
        files.append(fileName);
        enabledChannelsNumbers_.append(i);
      }
      k++;
    }
    this->enabledChannelsNumbers.clear();
    this->saveFiles.clear();
    this->saveFiles = files;
    this->enabledChannelsNumbers = enabledChannelsNumbers_;
}

void saveDataThread::startSaveThread(){
    this->enterRunFunction();
    this->start();
}

void saveDataThread::stopSaveThread(){
    if(this->isRunning() && this->channelsData_queue->isEmpty()){
        this->exitRunFunction();
        this->quit();
        this->wait();
    }
}

void saveDataThread::run(){
    while(this->keepRunning){
        this->saveMutex->lock();
        while(!this->channelsData_queue->isEmpty()){
            QVector<QVector<double>> data_ = this->channelsData_queue->dequeue();
            //this->saveMutex->unlock();
            this->saveMultiChannel(data_,*this->saveFormat);
        }
        this->saveMutex->unlock();
    }
}
