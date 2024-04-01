#include "dialogaligment.h"
#include "ui_dialogaligment.h"

DialogAligment::DialogAligment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAligment)
{
    ui->setupUi(this);
    this->setWindowTitle("ALIGNMENT");
    connect(ui->pushButtonAlign,SIGNAL(clicked(bool)), this, SLOT(pushButtonAlignPressed()));
    connect(ui->checkBoxVersion2A,SIGNAL(clicked(bool)),this, SLOT(checkBoxVersion2AClicked()));
    connect(ui->checkBoxVersion1,SIGNAL(clicked(bool)),this, SLOT(checkBoxVersion1Clicked()));
}

DialogAligment::~DialogAligment()
{
    delete ui;
}

void DialogAligment::pushButtonAlignPressed(){
    MainWindow *mymainwindow = reinterpret_cast<MainWindow*>(this->parent());
    int retry = ui->spinBoxRetries->value();
    try{
      for(int i = 0; i< retry; i++){
          //mymainwindow->sendFPGAReset();
          QVector<bool> isAfeAligned({0,0,0,0,0});
          QVector<QString> isAfeAlignedStr({"","","","",""});
          this->alignAFEs(mymainwindow->getSocket(), isAfeAligned, isAfeAlignedStr);
          if(this->areAFEsAligned(isAfeAligned)){
              this->writeAlignedLabels(isAfeAlignedStr);
              break;
          }
      }
    }catch(serialException &e){
      e.handleException(mymainwindow);
    }catch(ethernetUDPException &e){
      e.handleException(mymainwindow);
    }
}

bool DialogAligment::areAFEsAligned(const QVector<bool> &isAfeAligned){
    for(int i = 0; i<isAfeAligned.length(); i++){
        if(isAfeAligned[i] == 0){
            return false;
        }else{
            continue;
        }
    }
    return true;
}

void DialogAligment::writeAlignedLabels(const QVector<QString> &isAfeAlignedStr){

    ui->labelAFE0->setText(isAfeAlignedStr[0]);
    ui->labelAFE1->setText(isAfeAlignedStr[1]);
    ui->labelAFE2->setText(isAfeAlignedStr[2]);
    ui->labelAFE3->setText(isAfeAlignedStr[3]);
    ui->labelAFE4->setText(isAfeAlignedStr[4]);
}

void DialogAligment::checkBoxVersion2AClicked(){
  ui->checkBoxVersion2A->setChecked(true);
  ui->checkBoxVersion1->setChecked(false);
}

void DialogAligment::checkBoxVersion1Clicked(){
  ui->checkBoxVersion2A->setChecked(false);
  ui->checkBoxVersion1->setChecked(true);
}

void DialogAligment::alignAFEs(DaphneSocket *socket, QVector<bool> &isAfeAligned, QVector<QString> &isAfeAlignedStr){
  if(ui->checkBoxVersion2A->isChecked()){
    socket->alignAFEsV2A(ui->spinBoxRetries->value(),isAfeAligned,isAfeAlignedStr);
  }else if(ui->checkBoxVersion1->isChecked()){
    socket->alignAFEsV1(ui->spinBoxRetries->value(),isAfeAligned,isAfeAlignedStr);
  }
}
