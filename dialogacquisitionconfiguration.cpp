#include "dialogacquisitionconfiguration.h"
#include "ui_dialogacquisitionconfiguration.h"

DialogAcquisitionConfiguration::DialogAcquisitionConfiguration(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogAcquisitionConfiguration)
{
  ui->setupUi(this);
  connect(ui->pushButtonSelectAll,SIGNAL(clicked(bool)),this,SLOT(pushButtonSelectAllPressed()));
  connect(ui->pushButtonUnselect,SIGNAL(clicked(bool)),this,SLOT(pushButtonUnselectAllPressed()));
  configureCheckboxPointers();
}

DialogAcquisitionConfiguration::~DialogAcquisitionConfiguration()
{
  delete ui;
}

void DialogAcquisitionConfiguration::configureCheckboxPointers(){

//  for(QWidget *gbAFE : ui->groupBoxEnabledChannels->children()){
//    if(gbAFE->isClassType == "QGroupBox")
//    for(QObject *obj : ui->groupBoxAFE0->children()){
//      QCheckBox *checkbox = reinterpret_cast<QCheckBox*>(obj);
//      qDebug() << checkbox->objectName();
//      this->ptr_ch_enabled.append(checkbox);
//    }
//  }
    for(QObject *obj : ui->groupBoxAFE0->children()){
      QCheckBox *checkbox = reinterpret_cast<QCheckBox*>(obj);
      qDebug() << checkbox->objectName();
      this->ptr_ch_enabled.append(checkbox);
    }
    for(QObject *obj : ui->groupBoxAFE1->children()){
      QCheckBox *checkbox = reinterpret_cast<QCheckBox*>(obj);
      qDebug() << checkbox->objectName();
      this->ptr_ch_enabled.append(checkbox);
    }
    for(QObject *obj : ui->groupBoxAFE2->children()){
      QCheckBox *checkbox = reinterpret_cast<QCheckBox*>(obj);
      qDebug() << checkbox->objectName();
      this->ptr_ch_enabled.append(checkbox);
    }
    for(QObject *obj : ui->groupBoxAFE3->children()){
      QCheckBox *checkbox = reinterpret_cast<QCheckBox*>(obj);
      qDebug() << checkbox->objectName();
      this->ptr_ch_enabled.append(checkbox);
    }
    for(QObject *obj : ui->groupBoxAFE4->children()){
      QCheckBox *checkbox = reinterpret_cast<QCheckBox*>(obj);
      qDebug() << checkbox->objectName();
      this->ptr_ch_enabled.append(checkbox);
    }
}

QVector<bool> DialogAcquisitionConfiguration::getCheckBoxStates(){
  QVector<bool> checkboxStates;
  for(QCheckBox *chk : this->ptr_ch_enabled){
    bool state = chk->isChecked();
    checkboxStates.append(state);
  }
  return checkboxStates;
}

void DialogAcquisitionConfiguration::setCheckBoxStates(const QVector<bool> &states){
  int i = 0;
  for(QCheckBox *chk : this->ptr_ch_enabled){
    chk->setChecked(states.at(i));
    i++;
  }
}

int DialogAcquisitionConfiguration::getRecordLength(){
  return ui->spinBoxRecordLenght->value();
}

void DialogAcquisitionConfiguration::setRecordLength(const int &record_length){
  ui->spinBoxRecordLenght->setValue(record_length);;
}

void DialogAcquisitionConfiguration::pushButtonSelectAllPressed(){
  for(QCheckBox *chk : this->ptr_ch_enabled){
    chk->setChecked(true);
  }
}

void DialogAcquisitionConfiguration::pushButtonUnselectAllPressed(){
  for(QCheckBox *chk : this->ptr_ch_enabled){
    chk->setChecked(false);
  }
}


