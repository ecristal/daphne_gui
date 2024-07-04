#include "triggermenudialog.h"
#include "ui_triggermenudialog.h"

TriggerMenuDialog::TriggerMenuDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::TriggerMenuDialog)
{
  ui->setupUi(this);
  connect(ui->checkBoxSourceInternal,SIGNAL(clicked(bool)),this,SLOT(sourceInternalPressed()));
  connect(ui->checkBoxSourceExternal,SIGNAL(clicked(bool)),this,SLOT(sourceExternalPressed()));
  connect(ui->checkBoxSourceSoftware,SIGNAL(clicked(bool)),this,SLOT(sourceSoftwarePressed()));
  connect(ui->pushButtonSetThreshold,SIGNAL(clicked(bool)),this,SLOT(buttonSetThresholdPressed()));
  connect(ui->pushButtonSelectAll,SIGNAL(clicked(bool)),this,SLOT(pushButtonSelectAllPressed()));
  connect(ui->pushButtonUnselect,SIGNAL(clicked(bool)),this,SLOT(pushButtonUnselectAllPressed()));
  connect(ui->spinBoxChannel,SIGNAL(valueChanged(int)),this,SLOT(spinBoxChannelValueChanged()));
  connect(ui->spinBoxLevel,SIGNAL(valueChanged(int)),this,SLOT(spinBoxLevelValueChanged()));
  configureCheckboxPointers();
}

TriggerMenuDialog::~TriggerMenuDialog()
{
  delete ui;
}

void TriggerMenuDialog::configureCheckboxPointers(){

    for(QObject *obj : ui->groupBoxAFE0->children()){
      QCheckBox *checkbox = reinterpret_cast<QCheckBox*>(obj);
      this->ptr_ch_enabled.append(checkbox);
    }
    for(QObject *obj : ui->groupBoxAFE1->children()){
      QCheckBox *checkbox = reinterpret_cast<QCheckBox*>(obj);
      this->ptr_ch_enabled.append(checkbox);
    }
    for(QObject *obj : ui->groupBoxAFE2->children()){
      QCheckBox *checkbox = reinterpret_cast<QCheckBox*>(obj);
      this->ptr_ch_enabled.append(checkbox);
    }
    for(QObject *obj : ui->groupBoxAFE3->children()){
      QCheckBox *checkbox = reinterpret_cast<QCheckBox*>(obj);
      this->ptr_ch_enabled.append(checkbox);
    }
    for(QObject *obj : ui->groupBoxAFE4->children()){
      QCheckBox *checkbox = reinterpret_cast<QCheckBox*>(obj);
      this->ptr_ch_enabled.append(checkbox);
    }
}

QVector<bool> TriggerMenuDialog::getCheckBoxStates(){
  QVector<bool> checkboxStates;
  for(QCheckBox *chk : this->ptr_ch_enabled){
    bool state = chk->isChecked();
    checkboxStates.append(state);
  }
  return checkboxStates;
}

void TriggerMenuDialog::setCheckBoxStates(const QVector<bool> &states){
  int i = 0;
  for(QCheckBox *chk : this->ptr_ch_enabled){
    chk->setChecked(states.at(i));
    i++;
  }
}

void TriggerMenuDialog::pushButtonSelectAllPressed(){
  for(QCheckBox *chk : this->ptr_ch_enabled){
    chk->setChecked(true);
  }
}

void TriggerMenuDialog::pushButtonUnselectAllPressed(){
  for(QCheckBox *chk : this->ptr_ch_enabled){
    chk->setChecked(false);
  }
}

void TriggerMenuDialog::sourceInternalPressed(){
  ui->checkBoxSourceInternal->setChecked(true);
  ui->checkBoxSourceExternal->setChecked(false);
  ui->checkBoxSourceSoftware->setChecked(false);

}

void TriggerMenuDialog::sourceExternalPressed(){
  ui->checkBoxSourceInternal->setChecked(false);
  ui->checkBoxSourceExternal->setChecked(true);
  ui->checkBoxSourceSoftware->setChecked(false);

}

void TriggerMenuDialog::sourceSoftwarePressed(){
  ui->checkBoxSourceInternal->setChecked(false);
  ui->checkBoxSourceExternal->setChecked(false);
  ui->checkBoxSourceSoftware->setChecked(true);

}

QVector<bool> TriggerMenuDialog::getTriggerSource(){
  QVector<bool> triggerSource;
  triggerSource.append(ui->checkBoxSourceInternal->isChecked());
  triggerSource.append(ui->checkBoxSourceExternal->isChecked());
  triggerSource.append(ui->checkBoxSourceSoftware->isChecked());

  return triggerSource;

}

void TriggerMenuDialog::setTriggerSource(QVector<bool> &triggerSource){
  ui->checkBoxSourceInternal->setChecked(triggerSource[0]);
  ui->checkBoxSourceExternal->setChecked(triggerSource[1]);
  ui->checkBoxSourceSoftware->setChecked(triggerSource[2]);

}

int TriggerMenuDialog::getTriggerChannel(){
  return ui->spinBoxChannel->value();

}

void TriggerMenuDialog::setTriggerChannel(int &channel){
  ui->spinBoxChannel->setValue(channel);

}

void TriggerMenuDialog::setTriggerEnableChannel(QVector<bool> &triggerEnabled){
  this->triggerEnabled = triggerEnabled;

}

QVector<int> TriggerMenuDialog::getTriggerLevel(){
  return this->triggerLevel;
}

void TriggerMenuDialog::setTriggerLevel(QVector<int> &level){
  this->triggerLevel = level;
  ui->spinBoxLevel->setValue(level[ui->spinBoxChannel->value()]);
}

