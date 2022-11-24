#include "dialogafeconfiguration.h"
#include "ui_dialogafeconfiguration.h"

DialogAFEConfiguration::DialogAFEConfiguration(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogAFEConfiguration)
{
  ui->setupUi(this);
  this->setWindowTitle("AFE configuration");
}

DialogAFEConfiguration::~DialogAFEConfiguration()
{
  delete ui;
}

bool DialogAFEConfiguration::getCheckBoxHPFEnabled(){
  return ui->checkBoxDigitalHPF->isChecked();
}

bool DialogAFEConfiguration::getCheckBoxLFNSupressorEnabled(){
  return ui->checkBoxLFNSupressor->isChecked();
}

uint16_t DialogAFEConfiguration::getSpinBoxHPFKValue(){
  return (uint16_t)ui->spinBoxK->value();
}

bool DialogAFEConfiguration::getFPGAFilterEnabled(){
  return ui->checkBoxFpgaFilterEnabled->isChecked();
}

void DialogAFEConfiguration::setCheckBoxHPFEnabled(const bool &enabled){
  ui->checkBoxDigitalHPF->setChecked(enabled);
}

void DialogAFEConfiguration::setCheckBoxLFNSupressorEnabled(const bool &enabled){
  ui->checkBoxLFNSupressor->setChecked(enabled);
}

void DialogAFEConfiguration::setSpinBoxHPFKValue(const uint16_t &value){
  ui->spinBoxK->setValue((int)value);
}

void DialogAFEConfiguration::setFPGAFilterEnabled(const bool &enabled){
  ui->checkBoxFpgaFilterEnabled->setChecked(enabled);
}
