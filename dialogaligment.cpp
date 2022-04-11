#include "dialogaligment.h"
#include "ui_dialogaligment.h"

DialogAligment::DialogAligment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAligment)
{
    ui->setupUi(this);
    //connect(ui->)
}

DialogAligment::~DialogAligment()
{
    delete ui;
}

void DialogAligment::pushButtonAlignPressed(){

}
