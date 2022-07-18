#include "dialogivcurve.h"
#include "ui_dialogivcurve.h"

DialogIVcurve::DialogIVcurve(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogIVcurve)
{
  ui->setupUi(this);
  initializeWindow();
  connect(ui->spinBoxBiasLow,SIGNAL(valueChanged(int)),this,SLOT(spinBoxBiasLowValueChanged()));
  connect(ui->spinBoxBiasUpper,SIGNAL(valueChanged(int)),this,SLOT(spinBoxBiasUpperValueChanged()));
  connect(ui->pushButtonSaveData,SIGNAL(clicked(bool)),this,SLOT(pushButtonSaveDataPressed()));
  connect(ui->pushButtonStart,SIGNAL(clicked(bool)),this,SLOT(pushButtonStartPressed()));
  connect(ui->pushButtonPause,SIGNAL(clicked(bool)),this,SLOT(pushButtonPausePressed()));
  connect(ui->pushButtonStop,SIGNAL(clicked(bool)),this,SLOT(pushButtonStopPressed()));
}

DialogIVcurve::~DialogIVcurve()
{

  delete ui;

}

void DialogIVcurve::initializeWindow(){
  this->setWindowTitle("I-V Curve");
  ui->widgetIVgraph->addGraph();
  ui->widgetIVgraph->xAxis->setLabel("Bias Voltage [DAC]");
  ui->widgetIVgraph->yAxis->setLabel("Measure");
  ui->pushButtonPause->setEnabled(false);
  ui->pushButtonStop->setEnabled(false);
  ui->pushButtonSaveData->setEnabled(true); //just to test for now
}

void DialogIVcurve::spinBoxBiasLowValueChanged(){
  int biasLowValue = ui->spinBoxBiasLow->value();
  int biasUpperValue = ui->spinBoxBiasUpper->value();
  if(biasLowValue > biasUpperValue)
    ui->spinBoxBiasUpper->setValue(biasLowValue+2);
    ui->spinBoxBiasUpper->setMinimum(biasLowValue + 2);
    biasUpperValue = biasLowValue + 2;
  int biasStepMaximun = (biasUpperValue - biasLowValue)/2;
  ui->spinBoxBiasStep->setMaximum(biasStepMaximun);
}

void DialogIVcurve::spinBoxBiasUpperValueChanged(){
  int biasLowValue = ui->spinBoxBiasLow->value();
  int biasUpperValue = ui->spinBoxBiasUpper->value();
  int biasStepMaximun = (biasUpperValue - biasLowValue)/2;
  ui->spinBoxBiasStep->setMaximum(biasStepMaximun);
}

void DialogIVcurve::pushButtonSaveDataPressed(){

  try{
    if(this->yValues.length() == 0){
      throw this->yValues.length();
    }
    QFile f(QFileDialog::getSaveFileName(this, "Save file", "", ".dat"));
    f.open( QIODevice::WriteOnly );
    QTextStream writeToFile(&f);
    for(int i = 0; i < this->yValues.length(); i++){
      writeToFile << QString::number(this->xValues.at(i)) << "\t" << QString::number(this->yValues.at(i)) << "\n";
    }
    f.close();
  }catch(int number){
    QMessageBox::critical(this, tr("Error"), "No data to save.\nVector has length: " + QString::number(number));
  }

}

