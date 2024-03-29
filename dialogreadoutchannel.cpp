#include "dialogreadoutchannel.h"
#include "ui_dialogreadoutchannel.h"

#include <QTextStream>

# define M_PI           3.14159265358979323846

DialogReadoutChannel::DialogReadoutChannel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogReadoutChannel)
{
    ui->setupUi(this);
    this->setWindowTitle("READOUT");
    this->window_status = true;
    ui->widgetDaphneDataGraph->addGraph();
    //connect(ui->pushButtonTxt,SIGNAL(clicked(bool)),this,SLOT(pushButtonSaveToTxtPressed())); //Uncomment to have this function connected to any other signal, removed pushbutton
    connect(ui->buttonBoxCancelOk,SIGNAL(rejected()),this,SLOT(cancelButtonPressed()));
    connect(ui->pushButtonAutoSet,SIGNAL(clicked(bool)),this,SLOT(pushButtonAutoSetPressed()));
    connect(ui->spinBoxMinTime,SIGNAL(valueChanged(int)),this,SLOT(spinBoxPositionSCaleValueChanged()));
    connect(ui->spinBoxMaxTime,SIGNAL(valueChanged(int)),this,SLOT(spinBoxPositionSCaleValueChanged()));
    connect(ui->spinBoxPosition,SIGNAL(valueChanged(int)),this,SLOT(spinBoxPositionSCaleValueChanged()));
    connect(ui->spinBoxScale,SIGNAL(valueChanged(int)),this,SLOT(spinBoxPositionSCaleValueChanged()));
}

DialogReadoutChannel::~DialogReadoutChannel()
{
    delete ui;
}

void DialogReadoutChannel::plotData(const QByteArray &serial_data,const uint16_t &format){
    this->daphneData.clear();
    this->daphneTime.clear();
    int length_data = serial_data.length();
    ////qDebug() <<"length: "<< length_data;
    uint16_t byte_data;
    for(int i = 90; i<length_data; i+=2){
        byte_data = bytes2int16(serial_data[i],serial_data[i+1]);
        ////qDebug() << "Data: " << (double)this->formatData(byte_data);
        this->daphneData.append((double)this->formatData(byte_data, format));
    }

    this->generateTimeVector(length_data-90,1.0/62500000.0);
    //this->plot();
}

int DialogReadoutChannel::getSpinBoxPlotEveryWaveformsValue(){
    return ui->spinBoxPlotEveryWaveform->value();
}

void DialogReadoutChannel::pushButtonAutoSetPressed(){ 
    double max_daphne_data = *std::max_element(this->plot_ethernet_data->at(*this->plot_channel).constBegin(),this->plot_ethernet_data->at(*this->plot_channel).constEnd());
    double min_daphne_data = *std::min_element(this->plot_ethernet_data->at(*this->plot_channel).constBegin(),this->plot_ethernet_data->at(*this->plot_channel).constEnd());
    ui->spinBoxPosition->setValue((max_daphne_data+min_daphne_data)/2);
    ui->spinBoxScale->setValue((max_daphne_data-min_daphne_data)/2);
    ui->widgetDaphneDataGraph->replot();
}

void DialogReadoutChannel::spinBoxPositionSCaleValueChanged(){
    ui->widgetDaphneDataGraph->yAxis->setRange(ui->spinBoxPosition->value()-ui->spinBoxScale->value(),ui->spinBoxPosition->value()+ui->spinBoxScale->value());
    ui->widgetDaphneDataGraph->xAxis->setRange(16e-9*ui->spinBoxMinTime->value(),16e-9*ui->spinBoxMaxTime->value());
}

void DialogReadoutChannel::plotMultichannel(){

  if(ui->checkBoxEnablePlot->isChecked()){
    for(int i = 0; i<this->plot_ethernet_data->at(*this->plot_channel).length();i++){
        this->daphneData[i] = this->plot_ethernet_data->at(*this->plot_channel).at(i);
    }
    ui->widgetDaphneDataGraph->graph(0)->setData(this->daphneTime,this->daphneData);
    ui->widgetDaphneDataGraph->replot();
    ui->lcdNumberWaveform->setPalette(Qt::darkGreen);
    ui->lcdNumberWaveform->display(*this->wave_number);
  }
}

void DialogReadoutChannel::plotDataMultichannel(){

  if(this->plot_ethernet_data->at(*this->plot_channel).length() != this->daphneTime.length()){
      this->daphneTime.clear();
      int length_data = this->plot_ethernet_data->at(*this->plot_channel).length();
      this->generateTimeVectorEthernet(length_data,1.0/62500000.0);
      this->daphneData.resize(this->plot_ethernet_data->at(*this->plot_channel).length());
  }
  this->plotMultichannel();
}

void DialogReadoutChannel::cancelButtonPressed(){
    this->window_status = false;
}

bool DialogReadoutChannel::getWindowStatus(){
    return this->window_status;
}

void DialogReadoutChannel::setWindowStatus(bool status){
    this->window_status = status;
}

