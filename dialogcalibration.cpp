#include "dialogcalibration.h"
#include "ui_dialogcalibration.h"

#include <TCanvas.h>
#include <TApplication.h>
#include <TSystem.h>
#include <TRootCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TMath.h>

dialogCalibration::dialogCalibration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogCalibration)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(PlotSineWave()));
    connect(&this->rootProcessTimer,SIGNAL(timeout()),this,SLOT(processROOTEvents()));
    this->rootProcessTimer.start(20);
}

dialogCalibration::~dialogCalibration()
{
    this->rootProcessTimer.stop();
    delete ui;
}

void dialogCalibration::PlotSineWave() {
    // Create a TCanvas for displaying the graph

    int nPoints = 100;
    TCanvas *c1 = new TCanvas("c1", "Sine Wave", 800, 600);

    // Create arrays to store the x and y values for the sine wave
    double x[nPoints];
    double y[nPoints];

    // Fill the arrays with the sine wave data
    for (int i = 0; i < nPoints; ++i) {
        x[i] = i * (2 * TMath::Pi() / nPoints); // X values from 0 to 2*pi
        y[i] = TMath::Sin(x[i]); // Y values for the sine wave
    }

    // Create a TGraph object and set the data
    TGraph *graph = new TGraph(nPoints, x, y);

    // Set axis labels
    graph->GetXaxis()->SetTitle("x");
    graph->GetYaxis()->SetTitle("sin(x)");

    // Set the title of the graph
    graph->SetTitle("Sine Wave");

    // Draw the graph
    graph->Draw("AL"); // A: Axis, L: Line

    // Update the canvas
    c1->Update();

    // Keep the application running
    c1->Modified();
    c1->Update();
    c1->Draw();

}

void dialogCalibration::processROOTEvents(){
    gSystem->ProcessEvents();
}
