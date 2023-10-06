#include "dialogbiasconfiguration.h"
#include "ui_dialogbiasconfiguration.h"

DialogBiasConfiguration::DialogBiasConfiguration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBiasConfiguration)
{
    ui->setupUi(this);
}

DialogBiasConfiguration::~DialogBiasConfiguration()
{
    delete ui;
}

void DialogBiasConfiguration::setBiasControlValue(double biasControlValue){
    ui->spinBoxBiasControl->setValue(biasControlValue);
}

double DialogBiasConfiguration::getBiasControlValue(){
    return ui->spinBoxBiasControl->value();
}
