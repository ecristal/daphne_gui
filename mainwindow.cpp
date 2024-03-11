#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initializeWindow();
    connect(ui->pushButtonConnect,SIGNAL(clicked(bool)),this,SLOT(pushButtonConnectPressed()));
    connect(ui->pushButtonDisconnect,SIGNAL(clicked(bool)),this,SLOT(pushButtonDisconnectPressed()));
    connect(ui->comboBoxAvailableSerialPort,SIGNAL(activated(int)),this,SLOT(comboBoxAvailableSerialPortsValueChanged()));
    connect(ui->pushButtonRefresh,SIGNAL(clicked(bool)),this,SLOT(pushButtonRefreshPressed()));
    connect(this->serialPort_,SIGNAL(readyRead()),this,SLOT(readDataFromSerial()));
    connect(ui->pushButtonRDFPGA,SIGNAL(clicked(bool)),this,SLOT(pushButtonRDFPGAPressed()));
    connect(ui->pushButtonApplyBiasVoltages,SIGNAL(clicked(bool)),this,SLOT(pushButtonApplyBiasVoltages()));
    connect(&this->serialTimeoutTimer,SIGNAL(timeout()),this,SLOT(serialTimeoutTimerTriggered()));
    connect(&this->serialTimeoutTimer, SIGNAL(timeout()), &this->waitingForData,SLOT(quit()));
    connect(ui->pushButtonSETCONFIG,SIGNAL(clicked(bool)),this,SLOT(setConfig()));
    connect(ui->pushButtonApplyChannelOffset,SIGNAL(clicked(bool)),this,SLOT(pushButtonApplyOffsetPressed()));
    connect(ui->pushButtonApplyVGAIN,SIGNAL(clicked(bool)),this,SLOT(pushButtonApplyVGAINPressed()));
    connect(ui->checkBox2Complement,SIGNAL(stateChanged(int)),this,SLOT(checkBox2ComplementChecked()));
    connect(ui->checkBoxOffsetBinary,SIGNAL(stateChanged(int)),this,SLOT(checkBoxOffsetBinaryChecked()));
    connect(ui->checkBoxLSBFirst,SIGNAL(stateChanged(int)),this,SLOT(checkBoxLSBFirstChecked()));
    connect(ui->checkBoxMSBFirst,SIGNAL(stateChanged(int)),this,SLOT(checkBoxMSBFirstChecked()));
    connect(ui->pushButtonSendRawCommand,SIGNAL(clicked(bool)),this,SLOT(pushButtonSendRawCommandPressed()));
    connect(ui->pushButtonMultipleWaveformsDirectory,SIGNAL(clicked(bool)),this,SLOT(pushButtonMultipleWaveformsDirectoryPressed()));
    connect(ui->pushButtonGETCONFIG,SIGNAL(clicked(bool)),this,SLOT(pushButtonGETCONFIGPressed()));
    connect(ui->checkBoxEnableEthernet,SIGNAL(clicked(bool)),this,SLOT(checkBoxEnableEthernetPressed()));
    connect(ui->menuAlignment,SIGNAL(triggered(bool)),this,SLOT(menuAlignmentPressed()));
    connect(ui->actionEthernet,SIGNAL(triggered(bool)),this,SLOT(menuEthernetConfigurationPressed()));
    connect(ui->comboBoxAFE,SIGNAL(currentIndexChanged(int)),this,SLOT(populateComboBoxChannel()));
    connect(ui->actionAcquisition,SIGNAL(triggered(bool)),this,SLOT(menuAcquisitionConfigurationPressed()));
    connect(ui->actionAFE,SIGNAL(triggered(bool)),this,SLOT(menuAFEConfigurationPressed()));
    connect(ui->actionI_V_Curve,SIGNAL(triggered(bool)),this,SLOT(menuIVCurvePressed()));
    connect(ui->actionTrigger,SIGNAL(triggered(bool)),this,SLOT(menuTriggerPressed()));
    connect(ui->actionBias,SIGNAL(triggered(bool)),this,SLOT(menuBiasPressed()));
    connect(ui->checkBoxSaveWaveforms,SIGNAL(clicked(bool)),this,SLOT(checkBoxSaveWaveformsClicked()));
    connect(ui->comboBoxChannel,SIGNAL(currentTextChanged(QString)),this,SLOT(comboBoxChannelTextChanged()));
}

MainWindow::~MainWindow()
{
    this->dialogReadoutChannelWindow->~DialogReadoutChannel();
    delete ui;
}

void MainWindow::initializeWindow(){
    this->setWindowTitle("DAPHNE GUI TOOL");
    ui->menuAlignment->setEnabled(false);
    this->populateComboBoxAvailableSerialPorts();
    this->populateComboBoxPGAClampLevel();
    this->populateComboBoxAFE();
    this->populateComboBoxChannel();
    this->populateComboBoxLNAGain();
    this->populateComboBoxPGAGain();
    this->populateComboBoxImpedances();
    this->populateComboBoxLPF();
    //this->populateComboBoxVGainValues(); //obsoleto
    this->populateComboBoxLNAClampLevel();
    this->serialTimeoutTimer.setSingleShot(true);
    this->configureEnabledChannels();
    this->currentChannel = ui->comboBoxChannel->currentText().toInt();
    ui->pushButtonGETCONFIG->setEnabled(false);
    ui->pushButtonConnect->setEnabled(false);
    ui->pushButtonDisconnect->setEnabled(false);
    ui->spinBoxBaudRate->setValue(921600);
    ui->labelWorkingDirectory->setText("Working Directory: ");
    this->serialPort_ = new QSerialPort(this);
    this->dialogReadoutChannelWindow = new DialogReadoutChannel();
    this->channelsData.reserve(40);
    this->channelsData.resize(40);
    this->initializeChannelsData();
    this->dialogReadoutChannelWindow->setEthernetDataForThreadedPlotting(&this->channelsData);
    this->dialogReadoutChannelWindow->setChannelForThreadedPlotting(&this->currentChannel);
    this->saveThread.setChannelsDataQueuePointer(&this->channelsData_queue);
    this->saveThread.setFormatPointer(&this->saveFormat);
    this->saveThread.setMutexPointer(&this->saveMutex);
    this->Message("DAPHNE GUI TOOL\nAuthor: Ing. Esteban Cristaldo, MSc",0);
#ifdef QT_NO_DEBUG
    this->Message("Release " + QString::fromUtf8(GUI_VERSION) + "\n",0);
#endif
#ifdef QT_DEBUG
    this->Message("Release " + QString::fromUtf8(GUI_VERSION) + "\n",0);
#endif
}

void MainWindow::populateComboBoxAvailableSerialPorts(){

    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    for(QSerialPortInfo& port : availablePorts){
        ui->comboBoxAvailableSerialPort->addItem(port.portName());
    }
}

void MainWindow::configureEnabledChannels(){
  for(int i = 0; i < 40; i++){
    this->channelsEnabledState.append(true);
  }
}

void MainWindow::populateComboBoxLNAGain(){
    ui->comboBoxLNAGain->addItem("12dB");
    ui->comboBoxLNAGain->addItem("18dB");
    ui->comboBoxLNAGain->addItem("24dB");
}

void MainWindow::populateComboBoxPGAClampLevel(){
    ui->comboBoxPGAClampLevel->addItem("-2 dBFS");
    ui->comboBoxPGAClampLevel->addItem("0 dBFS");
    ui->comboBoxPGAClampLevel->addItem("DISABLED");
}

void MainWindow::populateComboBoxLNAClampLevel(){
    ui->comboBoxLNAClampLevel->addItem("AUTO");
    ui->comboBoxLNAClampLevel->addItem("1.5Vpp");
    ui->comboBoxLNAClampLevel->addItem("1.15Vpp");
    ui->comboBoxLNAClampLevel->addItem("0.6Vpp");
}

void MainWindow::populateComboBoxPGAGain(){
    ui->comboBoxPGAGain->addItem("24dB");
    ui->comboBoxPGAGain->addItem("30dB");
}

void MainWindow::populateComboBoxImpedances(){
    ui->comboBoxImpedance->addItem("50");
    ui->comboBoxImpedance->addItem("100");
    ui->comboBoxImpedance->addItem("200");
    ui->comboBoxImpedance->addItem("400");
}

void MainWindow::populateComboBoxLPF(){
    ui->comboBoxLPF->addItem("10 Mhz");
    ui->comboBoxLPF->addItem("15 Mhz");
    ui->comboBoxLPF->addItem("20 Mhz");
    ui->comboBoxLPF->addItem("30 Mhz");
}

