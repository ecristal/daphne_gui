#ifndef DIALOGREADOUTCHANNEL_H
#define DIALOGREADOUTCHANNEL_H

#include <QDialog>
#include <cmath>
#include <algorithm>
#include <QFile>

namespace Ui {
class DialogReadoutChannel;
}

class DialogReadoutChannel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogReadoutChannel(QWidget *parent = nullptr);
    void plotData(const QByteArray &serial_data,const uint16_t &format);
    QByteArray generateDaphneTestData(const uint32_t &length);
    ~DialogReadoutChannel();
    void uint162bytes(const uint16_t valor, uint8_t *byte_array);
    uint16_t bytes2int16(const uint8_t byte_array_1, const uint8_t byte_array_2);
    void generateTimeVector(const int &length, double Tm);
    int32_t formatData(const uint16_t &data, const uint16_t format);
    QVector<double> getDaphneData();
    void saveContinousWaveform(QString &address, int &wave_number); 
    bool getWindowStatus();
    void setWindowStatus(bool status);
private slots:
    void pushButtonSaveToTxtPressed();

    void cancelButtonPressed();
private:
    Ui::DialogReadoutChannel *ui;

    QVector<double> daphneTime;
    QVector<double> daphneData;
    QString saveDir;

    bool window_status;

    int16_t formatData2CompLSB(const uint16_t &data);
    int16_t formatData2CompMSB(const uint16_t &data);
    uint16_t formatDataOFFSETBinaryLSB(const uint16_t &data);
    uint16_t formatDataOFFSETBinaryMSB(const uint16_t &data);
    void writeDataToFile();
};
#endif // DIALOGREADOUTCHANNEL_H
