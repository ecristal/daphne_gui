#include "dialogethernetconfiguration.h"
#include "ui_dialogethernetconfiguration.h"

DialogEthernetConfiguration::DialogEthernetConfiguration(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogEthernetConfiguration)
{
  ui->setupUi(this);
  this->setWindowTitle("ETHERNET CONFIGURATION");
  connect(ui->pushButtonDefault,SIGNAL(clicked(bool)),this,SLOT(pushButtonDefaultPressed()));
}

DialogEthernetConfiguration::~DialogEthernetConfiguration()
{
  delete ui;
}

void DialogEthernetConfiguration::setDaphneIpAddress(const QString &addr){
  ui->lineEditDAPHNEIPaddr->setText(addr);
}

 void DialogEthernetConfiguration::setComputerIpAddress(const QString &addr){
  ui->lineEditComputerIPaddr->setText(addr);
}

void DialogEthernetConfiguration::setDaphnePortNumber(const int &portNumber){
  ui->spinBoxDAPHNEPortNumber->setValue(portNumber);
}

void DialogEthernetConfiguration::setComputerPortNumber(const int &portNumber){
  ui->spinBoxComputerPortNumber->setValue(portNumber);
}

QString DialogEthernetConfiguration::getDaphneIpAddress(){
  return ui->lineEditDAPHNEIPaddr->text();
}

QString DialogEthernetConfiguration::getComputerIpAddress(){
  return ui->lineEditComputerIPaddr->text();
}

int DialogEthernetConfiguration::getDaphnePortNumber(){
  return ui->spinBoxDAPHNEPortNumber->value();
}

int DialogEthernetConfiguration::getComputerPortNumber(){
  return ui->spinBoxComputerPortNumber->value();
}

void DialogEthernetConfiguration::pushButtonDefaultPressed(){
  this->setDaphneIpAddress("192.168.133.12");
  this->setComputerIpAddress("192.168.133.1");
  this->setDaphnePortNumber(2001);
  this->setComputerPortNumber(58789);
}


