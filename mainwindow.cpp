#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include "kde.h"
#include <iostream>
#include <fstream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    string fname = "data.csv";
    KDE kde;
    vector<float> f_DataList = kde.readData(fname);
    KDEResult kde_result = kde.get_kde_result(f_DataList);

    size_t DataLength = kde_result.DataLength;
    float DataMaxValue = kde_result.DataMaxValue;
    int x_axis_min = kde_result.i_xMin;
    int x_axis_max = kde_result.i_xMax;
    float y_axis_max = kde_result.i_yMax;

    QVector<double> x(DataLength);
    QVector<double> y(DataLength);

    for (size_t i=0; i < DataLength; i++)
    {
        x[i] = kde_result.f_xList[i];
        y[i] = kde_result.f_yList[i];
    }

        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setData(x, y);
        ui->customPlot->xAxis->setLabel("x");
        ui->customPlot->yAxis->setLabel("y");
        ui->customPlot->xAxis->setRange(x_axis_min, x_axis_max);
        ui->customPlot->yAxis->setRange(0, y_axis_max);
        ui->customPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