void MainWindow::setConfig(){
    uint16_t lna_gain_mask = this->getLNAGainMask();
    uint16_t eraser = MASK_ERASER_LNA_GAIN_CONTROL_REG_52;
    this->reg_52_value = this->eraseAndApplyMask(this->reg_52_value,lna_gain_mask,eraser);

    uint16_t pga_gain_mask = this->getPGAGainMask();
    eraser = MASK_ERASER_PGA_GAIN_CONTROL_REG_51;
    this->reg_51_value = this->eraseAndApplyMask(this->reg_51_value,pga_gain_mask,eraser);

    uint16_t impedance_mask = this->getImpedanceMask();
    eraser = MASK_ERASER_PRESET_ACTIVE_TERMINAIONS_REG_52;
    this->reg_52_value = this->eraseAndApplyMask(this->reg_52_value,impedance_mask,eraser);

    uint16_t lpf_mask = this->getLPFMask();
    eraser = MASK_ERASER_LPF_REG_51;
    this->reg_51_value = this->eraseAndApplyMask(this->reg_51_value,lpf_mask,eraser);

    uint16_t lna_integrator_mask = this->getCheckBoxLNAIntegratorMask();
    eraser = MASK_ERASER_LNA_INTEGRATOR_REG_52;
    this->reg_52_value = this->eraseAndApplyMask(this->reg_52_value,lna_integrator_mask,eraser);

    uint16_t pga_integrator_mask = this->getCheckBoxPGAIntegratorMask();
    eraser = MASK_ERASER_PGA_INTEGRATOR_REG_51;
    this->reg_51_value = this->eraseAndApplyMask(this->reg_51_value,pga_integrator_mask,eraser);

    uint16_t pga_clamp_level_mask = this->getPGAClampLevelMask();
    eraser = MASK_ERASER_PGA_CLAMP_LEVEL_REG_51;
    this->reg_51_value = this->eraseAndApplyMask(this->reg_51_value,pga_clamp_level_mask,eraser);

    uint16_t lna_clamp_level_mask = this->getLNAClampLevelMask();
    eraser = MASK_ERASER_LNA_CLAMP_LEVEL_REG_52;
    this->reg_52_value = this->eraseAndApplyMask(this->reg_52_value,lna_clamp_level_mask,eraser);

    uint16_t active_termination_mask = this->getCheckBoxActiveTerminationMask();
    eraser = MASK_ERASER_ACTIVE_TERMINATION_ENABLE_REG_52;
    this->reg_52_value = this->eraseAndApplyMask(this->reg_52_value,active_termination_mask,eraser);

    uint16_t adc_format_mask = this->getADCFormatMask();
    eraser = MASK_ERASER_ADC_FORMAT_REG_4;
    this->reg_4_value = this->eraseAndApplyMask(this->reg_4_value,adc_format_mask,eraser);

    try{
      if(ui->checkBoxAllAFEConfigs->isChecked()){
        for(int i=0; i<5; i++){
          this->setAFEConfiguration(QString::number(i));
          this->AFE_LNA_gain[i] = ui->comboBoxLNAGain->currentIndex();
          this->AFE_PGA_gain[i] = ui->comboBoxPGAGain->currentIndex();
          this->AFE_LNA_integrator_enabled[i] = ui->checkBoxLNAIntegrator->isChecked();
          this->AFE_PGA_integrator_enabled[i] = ui->checkBoxPGAIntegrator->isChecked();
          this->AFE_active_termination_enabled[i] = ui->checkBoxActiveTerminationEnable->isChecked();
          this->AFE_active_termination_value[i] = ui->comboBoxImpedance->currentIndex();
          this->AFE_LPF[i] = ui->comboBoxLPF->currentIndex();
          this->AFE_LNA_clamp[i] = ui->comboBoxLNAClampLevel->currentIndex();
          this->AFE_PGA_clamp[i] = ui->comboBoxPGAClampLevel->currentIndex();
        }
      }else{
        int i = ui->comboBoxAFE->currentText().toInt();
        this->setAFEConfiguration(ui->comboBoxAFE->currentText());
        this->AFE_LNA_gain[i] = ui->comboBoxLNAGain->currentIndex();
        this->AFE_PGA_gain[i] = ui->comboBoxPGAGain->currentIndex();
        this->AFE_LNA_integrator_enabled[i] = ui->checkBoxLNAIntegrator->isChecked();
        this->AFE_PGA_integrator_enabled[i] = ui->checkBoxPGAIntegrator->isChecked();
        this->AFE_active_termination_enabled[i] = ui->checkBoxActiveTerminationEnable->isChecked();
        this->AFE_active_termination_value[i] = ui->comboBoxImpedance->currentIndex();
        this->AFE_LPF[i] = ui->comboBoxLPF->currentIndex();
        this->AFE_LNA_clamp[i] = ui->comboBoxLNAClampLevel->currentIndex();
        this->AFE_PGA_clamp[i] = ui->comboBoxPGAClampLevel->currentIndex();
      }
    }catch(serialException &serial_exception){
      serial_exception.handleException(this);
      return;
    }

}

void MainWindow::setAFEConfiguration(const QString &afe_number){

  QString command = "WR AFE ";
  command = command + afe_number;
  command = command + " REG 52 V ";
  command = command + QString::number(this->reg_52_value);
  command = command + "\r\n";
  this->Message("Writing on R52: bin" + QString::number(this->reg_52_value, 2) + " :: hex: " + QString::number(this->reg_52_value, 16),0);
  this->sendCommand(command);

  command = "WR AFE ";
  command = command + afe_number;
  command = command + " REG 51 V ";
  command = command + QString::number(this->reg_51_value);
  command = command + "\r\n";
  this->Message("Writing on R51: bin" + QString::number(this->reg_51_value, 2) + " :: hex: " + QString::number(this->reg_51_value, 16),0);
  this->sendCommand(command);

  command = "WR AFE ";
  command = command + afe_number;
  command = command + " REG 4 V ";
  command = command + QString::number(this->reg_4_value);
  command = command + "\r\n";
  this->Message("Writing on R4: bin" + QString::number(this->reg_4_value, 2) + " :: hex: " + QString::number(this->reg_4_value, 16),0);
  this->sendCommand(command);

  command = "WR AFE ";
  command = command + afe_number;
  command = command + " REG 1 V ";
  command = command + QString::number(this->reg_1_value);
  command = command + "\r\n";
  this->Message("Writing on R1: bin" + QString::number(this->reg_1_value, 2) + " :: hex: " + QString::number(this->reg_1_value, 16),0);
  this->sendCommand(command);

  command = "WR AFE ";
  command = command + afe_number;
  command = command + " REG 21 V ";
  command = command + QString::number(this->reg_21_value);
  command = command + "\r\n";
  this->Message("Writing on R21: bin" + QString::number(this->reg_21_value, 2) + " :: hex: " + QString::number(this->reg_21_value, 16),0);
  this->sendCommand(command);

  command = "WR AFE ";
  command = command + afe_number;
  command = command + " REG 33 V ";
  command = command + QString::number(this->reg_33_value);
  command = command + "\r\n";
  this->Message("Writing on R33: bin" + QString::number(this->reg_33_value, 2) + " :: hex: " + QString::number(this->reg_33_value, 16),0);
  this->sendCommand(command);

  command = "WR AFE ";
  command = command + afe_number;
  command = command + " REG 59 V ";
  command = command + QString::number(this->reg_59_value);
  command = command + "\r\n";
  this->Message("Writing on R59: bin" + QString::number(this->reg_59_value, 2) + " :: hex: " + QString::number(this->reg_59_value, 16),0);
  this->sendCommand(command);

}

void MainWindow::setOFFSETGain(const int &i){

  if(ui->checkBoxAllAFEChannelOffsetGAIN->isChecked()){
    QString command = "CFG OFFSET CH ";
    command = command + QString::number(i);
    command = command + " GAIN ";
    command = command + QString::number(2);
    command = command + "\r\n";
    this->sendCommand(command);
  }else{
    QString command = "CFG OFFSET CH ";
    command = command + QString::number(i);
    command = command + " GAIN ";
    command = command + QString::number(1);
    command = command + "\r\n";
    this->sendCommand(command);
  }
}

void MainWindow::pushButtonApplyOffsetPressed(){
  try{
    if(ui->checkBoxAllCHOffset->isChecked()){
      for(int i = 0; i < 40; i++){
        this->setOFFSETGain(i);
        QString command = "WR OFFSET CH ";
        command = command + QString::number(i);
        command = command + " V ";
        command = command + QString::number(ui->spinBoxOffsetVoltage->value());
        command = command + "\r\n";
        this->sendCommand(command);
        this->OFFSETSetValue[i] = ui->spinBoxOffsetVoltage->value();
      }
    }else{
      if(ui->checkBoxAllAFEChannelOffset->isChecked()){
        for(int i = 0; i < ui->comboBoxChannel->count(); i++){
          this->setOFFSETGain(ui->comboBoxChannel->itemText(i).toInt());
          QString command = "WR OFFSET CH ";
          command = command + ui->comboBoxChannel->itemText(i);
          command = command + " V ";
          command = command + QString::number(ui->spinBoxOffsetVoltage->value());
          command = command + "\r\n";
          this->sendCommand(command);
          this->OFFSETSetValue[ui->comboBoxChannel->itemText(i).toInt()] = ui->spinBoxOffsetVoltage->value();
        }
      }else{
        this->setOFFSETGain(ui->comboBoxChannel->currentText().toInt());
        QString command = "WR OFFSET CH ";
        command = command + ui->comboBoxChannel->currentText();
        command = command + " V ";
        command = command + QString::number(ui->spinBoxOffsetVoltage->value());
        command = command + "\r\n";
        this->sendCommand(command);
        this->OFFSETSetValue[this->currentChannel] = ui->spinBoxOffsetVoltage->value();
      }
    }
  }catch(serialException &e){
    e.handleException(this);
  }
}