void DialogReadoutChannel::generateTimeVector(const int &length, double Tm){
    double time_ = 0.0;
    for(int i=0; i<length; i+=2){
        time_ = time_ + Tm;
        this->daphneTime.append(time_);
    }
}

void DialogReadoutChannel::generateTimeVectorEthernet(const int &length, double Tm){
    double time_ = 0.0;
    for(int i=0; i<length; i++){
        time_ = time_ + Tm;
        this->daphneTime.append(time_);
    }
}

QByteArray DialogReadoutChannel::generateDaphneTestData(const uint32_t &length){
    QByteArray dataArray;
    double amplitude = 1500;
    double period = 1;
    double omega = 2*M_PI*(1/period);
    double step = period/((double)length);
    double tiempo_ = 0.0;
    int16_t senoidal_ = 0;
    uint8_t* byte_data = new uint8_t[2];
    for(uint32_t i=0;i<length;i++){
        senoidal_ = (int16_t)amplitude*std::sin(omega*tiempo_);
        //qDebug() << "Data senoidal: " << (double)senoidal_;
        tiempo_ = tiempo_ + step;
        this->daphneTime.append(tiempo_);
        this->uint162bytes(senoidal_, byte_data);
        dataArray.append(byte_data[0]);
        dataArray.append(byte_data[1]);
    }
    delete []byte_data;
    return dataArray;
}

int32_t DialogReadoutChannel::formatData(const uint16_t &data, const uint16_t format){

    switch(format){
        case 0x0:
            return (int32_t)this->formatData2CompLSB(data);
        break;
        case 0x8:
            return (int32_t)this->formatDataOFFSETBinaryLSB(data);
        break;
        case 0x10:
            return (int32_t)this->formatData2CompMSB(data);
        break;
        case 0x18:
            return (int32_t)this->formatDataOFFSETBinaryMSB(data);
        break;
        default:
            return (int32_t)this->formatData2CompLSB(data);
        break;
    }

}

int16_t DialogReadoutChannel::formatData2CompLSB(const uint16_t &data){
    //qDebug() << __PRETTY_FUNCTION__;
    //qDebug() << "Data binary before format: " << QString::number(data,2);
    uint16_t b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,ba,bb,bc,bd,be,bf;
    b0 = (data >> 15) & 0b00000001;
    b1 = (data >> 13) & 0b00000010;
    b2 = (data >> 11) & 0b00000100;
    b3 = (data >> 9)  & 0b00001000;
    b4 = (data >> 7)  & 0b00010000;
    b5 = (data >> 5)  & 0b00100000;
    b6 = (data >> 3)  & 0b01000000;
    b7 = (data >> 1)  & 0b10000000;

    b8 = (data << 1)  & 0b0000000100000000;
    b9 = (data << 3)  & 0b0000001000000000;
    ba = (data << 5)  & 0b0000010000000000;
    bb = (data << 7)  & 0b0000100000000000;
    bc = (data << 9)  & 0b0001000000000000;
    bd = (data << 11) & 0b0010000000000000;
    be = (data << 13) & 0b0100000000000000;
    bf = (data << 15) & 0b1000000000000000; // Por default DAPHNE envia el LSB primero. Aqui Manuel calcula las potencias de 2 y luego las suma, pero antes realiza un cambio de LSB a MSB.

    uint16_t value = b0 + b1 + b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + ba + bb + bc + bd + be + bf;
    int16_t return_value = 0;
    value = (value>>2)&0b11111111111111; // El ADC es de 14 bits, entonces manuel se encarga de hacer un shift de 2 posiciones a la derecha y pone
    //qDebug() << "Data binary after format: " << QString::number(value,2);
//    return_value = value;                                            // ceros en el bit 15 y 14;
//    if (value>=(8192)){
//        return_value=-1*((value^0b11111111111111)+1); // Aqui se realiza la inversion del complemento a 2, pero creo que no es necesario si hago un cast de uint16_t a int16_t;
//    }
    return_value = ((int16_t)(value << 2)) / 4 ;
    //qDebug() << "Data binary after signed conversion: " << QString::number(return_value,2) << " int16: " << QString::number(return_value);
    return return_value;
}

int16_t DialogReadoutChannel::formatData2CompMSB(const uint16_t &data){

    //qDebug() << __PRETTY_FUNCTION__;
    //qDebug() << "Data binary before format: " << QString::number(data,2);
    uint16_t value; // En este caso no es necesario hacer las inversion del LSB y MSB porque los datos se reciben en el orden correcto.
    value = data; // Aqui solo se toman los 14 bits
    //qDebug() << "Data binary after format: " << QString::number(value,2);
    int16_t return_value = ((int16_t)(value << 2)) / 4;
    //qDebug() << "Data binary after signed conversion: " << QString::number(return_value,2) << " int16: " << QString::number(return_value);
    return return_value;
}

