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
  connect(ui->spinBoxChannel,SIGNAL(valueChanged(int)),this,SLOT(spinBoxChannelValueChanged()));
  connect(ui->spinBoxLevel,SIGNAL(valueChanged(int)),this,SLOT(spinBoxLevelValueChanged()));
}

TriggerMenuDialog::~TriggerMenuDialog()
{
  delete ui;
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
}

void TriggerMenuDialog::buttonSetThresholdPressed(){
  MainWindow *mymainwindow = reinterpret_cast<MainWindow*>(this->parent());
  bool config_all = ui->checkBoxConfigAll->isChecked();
  try{
    if(config_all){
      this->configTresholdAllChannels(mymainwindow->getSocket());
    }else{
      uint32_t channel = ui->spinBoxChannel->value();
      this->configTresholdSingleChannel(channel, mymainwindow->getSocket());
    }
    this->configTriggerEnable(mymainwindow->getSocket());
  }catch(ethernetUDPException &e){
    e.handleException(mymainwindow);
  }
}

void TriggerMenuDialog::configTresholdSingleChannel(const uint32_t &channel, DaphneSocket *socket){
  int threshold_level = ui->spinBoxLevel->value();
  int multiplier = ui->spinBoxMultiplier->value();

  uint64_t concat_value = 0;
  concat_value = concat_value | ((uint64_t)multiplier << 40);
  concat_value = concat_value | ((uint64_t)channel << 32);
  concat_value = concat_value | threshold_level;
  socket->sendSingleCommand(0x2021,concat_value);
  socket->flushReceivedData();

  socket->read(0x2021,1);
  socket->delayMilli(10);
  QVector<QByteArray> receivedData = socket->getReceivedData();

  qDebug()<< channel << " :: sended :: "  << QString::number(concat_value,2);

  uint64_t u64_data = 0;
  u64_data = (uint64_t)receivedData.at(0).at(2) | ((uint64_t)receivedData.at(0).at(3) << 8*1) | ((uint64_t)receivedData.at(0).at(4) << 8*2) | ((uint64_t)receivedData.at(0).at(5) << 8*3) | ((uint64_t)receivedData.at(0).at(6) << 8*4) |
             ((uint64_t)receivedData.at(0).at(7) << 8*5) | ((uint64_t)receivedData.at(0).at(8) << 8*6) | ((uint64_t)receivedData.at(0).at(9) << 8*7);

  qDebug()<< channel << " :: recieved :: "  << QString::number(u64_data,2);
  socket->flushReceivedData();

  socket->read(0x2024,1);
  socket->delayMilli(10);
  QVector<QByteArray> receivedData2 = socket->getReceivedData();

  uint64_t u64_data2 = 0;

  u64_data2 = (uint64_t)receivedData2.at(0).at(2) | ((uint64_t)receivedData2.at(0).at(3) << 8*1) | ((uint64_t)receivedData2.at(0).at(4) << 8*2) | ((uint64_t)receivedData2.at(0).at(5) << 8*3) | ((uint64_t)receivedData2.at(0).at(6) << 8*4) |
             ((uint64_t)receivedData2.at(0).at(7) << 8*5) | ((uint64_t)receivedData2.at(0).at(8) << 8*6) | ((uint64_t)receivedData2.at(0).at(9) << 8*7);

  qDebug()<< channel << " :: recieved :: "  << QString::number(u64_data2,2);
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

