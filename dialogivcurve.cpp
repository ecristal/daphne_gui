#include "dialogivcurve.h"
#include "ui_dialogivcurve.h"

DialogIVcurve::DialogIVcurve(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogIVcurve)
{
  ui->setupUi(this);
  initializeWindow();
  connect(ui->spinBoxBiasLow,SIGNAL(valueChanged(int)),this,SLOT(spinBoxBiasLowValueChanged()));
  connect(ui->spinBoxBiasUpper,SIGNAL(valueChanged(int)),this,SLOT(spinBoxBiasUpperValueChanged()));
  connect(ui->pushButtonSaveData,SIGNAL(clicked(bool)),this,SLOT(pushButtonSaveDataPressed()));
  connect(ui->pushButtonStart,SIGNAL(clicked(bool)),this,SLOT(pushButtonStartPressed()));
  connect(ui->pushButtonPause,SIGNAL(clicked(bool)),this,SLOT(pushButtonPausePressed()));
  connect(ui->pushButtonStop,SIGNAL(clicked(bool)),this,SLOT(pushButtonStopPressed()));
  connect(ui->pushButtonSelectFile,SIGNAL(clicked(bool)),this,SLOT(openIVCurveFile()));
  connect(ui->pushButtonCalculate,SIGNAL(clicked(bool)),this,SLOT(plotSignalsAndCalculateBreakdown()));

  this->F.resize(2,2);
  this->F(0,0) = 1.889033079394525;
  this->F(0,1) = -0.894874344616636;
  this->F(1,0) = 1.0;
  this->F(1,1) = 0.0;

  this->G.resize(2,1);
  this->G(0,0) = 0.062500000000000;
  this->G(1,0) = 0.0;

  this->H.resize(1,2);
  this->H(0,0) = 0.090867494697225;
  this->H(0,1) = 0.002456267338970;

  this->D.resize(1,1);
  this->D(0,0) = 0.001460316305528;
}

DialogIVcurve::~DialogIVcurve()
{

  delete ui;

}

void DialogIVcurve::initializeWindow(){
  this->setWindowTitle("I-V Curve");
  ui->widgetIVgraph->addGraph();
  ui->widgetBreakdownCalculation->addGraph();
  ui->widgetBreakdownCalculation->addGraph(ui->widgetBreakdownCalculation->xAxis,ui->widgetBreakdownCalculation->yAxis2);
  ui->widgetBreakdownCalculation->addGraph(ui->widgetBreakdownCalculation->xAxis,ui->widgetBreakdownCalculation->yAxis2);
  ui->widgetBreakdownCalculation->graph(0)->setPen(QPen(Qt::darkRed));
  ui->widgetBreakdownCalculation->graph(0)->setName("SiPM current");
  ui->widgetBreakdownCalculation->graph(1)->setName("diff - SiPM current");
  ui->widgetBreakdownCalculation->graph(2)->setName("Est. Breakdown Voltage");
  ui->widgetBreakdownCalculation->graph(1)->setPen(QPen(Qt::blue));
  ui->widgetBreakdownCalculation->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
  QPen redDotPen;
  redDotPen.setColor(Qt::red);
  redDotPen.setStyle(Qt::DotLine);
  ui->widgetBreakdownCalculation->graph(2)->setPen(redDotPen);
  ui->widgetBreakdownCalculation->yAxis->setScaleType(QCPAxis::stLogarithmic);
  ui->widgetBreakdownCalculation->yAxis2->setScaleType(QCPAxis::stLogarithmic);
  ui->widgetBreakdownCalculation->yAxis2->setVisible(true);
  ui->widgetBreakdownCalculation->legend->setVisible(true);
  ui->widgetIVgraph->xAxis->setLabel("Bias Voltage [Volts]");
  ui->widgetIVgraph->yAxis->setLabel("Measure");
  ui->widgetBreakdownCalculation->xAxis->setLabel("Bias Voltage [Volts]");
  ui->widgetBreakdownCalculation->yAxis->setLabel("Measure");
  ui->pushButtonPause->setEnabled(false);
  ui->pushButtonStop->setEnabled(false);
  ui->pushButtonSaveData->setEnabled(true); //just to test for now
}

