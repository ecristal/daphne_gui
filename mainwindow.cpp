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
    connect(ui->pushButtonInitial,SIGNAL(clicked(bool)),this,SLOT(pushButtonInitialPressed()));
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
}

MainWindow::~MainWindow()
{
    this->dialogReadoutChannelWindow->~DialogReadoutChannel();
    delete ui;
}

void MainWindow::initializeWindow(){
    this->populateComboBoxAvailableSerialPorts();
    this->populateComboBoxAFE();
    this->populateComboBoxChannel();
    this->populateComboBoxLNAGain();
    this->populateComboBoxPGAGain();
    this->populateComboBoxImpedances();
    this->populateComboBoxLPF();
    this->populateComboBoxVGainValues();
    this->serialTimeoutTimer.setSingleShot(true);
    ui->pushButtonConnect->setEnabled(false);
    ui->pushButtonDisconnect->setEnabled(false);
    ui->spinBoxBaudRate->setValue(1842300);
    this->serialPort_ = new QSerialPort(this);
    this->dialogReadoutChannelWindow = new DialogReadoutChannel();
    this->Message("DAPHNE Test",0);
}

void MainWindow::populateComboBoxAvailableSerialPorts(){

    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    for(QSerialPortInfo port : availablePorts){
        ui->comboBoxAvailableSerialPort->addItem(port.portName());
    }
}

void MainWindow::populateComboBoxLNAGain(){
    ui->comboBoxLNAGain->addItem("12dB");
    ui->comboBoxLNAGain->addItem("18dB");
    ui->comboBoxLNAGain->addItem("24dB");
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

    uint16_t active_termination_mask = this->getCheckBoxActiveTerminationMask();
    eraser = MASK_ERASER_ACTIVE_TERMINATION_ENABLE_REG_52;
    this->reg_52_value = this->eraseAndApplyMask(this->reg_52_value,active_termination_mask,eraser);

    uint16_t adc_format_mask = this->getADCFormatMask();
    eraser = MASK_ERASER_ADC_FORMAT_REG_4;
    this->reg_4_value = this->eraseAndApplyMask(this->reg_4_value,adc_format_mask,eraser);

    qDebug() << "REG 51: " << QString::number(this->reg_51_value, 2) << " :: " << QString::number(this->reg_51_value, 16);
    qDebug() << "REG 52: " << QString::number(this->reg_52_value, 2) << " :: " << QString::number(this->reg_52_value, 16);

    QString command = "WR AFE ";
    command = command + ui->comboBoxAFE->currentText();
    command = command + " REG 52 V ";
    command = command + QString::number(this->reg_52_value);
    command = command + "\r\n";
    this->Message("Writing on R52: bin" + QString::number(this->reg_52_value, 2) + " :: hex: " + QString::number(this->reg_52_value, 16),0);
    this->sendCommand(command);


    command = "WR AFE ";
    command = command + ui->comboBoxAFE->currentText();
    command = command + " REG 51 V ";
    command = command + QString::number(this->reg_51_value);
    command = command + "\r\n";
    this->Message("Writing on R51: bin" + QString::number(this->reg_51_value, 2) + " :: hex: " + QString::number(this->reg_51_value, 16),0);
    this->sendCommand(command);

    command = "WR AFE ";
    command = command + ui->comboBoxAFE->currentText();
    command = command + " REG 4 V ";
    command = command + QString::number(this->reg_4_value);
    command = command + "\r\n";
    this->Message("Writing on R4: bin" + QString::number(this->reg_4_value, 2) + " :: hex: " + QString::number(this->reg_4_value, 16),0);
    this->sendCommand(command);

}

void MainWindow::pushButtonApplyOffsetPressed(){

    QString command = "WR OFFSET CH ";
    command = command + ui->comboBoxChannel->currentText();
    command = command + " V ";
    command = command + QString::number(ui->spinBoxOffsetVoltage->value());
    command = command + "\r\n";
    this->sendCommand(command);

}

void MainWindow::pushButtonApplyVGAINPressed(){

    QString command = "WR AFE ";
    command = command + ui->comboBoxChannel->currentText();
    command = command + " VGAIN V ";
    command = command + QString::number(this->calculateVGainReferenceValue());
    command = command + "\r\n";
    this->sendCommand(command);

}

int MainWindow::calculateVGainReferenceValue(){

    double vGain_value = ui->comboBoxVGainValues->currentText().toDouble();
    vGain_value = ((2.49 + 1.5)/1.5)*vGain_value;
    return (int)(vGain_value*1000.0);
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
    qDebug() << "LNA MASK: " << QString::number(mask, 2) << " :: " << QString::number(mask, 16);
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
    qDebug() << "PGA MASK: " << QString::number(mask, 2) << " :: " << QString::number(mask, 16);
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
    qDebug() << "LPF MASK: " << QString::number(mask, 2) << " :: " << QString::number(mask, 16);
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
    qDebug() << "IMPEDANCE MASK: " << QString::number(mask, 2) << " :: " << QString::number(mask, 16);
    return mask;
}

