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

public slots:
    void plotDataMultichannel();

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
    void saveContinousWaveform(const QString &address, int &wave_number);
    bool getWindowStatus();
    void setWindowStatus(bool status);
    void setEthernetDataForThreadedPlotting(QVector<QVector<uint16_t>> *ethernet_data);
    void setChannelForThreadedPlotting(int *channel);
    void setWaveNumberPointer(int *wave_number);
    int getSpinBoxPlotEveryWaveformsValue();

    QPushButton *getOKButtonPointer();
private slots:
    void pushButtonSaveToTxtPressed();

    void cancelButtonPressed();
    void pushButtonAutoSetPressed();
    void spinBoxPositionSCaleValueChanged();
private:
    Ui::DialogReadoutChannel *ui;

    QVector<double> daphneTime;
    QVector<double> daphneData;
    QString saveDir;
    QVector<QString> saveFiles;
    QVector<int> enabledChannelsNumbers;

    bool window_status;
    QVector<QVector<uint16_t>> *plot_ethernet_data;
    int *plot_channel;
    int *wave_number;

    int16_t formatData2CompLSB(const uint16_t &data);
    int16_t formatData2CompMSB(const uint16_t &data);
    uint16_t formatDataOFFSETBinaryLSB(const uint16_t &data);
    uint16_t formatDataOFFSETBinaryMSB(const uint16_t &data);
    void writeDataToFile();
    void generateTimeVectorEthernet(const int &length, double Tm);
    void plotMultichannel();
};
#endif // DIALOGREADOUTCHANNEL_H