void DialogIVcurve::spinBoxBiasLowValueChanged(){
  int biasLowValue = ui->spinBoxBiasLow->value();
  int biasUpperValue = ui->spinBoxBiasUpper->value();
  if(biasLowValue > biasUpperValue){
    ui->spinBoxBiasUpper->setValue(biasLowValue+2);
    ui->spinBoxBiasUpper->setMinimum(biasLowValue + 2);
    biasUpperValue = biasLowValue + 2;
  }
  int biasStepMaximun = (biasUpperValue - biasLowValue)/2;
  ui->spinBoxBiasStep->setMaximum(biasStepMaximun);
}

void DialogIVcurve::spinBoxBiasUpperValueChanged(){
  int biasLowValue = ui->spinBoxBiasLow->value();
  int biasUpperValue = ui->spinBoxBiasUpper->value();
  int biasStepMaximun = (biasUpperValue - biasLowValue)/2;
  ui->spinBoxBiasStep->setMaximum(biasStepMaximun);
}

void DialogIVcurve::pushButtonSaveDataPressed(){

  try{
    if(this->yValues.length() == 0){
      throw this->yValues.length();
    }
    QFile f(QFileDialog::getSaveFileName(this, "Save file", "", ".dat"));
    f.open( QIODevice::WriteOnly );
    QTextStream writeToFile(&f);
    for(int i = 0; i < this->yValues.length(); i++){
      writeToFile << QString::number(ui->spinBoxUpperBiasVoltage->value() - this->xValues.at(i)/1000.0) << "\t" << QString::number(this->yValues.at(i)) << "\n";
    }
    f.close();
  }catch(int number){
    QMessageBox::critical(this, tr("Error"), "No data to save.\nVector has length: " + QString::number(number));
  }

}

