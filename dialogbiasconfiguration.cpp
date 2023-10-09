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

void DialogBiasConfiguration::setBiasOffsetValues(QVector<double> biasOFFSETvalues){
    ui->spinBox_AFE0_BiasOffset->setValue(biasOFFSETvalues[0]);
    ui->spinBox_AFE1_BiasOffset->setValue(biasOFFSETvalues[1]);
    ui->spinBox_AFE2_BiasOffset->setValue(biasOFFSETvalues[2]);
    ui->spinBox_AFE3_BiasOffset->setValue(biasOFFSETvalues[3]);
    ui->spinBox_AFE4_BiasOffset->setValue(biasOFFSETvalues[4]);
}

QVector<double> DialogBiasConfiguration::getBiasOffsetValues(){
    QVector<double> offsetValues;
    offsetValues.append(ui->spinBox_AFE0_BiasOffset->value());
    offsetValues.append(ui->spinBox_AFE1_BiasOffset->value());
    offsetValues.append(ui->spinBox_AFE2_BiasOffset->value());
    offsetValues.append(ui->spinBox_AFE3_BiasOffset->value());
    offsetValues.append(ui->spinBox_AFE4_BiasOffset->value());
    return offsetValues;
}
