#include "dialogaligment.h"
#include "ui_dialogaligment.h"

DialogAligment::DialogAligment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAligment)
{
    ui->setupUi(this);
    this->setWindowTitle("ALIGNMENT");
    connect(ui->pushButtonAlign,SIGNAL(clicked(bool)), this, SLOT(pushButtonAlignPressed()));
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
          mymainwindow->sendFPGAReset();
          QVector<bool> isAfeAligned({0,0,0,0,0});
          QVector<QString> isAfeAlignedStr({"","","","",""});
          mymainwindow->getSocket()->alignAFEs(100,isAfeAligned,isAfeAlignedStr);
          if(this->areAFEsAligned(isAfeAligned)){
              this->writeAlignedLabels(isAfeAlignedStr);
              break;
          }
      }
    }catch(serialException &e){
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