void DialogIVcurve::pushButtonStartPressed(){

  ui->pushButtonStart->setText("Start");
  ui->pushButtonStart->setEnabled(false);
  ui->pushButtonPause->setEnabled(true);
  ui->pushButtonStop->setEnabled(false);

  ui->spinBoxBiasLow->setEnabled(false);
  ui->spinBoxBiasStep->setEnabled(false);
  ui->spinBoxBiasUpper->setEnabled(false);
  ui->spinBoxChannelNumber->setEnabled(false);
  ui->spinBoxUpperBiasVoltage->setEnabled(false);

  int biasLowValue = ui->spinBoxBiasLow->value();
  int biasUpperValue = ui->spinBoxBiasUpper->value();
  int biasStepValues = ui->spinBoxBiasStep->value();
  int points = (biasUpperValue - biasLowValue)/biasStepValues + 1;
  QVector<double> x_graph;
  if(this->pausePressedFLAG){
    for(int i = 0; i<this->initialPoistion;i++){
      x_graph.append(ui->spinBoxUpperBiasVoltage->value() - this->xValues.at(i)/1000.0);
    }
    this->pausePressedFLAG = false;
  }else{
    this->initialPoistion = 0;
    this->xValues.clear();
    this->yValues.clear();
    for(int i = 0; i < points; i++){
      //this->xValues.append(biasStepValues*i + biasLowValue);
      this->xValues.append(biasUpperValue - biasStepValues*i);
    }
    if(this->xValues.last() != biasLowValue){
      this->xValues.append(biasLowValue);
    }
  }

  ui->widgetIVgraph->xAxis->setRange(ui->spinBoxUpperBiasVoltage->value() - ui->spinBoxBiasUpper->value()/1000.0,ui->spinBoxUpperBiasVoltage->value());
  MainWindow *mymainwindow = reinterpret_cast<MainWindow*>(this->parent());

  try{

    this->runningFLAG = true;
    ui->spinBoxChannelNumber->setEnabled(false);
    ui->pushButtonSaveData->setEnabled(false);

    //QString command = "WR VBIASCTRL V 1100\r\n";
    //mymainwindow->sendCommand(command);

    if(ui->spinBoxUpperBiasVoltage->value() > mymainwindow->getBiasControlValue()){
        throw biasVoltageException(2);
    }else if(ui->spinBoxUpperBiasVoltage->value() < biasUpperValue/1000.0){
        throw biasVoltageException(1);
    }


    QString command = "WR AFE ";
    int afe_number = mymainwindow->getAFENumberFromChannelNumber(ui->spinBoxChannelNumber->value());
    command = command + QString::number(afe_number);
    command = command + " BIASSET V ";
    double biasVoltageValue = ui->spinBoxUpperBiasVoltage->value() + mymainwindow->getBIASOffsetValues().at(afe_number);
    command = command + QString::number((int)(1000*biasVoltageValue/39.314));
    command = command + "\r\n";
    mymainwindow->sendCommand(command);
    qDebug() << command;

    for(int i=this->initialPoistion; i<this->xValues.length();i++){
      if(this->pausePressedFLAG){
        ui->pushButtonStop->setEnabled(true);
        ui->pushButtonPause->setEnabled(false);
        ui->pushButtonSaveData->setEnabled(true);
        ui->pushButtonStart->setEnabled(true);
        ui->pushButtonStart->setText("Resume");
        break;
      }

      // comunication code here
      command = "WR TRIM CH ";
      command = command + QString::number(ui->spinBoxChannelNumber->value());
      command = command + " V ";
      command = command + QString::number(this->xValues.at(i));
      command = command + "\r\n";
      mymainwindow->sendCommand(command);
      qDebug() << command;

//      command = "WR AFE ";
//      int afe_number = mymainwindow->getAFENumberFromChannelNumber(ui->spinBoxChannelNumber->value());
//      command = command + QString::number(afe_number);
//      command = command + " BIASSET V ";
//      command = command + QString::number(this->xValues.at(i));
//      command = command + "\r\n";
//      mymainwindow->sendCommand(command);
//      qDebug() << command;

      command = "RD CM CH ";
      command = command + QString::number(ui->spinBoxChannelNumber->value());
      command = command + "\r\n";
      mymainwindow->sendCommand(command);
      qDebug() << command;

      QString serial_string = mymainwindow->getSerialString();

      QStringList serial_split = serial_string.split("=");
      serial_string = serial_split.last();
      //qDebug() << "Serial String :: " << serial_string;

      serial_split = serial_string.split("\r");
      serial_string = serial_split.first();
      double adc_value = serial_string.toDouble();
      //qDebug() << "Serial String :: " << serial_string << "double :: " << adc_value;
      if(adc_value == 0){
          i--;
      }else{
          this->yValues.append(adc_value);
          //this->yValues.append(this->rg.generateDouble()*200); // comment out
          //

          double y_min = *std::min_element(this->yValues.begin(),this->yValues.end());
          double y_max = *std::max_element(this->yValues.begin(),this->yValues.end());
          double lim = 0.1*std::abs(y_max - y_min);

          ui->widgetIVgraph->yAxis->setRange(y_min - 0.1*lim, y_max + 0.1*lim);
          x_graph.append(ui->spinBoxUpperBiasVoltage->value() - this->xValues.at(i)/1000.0);
          ui->widgetIVgraph->graph(0)->setData(x_graph,this->yValues);
          //qDebug() << this->xValues.at(i) << "::" << this->yValues.at(i);
          //qDebug() << x_graph.length() << "::" << this->yValues.length();
          ui->widgetIVgraph->replot();
          this->initialPoistion++;
          //mymainwindow->delayMilli(500);
      }
    }
    if(!this->pausePressedFLAG){
      ui->pushButtonStart->setEnabled(true);
      ui->pushButtonPause->setEnabled(false);
      ui->pushButtonStop->setEnabled(false);

      ui->spinBoxBiasLow->setEnabled(true);
      ui->spinBoxBiasStep->setEnabled(true);
      ui->spinBoxBiasUpper->setEnabled(true);
      ui->spinBoxChannelNumber->setEnabled(true);
      ui->spinBoxUpperBiasVoltage->setEnabled(true);

      QString command = "WR TRIM CH ";
      command = command + QString::number(ui->spinBoxChannelNumber->value());
      command = command + " V ";
      command = command + QString::number(0);
      command = command + "\r\n";
      qDebug() << command;

      command = "WR AFE ";
      int afe_number = mymainwindow->getAFENumberFromChannelNumber(ui->spinBoxChannelNumber->value());
      command = command + QString::number(afe_number);
      command = command + " BIASSET V ";
      command = command + QString::number(0);
      command = command + "\r\n";
      mymainwindow->sendCommand(command);
      qDebug() << command;

      ui->spinBoxChannelNumber->setEnabled(true);
      ui->pushButtonSaveData->setEnabled(true);
      this->runningFLAG = false;
    }
  }catch(serialException &e){
    e.handleException(mymainwindow);
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);
    ui->pushButtonSaveData->setEnabled(true);
    ui->spinBoxChannelNumber->setEnabled(true);
    ui->spinBoxBiasLow->setEnabled(true);
    ui->spinBoxBiasStep->setEnabled(true);
    ui->spinBoxBiasUpper->setEnabled(true);
    ui->spinBoxUpperBiasVoltage->setEnabled(true);

    this->runningFLAG = false;
  }catch(biasVoltageException &e){
    e.handleException(mymainwindow);
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);
    ui->pushButtonSaveData->setEnabled(true);
    ui->spinBoxChannelNumber->setEnabled(true);
    ui->spinBoxBiasLow->setEnabled(true);
    ui->spinBoxBiasStep->setEnabled(true);
    ui->spinBoxBiasUpper->setEnabled(true);
    ui->spinBoxUpperBiasVoltage->setEnabled(true);
    this->runningFLAG = false;
  }
}

