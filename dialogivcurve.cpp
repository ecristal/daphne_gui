#include "dialogivcurve.h"
#include "ui_dialogivcurve.h"

DialogIVcurve::DialogIVcurve(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogIVcurve)
{
  ui->setupUi(this);
}

DialogIVcurve::~DialogIVcurve()
{
  delete ui;
  ui->widgetIVgraph->addGraph();
 //ui->widgetIVgraph->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, "I-V Curve", QFont("sans", 12, QFont::Bold)));
  ui->widgetIVgraph->xAxis->setLabel("Bias Voltage [DAC]");
  ui->widgetIVgraph->yAxis->setLabel("Current");
}
