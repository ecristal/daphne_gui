#ifndef SAVEDATATHREAD_H
#define SAVEDATATHREAD_H

#include <QThread>
#include <QQueue>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QMutex>

class saveDataThread : public QThread
{
protected:
    void run() override;
public:
    saveDataThread();
    void setChannelsDataQueuePointer(QQueue<QVector<QVector<double>>> *channelsData_queue_);
    void setFormatPointer(bool *saveFormat_);
    void setMutexPointer(QMutex *saveMutex_);
    void exitRunFunction();
    void enterRunFunction();
    void createFileNames(const QString &address, const QVector<bool> &enabledChannels);
    void startSaveThread();
    void stopSaveThread();
private:
    bool keepRunning = true;
    QVector<bool> *channelsEnabledState;
    QQueue<QVector<QVector<double>>> *channelsData_queue;
    QMutex *saveMutex;

    bool *saveFormat;
    QString saveDir;
    QVector<QString> saveFiles;
    QVector<int> enabledChannelsNumbers;

    void saveMultiChannel(const QVector<QVector<double>> &data, const bool &format);
};
#endif // SAVEDATATHREAD_H