void DialogIVcurve::pushButtonPausePressed(){

  this->pausePressedFLAG = true;

}

void DialogIVcurve::pushButtonStopPressed(){

  this->pausePressedFLAG = false;
  this->runningFLAG = false;
  ui->spinBoxChannelNumber->setEnabled(true);
  ui->pushButtonStop->setEnabled(false);
  ui->pushButtonPause->setEnabled(false);
  ui->pushButtonStart->setEnabled(true);
  ui->pushButtonStart->setText("Start");

  ui->spinBoxBiasLow->setEnabled(true);
  ui->spinBoxBiasStep->setEnabled(true);
  ui->spinBoxBiasUpper->setEnabled(true);
  ui->spinBoxChannelNumber->setEnabled(true);
  ui->spinBoxUpperBiasVoltage->setEnabled(true);

  MainWindow *mymainwindow = reinterpret_cast<MainWindow*>(this->parent());
  try{
      QString command = "WR TRIM CH ";
      command = command + QString::number(ui->spinBoxChannelNumber->value());
      command = command + " V ";
      command = command + QString::number(0);
      command = command + "\r\n";
      qDebug() << command;

      command = "WR AFE ";
      int afe_number = mymainwindow->getAFENumberFromChannelNumber(ui->spinBoxChannelNumber->value());
      command = command + QString::number(afe_number);
      command = command + " BIASSET V ";
      command = command + QString::number(0);
      command = command + "\r\n";
      mymainwindow->sendCommand(command);
      qDebug() << command;
  }catch(serialException &e){
      e.handleException(mymainwindow);
  }

}

void DialogIVcurve::closeEvent(QCloseEvent *event)
{
  if(this->runningFLAG){
    QMessageBox::warning(this, tr("Warning"), "IV curve is still running.\nPlease PAUSE and STOP the process to exit.");
    event->ignore();
  }else{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "DAPHNE GUI",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
      event->ignore();
    } else {
      event->accept();
    }
  }
}

void DialogIVcurve::openIVCurveFile(){
    QString QFileDialog_returned_string = QFileDialog::getOpenFileName(this,"Open IV File",this->multiple_waveforms_folder_address,"*.dat");
    if(!QFileDialog_returned_string.isEmpty()){
        this->IVcurveFileName = QFileDialog_returned_string;
        ui->lineEditSelectedFile->setText(this->IVcurveFileName);
        ui->pushButtonCalculate->setEnabled(true);
    }
}

void DialogIVcurve::setWorkingFolder(QString workingFolder){
     this->multiple_waveforms_folder_address = workingFolder;
}