void MainWindow::pushButtonApplyVGAINPressed(){
  try{
    if(ui->checkBoxAllAFEVgain->isChecked()){
      for(int i = 0; i < ui->comboBoxAFE->count(); i++){
        QString command = "WR AFE ";
        command = command + ui->comboBoxAFE->itemText(i);
        command = command + " VGAIN V ";
        command = command + QString::number(this->calculateVGainReferenceValue());
        command = command + "\r\n";
        this->sendCommand(command);
        this->VGAINSetValue[i] = ui->spinBoxVGainValues->value();
      }
    }else{
      QString command = "WR AFE ";
      command = command + ui->comboBoxAFE->currentText();
      command = command + " VGAIN V ";
      command = command + QString::number(this->calculateVGainReferenceValue());
      command = command + "\r\n";
      this->sendCommand(command);
      this->VGAINSetValue[this->currentAFE] = ui->spinBoxVGainValues->value();
    }
  }catch(serialException &e){
    e.handleException(this);
  }
}

int MainWindow::calculateVGainReferenceValue(){

    double vGain_value = ui->spinBoxVGainValues->value();
    vGain_value = ((2.49 + 1.5)/1.5)*vGain_value;
    return (int)(vGain_value*1000.0);
}

int MainWindow::calculateVGainReferenceValue(const double vgain_volts){

    double vGain_value = ((2.49 + 1.5)/1.5)*vgain_volts;
    return (int)(vGain_value*1000.0);
}

int MainWindow::calculateBIASReferenceValue(){

    double bias_value = ui->spinBoxBiasVoltage->value();
    bias_value = bias_value + this->biasOFFSETValue.at(this->currentAFE);
    bias_value = 1000*bias_value/39.314;
    return (int)(bias_value);
}

int MainWindow::calculateBIAS_CTRLReferenceValue(){

    double biascontrol_value = this->biasControlValue;
    biascontrol_value = 1000*biascontrol_value/74;
    return (int)(biascontrol_value);
}

uint16_t MainWindow::eraseAndApplyMask(uint16_t &reg, uint16_t &mask, uint16_t &eraser){
    uint16_t value_reg,eraser_;
    eraser_ = ~eraser;
    value_reg = reg & eraser_;
    value_reg = value_reg | mask;
    return value_reg;
}

uint16_t MainWindow::getCheckBoxLNAIntegratorMask(){
    if(ui->checkBoxLNAIntegrator->isChecked()){
        return MASK_LNA_INTEGRATOR_EN_REG_52;
    }else{
        return MASK_LNA_INTEGRATOR_DIS_REG_52;
    }
}

uint16_t MainWindow::getCheckBoxPGAIntegratorMask(){
    if(ui->checkBoxPGAIntegrator->isChecked()){
        return MASK_PGA_INTEGRATOR_EN_REG_51;
    }else{
        return MASK_PGA_INTEGRATOR_DIS_REG_51;
    }
}

uint16_t MainWindow::getCheckBoxActiveTerminationMask(){
    if(ui->checkBoxActiveTerminationEnable->isChecked()){
        return MASK_ACTIVE_TERMINATION_EN_REG_52;
    }else{
        return MASK_ACTIVE_TERMINATION_DIS_REG_52;
    }
}

uint16_t MainWindow::getLNAGainMask(){
    int pos = ui->comboBoxLNAGain->currentIndex();
    uint16_t mask;
    switch(pos){
        case 0:
            mask = MASK_LNA_GAIN_12DB_REG_52;
            break;
        case 1:
            mask = MASK_LNA_GAIN_18DB_REG_52;
            break;
        case 2:
            mask = MASK_LNA_GAIN_24DB_REG_52;
            break;
        default:
            mask = 0;
            break;
    }
    return mask;
}

uint16_t MainWindow::getPGAClampLevelMask(){
    int pos = ui->comboBoxPGAClampLevel->currentIndex();
    uint16_t mask;
    switch(pos){
        case 0:
            mask = MASK_PGA_CLAMP_LEVEL_N2DBFS_REG_51;
            break;
        case 1:
            mask = MASK_PGA_CLAMP_LEVEL_0DBFS_REG_51;
            break;
        case 2:
            mask = MASK_PGA_CLAMP_LEVEL_DISABLED_REG_51;
            break;
        default:
            mask = 0;
            break;
    }
    return mask;
}

uint16_t MainWindow::getLNAClampLevelMask(){
    int pos = ui->comboBoxLNAClampLevel->currentIndex();
    uint16_t mask;
    switch(pos){
        case 0:
            mask = MASK_LNA_CLAMP_LEVEL_AUTO_REG_52;
            break;
        case 1:
            mask = MASK_LNA_CLAMP_LEVEL_15VPP_REG_52;
            break;
        case 2:
            mask = MASK_LNA_CLAMP_LEVEL_115VPP_REG_52;
            break;
        case 3:
            mask = MASK_LNA_CLAMP_LEVEL_06VPP_REG_52;
            break;
        default:
            mask = 0;
            break;
    }
    return mask;
}

uint16_t MainWindow::getPGAGainMask(){
    int pos = ui->comboBoxPGAGain->currentIndex();
    uint16_t mask;
    switch(pos){
        case 0:
            mask = MASK_PGA_GAIN_24DB_CONTROL_REG_51;
            break;
        case 1:
            mask = MASK_PGA_GAIN_30DB_CONTROL_REG_51;
            break;
        default:
            mask = 0;
            break;
    }
    return mask;
}

uint16_t MainWindow::getLPFMask(){
    int pos = ui->comboBoxLPF->currentIndex();
    uint16_t mask;
    switch(pos){
        case 0:
            mask = MASK_LPF_10MHZ_PROGRAMABILITY_REG_51;
            break;
        case 1:
            mask = MASK_LPF_15MHZ_PROGRAMABILITY_REG_51;
            break;
        case 2:
            mask = MASK_LPF_20MHZ_PROGRAMABILITY_REG_51;
            break;
        case 3:
            mask = MASK_LPF_30MHZ_PROGRAMABILITY_REG_51;
        break;
        default:
            mask = 0;
            break;
    }
    return mask;
}

uint16_t MainWindow::getImpedanceMask(){
    int pos = ui->comboBoxImpedance->currentIndex();
    uint16_t mask;
    switch(pos){
        case 0:
            mask = MASK_PRESET_ACTIVE_TERMINATION_50OHMS_REG_52;
            break;
        case 1:
            mask = MASK_PRESET_ACTIVE_TERMINATION_100OHMS_REG_52;
            break;
        case 2:
            mask = MASK_PRESET_ACTIVE_TERMINATION_200OHMS_REG_52;
            break;
        case 3:
            mask = MASK_PRESET_ACTIVE_TERMINATION_400OHMS_REG_52;
        break;
        default:
            mask = 0;
            break;
    }
    return mask;
}

void MainWindow::pushButtonConnectPressed(){
    QString portName = ui->comboBoxAvailableSerialPort->currentText();
    int baudRate = ui->spinBoxBaudRate->value();
    this->serialPort_->setPortName(portName);
    this->serialPort_->setBaudRate(baudRate);
    if (this->serialPort_->open(QIODevice::ReadWrite)) {
        QMessageBox::information(this, tr("Success"), "Successfully opened port: " + this->serialPort_->portName());
        ui->pushButtonConnect->setEnabled(false);
        ui->pushButtonDisconnect->setEnabled(true);
    } else {
        QMessageBox::critical(this, tr("Error"), this->serialPort_->errorString());
    }
}

void MainWindow::pushButtonDisconnectPressed(){
    //qDebug()<<__func__;
    if (this->serialPort_->isOpen()){
        //qDebug()<<__func__<< " line:" <<__LINE__;
        this->serialPort_->close();
        ui->pushButtonDisconnect->setEnabled(false);
        ui->pushButtonConnect->setEnabled(true);
    }
}

void MainWindow::comboBoxAvailableSerialPortsValueChanged(){
    if(!this->serialPort_->isOpen())
        ui->pushButtonConnect->setEnabled(true);
}

void MainWindow::pushButtonRefreshPressed(){
    ui->comboBoxAvailableSerialPort->clear();
    this->populateComboBoxAvailableSerialPorts();
}

