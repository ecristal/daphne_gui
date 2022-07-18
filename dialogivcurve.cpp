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
}

DialogIVcurve::~DialogIVcurve()
{
  delete ui;

}

void DialogIVcurve::initializeWindow(){
  ui->widgetIVgraph->addGraph();
 //ui->widgetIVgraph->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, "I-V Curve", QFont("sans", 12, QFont::Bold)));
  ui->widgetIVgraph->xAxis->setLabel("Bias Voltage [DAC]");
  ui->widgetIVgraph->yAxis->setLabel("Current");
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

  QFile f(QFileDialog::getSaveFileName(this, "Save file", "", ".dat"));
  f.open( QIODevice::WriteOnly );
  // store data in f
  f.close();

}

void DialogIVcurve::pushButtonStartPressed(){

  int biasLowValue = ui->spinBoxBiasLow->value();
  int biasUpperValue = ui->spinBoxBiasUpper->value();
  int biasStepValues = ui->spinBoxBiasStep->value();
  this->xValues.clear();
  int points = (biasUpperValue - biasLowValue)/biasStepValues + 1;
  for(int i = 0; i < points; i++){
    this->xValues.append(biasStepValues*i + biasLowValue);
  }
  if(this->xValues.last() != biasUpperValue){
    this->xValues.append(biasUpperValue);
  }
  qDebug() << points << "::" << this->xValues;
}