QVector<double> DialogIVcurve::calculateFilteredSignal(QVector<double> &inputData){

    Eigen::MatrixXd Hc_f;
    Eigen::MatrixXd Hc_b;
    Eigen::MatrixXd Oc_f;
    Eigen::MatrixXd Oc_b;

    Eigen::MatrixPower<Eigen::MatrixXd> Fpow(F);
    Eigen::MatrixXd Fpow_res;
    Eigen::MatrixXd Hrow;
    Hrow.resize(1,2);
    Fpow_res.resize(2,2);

    const int data_len = inputData.length();

    Eigen::MatrixXd signal;
    Eigen::MatrixXd Yfb_0;
    signal.resize(data_len,1);
    Yfb_0.resize(data_len,1);

    Hc_f.resize(data_len,data_len);
    Hc_b.resize(data_len,data_len);
    Oc_f.resize(data_len,2);
    Oc_b.resize(data_len,2);

    for(int i = 0; i < data_len; i++){
        for(int j = 0; j < data_len; j++){
            if(i == j){
                Hc_f(i,j) = D(0,0);
            }
            else if(i > j){
                Fpow.compute(Fpow_res,i-j-1);
                Hc_f(i,j) = (H*(Fpow_res)*G)(0,0);
            }
        }
        Fpow.compute(Fpow_res,i);
        Hrow = H*Fpow_res;
        Oc_f.block(i,0,1,2) = Hrow;
        signal(i,0) = inputData[i];
    }

    Hc_b = Hc_f.transpose();
    Oc_b = Oc_f.colwise().reverse();

    Yfb_0 = Hc_b*Hc_f*signal;

    Eigen::MatrixXd pinV_mat;
    pinV_mat.resize(data_len,4);
    pinV_mat.block(0,0,data_len,2) = Hc_b*Oc_f-Oc_f;
    pinV_mat.block(0,2,data_len,2) = Oc_b-Hc_f*Oc_b;

    Eigen::MatrixXd pinV_mat_i;
    pinV_mat_i = pinV_mat.completeOrthogonalDecomposition().pseudoInverse();

    Eigen::MatrixXd x_initials;
    x_initials.resize(4,1);
    x_initials = pinV_mat_i*(-Hc_b*Hc_f+Hc_f*Hc_b)*signal;

    Eigen::MatrixXd Omat;
    Omat.resize(data_len,4);
    Omat.block(0,0,data_len,2) = Hc_b*Oc_f;
    Omat.block(0,2,data_len,2) = Oc_b;

    Eigen::MatrixXd  filtered_signal_fb;
    filtered_signal_fb.resize(data_len,1);
    filtered_signal_fb = Yfb_0 + Omat*x_initials;

    QVector<double> filtered_signal;
    filtered_signal.resize(data_len);

    for(int i = 0; i < data_len; i++){
        filtered_signal[i] = filtered_signal_fb(i,0);
    }

    return filtered_signal;
}

