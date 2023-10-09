#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define GUI_VERSION "V2_03_02"

#define MASK_LOW_FREQ_NOISE_SUPR_REG_1 0x800

#define MASK_CHANNEL_OFFSET_SUBSTRACTION_ENABLE_REG3 0x100
#define MASK_DIGITAL_GAIN_ENABLED_REG3 0x1000

#define MASK_OFFSET_CH1_SUBSTRACT_REG13 0x3FF //aqui debo realizar una operacion
#define MASK_OFFSET_CH2_SUBSTRACT_REG15 0x3FF //realmente no es necesario
#define MASK_OFFSET_CH3_SUBSTRACT_REG17 0x3FF // colocar todas las variables pues tienen el mismo valor
#define MASK_OFFSET_CH4_SUBSTRACT_REG19 0x3FF // pero es util para saber cual registro escribir
#define MASK_OFFSET_CH5_SUBSTRACT_REG31 0x3FF
#define MASK_OFFSET_CH6_SUBSTRACT_REG29 0x3FF
#define MASK_OFFSET_CH7_SUBSTRACT_REG27 0x3FF
#define MASK_OFFSET_CH8_SUBSTRACT_REG25 0x3FF

#define MASK_DIGITAL_GAIN_CH1_REG13 0xF800 //aqui debo realizar una operacion
#define MASK_DIGITAL_GAIN_CH2_REG15 0xF800 //realmente no es necesario
#define MASK_DIGITAL_GAIN_CH3_REG17 0xF800 // colocar todas las variables pues tienen el mismo valor
#define MASK_DIGITAL_GAIN_CH4_REG19 0xF800 // pero es util para saber cual registro escribir
#define MASK_DIGITAL_GAIN_CH5_REG31 0xF800
#define MASK_DIGITAL_GAIN_CH6_REG29 0xF800
#define MASK_DIGITAL_GAIN_CH7_REG27 0xF800
#define MASK_DIGITAL_GAIN_CH8_REG25 0xF800

#define MASK_DIGITAL_HPF_FILTER_CH_1_4_ENABLE_REG21 0x1
#define MASK_DIGITAL_HPF_FILTER_CH_1_4_K_VALUE_REG21 0x1E

#define MASK_DIGITAL_HPF_FILTER_CH_5_8_ENABLE_REG33 0x1
#define MASK_DIGITAL_HPF_FILTER_CH_5_8_K_VALUE_REG33 0x1E

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
#define MASK_ERASER_PGA_CLAMP_LEVEL_REG_51 0xe0
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

#define MASK_LNA_CLAMP_LEVEL_AUTO_REG_52 0x0
#define MASK_LNA_CLAMP_LEVEL_15VPP_REG_52 0x200
#define MASK_LNA_CLAMP_LEVEL_115VPP_REG_52 0x400
#define MASK_LNA_CLAMP_LEVEL_06VPP_REG_52 0x600

#define MASK_LNA_GAIN_18DB_REG_52 0x0
#define MASK_LNA_GAIN_24DB_REG_52 0x2000
#define MASK_LNA_GAIN_12DB_REG_52 0x4000

#define MASK_ERASER_PRESET_ACTIVE_TERMINAIONS_REG_52 0xC0
#define MASK_ERASER_ACTIVE_TERMINATION_ENABLE_REG_52 0x100
#define MASK_ERASER_LNA_INTEGRATOR_REG_52 0x1000
#define MASK_ERASER_LNA_GAIN_CONTROL_REG_52 0x6000
#define MASK_ERASER_LNA_CLAMP_LEVEL_REG_52 0x600

#define MASK_HPF_LNA_LEVEL_0_REG59 0x4
#define MASK_HPF_LNA_LEVEL_1_REG59 0x0
#define MASK_HPF_LNA_LEVEL_2_REG59 0xC
#define MASK_HPF_LNA_LEVEL_3_REG59 0x8
#define MASK_ERASER_HPF_LNA_REG59 0xC

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
#include <QUdpSocket>
#include <QHostAddress>
#include <chrono>
#include <QCloseEvent>
#include <QElapsedTimer>
#include <chrono>
#include <QQueue>
#include <QMutex>

#include "dialogreadoutchannel.h"
#include "daphnesocket.h"
#include "dialogaligment.h"
#include "dialogethernetconfiguration.h"
#include "dialogacquisitionconfiguration.h"
#include "serialexception.h"
#include "dialogafeconfiguration.h"
#include "dialogivcurve.h"
#include "triggermenudialog.h"
#include "savedatathread.h"
#include "dialogbiasconfiguration.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sendFPGAReset();

    DaphneSocket *getSocket();
    void Message(QString message, int msgCode);
    void displayErrorMessageBox(const QString &msg);
    void sendCommand(const QString &command);
    void delayMilli(const int &delay_milli);
    void delayMicro(const int &delay_micro);

    QString getSerialString();
    int getAFENumberFromChannelNumber(const int &channelNumber);
    const QCheckBox *getEthernetCheckboxPointer();
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
    void pushButtonGETCONFIGPressed();
    void checkBoxEnableEthernetPressed();
    void menuAlignmentPressed();
    void menuEthernetConfigurationPressed();
    void populateComboBoxChannel();
    void menuAcquisitionConfigurationPressed();
    void menuAFEConfigurationPressed();
    void menuIVCurvePressed();
    void menuTriggerPressed();
    void checkBoxSaveWaveformsClicked();
    void comboBoxChannelTextChanged();
    void menuBiasPressed();