void DialogIVcurve::pushButtonStartPressed(){

  ui->pushButtonStart->setText("Start");
  ui->pushButtonStart->setEnabled(false);
  ui->pushButtonPause->setEnabled(true);
  ui->pushButtonStop->setEnabled(false);
  int biasLowValue = ui->spinBoxBiasLow->value();
  int biasUpperValue = ui->spinBoxBiasUpper->value();
  int biasStepValues = ui->spinBoxBiasStep->value();
  int points = (biasUpperValue - biasLowValue)/biasStepValues + 1;
  QVector<double> x_graph;
  if(this->pausePressedFLAG){
    for(int i = 0; i<this->initialPoistion;i++){
      x_graph.append(this->xValues.at(i));
    }
    this->pausePressedFLAG = false;
  }else{
    this->initialPoistion = 0;
    this->xValues.clear();
    this->yValues.clear();
    for(int i = 0; i < points; i++){
      this->xValues.append(biasStepValues*i + biasLowValue);
    }
    if(this->xValues.last() != biasUpperValue){
      this->xValues.append(biasUpperValue);
    }
  }

  ui->widgetIVgraph->xAxis->setRange(this->xValues.front(),this->xValues.last());
  MainWindow *mymainwindow = reinterpret_cast<MainWindow*>(this->parent());

  try{

    this->runningFLAG = true;
    ui->spinBoxChannelNumber->setEnabled(false);
    ui->pushButtonSaveData->setEnabled(false);

    QString command = "WR VBIASCTRL V 1100\r\n";
    mymainwindow->sendCommand(command);
    for(int i=this->initialPoistion; i<this->xValues.length();i++){
      if(this->pausePressedFLAG){
        ui->pushButtonStop->setEnabled(true);
        ui->pushButtonPause->setEnabled(false);
        ui->pushButtonSaveData->setEnabled(true);
        ui->pushButtonStart->setEnabled(true);
        ui->pushButtonStart->setText("Resume");
        break;
      }

      // comunication code here
      command = "WR AFE ";
      int afe_number = mymainwindow->getAFENumberFromChannelNumber(ui->spinBoxChannelNumber->value());
      command = command + QString::number(afe_number);
      command = command + " BIASSET V ";
      command = command + QString::number(this->xValues.at(i));
      command = command + "\r\n";
      mymainwindow->sendCommand(command);
      qDebug() << command;

      command = "RD CM CH ";
      command = command + QString::number(ui->spinBoxChannelNumber->value());
      command = command + "\r\n";
      mymainwindow->sendCommand(command);
      qDebug() << command;

      QString serial_string = mymainwindow->getSerialString();

      QStringList serial_split = serial_string.split("=");
      serial_string = serial_split.last();
      //qDebug() << "Serial String :: " << serial_string;

      serial_split = serial_string.split("\r");
      serial_string = serial_split.first();
      double adc_value = serial_string.toDouble();
      //qDebug() << "Serial String :: " << serial_string << "double :: " << adc_value;

      this->yValues.append(adc_value);
      //this->yValues.append(this->rg.generateDouble()*200); // comment out
      //

      double y_min = *std::min_element(this->yValues.begin(),this->yValues.end());
      double y_max = *std::max_element(this->yValues.begin(),this->yValues.end());
      double lim = 0.1*std::abs(y_max - y_min);

      ui->widgetIVgraph->yAxis->setRange(y_min - 0.1*lim, y_max + 0.1*lim);
      x_graph.append(this->xValues.at(i));
      ui->widgetIVgraph->graph(0)->setData(x_graph,this->yValues);
      qDebug() << this->xValues.at(i) << "::" << this->yValues.at(i);
      qDebug() << x_graph.length() << "::" << this->yValues.length();
      ui->widgetIVgraph->replot();
      this->initialPoistion++;
      //mymainwindow->delayMilli(500);
    }
    if(!this->pausePressedFLAG){
      ui->pushButtonStart->setEnabled(true);
      ui->pushButtonPause->setEnabled(false);
      ui->pushButtonStop->setEnabled(false);

      command = "WR AFE ";
      int afe_number = mymainwindow->getAFENumberFromChannelNumber(ui->spinBoxChannelNumber->value());
      command = command + QString::number(afe_number);
      command = command + " BIASSET V ";
      command = command + QString::number(0);
      command = command + "\r\n";
      mymainwindow->sendCommand(command);
      qDebug() << command;

      ui->spinBoxChannelNumber->setEnabled(true);
      ui->pushButtonSaveData->setEnabled(true);
      this->runningFLAG = false;
    }
  }catch(serialException &e){
    e.handleException(mymainwindow);
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);
    ui->spinBoxChannelNumber->setEnabled(true);
    ui->pushButtonSaveData->setEnabled(true);
    this->runningFLAG = false;
  }
}

void DialogIVcurve::pushButtonPausePressed(){

  this->pausePressedFLAG = true;

}

void DialogIVcurve::pushButtonStopPressed(){

  this->pausePressedFLAG = false;
  this->runningFLAG = false;
  ui->spinBoxChannelNumber->setEnabled(true);
  ui->pushButtonStop->setEnabled(false);
  ui->pushButtonPause->setEnabled(false);
  ui->pushButtonStart->setEnabled(true);
  ui->pushButtonStart->setText("Start");

  MainWindow *mymainwindow = reinterpret_cast<MainWindow*>(this->parent());
  try{
    QString command = "WR AFE ";
    int afe_number = mymainwindow->getAFENumberFromChannelNumber(ui->spinBoxChannelNumber->value());
    command = command + QString::number(afe_number);
    command = command + " BIASSET V ";
    command = command + QString::number(0);
    command = command + "\r\n";
    mymainwindow->sendCommand(command);
    qDebug() << command;
  }catch(serialException &e){
      e.handleException(mymainwindow);
  }

}

void DialogIVcurve::closeEvent(QCloseEvent *event)
{
  if(this->runningFLAG){
    QMessageBox::warning(this, tr("Warning"), "IV curve is still running.\nPlease PAUSE and STOP the process to exit.");
    event->ignore();
  }else{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "DAPHNE GUI",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
      event->ignore();
    } else {
      event->accept();
    }
  }
}



