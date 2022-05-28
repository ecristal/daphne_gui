#include "dialogacquisitionconfiguration.h"
#include "ui_dialogacquisitionconfiguration.h"

DialogAcquisitionConfiguration::DialogAcquisitionConfiguration(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogAcquisitionConfiguration)
{
  ui->setupUi(this);
}

DialogAcquisitionConfiguration::~DialogAcquisitionConfiguration()
{
  delete ui;
}
