#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <fstream>
#include <algorithm>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makePlot(bool firstTime)
{
    auto numberOfValues=m_yVec.size();
    QVector<double> xAxis(numberOfValues);
    std::iota(xAxis.begin(),xAxis.end(),0);
    if(!firstTime)
        ui->customPlot->removeGraph(0);
    ui->customPlot->addGraph(0);

    ui->customPlot->graph(0)->setData(xAxis, m_yVec);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, xAxis.size());
    ui->customPlot->yAxis->setRange(*(std::min_element(m_yVec.begin(),m_yVec.end()))-2,  *(std::max_element(m_yVec.begin(),m_yVec.end()))+2);

    ui->customPlot->replot();
}

void  MainWindow::KeepPeaksAndValleys()
{

    for(auto i=1;i<m_yVec.size()-1;i++)
    {
       if((m_yVec[i]>m_yVec[i-1] && m_yVec[i]>m_yVec[i+1]) || (m_yVec[i]<m_yVec[i-1] && m_yVec[i]<m_yVec[i+1])) continue;
       m_yVec.removeAt(i);
       i--;
    }
}

void MainWindow::on_Btn_loadFile_clicked()
{
    QString filename=QFileDialog::getOpenFileName(
                this,tr("Open file"),"/home","All files(*.*);;Text file(*.txt)"
                );
    std::ifstream ifs (filename.toStdString(), std::ifstream::in);
     while (ifs.good()) {
       double x;
       ifs>>x;
       m_yVec.append(x);
     }
     ifs.close();
     KeepPeaksAndValleys();
     makePlot();

}

void MainWindow::on_btn_cycle_clicked()
{
    for(auto i=0;i<m_yVec.size()-3;i++) {
         if(((m_yVec[i] <= m_yVec[i+1] && m_yVec[i]<=m_yVec[i+2])&& (m_yVec[i+3] >= m_yVec[i+1] && m_yVec[i+3]>=m_yVec[i+2]))||
                 ((m_yVec[i] >= m_yVec[i+1] && m_yVec[i]>=m_yVec[i+2])&& (m_yVec[i+3] <= m_yVec[i+1] && m_yVec[i+3]<=m_yVec[i+2]))) {
            auto outerStress = std::abs(m_yVec[i+3]-m_yVec[i]);
            auto innerStress = std::abs(m_yVec[i+2]-m_yVec[i+1]);
            if(innerStress<=outerStress) {
                m_yVec.removeAt(i+1);
                m_yVec.removeAt(i+1);
                break;

            }
        }
    }
    makePlot(false);

}