private:
    Ui::MainWindow *ui;

    QSerialPort* serialPort_;
    bool receivingSerialDataFlag = false;
    QTimer serialTimeoutTimer;
    QEventLoop waitingForData;
    QByteArray serialData;

    QString serial_data_string, serial_data_string_success;

    uint16_t reg_1_value = 0;
    uint16_t reg_21_value = 0;
    uint16_t reg_33_value = 0;
    uint16_t reg_4_value = 0;
    uint16_t reg_51_value = 0;
    uint16_t reg_52_value = 0;
    uint16_t reg_59_value = 0;

    // main window menu

    int currentChannel = 0;
    int currentAFE = 0;
    QVector<double> VGAINSetValue = {0.89,0.89,0.89,0.89,0.89}; //default value aprox. for 2000 P.E.
    QVector<uint16_t> OFFSETSetValue = {2100,2100,2100,2100,2100,2100,2100,2100,2100,2100,
                                        2100,2100,2100,2100,2100,2100,2100,2100,2100,2100,
                                        2100,2100,2100,2100,2100,2100,2100,2100,2100,2100,
                                        2100,2100,2100,2100,2100,2100,2100,2100,2100,2100}; //dafault value aprox. for center offset.



    QString multiple_waveforms_folder_address;

    DialogReadoutChannel* dialogReadoutChannelWindow;

    saveDataThread saveThread;
    QMutex saveMutex;

    // Ethernet menu
    DaphneSocket *socket;
    QString daphneIPAddr = "192.168.133.12";
    QString computerIPAddr = "192.168.133.1";
    int daphnePortNumber = 2001;
    int computerPortNumber = 58789;
    int received_datagrams, expected_datagrams;
    int udpWaitforDatagram = 5;

    // TriggerMenu
    QVector<bool> triggerSource = {false, true, false};
    int triggerChannel = 0;
    QVector<int> triggerLevel = {99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999,
                                 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999,
                                 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999,
                                 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999};
    uint16_t preTriggerMultiplier = 0;

    // Acquisition menu
    QVector<bool> channelsEnabledState;
    int recordLength = 256;
    QVector<uint16_t> ethernetData;
    QVector<QVector<uint16_t>> channelsData;
    QQueue<QVector<QVector<uint16_t>>> channelsData_queue;
    bool saveToTxtState = false;
    bool saveToBinaryState = true;
    bool saveFormat = true;

    //AFE configuration menu
    bool lowNoiseSupressionEnabled = false;
    bool FPGAFilterEnabled = false;
    uint16_t digitalHPFKValue = 9;
    bool digitalHPFEnabled = false;
    uint8_t digitalFilterOutputSelector = 0;
    uint8_t HPFLNAlevel = 0;

    //Bias configuration menu
    double biasControlValue = 0;
    QVector<double> biasOFFSETValue = {0.0,0.0,0.0,0.0,0.0};
    QVector<double> biasSetValue = {0.0,0.0,0.0,0.0,0.0};
    QVector<uint16_t> trimSetValue = {0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0};

    bool ethernetCheckBoxCheckedFlag = false;

    void populateComboBoxAvailableSerialPorts();
    void populateComboBoxAFE();
    void initializeWindow();
    void sendDataFromSerial(const QString &send_data);
    void waitForResponse(uint timeout);
    void delay(int delay_);
    void populateComboBoxLNAGain();
    void populateComboBoxPGAGain();
    void populateComboBoxImpedances();
    void populateComboBoxLPF();
    bool sendCommand_(const QString &command);
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
    void readAndPlotDataSerial();
    void populateComboBoxLNAClampLevel();
    uint16_t getLNAClampLevelMask();
    void acquireWaveform();
    void readAndPlotDataEthernet(const int &channel);
    int getSpyBufferFromChannel(const int &channel);
    void configureEnabledChannels();
    int requestDataFromChannel(const int &channel,const int &length, int &requested_data);
    int readChannelsEthernet(const QVector<bool> &enabledChannels);
    void acquireWaveformEnabled();
    void readMultichannelEthernet_vector(const QVector<bool> &enabledChannels);
    void setOFFSETGain(const int &i);
    void setAFEConfiguration(const QString &afe_number);
    void handleNewEthernetSocket();
    void closeEvent(QCloseEvent *event);
    QString parseSerialDataStringSuccess();
    bool isSerialCommandSuccesful();
    void sendSoftwareTriggerDeadTime();
    void sendSoftwareTrigger();
    int getNumberOfExpectedDatagrams();
    void initializeChannelsData();
    void parseEthernetData();
    void mainWaveformsAcquisition();
    void offsetSweepWaveformsAcquisitions();
    void displayWarningMessageBox(const QString &msg);
    void enabledFunctionsDuringDatataking(const bool &enable);
    int calculateBIASReferenceValue();
    int calculateBIAS_CTRLReferenceValue();
};
#endif // MAINWINDOW_H