uint16_t DialogReadoutChannel::formatDataOFFSETBinaryLSB(const uint16_t &data){
    //qDebug() << __PRETTY_FUNCTION__;
    //qDebug() << "Data binary before format: " << QString::number(data,2);
    uint16_t b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,ba,bb,bc,bd,be,bf;
    b0 = (data >> 15) & 0b00000001;
    b1 = (data >> 13) & 0b00000010;
    b2 = (data >> 11) & 0b00000100;
    b3 = (data >> 9)  & 0b00001000;
    b4 = (data >> 7)  & 0b00010000;
    b5 = (data >> 5)  & 0b00100000;
    b6 = (data >> 3)  & 0b01000000;
    b7 = (data >> 1)  & 0b10000000;

    b8 = (data << 1)  & 0b0000000100000000;
    b9 = (data << 3)  & 0b0000001000000000;
    ba = (data << 5)  & 0b0000010000000000;
    bb = (data << 7)  & 0b0000100000000000;
    bc = (data << 9)  & 0b0001000000000000;
    bd = (data << 11) & 0b0010000000000000;
    be = (data << 13) & 0b0100000000000000;
    bf = (data << 15) & 0b1000000000000000; // Por default DAPHNE envia el LSB primero. Aqui Manuel calcula las potencias de 2 y luego las suma, pero antes realiza un cambio de LSB a MSB.

    uint16_t value = b0 + b1 + b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + ba + bb + bc + bd + be + bf;

    value = (value>>2)&0b11111111111111; // El ADC es de 14 bits, entonces manuel se encarga de hacer un shift de 2 posiciones a la derecha y pone
                                                // ceros en el bit 15 y 14;
    //qDebug() << "Data binary before format: " << QString::number(value,2);
    //qDebug() << "Data binary after signed conversion: " << QString::number(value,2) << " int16: " << QString::number(value);
    return value;
}

uint16_t DialogReadoutChannel::formatDataOFFSETBinaryMSB(const uint16_t &data){
    //qDebug() << __PRETTY_FUNCTION__;
    //qDebug() << "Data binary before format: " << QString::number(data,2);
    uint16_t value; // En este caso no es necesario hacer las inversion del LSB y MSB porque los datos se reciben en el orden correcto.
    value = data; // Aqui solo se toman los 14 bits
    //qDebug() << "Data binary before format: " << QString::number(value,2);
    //qDebug() << "Data binary after signed conversion: " << QString::number(value,2) << " int16: " << QString::number(value);
    return value;
}

void DialogReadoutChannel::uint162bytes(const uint16_t valor,uint8_t* byte_array){
 union{
   uint16_t variable;
   uint8_t temp_array[2];
 }u;
 u.variable = valor;
 byte_array[0] = u.temp_array[0];
 byte_array[1] = u.temp_array[1];
}

uint16_t DialogReadoutChannel::bytes2int16(const uint8_t byte_array_1, const uint8_t byte_array_2){
 union{
   int16_t variable;
   uint8_t temp_array[2];
 }u;
 u.temp_array[1] = byte_array_1;
 u.temp_array[0] = byte_array_2;
 return u.variable;
}

void DialogReadoutChannel::pushButtonSaveToTxtPressed(){
    this->writeDataToFile();
}

void DialogReadoutChannel::writeDataToFile(){

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Waveforms"), this->saveDir, tr("Waveforms (*.txt)"));
    QStringList split_fileName = fileName.split('/');
    ////qDebug() << fileName;
    ////qDebug() << split_fileName.last();
    if(split_fileName.last() != ".txt" && fileName != ""){
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        QTextStream writeToFile(&file);
        for(int i = 0; i<this->daphneData.length(); i++){
            ////qDebug() << this->daphneData.at(i);
            writeToFile << QString::number(this->daphneData.at(i))<<"\n";
        }
    }else{
    }
    this->saveDir = fileName;
}

QVector<double> DialogReadoutChannel::getDaphneData(){
    return this->daphneData;
}

void DialogReadoutChannel::saveContinousWaveform(const QString &address,int &wave_number){

    QString fileName = address + "/waveform_" + QString::number(wave_number) + ".txt";
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QTextStream writeToFile(&file);
    for(int i = 0; i<this->daphneData.length(); i++){
        ////qDebug() << this->daphneData.at(i);
        writeToFile << QString::number(this->daphneData.at(i))<<"\n";
    }
    ui->lcdNumberWaveform->setPalette(Qt::darkGreen);
    ui->lcdNumberWaveform->display(wave_number);
}

void DialogReadoutChannel::setEthernetDataForThreadedPlotting(QVector<QVector<uint16_t> > *ethernet_data){
    this->plot_ethernet_data = ethernet_data;
}

void DialogReadoutChannel::setChannelForThreadedPlotting(int *channel){
    this->plot_channel = channel;
}

void DialogReadoutChannel::setWaveNumberPointer(int *wave_number){
    this->wave_number = wave_number;
}

QPushButton* DialogReadoutChannel::getOKButtonPointer(){
    return ui->buttonBoxCancelOk->button(QDialogButtonBox::Ok);
}