void DialogIVcurve::plotSignalsAndCalculateBreakdown(){

    std::ifstream input_file(this->IVcurveFileName.toStdString());
    double voltage, measure;
    QVector<double> voltage_v, measure_v, filtered_measure, log_filtered_measure;
    QVector<double> diff_log_filtered_measure;
    while(input_file >> voltage >> measure){
        voltage_v.append(voltage);
        measure_v.append(measure);
    }

    filtered_measure = this->calculateFilteredSignal(measure_v);

    int pop_front_i = ui->spinBoxXMin->value();
    int pop_back_i = ui->spinBoxXMax->value();

    try{
        for(int i = 0; i < pop_front_i; i++){
            filtered_measure.pop_front();
            voltage_v.pop_front();
            if(filtered_measure.length() == 1 || voltage_v.length() == 1){
                throw std::exception();
            }
        }
    }catch(std::exception e){
        QMessageBox::critical(this, tr("Error"), "Index out of range.\nVector has length: " + QString::number(measure_v.length()));
        return;
    }

    try{
        for(int i = 0; i < pop_back_i; i++){
            filtered_measure.pop_back();
            voltage_v.pop_back();
            if(filtered_measure.length() == 1 || voltage_v.length() == 1){
                throw std::exception();
            }
        }
    }catch(std::exception e){
        QMessageBox::critical(this, tr("Error"), "Index out of range.\nVector has length: " + QString::number(measure_v.length()));
        return;
    }

    double y_min_filt = *std::min_element(filtered_measure.begin(),filtered_measure.end());

    for(int i = 0; i < filtered_measure.length(); i++){
        log_filtered_measure.append(std::log(filtered_measure[i] - y_min_filt));
    }

    for(int i = 1; i < log_filtered_measure.length(); i++){
        diff_log_filtered_measure.append(log_filtered_measure[i] - log_filtered_measure[i-1]);
    }

    for(int i = 0; i < log_filtered_measure.length()-1; i++){
        log_filtered_measure[i] = std::exp(log_filtered_measure[i]);
        diff_log_filtered_measure[i] = std::exp(diff_log_filtered_measure[i]);
    }

    log_filtered_measure[diff_log_filtered_measure.length() - 1] = std::exp(log_filtered_measure[diff_log_filtered_measure.length() - 1]);

    log_filtered_measure.pop_front();
    log_filtered_measure.pop_front();
    diff_log_filtered_measure.pop_front();
    voltage_v.pop_front();
    voltage_v.pop_front();

    QVector<double> voltage_v_graph, log_filtered_measure_graph,diff_log_filtered_measure_graph, est_breakdown_v, est_breakdown_y;

    int graph_min_i = ui->spinBoxXGraphMin->value();
    int graph_max_i = ui->spinBoxXGraphMax->value();

    try{
        for(int i = graph_min_i; i < voltage_v.length() - graph_max_i; i++){
            voltage_v_graph.append(voltage_v[i]);
            log_filtered_measure_graph.append(log_filtered_measure[i]);
            diff_log_filtered_measure_graph.append(diff_log_filtered_measure[i]);
        }
    }catch(std::exception e){
        QMessageBox::critical(this, tr("Error"), "Index out of range.\nVector has length: " + QString::number(voltage_v.length()));
        return;
    }


    double x_min = *std::min_element(voltage_v_graph.begin(),voltage_v_graph.end());
    double x_max = *std::max_element(voltage_v_graph.begin(),voltage_v_graph.end());

    double y_min_log_diff = *std::min_element(diff_log_filtered_measure_graph.begin(),diff_log_filtered_measure_graph.end());
    double y_max_log_diff = *std::max_element(diff_log_filtered_measure_graph.begin(),diff_log_filtered_measure_graph.end());

    double y_min_log = *std::min_element(log_filtered_measure_graph.begin(),log_filtered_measure_graph.end());
    double y_max_log = *std::max_element(log_filtered_measure_graph.begin(),log_filtered_measure_graph.end());

    int i_max_log = std::distance(diff_log_filtered_measure_graph.begin(),std::max_element(diff_log_filtered_measure_graph.begin(),diff_log_filtered_measure_graph.end()));

    ui->widgetBreakdownCalculation->xAxis->setRange(x_min, x_max);
    ui->widgetBreakdownCalculation->yAxis->setRange(y_min_log, y_max_log);
    ui->widgetBreakdownCalculation->yAxis2->setRange(y_min_log_diff, y_max_log_diff+0.005*y_max_log_diff);

    est_breakdown_y.append(y_min_log_diff);
    est_breakdown_y.append(y_max_log_diff);

    est_breakdown_v.append(voltage_v_graph[i_max_log]);
    est_breakdown_v.append(voltage_v_graph[i_max_log]);

    ui->lineEditVbrResult->setText(QString::number(voltage_v_graph[i_max_log]));

    ui->widgetBreakdownCalculation->graph(0)->setData(voltage_v_graph,log_filtered_measure_graph);
    ui->widgetBreakdownCalculation->graph(1)->setData(voltage_v_graph,diff_log_filtered_measure_graph);
    ui->widgetBreakdownCalculation->graph(2)->setData(est_breakdown_v,est_breakdown_y);
    ui->widgetBreakdownCalculation->replot();
}
