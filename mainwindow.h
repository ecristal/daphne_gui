#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MASK_ADC_OUTPUT_FORMAT_2COMP_REG_4 0x0
#define MASK_ADC_OUTPUT_FORMAT_OFFSET_BIN_REG_4 0x8
#define MASK_LSB_FIRST_REG_4 0x0
#define MASK_MSB_FIRST_REG_4 0x10
#define MASK_ERASER_ADC_FORMAT_REG_4 0x18

#define MASK_PGA_INTEGRATOR_DIS_REG_51 0x10
#define MASK_PGA_INTEGRATOR_EN_REG_51 0x0
#define MASK_PGA_GAIN_30DB_CONTROL_REG_51 0x2000
#define MASK_PGA_GAIN_24DB_CONTROL_REG_51 0x0
#define MASK_LPF_15MHZ_PROGRAMABILITY_REG_51 0x0
#define MASK_LPF_20MHZ_PROGRAMABILITY_REG_51 0x4
#define MASK_LPF_30MHZ_PROGRAMABILITY_REG_51 0x6
#define MASK_LPF_10MHZ_PROGRAMABILITY_REG_51 0x8
#define MASK_PGA_CLAMP_LEVEL_N2DBFS_REG_51 0x0
#define MASK_PGA_CLAMP_LEVEL_0DBFS_REG_51 0x40
#define MASK_PGA_CLAMP_LEVEL_DISABLED_REG_51 0x80
#define MASK_ERASER_CLAMP_LEVEL_REG_51 0xe0
#define MASK_ERASER_LPF_REG_51 0xe
#define MASK_ERASER_PGA_INTEGRATOR_REG_51 0x10
#define MASK_ERASER_PGA_GAIN_CONTROL_REG_51 0x2000


#define MASK_LNA_INTEGRATOR_DIS_REG_52 0x1000
#define MASK_LNA_INTEGRATOR_EN_REG_52 0x0
#define MASK_ACTIVE_TERMINATOR_INDIVIDUAL_RESISTOR_ENABLE_REG_52 0x10
#define MASK_PRESET_ACTIVE_TERMINATION_50OHMS_REG_52 0x0
#define MASK_PRESET_ACTIVE_TERMINATION_100OHMS_REG_52 0x40
#define MASK_PRESET_ACTIVE_TERMINATION_200OHMS_REG_52 0x80
#define MASK_PRESET_ACTIVE_TERMINATION_400OHMS_REG_52 0xC0
#define MASK_ACTIVE_TERMINATION_EN_REG_52 0x100
#define MASK_ACTIVE_TERMINATION_DIS_REG_52 0x0
#define MASK_LNA_GAIN_18DB_REG_52 0x0
#define MASK_LNA_GAIN_24DB_REG_52 0x2000
#define MASK_LNA_GAIN_12DB_REG_52 0x4000
#define MASK_ERASER_PRESET_ACTIVE_TERMINAIONS_REG_52 0xC0
#define MASK_ERASER_ACTIVE_TERMINATION_ENABLE_REG_52 0x100
#define MASK_ERASER_LNA_INTEGRATOR_REG_52 0x1000
#define MASK_ERASER_LNA_GAIN_CONTROL_REG_52 0x6000

#include <QMainWindow>
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QTimer>
#include <QEventLoop>
#include <QTextCursor>
#include <QTime>
#include <QException>
#include <QDir>
#include <QFileDialog>

#include "dialogreadoutchannel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void pushButtonConnectPressed();
    void pushButtonDisconnectPressed();
    void comboBoxAvailableSerialPortsValueChanged();
    void pushButtonRefreshPressed();
    void readDataFromSerial();
    void pushButtonInitialPressed();
    void pushButtonRDFPGAPressed();
    void pushButtonApplyBiasVoltages();
    void serialTimeoutTimerTriggered();
    void setConfig();
    void pushButtonApplyOffsetPressed();
    void pushButtonApplyVGAINPressed();
    void checkBox2ComplementChecked();
    void checkBoxOffsetBinaryChecked();
    void checkBoxLSBFirstChecked();
    void checkBoxMSBFirstChecked();
    void pushButtonSendRawCommandPressed();
    void pushButtonMultipleWaveformsDirectoryPressed();
private:
    Ui::MainWindow *ui;

    QSerialPort* serialPort_;
    bool receivingDataFlag = false;
    QTimer serialTimeoutTimer;
    QEventLoop waitingForData;
    QByteArray serialData;

    uint16_t reg_4_value = 0;
    uint16_t reg_51_value = 0;
    uint16_t reg_52_value = 0;

    QString mutliple_waveforms_folder_address;

    DialogReadoutChannel* dialogReadoutChannelWindow;

    void populateComboBoxAvailableSerialPorts();
    void populateComboBoxAFE();
    void initializeWindow();
    void Message(QString message, int msgCode);
    void sendDataFromSerial(const QString &send_data);
    bool sendCommand(const QString &command);
    void populateComboBoxChannel();
    void waitForResponse(uint timeout);
    void delay(int delay_);
    void populateComboBoxLNAGain();
    void populateComboBoxPGAGain();
    void populateComboBoxImpedances();
    void populateComboBoxLPF();
    uint16_t getLNAGainMask();
    uint16_t getPGAGainMask();
    uint16_t getImpedanceMask();
    uint16_t getLPFMask();
    uint16_t eraseAndApplyMask(uint16_t &reg, uint16_t &mask, uint16_t &eraser);
    uint16_t getCheckBoxLNAIntegratorMask();
    uint16_t getCheckBoxPGAIntegratorMask();
    uint16_t getCheckBoxActiveTerminationMask();
    uint16_t getADCFormatMask();
    void populateComboBoxVGainValues();
    int calculateVGainReferenceValue();
    void populateComboBoxPGAClampLevel();
    uint16_t getPGAClampLevelMask();
    void readAndPlotData();
};
#endif // MAINWINDOW_H
