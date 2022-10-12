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

double TriggerMenuDialog::getTriggerLevel(){
  return ui->doubleSpinBoxLevel->value();

}

void TriggerMenuDialog::setTriggerLevel(double &level){
  ui->doubleSpinBoxLevel->setValue(level);

}