void MainWindow::Message(QString message, int msgCode){

    QString msg;
    QTextCursor cursor_ = ui->textEditMessages->textCursor();
    QTextCharFormat textFormat;
    switch(msgCode){
    case 0:
        msg +=message + "\n";
        textFormat.setForeground(Qt::darkGreen);
        cursor_.insertText(msg,textFormat);
        ui->textEditMessages->setTextCursor(cursor_);
        break;
    case 1:
        msg += message + "\n";
        textFormat.setForeground(Qt::darkBlue);
        cursor_.insertText(msg,textFormat);
        ui->textEditMessages->setTextCursor(cursor_);
        break;
    case 2:
        msg += "Internal GUI error: " + message + "\n";
        textFormat.setForeground(Qt::darkRed);
        cursor_.insertText(msg,textFormat);
        ui->textEditMessages->setTextCursor(cursor_);
        break;
    case 3:
        msg += "Internal GUI Warning: " + message + "\n";
        textFormat.setForeground(Qt::darkYellow);
        cursor_.insertText(msg,textFormat);
        ui->textEditMessages->setTextCursor(cursor_);
        break;
    default:
    break;
    }
}

void MainWindow::delay(int delay_){
    QTime dieTime = QTime::currentTime().addSecs(delay_);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::delayMilli(const int &delay_milli){
    QTime dieTime = QTime::currentTime().addMSecs(delay_milli);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::delayMicro(const int &delay_micro){
  std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto dieTime = std::chrono::duration_cast<std::chrono::microseconds>(duration).count() + delay_micro;
  while(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::readDataFromSerial(){
    QByteArray serial_data_ = this->serialPort_->readAll();
    this->serialData.append(serial_data_);
    this->serial_data_string = QString::fromUtf8(serial_data_.data(),serial_data_.size());
    this->serial_data_string_success = this->serial_data_string_success + this->serial_data_string;
    this->Message(this->serial_data_string ,1);
    this->serialTimeoutTimer.stop();
    this->waitingForData.quit();
}

void MainWindow::sendDataFromSerial(const QString &send_data){
    QByteArray send_data_byte_array = send_data.toUtf8();
    //qDebug() <<"send_data: " <<send_data_byte_array.toHex();
    this->serialPort_->write(send_data_byte_array);
}

bool MainWindow::sendCommand_(const QString &command){
  try{
    if(!this->serialPort_->isOpen()){
      throw(serialException(1));
    }
    this->serialData.clear();
    this->Message(command,0);
    this->sendDataFromSerial(command);
    this->waitForResponse(5); //throws 2
    if(!this->receivingSerialDataFlag){
       throw(serialException(2));
    }
    this->delayMilli(750);
    return true;
    }catch(serialException &serial_exception){
      serial_exception.handleException(this);
      return false;
    }
}

void MainWindow::sendCommand(const QString &command){

  this->serial_data_string_success = "";
  if(!this->sendCommand_(command)){
    throw(serialException(3));
  }
  // reapeat if not success
  int counter_max_retry = 0;
  while(!this->isSerialCommandSuccesful()){
    this->Message("Command Failed: retry n. " + QString::number(counter_max_retry),2);
    if(counter_max_retry == 4){
      throw(serialException(4));
    }else{
      this->serial_data_string_success = "";
      if(!this->sendCommand_(command)){
        throw(serialException(3));
      }
      counter_max_retry++;
    }
  }

}

bool MainWindow::isSerialCommandSuccesful(){
  QString str_success = this->parseSerialDataStringSuccess();
  int is_success = str_success.compare("success");
  //qDebug() << is_success;
  int is_updated = str_success.compare("updated");
  //qDebug() << is_updated;
  if(((is_success) == 0) || ((is_updated) == 0)){
    //qDebug() << "success";
    return true;
  }else{
    //qDebug() << "error";
    return false;
  }
}

QString MainWindow::parseSerialDataStringSuccess(){

  QStringList str_sucess_list = this->serial_data_string_success.split("...");
  QString str_sucess = str_sucess_list.last();
  str_sucess_list = str_sucess.split("\r");
  str_sucess = str_sucess_list.first();
  str_sucess_list = str_sucess.split(" ");
  str_sucess = str_sucess_list.last();
  return str_sucess;
}

void MainWindow::pushButtonInitialPressed(){
    QString command = "WR AFE ";
    command = command + ui->comboBoxChannel->currentText();
    command = command + " INITIAL\r\n";
    this->sendCommand(command);
}

void MainWindow::readAndPlotDataSerial(){
    QString command = "RD FPGA\r\n";
    try{
      this->sendCommand(command);
      this->dialogReadoutChannelWindow->show();
      this->dialogReadoutChannelWindow->plotData(this->serialData, this->reg_4_value);
    }
    catch(serialException &e){
      e.handleException(this);
    }
}

void MainWindow::mainWaveformsAcquisition(){
    int sampling_iterations = ui->spinBoxMultipleWaveforms->value();
    this->saveThread.createFileNames(this->multiple_waveforms_folder_address,this->channelsEnabledState);
    int lost_datagram_counter = 0;
    this->dialogReadoutChannelWindow->show();
    this->socket->startUdpThread();
    this->saveThread.startSaveThread(ui->checkBoxSaveWaveforms->isChecked());
    float duration1 = 0, duration2 = 0;
    for(int i = 0; i< sampling_iterations; i++){
      if(this->dialogReadoutChannelWindow->getWindowStatus() == false){
        break;
      }
        this->acquireWaveformEnabled();
        if(this->received_datagrams == this->expected_datagrams){
        #ifdef QT_DEBUG
            auto timing_start = std::chrono::high_resolution_clock::now();
        #endif
            if(i%this->dialogReadoutChannelWindow->getSpinBoxPlotEveryWaveformsValue() == 0){
                this->dialogReadoutChannelWindow->setWaveNumberPointer(&i);
                this->dialogReadoutChannelWindow->plotDataMultichannel();
                this->delayMicro(this->udpWaitforDatagram);
            }
        lost_datagram_counter = 0;
        #ifdef QT_DEBUG
            //qDebug()<< "********** Timing acquisition *******************";
            auto timing_stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timing_stop - timing_start);
            duration1 = duration1 + duration.count();
            qDebug()<< "Function this->dialogReadoutChannelWindow->plotDataMultichannel(this->channelsData,channel) took: " << duration1/(i+1) << " us";
            timing_start = std::chrono::high_resolution_clock::now();
        #endif
          if(ui->checkBoxMultipleWaveformsContinous->isChecked()){
            i--;
          }
          if(ui->checkBoxSaveWaveforms->isChecked()){
            this->saveMutex.lock();
            this->channelsData_queue.enqueue(this->channelsData);
            this->saveMutex.unlock();
          }
        #ifdef QT_DEBUG
            //qDebug()<< "********** Timing acquisition *******************";
            timing_stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(timing_stop - timing_start);
            duration2 = duration2 + duration.count();
            qDebug()<< "Function this->dialogReadoutChannelWindow->saveMultiChannel(i,this->channelsData, this->saveFormat); took: " << duration2/(i+1) << " us";
            timing_start = std::chrono::high_resolution_clock::now();
        #endif

        }else if(this->received_datagrams == -99){
            this->Message("Recieved datagram error code -99: Posible lost connection to DAPHNE",2);
        }else{
          if(i >= 0){
            i--;
            lost_datagram_counter++;
            this->Message("Lost datagrams, please consider incresing the Datagram Wait Time DWT\nConfiguration->Ethernet",3);
          }
        }
        if(lost_datagram_counter > 20){
            this->Message("Too many lost datagrams, please consider incresing the Datagram Wait Time DWT\nConfiguration->Ethernet\nAborting..",2);
            break;
        }
        this->expected_datagrams = 0;
    }
    this->socket->stopUdpThread();
    this->saveThread.stopSaveThread();
}

void MainWindow::offsetSweepWaveformsAcquisitions(){
    int start_value = ui->spinBoxChannelOffsetSweepStartValue->value();
    int end_value = ui->spinBoxChannelOffsetSweepEndValue->value();
    int step = ui->spinBoxChannelOffsetSweepStep->value();
    bool break_flag = false;
    for(int i = start_value;i <= (end_value + step); i+= step){
        if(i > end_value){
            i = end_value;
            break_flag = true;
        }
        ui->spinBoxOffsetVoltage->setValue(i);
        this->pushButtonApplyOffsetPressed();
        this->acquireWaveform();
        this->acquireWaveform();
        this->dialogReadoutChannelWindow->saveContinousWaveform(this->multiple_waveforms_folder_address,i);
        if(break_flag){
            break;
        }
    }
}

void MainWindow::enabledFunctionsDuringDatataking(const bool &enable){
    ui->pushButtonRDFPGA->setEnabled(enable);
    ui->checkBoxSaveWaveforms->setEnabled(enable);
    ui->checkBoxEnableEthernet->setEnabled(enable);
    this->dialogReadoutChannelWindow->getOKButtonPointer()->setEnabled(enable);
    ui->actionI_V_Curve->setEnabled(enable);
    ui->actionTrigger->setEnabled(enable);
    ui->actionAFE->setEnabled(enable);
}

void MainWindow::pushButtonRDFPGAPressed(){
    try{
        this->enabledFunctionsDuringDatataking(false);
        this->dialogReadoutChannelWindow->setWindowStatus(true);
        if(!ui->checkBoxEnableEthernet->isChecked()){
            throw ethernetUDPException(3);
        }
        if(ui->checkBoxEnableChannelOffsetSweep->isChecked()){
            this->offsetSweepWaveformsAcquisitions();
        }else{
            if(ui->checkBoxSaveWaveforms->isChecked() && ui->checkBoxMultipleWaveformsContinous->isChecked()){
                int ret = QMessageBox::warning(this, tr("Warning"),
                                               tr("Checkbox Save and Continous are checked.\nData will be saved until the user cancels datataking.\n"
                                                  "Do you still want to proceed?"),
                                               QMessageBox::No | QMessageBox::Yes,
                                               QMessageBox::No);
                switch (ret) {
                case QMessageBox::Yes:
                    this->mainWaveformsAcquisition();
                    this->dialogReadoutChannelWindow->show();
                    break;
                default:
                    //do nothing
                    break;
                }
            }else{
                this->mainWaveformsAcquisition();
                this->dialogReadoutChannelWindow->show();
            }
        }
        this->enabledFunctionsDuringDatataking(true);
    }catch(ethernetUDPException &e){
        e.handleException(this);
        this->enabledFunctionsDuringDatataking(true);
    }
}

void MainWindow::sendFPGAReset(){
    QString command = "CFG FPGA RESET\r\n";
    this->sendCommand(command);
}

void MainWindow::pushButtonApplyBiasVoltages(){
  try{
    if(ui->checkBoxOnlyTrim->isChecked()){
      if((ui->spinBoxTrim->value()/1000.0) > ui->spinBoxBiasVoltage->value()){
          throw biasVoltageException(1);
      }else{
          QString command = "WR TRIM CH ";
          command = command + ui->comboBoxChannel->currentText();
          command = command + " V ";
          command = command + QString::number(ui->spinBoxTrim->value());
          command = command + "\r\n";
          this->sendCommand(command);
          this->trimSetValue[this->currentChannel] = (uint16_t)ui->spinBoxTrim->value();
      }
    }else{
      //QString command = "WR VBIASCTRL V 1100\r\n";
      //this->sendCommand(command);
      if((ui->spinBoxTrim->value()/1000.0) > ui->spinBoxBiasVoltage->value()){
          throw biasVoltageException(1);
      }else if(ui->spinBoxBiasVoltage->value() > this->biasControlValue){
          throw biasVoltageException(2);
      }else{
          if(ui->spinBoxBiasVoltage->value() == 0.0){
              QString command = "WR TRIM CH ";
              command = command + ui->comboBoxChannel->currentText();
              command = command + " V ";
              command = command + QString::number(ui->spinBoxTrim->value());
              command = command + "\r\n";
              this->sendCommand(command);
              this->trimSetValue[this->currentChannel] = (uint16_t)ui->spinBoxTrim->value();
              command = "WR AFE ";
              command = command + ui->comboBoxAFE->currentText();
              command = command + " BIASSET V ";
              command = command + QString::number(0);
              command = command + "\r\n";
              this->sendCommand(command);
              this->biasSetValue[this->currentAFE] = ui->spinBoxBiasVoltage->value();
          }else{
              QString command = "WR AFE ";
              command = command + ui->comboBoxAFE->currentText();
              command = command + " BIASSET V ";
              command = command + QString::number(this->calculateBIASReferenceValue());
              command = command + "\r\n";
              this->sendCommand(command);
              this->biasSetValue[this->currentAFE] = ui->spinBoxBiasVoltage->value();
              command = "WR TRIM CH ";
              command = command + ui->comboBoxChannel->currentText();
              command = command + " V ";
              command = command + QString::number(ui->spinBoxTrim->value());
              command = command + "\r\n";
              this->sendCommand(command);
              this->trimSetValue[this->currentChannel] = (uint16_t)ui->spinBoxTrim->value();
          }
      }
    }
  }catch(serialException &e){
    e.handleException(this);
  }catch(biasVoltageException &e){
    e.handleException(this);
  }
}

void MainWindow::waitForResponse(uint timeout){
    this->receivingSerialDataFlag = true;
    serialTimeoutTimer.start(timeout*1000);
    this->waitingForData.exec();
}

void MainWindow::serialTimeoutTimerTriggered(){
    this->Message("Timeout waiting for DAPHNE serial response",2);
    this->receivingSerialDataFlag = false;
}

void MainWindow::populateComboBoxChannel(){
    ui->comboBoxChannel->clear();
    int afe = ui->comboBoxAFE->currentText().toInt();

    ui->comboBoxLNAGain->setCurrentIndex(this->AFE_LNA_gain[afe]);
    ui->comboBoxPGAGain->setCurrentIndex(this->AFE_PGA_gain[afe]);
    ui->checkBoxLNAIntegrator->setChecked(this->AFE_LNA_integrator_enabled[afe]);
    ui->checkBoxPGAIntegrator->setChecked(this->AFE_PGA_integrator_enabled[afe]);
    ui->checkBoxActiveTerminationEnable->setChecked(this->AFE_active_termination_enabled[afe]);
    ui->comboBoxImpedance->setCurrentIndex(this->AFE_active_termination_value[afe]);
    ui->comboBoxLPF->setCurrentIndex(this->AFE_LPF[afe]);
    ui->comboBoxLNAClampLevel->setCurrentIndex(this->AFE_LNA_clamp[afe]);
    ui->comboBoxPGAClampLevel->setCurrentIndex(this->AFE_PGA_clamp[afe]);

    switch(afe){
      case 0:
        for(int i=0;i<8;i++){
            ui->comboBoxChannel->addItem(QString::number(i));
        }
      break;
      case 1:
        for(int i=8;i<16;i++){
            ui->comboBoxChannel->addItem(QString::number(i));
        }
      break;
      case 2:
        for(int i=16;i<24;i++){
            ui->comboBoxChannel->addItem(QString::number(i));
        }
      break;
      case 3:
        for(int i=24;i<32;i++){
            ui->comboBoxChannel->addItem(QString::number(i));
        }
      break;
      case 4:
        for(int i=32;i<40;i++){
            ui->comboBoxChannel->addItem(QString::number(i));
        }
      break;
      default:
        for(int i=0;i<40;i++){
            ui->comboBoxChannel->addItem(QString::number(i));
        }
      break;
    }
}

int MainWindow::getAFENumberFromChannelNumber(const int &channelNumber){

  if(channelNumber >= 0 && channelNumber <8){
    return 0;
  }else if(channelNumber >= 8 && channelNumber <16){
    return 1;
  }else if (channelNumber >= 16 && channelNumber <24){
    return 2;
  }else if(channelNumber >= 24 && channelNumber <32){
    return 3;
  }else if(channelNumber >= 32 && channelNumber <40){
    return 4;
  }else{
    return -99;
  }

}

void MainWindow::populateComboBoxAFE(){
    for(int i=0;i<5;i++){
        ui->comboBoxAFE->addItem(QString::number(i));
    }
}

void MainWindow::checkBox2ComplementChecked(){
    if(ui->checkBox2Complement->isChecked()){
        ui->checkBox2Complement->setChecked(true);
        ui->checkBoxOffsetBinary->setChecked(false);
    }else{
        ui->checkBox2Complement->setChecked(false);
        ui->checkBoxOffsetBinary->setChecked(true);
    }
}

void MainWindow::checkBoxOffsetBinaryChecked(){
    if(ui->checkBoxOffsetBinary->isChecked()){
        ui->checkBox2Complement->setChecked(false);
        ui->checkBoxOffsetBinary->setChecked(true);
    }else{
        ui->checkBox2Complement->setChecked(true);
        ui->checkBoxOffsetBinary->setChecked(false);
    }
}

void MainWindow::checkBoxLSBFirstChecked(){
    if(ui->checkBoxLSBFirst->isChecked()){
        ui->checkBoxMSBFirst->setChecked(false);
        ui->checkBoxLSBFirst->setChecked(true);
    }else{
        ui->checkBoxMSBFirst->setChecked(true);
        ui->checkBoxLSBFirst->setChecked(false);
    }
}

void MainWindow::checkBoxMSBFirstChecked(){
    if(ui->checkBoxMSBFirst->isChecked()){
        ui->checkBoxMSBFirst->setChecked(true);
        ui->checkBoxLSBFirst->setChecked(false);
    }else{
        ui->checkBoxMSBFirst->setChecked(false);
        ui->checkBoxLSBFirst->setChecked(true);
    }
}

uint16_t MainWindow::getADCFormatMask(){
    uint16_t mask_format, mask_endian;
    if(ui->checkBox2Complement->isChecked()){
        mask_format = MASK_ADC_OUTPUT_FORMAT_2COMP_REG_4;
    }else{
        mask_format = MASK_ADC_OUTPUT_FORMAT_OFFSET_BIN_REG_4;
    }
    if(ui->checkBoxLSBFirst->isChecked()){
        mask_endian = MASK_LSB_FIRST_REG_4;
    }else{
        mask_endian = MASK_MSB_FIRST_REG_4;
    }
    return mask_format | mask_endian;
}

void MainWindow::pushButtonSendRawCommandPressed(){
  try{
    QString command = ui->lineEditSendRawCommand->text();
    command = command + "\r\n";
    this->sendCommand(command);
  }catch(serialException &e){
    e.handleException(this);
  }
}

void MainWindow::pushButtonMultipleWaveformsDirectoryPressed(){
    QString QFileDialog_returned_string = QFileDialog::getExistingDirectory(0, ("Select Output Folder"), this->multiple_waveforms_folder_address + "/..");
    if(!QFileDialog_returned_string.isEmpty()){
        this->multiple_waveforms_folder_address = QFileDialog_returned_string;
        ui->labelWorkingDirectory->setText("Working Directory: " + this->multiple_waveforms_folder_address);
    }
    //qDebug()<<this->multiple_waveforms_folder_address;
}

void MainWindow::pushButtonGETCONFIGPressed(){
    //do something
}

void MainWindow::checkBoxEnableEthernetPressed(){
 this->handleNewEthernetSocket();
}

void MainWindow::handleNewEthernetSocket(){
  try{
    if(this->ethernetCheckBoxCheckedFlag == false){
        this->socket = new DaphneSocket(this->computerIPAddr, this->daphneIPAddr, this->computerPortNumber, this->daphnePortNumber);
        this->Message(this->socket->getBindedToStr(),0);
        this->ethernetCheckBoxCheckedFlag = true;
        ui->menuAlignment->setEnabled(true);
        ui->actionEthernet->setEnabled(false);
    }else{
        this->ethernetCheckBoxCheckedFlag = false;
        this->socket->~DaphneSocket();
        ui->menuAlignment->setEnabled(false);
        ui->actionEthernet->setEnabled(true);
    }
  }catch(ethernetUDPException &e){
    e.handleException(this);
    //this->socket->~DaphneSocket();
    ui->checkBoxEnableEthernet->setChecked(false);
    ui->menuAlignment->setEnabled(false);
    this->ethernetCheckBoxCheckedFlag = false;
  }
}

void MainWindow::acquireWaveform(){
    if(ui->checkBoxEnableEthernet->isChecked()){
        int channel = ui->comboBoxChannel->currentText().toInt();
        this->sendSoftwareTrigger();
        this->sendSoftwareTriggerDeadTime();
        this->readAndPlotDataEthernet(channel);
        this->sendSoftwareTriggerDeadTime();
    }else{
        this->readAndPlotDataSerial();
    }
}

void MainWindow::acquireWaveformEnabled(){
  this->sendSoftwareTrigger();
  this->sendSoftwareTriggerDeadTime();
  this->readMultichannelEthernet_vector(this->channelsEnabledState);
  this->sendSoftwareTriggerDeadTime();
}

int MainWindow::requestDataFromChannel(const int &channel,const int &length, int &requested_data){

  try{
    int minimunDatagramSize = 182;
    int numberOfRequest = (int)(std::ceil(((float)length)/((float)minimunDatagramSize)));
    int lastRequestSize = length%minimunDatagramSize;
    int spyBuffer = this->getSpyBufferFromChannel(channel);
    int rec_dat = -99;
    for(int i = 0; i < numberOfRequest; i++){
      if(i == numberOfRequest - 1 && lastRequestSize != 0){
        this->socket->read(spyBuffer + i*minimunDatagramSize, lastRequestSize);
        requested_data++;
        //this->delayMicro(this->udpWaitforDatagram);
      }else{
        this->socket->read(spyBuffer + i*minimunDatagramSize,minimunDatagramSize);
        requested_data++;
        //this->delayMicro(this->udpWaitforDatagram);
      }

      if(requested_data%3 == 0 || requested_data%numberOfRequest == 0){
          while(rec_dat - requested_data != 0){
            rec_dat = this->socket->receivedDataLength();
          }
        }
    }
    return numberOfRequest;
  }catch(ethernetUDPException &e){
    e.handleException(this);
  }
}

void MainWindow::readAndPlotDataEthernet(const int &channel){
  try{
    this->socket->flushReceivedData();
    this->expected_datagrams = 0;
    this->socket->startUdpThread();
    this->requestDataFromChannel(channel,this->recordLength,this->expected_datagrams);
    this->socket->stopUdpThread();
    this->parseEthernetData();
    this->dialogReadoutChannelWindow->show();
    int channel = ui->comboBoxChannel->currentText().toInt();
    int wave_number = 0;
    this->dialogReadoutChannelWindow->setChannelForThreadedPlotting(&channel);
    this->dialogReadoutChannelWindow->setWaveNumberPointer(&wave_number);
    this->dialogReadoutChannelWindow->plotDataMultichannel();
    this->socket->flushReceivedData();
  }catch(ethernetUDPException &e){
    e.handleException(this);
  }
}

void MainWindow::readMultichannelEthernet_vector(const QVector<bool> &enabledChannels){

#ifdef QT_DEBUG
    auto timing_start = std::chrono::high_resolution_clock::now();
#endif
  this->received_datagrams = this->readChannelsEthernet(enabledChannels);

#ifdef QT_DEBUG
    //qDebug()<< "********** Timing acquisition *******************";
    auto timing_stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timing_stop - timing_start);
    //qDebug()<< "Function this->readChannelsEthernet(enabledChannels) took: " << duration.count() << " us";
    timing_start = std::chrono::high_resolution_clock::now();
#endif

  QVector<uint16_t> ethernetData_aux(this->recordLength);

  int k = 0;
  for(int ch = 0; ch < enabledChannels.length(); ch++){
    if(enabledChannels.at(ch)){
      for(int i = 0; i < this->recordLength; i++){
        if(k < this->ethernetData.length()){
          ethernetData_aux[i] = this->ethernetData.at(k);
        }
        k++;
      }
      this->channelsData[ch] = ethernetData_aux;
    }
  }
#ifdef QT_DEBUG
   timing_stop = std::chrono::high_resolution_clock::now();
   duration = std::chrono::duration_cast<std::chrono::microseconds>(timing_stop - timing_start);
   qDebug()<< "moving data to QVector took: " << duration.count() << " us";
#endif
}

int MainWindow::readChannelsEthernet(const QVector<bool> &enabledChannels){

  try{
#ifdef QT_DEBUG
    auto timing_start = std::chrono::high_resolution_clock::now();
    //qDebug()<< "----------- INSIDE readChannelsEthernet: ----------";
#endif
    this->expected_datagrams = 0;
    this->socket->flushReceivedData();
    for(int i = 0; i < enabledChannels.length(); i++){
      if(enabledChannels.at(i)){
        this->requestDataFromChannel(i,this->recordLength,this->expected_datagrams);
      }
    }
#ifdef QT_DEBUG
   auto timing_stop = std::chrono::high_resolution_clock::now();
   auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timing_stop - timing_start);
   qDebug()<< "requesting data took: " << duration.count() << " us";
   timing_start = std::chrono::high_resolution_clock::now();
#endif
   this->parseEthernetData();
#ifdef QT_DEBUG
   timing_stop = std::chrono::high_resolution_clock::now();
   duration = std::chrono::duration_cast<std::chrono::microseconds>(timing_stop - timing_start);
   qDebug()<< "parsing data took: " << duration.count() << " us\n -------------END----------------";
#endif
    return this->socket->getReceivedData()->length();
  }catch(ethernetUDPException &e){
    e.handleException(this);
    return -99; //this could cause problems if not handled correctly
  }
}

void MainWindow::parseEthernetData(){
    this->ethernetData.clear();
    for(QByteArray &data : *this->socket->getReceivedData()){
        uint16_t *u16_data = reinterpret_cast<uint16_t*>(data.begin());
        for(int i = 0; i<(data.length())/2;i++){
            this->ethernetData.append(u16_data[i]);
        }
    }
}

int MainWindow::getSpyBufferFromChannel(const int &channel){
  int spyBuffer = 0x40000000;
  int afe = 0;
  if(channel >= 0 && channel< 8){
    afe = 0;
    spyBuffer = spyBuffer + 0x100000*afe + 0x10000*(channel%8);
  }else if(channel >= 8 && channel< 16){
    afe = 1;
    spyBuffer = spyBuffer + 0x100000*afe + 0x10000*(channel%8);
  }else if(channel >= 16 && channel< 24){
    afe = 2;
    spyBuffer = spyBuffer + 0x100000*afe + 0x10000*(channel%8);
  }else if(channel >= 24 && channel< 32){
    afe = 3;
    spyBuffer = spyBuffer + 0x100000*afe + 0x10000*(channel%8);
  }else if(channel >= 32 && channel< 40){
    afe = 4;
    spyBuffer = spyBuffer + 0x100000*afe + 0x10000*(channel%8);
  }else{
    afe = 0;
    spyBuffer = spyBuffer + 0x100000*afe + 0x10000*(channel%8);
  }
  ////qDebug() << "spyBuffer :: " << QString::number(spyBuffer, 16);
  return spyBuffer;
}



void MainWindow::menuAlignmentPressed(){
    DialogAligment alignment(this);
    alignment.exec();
}

void MainWindow::menuEthernetConfigurationPressed(){
    DialogEthernetConfiguration ethernetConfig(this);
    ethernetConfig.setDaphneIpAddress(this->daphneIPAddr);
    ethernetConfig.setComputerIpAddress(this->computerIPAddr);
    ethernetConfig.setDaphnePortNumber(this->daphnePortNumber);
    ethernetConfig.setComputerPortNumber(this->computerPortNumber);
    ethernetConfig.setDWT(this->udpWaitforDatagram);
    int exec_code = ethernetConfig.exec();
    if(exec_code){
        //qDebug()<<__PRETTY_FUNCTION__<< "::Accepted";
        this->daphneIPAddr = ethernetConfig.getDaphneIpAddress();
        this->daphnePortNumber = ethernetConfig.getDaphnePortNumber();
        this->computerIPAddr = ethernetConfig.getComputerIpAddress();
        this->computerPortNumber = ethernetConfig.getComputerPortNumber();
        this->udpWaitforDatagram = ethernetConfig.getDWT();

        if(ui->checkBoxEnableEthernet->isChecked()){
            this->socket->~DaphneSocket();
            this->handleNewEthernetSocket();
        }else{
            //... do nothing
        }
    }else{
        //qDebug()<<__PRETTY_FUNCTION__<<"::Rejected";
    }
}

void MainWindow::menuAcquisitionConfigurationPressed(){
  DialogAcquisitionConfiguration acquisitionConfig(this);
  acquisitionConfig.setCheckBoxStates(this->channelsEnabledState);
  acquisitionConfig.setRecordLength(this->recordLength);
  acquisitionConfig.setCheckBoxSaveTextState(this->saveToTxtState);
  acquisitionConfig.setCheckBoxSaveBinaryState(this->saveToBinaryState);
  int exec_code = acquisitionConfig.exec();
  if(exec_code){
    //... accepted config
    this->channelsEnabledState = acquisitionConfig.getCheckBoxStates();
    this->recordLength = acquisitionConfig.getRecordLength();
    this->saveToTxtState = acquisitionConfig.getCheckBoxSaveTextState();
    this->saveToBinaryState = acquisitionConfig.getCheckBoxSaveBinaryState();
    this->saveFormat = this->saveToBinaryState;
    this->initializeChannelsData();
    this->dialogReadoutChannelWindow->setEthernetDataForThreadedPlotting(&this->channelsData);
  }else{
    //... rejected config
  }
}

void MainWindow::menuBiasPressed(){
  DialogBiasConfiguration biasConfig(this);
  biasConfig.setBiasControlValue(this->biasControlValue);
  biasConfig.setBiasOffsetValues(this->biasOFFSETValue);
  int exec_code = biasConfig.exec();
  if(exec_code){
    //... accepted config
    this->biasControlValue = biasConfig.getBiasControlValue();
    this->biasOFFSETValue = biasConfig.getBiasOffsetValues();
    // calculation of biascontrol voltage
    uint16_t biasControltoSet = (uint16_t)(this->calculateBIAS_CTRLReferenceValue());
    qDebug() << "bias control value " << this->biasControlValue << "V :: Value to set " << biasControltoSet;
    QString command = "WR VBIASCTRL V " + QString::number(biasControltoSet) + "\r\n";
    try{
        this->sendCommand(command);
    }catch(serialException &e){
      e.handleException(this);
    }
  }else{
    //... rejected config
  }
}

void MainWindow::menuAFEConfigurationPressed(){
  DialogAFEConfiguration afeConfig(this);
  afeConfig.setSpinBoxHPFKValue(this->digitalHPFKValue);
  afeConfig.setCheckBoxHPFEnabled(this->digitalHPFEnabled);
  afeConfig.setCheckBoxLFNSupressorEnabled(this->lowNoiseSupressionEnabled);
  afeConfig.setFPGAFilterEnabled(this->FPGAFilterEnabled);
  afeConfig.setFPGAFilterOutputValue(this->digitalFilterOutputSelector);
  afeConfig.setHPFLNAlevel(this->HPFLNAlevel);
  int exec_code = afeConfig.exec();
  if(exec_code){
    //... accepted config
    this->digitalHPFKValue = afeConfig.getSpinBoxHPFKValue();
    this->digitalHPFEnabled = afeConfig.getCheckBoxHPFEnabled();
    this->lowNoiseSupressionEnabled = afeConfig.getCheckBoxLFNSupressorEnabled();
    this->FPGAFilterEnabled = afeConfig.getFPGAFilterEnabled();
    this->digitalFilterOutputSelector = afeConfig.getFPGAFilterOutputValue();
    this->HPFLNAlevel = afeConfig.getHPFLNALevel();

    uint16_t digitalHPFKValue_register = this->digitalHPFKValue << 1;
    uint16_t lowNoiseSupressionEnabled_register = (uint16_t)this->lowNoiseSupressionEnabled << 11;
    uint16_t digitalHPFEnabled_register = (uint16_t)this->digitalHPFEnabled;
    uint16_t HPFLNAlevel_register = 0;

    switch(this->HPFLNAlevel){
        case 0:
            HPFLNAlevel_register = MASK_HPF_LNA_LEVEL_0_REG59;
            break;
        case 1:
            HPFLNAlevel_register = MASK_HPF_LNA_LEVEL_1_REG59;
            break;
        case 2:
            HPFLNAlevel_register = MASK_HPF_LNA_LEVEL_2_REG59;
            break;
        case 3:
            HPFLNAlevel_register = MASK_HPF_LNA_LEVEL_3_REG59;
            break;
        default:
            HPFLNAlevel_register = MASK_HPF_LNA_LEVEL_0_REG59;
            break;
    }

    uint16_t eraser = MASK_DIGITAL_HPF_FILTER_CH_1_4_K_VALUE_REG21;
    this->reg_21_value = this->eraseAndApplyMask(this->reg_21_value,digitalHPFKValue_register,eraser);

    eraser = MASK_DIGITAL_HPF_FILTER_CH_1_4_ENABLE_REG21;
    this->reg_21_value = this->eraseAndApplyMask(this->reg_21_value,digitalHPFEnabled_register,eraser);

    eraser = MASK_DIGITAL_HPF_FILTER_CH_5_8_K_VALUE_REG33;
    this->reg_33_value = this->eraseAndApplyMask(this->reg_33_value,digitalHPFKValue_register,eraser);

    eraser = MASK_DIGITAL_HPF_FILTER_CH_5_8_ENABLE_REG33;
    this->reg_33_value = this->eraseAndApplyMask(this->reg_33_value,digitalHPFEnabled_register,eraser);

    eraser = MASK_LOW_FREQ_NOISE_SUPR_REG_1;
    this->reg_1_value = this->eraseAndApplyMask(this->reg_1_value,lowNoiseSupressionEnabled_register,eraser);

    eraser = MASK_ERASER_HPF_LNA_REG59;
    this->reg_59_value = this->eraseAndApplyMask(this->reg_59_value,HPFLNAlevel_register,eraser);

    this->Message("AFE registers changes will take effect after SET CONFIG button is pressed.",3);

    if(ui->checkBoxEnableEthernet->isChecked()){
      uint8_t fpgaOutputValue = afeConfig.getFPGAFilterOutputValue();
      if(this->FPGAFilterEnabled){
        this->socket->sendSingleCommand(0x2023,0x1 | (fpgaOutputValue << 1));
      }else{
        this->socket->sendSingleCommand(0x2023,0x0 | (fpgaOutputValue << 1));
      }
    }else{
      this->displayWarningMessageBox("FPGA filter was not configured.\nPlease ensure there is a valid Ethernet connection.");
    }

  }else{
    //... rejected config
  }
}

DaphneSocket* MainWindow::getSocket(){
    return this->socket;
}

void MainWindow::displayErrorMessageBox(const QString &msg){
  QMessageBox::critical(this, tr("Error"), msg);
}

void MainWindow::displayWarningMessageBox(const QString &msg){
  QMessageBox::warning(this, tr("Warning"), msg);
}

QString MainWindow::getSerialString(){
  return this->serial_data_string;
}

void MainWindow::menuIVCurvePressed(){
  DialogIVcurve IVcurve(this);
  IVcurve.setWorkingFolder(this->multiple_waveforms_folder_address);
  IVcurve.exec();
}

void MainWindow::menuTriggerPressed(){
  TriggerMenuDialog triggerDialog(this);
  triggerDialog.setTriggerSource(this->triggerSource);
  triggerDialog.setTriggerLevel(this->triggerLevel);
  triggerDialog.setTriggerChannel(this->triggerChannel);
  triggerDialog.setTriggerEnableChannel(this->channelsEnabledState);
  triggerDialog.setMultiplierValue(this->preTriggerMultiplier);
  int exec_code = triggerDialog.exec();
  if(exec_code){
    //... accepted config
    this->triggerSource = triggerDialog.getTriggerSource();
    this->triggerChannel = triggerDialog.getTriggerChannel();
    this->triggerLevel = triggerDialog.getTriggerLevel();
    this->preTriggerMultiplier = triggerDialog.getMultiplierValue();
  }else{
    //... rejected config
  }
}

void MainWindow::sendSoftwareTrigger(){
  if(this->triggerSource[2] == true){
    this->socket->sendSoftwareTrigger();
  }
}

void MainWindow::sendSoftwareTriggerDeadTime(){
    this->socket->sendSoftwareTriggerDeadTime();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "DAPHNE GUI",
                                                                tr("Are you sure?\n Please consider that Bias Voltages will no be set back to zero"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
        qApp->quit();
    }
}

int MainWindow::getNumberOfExpectedDatagrams(){
  int numberOfRequest = (int)(this->recordLength/128);
  int lastRequestSize = this->recordLength%128;

  int counter_channels = 0;

  for(bool &enabled_ch : this->channelsEnabledState){
    if(enabled_ch){
      counter_channels++;
    }
  }

  if(lastRequestSize != 0){
    numberOfRequest++;
  }
  return numberOfRequest*counter_channels;
}

void MainWindow::initializeChannelsData(){
    QVector<uint16_t> init_value;
    init_value.resize(this->recordLength);
    for(int i = 0; i < init_value.length(); i++){
        init_value[i] = 0.0;
    }
    for(QVector<uint16_t> &channel_: this->channelsData){
        channel_ = init_value;
    }
}

void MainWindow::checkBoxSaveWaveformsClicked(){
    if(this->multiple_waveforms_folder_address.isEmpty()){
        this->displayErrorMessageBox("Please select a valid directory to save waveforms.");
        ui->checkBoxSaveWaveforms->setChecked(false);
    }
}

void MainWindow::comboBoxChannelTextChanged(){
    this->currentChannel = ui->comboBoxChannel->currentText().toInt();
    this->currentAFE = ui->comboBoxAFE->currentText().toInt();
    //populate current bias and trim configurations
    ui->spinBoxBiasVoltage->setValue(this->biasSetValue[this->currentAFE]);
    ui->spinBoxTrim->setValue(this->trimSetValue[this->currentChannel]);
    ui->spinBoxOffsetVoltage->setValue(this->OFFSETSetValue[this->currentChannel]);
    ui->spinBoxVGainValues->setValue(this->VGAINSetValue[this->currentAFE]);
}

const QCheckBox *MainWindow::getEthernetCheckboxPointer(){
    return ui->checkBoxEnableEthernet;
}

QVector<double> MainWindow::getBIASOffsetValues(){
    return this->biasOFFSETValue;
}

double MainWindow::getBiasControlValue(){
    return this->biasControlValue;
}

void MainWindow::saveConfigurationString(){
    this->configurationString = "******Configuration******\n";
    this->configurationString = this->configurationString + "\n***AFE registers values***\n";
    this->configurationString = this->configurationString + "reg_1_value: " + QString::number(this->reg_1_value) + "\n";
    this->configurationString = this->configurationString + "reg_21_value: " + QString::number(this->reg_21_value) + "\n";
    this->configurationString = this->configurationString + "reg_33_value: " + QString::number(this->reg_33_value) + "\n";
    this->configurationString = this->configurationString + "reg_4_value: " + QString::number(this->reg_4_value) + "\n";
    this->configurationString = this->configurationString + "reg_51_value: " + QString::number(this->reg_51_value) + "\n";
    this->configurationString = this->configurationString + "reg_52_value: " + QString::number(this->reg_52_value) + "\n";
    this->configurationString = this->configurationString + "reg_59_value: " + QString::number(this->reg_59_value) + "\n";

    this->configurationString = this->configurationString + "\n***WAVEFORMS settings***\n";
    this->configurationString = this->configurationString + "Number of waveforms: " + QString::number(ui->spinBoxMultipleWaveforms->value()) + "\n";
    this->configurationString = this->configurationString + "Samples per waveform: " + QString::number(this->recordLength) + "\n";
    if(this->saveToBinaryState)
        this->configurationString = this->configurationString + "Dataformat : uint16 IEEE little endian\n";
    if(this->saveToTxtState)
        this->configurationString = this->configurationString + "Dataformat : textfile \n Delimiter: Carriage Return\n";

    this->configurationString = this->configurationString + "\n***VGAIN settings***\n";
    int i = 0;
    for(double vgain_setting : this->VGAINSetValue){
        this->configurationString = this->configurationString + "AFE" + QString::number(i) + ": " + QString::number(vgain_setting) +
                "(" + QString::number(this->calculateVGainReferenceValue(vgain_setting)) + ")\n";
        i++;
    }

    this->configurationString = this->configurationString + "\n***OFFSET settings***\n";
    this->configurationString = this->configurationString + this->printVectorItems("OFFSET Values: ", this->OFFSETSetValue, 8);

    this->configurationString = this->configurationString + "\n***TRIGGER settings***\n";
    this->configurationString = this->configurationString + this->printVectorItems("Trigger Source (internal, external, software): ", this->triggerSource, 8);
    this->configurationString = this->configurationString + this->printVectorItems("Trigger levels : ", this->triggerLevel, 8);
    this->configurationString = this->configurationString + "Pre-trigger multiplier: " + QString::number(this->preTriggerMultiplier) + "\n";

    this->configurationString = this->configurationString + "\n***AFE settings***\n";
    this->configurationString = this->configurationString + this->printComboBoxConfigurationItems("LNA GAIN: ", this->AFE_LNA_gain, ui->comboBoxLNAGain);
    this->configurationString = this->configurationString + this->printComboBoxConfigurationItems("PGA GAIN: ", this->AFE_PGA_gain, ui->comboBoxPGAGain);
    this->configurationString = this->configurationString + this->printVectorItems("AFE Active Termination", this->AFE_active_termination_enabled, 8);
    this->configurationString = this->configurationString + this->printComboBoxConfigurationItems("AFE Active Termination Value: ", this->AFE_active_termination_value, ui->comboBoxImpedance);
    this->configurationString = this->configurationString + this->printVectorItems("LNA Integrators Enabled: ", this->AFE_LNA_integrator_enabled, 8);
    this->configurationString = this->configurationString + this->printVectorItems("PGA Integrators Enabled: ", this->AFE_PGA_integrator_enabled, 8);
    this->configurationString = this->configurationString + this->printComboBoxConfigurationItems("AFE LPF: ",this->AFE_LPF, ui->comboBoxLPF);
    this->configurationString = this->configurationString + this->printComboBoxConfigurationItems("AFE LNA Clamp Level: ", this->AFE_LNA_clamp, ui->comboBoxLNAClampLevel);
    this->configurationString = this->configurationString + this->printComboBoxConfigurationItems("AFE PGA Clamp Level: ", this->AFE_PGA_clamp, ui->comboBoxPGAClampLevel);
    this->configurationString = this->configurationString + "Low Noise Suppresion Enabled: " + QString::number(this->lowNoiseSupressionEnabled) + "\n";
    this->configurationString = this->configurationString + "FPGA Filter Enabled: " + QString::number(this->FPGAFilterEnabled) + "\n";
    this->configurationString = this->configurationString + "FPGA Filter Output Selector: " + QString::number(this->digitalFilterOutputSelector) + "\n";
    this->configurationString = this->configurationString + "Digital HPF Enabled: " + QString::number(this->digitalHPFEnabled) + "\n";
    this->configurationString = this->configurationString + "Digital HPF Value: " + QString::number(this->digitalHPFKValue) + "\n";
    this->configurationString = this->configurationString + "Digital HPF LNA Level: " + QString::number(this->HPFLNAlevel) + "\n";

    this->configurationString = this->configurationString + "\n***BIAS settings***\n";
    this->configurationString = this->configurationString + "Bias Control Value: " + QString::number(this->biasControlValue) + "\n";
    this->configurationString = this->configurationString + this->printVectorItems("Bias OFFSETS: ", this->biasOFFSETValue, 8);
    this->configurationString = this->configurationString + this->printVectorItems("Trim Values", this->trimSetValue, 8);

}

template <class T> QString MainWindow::printComboBoxConfigurationItems(const QString &title, const QVector<T> &vec, QComboBox* comboBox){
    QString str = "";
    str = str + title;
    str = str + "[";
    for(T vec_index : vec){
        str = str + comboBox->itemText(vec_index) + ", ";
    }
    str.remove(str.length()-2,2);
    str = str + "]\n";
    return str;
}

template <class T> QString MainWindow::printVectorItems(const QString &title, const QVector<T> &vec, const int &items_per_line){
    QString str = "";
    str = str + title;
    str = str + "[";
    int i = 0;
    for(T vec_index : vec){
        str = str + QString::number(vec_index) + ", ";
        i++;
        if(i == items_per_line){
            str = str + "\n";
        }
    }
    str.remove(str.length()-2,2);
    str = str + "]\n";
    return str;
}