void MainWindow::pushButtonConnectPressed(){
    QString portName = ui->comboBoxAvailableSerialPort->currentText();
    int baudRate = ui->spinBoxBaudRate->value();
    this->serialPort_->setPortName(portName);
    this->serialPort_->setBaudRate(baudRate);
    if (this->serialPort_->open(QIODevice::ReadWrite)) {
        QMessageBox::information(this, tr("Success"), "Success in opening the port");
        ui->pushButtonConnect->setEnabled(false);
        ui->pushButtonDisconnect->setEnabled(true);
    } else {
        QMessageBox::critical(this, tr("Error"), this->serialPort_->errorString());
    }
}

void MainWindow::pushButtonDisconnectPressed(){
    qDebug()<<__func__;
    if (this->serialPort_->isOpen()){
        qDebug()<<__func__<< " line:" <<__LINE__;
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
    default:
    break;
    }
}

void MainWindow::delay(int delay_){
    QTime dieTime = QTime::currentTime().addSecs(delay_);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::readDataFromSerial(){
    QByteArray serial_data_ = this->serialPort_->readAll();
    this->serialData.append(serial_data_);
    QString data_string = QString::fromUtf8(serial_data_.data(),serial_data_.size());
    this->Message(data_string,1);
    this->serialTimeoutTimer.stop();
    this->waitingForData.quit();
}

void MainWindow::sendDataFromSerial(const QString &send_data){
    QByteArray send_data_byte_array = send_data.toUtf8();
    qDebug() <<"send_data: " <<send_data_byte_array.toHex();
    this->serialPort_->write(send_data_byte_array);
}

bool MainWindow::sendCommand(const QString &command){
    if(this->serialPort_->isOpen()){
        this->serialData.clear();
        this->Message(command,0);
        this->sendDataFromSerial(command);
        this->waitForResponse(5);
        this->delay(1);
        return true;
    }else{
        QMessageBox::critical(this, tr("Error"), "Serial Port is closed.");
        return false;
    }
}

void MainWindow::pushButtonInitialPressed(){
    QString command = "WR AFE ";
    command = command + ui->comboBoxChannel->currentText();
    command = command + " INITIAL\r\n";
    this->sendCommand(command);
}

void MainWindow::pushButtonRDFPGAPressed(){
    if(!ui->spinBoxMultipleWaveformsEnable->isChecked()){
        QString command = "RD FPGA\r\n";
        if(this->sendCommand(command)){
            this->dialogReadoutChannelWindow->show();
            this->dialogReadoutChannelWindow->plotData(this->serialData, this->reg_4_value);
        }
        bool test = false;
        if(test){
            this->dialogReadoutChannelWindow->show();
            this->dialogReadoutChannelWindow->plotData(this->dialogReadoutChannelWindow->generateDaphneTestData(300), this->reg_4_value);
        }
     }else{
        int sampling_iterations = ui->spinBoxMultipleWaveforms->value();
        for(int i = 0; i< sampling_iterations; i++){
            QString command = "RD FPGA\r\n";
            if(this->sendCommand(command)){
                this->dialogReadoutChannelWindow->show();
                this->dialogReadoutChannelWindow->plotData(this->serialData, this->reg_4_value);
            }
            this->dialogReadoutChannelWindow->saveContinousWaveform(this->mutliple_waveforms_folder_address,i);
        }
    }
}

void MainWindow::pushButtonApplyBiasVoltages(){
    QString command = "WR VBIASCTRL V 1100\r\n";
    this->sendCommand(command);
    command = "WR AFE ";
    command = command + ui->comboBoxChannel->currentText();
    command = command + " BIASSET V ";
    command = command + QString::number(ui->spinBoxBiasVoltage->value());
    command = command + "\r\n";
    this->sendCommand(command);
    command = "WR TRIM CH ";
    command = command + ui->comboBoxChannel->currentText();
    command = command + " V ";
    command = command + QString::number(ui->spinBoxTrim->value());
    command = command + "\r\n";
    this->sendCommand(command);
}

void MainWindow::waitForResponse(uint timeout){
    this->receivingDataFlag = true;
    serialTimeoutTimer.start(timeout*1000);
    this->waitingForData.exec();
//    while(this->receivingDataFlag){
//        if(!this->receivingDataFlag)
//            break;
//    }
}

void MainWindow::serialTimeoutTimerTriggered(){
    this->Message("Timout waiting for DAPHNE response",2);
}

void MainWindow::populateComboBoxChannel(){
    for(int i=0;i<40;i++){
        ui->comboBoxChannel->addItem(QString::number(i));
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
    QString command = ui->lineEditSendRawCommand->text();
    command = command + "\r\n";
    this->sendCommand(command);
}

void MainWindow::populateComboBoxVGainValues(){
    for(int i = 0; i < 16; i++){
        ui->comboBoxVGainValues->addItem(QString::number(1.5 - 0.1*i));
    }
}

void MainWindow::pushButtonMultipleWaveformsDirectoryPressed(){
    this->mutliple_waveforms_folder_address = QFileDialog::getExistingDirectory(0, ("Select Output Folder"), QDir::currentPath());
    qDebug()<<this->mutliple_waveforms_folder_address;
}
