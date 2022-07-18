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
  ui->spinBoxBiasUpper->setMinimum(biasLowValue + 2);
  if(biasLowValue < biasUpperValue)
    ui->spinBoxBiasUpper->setValue(biasLowValue+2);
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
    for(int i=this->initialPoistion; i<this->xValues.length();i++){
      //mymainwindow->sendCommand("comando");
      if(this->pausePressedFLAG){
        break;
      }
      this->yValues.append(this->rg.generateDouble()*200);
      double y_min = *std::min_element(this->yValues.begin(),this->yValues.end());
      double y_max = *std::max_element(this->yValues.begin(),this->yValues.end());
      ui->widgetIVgraph->yAxis->setRange(y_min - 0.1*y_max, y_max + 0.1*y_max);
      x_graph.append(this->xValues.at(i));
      ui->widgetIVgraph->graph(0)->setData(x_graph,this->yValues);
      qDebug() << this->xValues.at(i) << "::" << this->yValues.at(i);
      qDebug() << x_graph.length() << "::" << this->yValues.length();
      ui->widgetIVgraph->replot();
      this->initialPoistion++;
      mymainwindow->delayMilli(500);
    }
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);
  }catch(serialException &e){
    e.handleException(mymainwindow);
  }
}

void DialogIVcurve::pushButtonPausePressed(){

  this->pausePressedFLAG = true;
  ui->pushButtonStop->setEnabled(true);
  ui->pushButtonStart->setEnabled(false);
  ui->pushButtonStart->setEnabled(true);
  ui->pushButtonStart->setText("Resume");
}

void DialogIVcurve::pushButtonStopPressed(){

}