void TriggerMenuDialog::spinBoxChannelValueChanged(){
  int value = ui->spinBoxChannel->value();
  ui->spinBoxLevel->setValue(this->triggerLevel[value]);
}

void TriggerMenuDialog::spinBoxLevelValueChanged(){
  this->triggerLevel[ui->spinBoxChannel->value()] = ui->spinBoxLevel->value();
  ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  ui->buttonBox->button(QDialogButtonBox::Cancel)->setEnabled(true);
}

void TriggerMenuDialog::buttonSetThresholdPressed(){
  MainWindow *mymainwindow = reinterpret_cast<MainWindow*>(this->parent());
  bool config_all = ui->checkBoxConfigAll->isChecked();
  try{
    if(!mymainwindow->getEthernetCheckboxPointer()->isChecked()){
        throw ethernetUDPException(3);
    }
    if(config_all){
      this->configTresholdAllChannels(mymainwindow->getSocket());
    }else{
      uint32_t channel = ui->spinBoxChannel->value();
      this->configTresholdSingleChannel(channel, mymainwindow->getSocket());
    }
    this->configTriggerEnable(mymainwindow->getSocket());
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setEnabled(false);
  }catch(ethernetUDPException &e){
    e.handleException(mymainwindow);
  }
}

uint64_t TriggerMenuDialog::getTriggerEnabledConfig(){
    QVector<bool> enableStates = this->getCheckBoxStates();
    int lengthStates = enableStates.length();
    uint64_t enabledStatesValues = 0;
    for(int i = 0; i<lengthStates; i++){
        enabledStatesValues = enabledStatesValues | ((uint64_t)enableStates[i] << i);
    }
    qDebug()<< " :: sended :: "  << QString::number(enabledStatesValues,2);
    return enabledStatesValues;
}

void TriggerMenuDialog::configTresholdSingleChannel(const uint32_t &channel, DaphneSocket *socket){
  int threshold_level = ui->spinBoxLevel->value();
  int multiplier = ui->spinBoxMultiplier->value();
  socket->setDataIs64bits2Thread(true);
  socket->startUdpThread();
  socket->flushReceivedData();
  uint64_t concat_value = 0;
  concat_value = concat_value | ((uint64_t)multiplier << 40);
  concat_value = concat_value | ((uint64_t)channel << 32);
  concat_value = concat_value | (uint64_t)threshold_level;
  //GUI firmware
  socket->sendSingleCommand(0x2021,concat_value);
  //oficial firmware
  concat_value = 0;
  concat_value = concat_value | (uint64_t)threshold_level;
  socket->sendSingleCommand(0x6000,concat_value);
  socket->sendSingleCommand(0x6001,this->getTriggerEnabledConfig());

  socket->read(0x6001,1);
  socket->delayMilli(10);

  QVector<QByteArray>* receivedData = socket->getReceivedData();

  qDebug()<< channel << " :: sended :: "  << QString::number(concat_value,2);

  uint64_t u64_data = 0;
  u64_data = (uint64_t)receivedData->at(0).at(0) | ((uint64_t)receivedData->at(0).at(1) << 8*1) | ((uint64_t)receivedData->at(0).at(2) << 8*2) | ((uint64_t)receivedData->at(0).at(3) << 8*3) | ((uint64_t)receivedData->at(0).at(4) << 8*4) |
             ((uint64_t)receivedData->at(0).at(5) << 8*5) | ((uint64_t)receivedData->at(0).at(6) << 8*6) | ((uint64_t)receivedData->at(0).at(7) << 8*7);

  qDebug()<< channel << " :: recieved :: "  << QString::number(u64_data,2);
  socket->flushReceivedData();

  socket->read(0x2024,1);
  socket->delayMilli(10);
  QVector<QByteArray>* receivedData2 = socket->getReceivedData();
  socket->stopUdpThread();
  socket->setDataIs64bits2Thread(false);
  uint64_t u64_data2 = 0;

  u64_data2 = (uint64_t)receivedData2->at(0).at(0) | ((uint64_t)receivedData2->at(0).at(1) << 8*1) | ((uint64_t)receivedData2->at(0).at(2) << 8*2) | ((uint64_t)receivedData2->at(0).at(3) << 8*3) | ((uint64_t)receivedData2->at(0).at(4) << 8*4) |
             ((uint64_t)receivedData2->at(0).at(5) << 8*5) | ((uint64_t)receivedData2->at(0).at(6) << 8*6) | ((uint64_t)receivedData2->at(0).at(7) << 8*7);

  qDebug()<< channel << " :: recieved :: "  << QString::number(u64_data2,2);
  socket->flushReceivedData();
}

void TriggerMenuDialog::configTresholdAllChannels(DaphneSocket *socket){
  for(int i = 0;i <40; i++){
    this->configTresholdSingleChannel(i,socket);
  }
}

void TriggerMenuDialog::configTriggerEnable(DaphneSocket *socket){
  uint64_t concat_value = 0;
  uint64_t shft = 1;
  for(int i = 0;i <40; i++){
    if(this->triggerEnabled[i]){
      concat_value = concat_value | ((shft << i));

    }
  }
  qDebug() << QString::number(concat_value,2);
  socket->sendSingleCommand(0x2022,concat_value);
}

void TriggerMenuDialog::setMultiplierValue(uint16_t &multiplier){
    ui->spinBoxMultiplier->setValue((int)multiplier);
}

uint16_t TriggerMenuDialog::getMultiplierValue(){
    return (uint16_t)ui->spinBoxMultiplier->value();
}


