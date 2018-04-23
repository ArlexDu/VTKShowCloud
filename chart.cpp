#include "chart.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtDebug>

#include<iostream>
#include <string>
#include "HS_Lidar.h"
using namespace std;
QT_CHARTS_USE_NAMESPACE



Chart::Chart()
{
    chart = new QtCharts::QChart();
    lines1 = new QtCharts::QLineSeries();
    lines2 = new QtCharts::QLineSeries();
    lines3 = new QtCharts::QLineSeries();
    lines4 = new QtCharts::QLineSeries();
    QLineSeries  *lines = new QtCharts::QLineSeries();
    lines->setPointLabelsVisible(false);

    frameSize = 0;
    offset = 0;
    isPlay = false;


//    lines1->setUseOpenGL(true);
//    lines2->setUseOpenGL(true);
//    lines3->setUseOpenGL(true);
//    lines4->setUseOpenGL(true);

    lines1->setName("CH0");
    lines2->setName("CH1");
    lines3->setName("CH2");
    lines4->setName("CH3");

    lines1->setPointLabelsVisible(true);
    lines2->setPointLabelsVisible(true);
    lines3->setPointLabelsVisible(true);
    lines4->setPointLabelsVisible(true);

    lines2->setColor(QColor(255, 0, 0, 127));
    lines3->setColor(QColor(0, 255, 0, 127));
    lines4->setColor(QColor(0, 0, 255, 127));
    lines1->setColor(QColor(205, 149, 14, 127));



    chart->addSeries(lines);

    chart->createDefaultAxes();
    chart->axisX()->setRange(0, 100);
    chart->axisY()->setRange(0, 100);


    chart->legend()->setVisible(false);
    chart->setBackgroundVisible(true);
    chart->setBackgroundPen(QPen(Qt::lightGray));
    chart->setBackgroundBrush(QBrush(QColor(240, 240, 240)));




}


void Chart::newFrame(DrawData info){

    lines1->clear();
    lines2->clear();
    lines3->clear();
    lines4->clear();

    chart->removeAllSeries();

    lines1= new QLineSeries();
    lines2= new QLineSeries();
    lines3= new QLineSeries();
    lines4= new QLineSeries();

//    lines1->setUseOpenGL(true);
//    lines2->setUseOpenGL(true);
//    lines3->setUseOpenGL(true);
//    lines4->setUseOpenGL(true);

    lines1->setName("CH0");
    lines2->setName("CH1");
    lines3->setName("CH2");
    lines4->setName("CH3");


    lines1->setColor(QColor(205, 149, 14, 127));
    lines2->setColor(QColor(255, 0, 0, 127));
    lines3->setColor(QColor(0, 255, 0, 127));
    lines4->setColor(QColor(0, 0, 255, 127));

    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0,info.x1);
    axisX->setTickCount(11);
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(info.minY,info.maxY);





    offset = info.i;

    for(int i=0;i<info.size1;i++){
        lines1->append(QPointF(i, info.line1[i]));
    }
    for(int i=0;i<info.size2;i++){
        lines2->append(QPointF(i, info.line2[i]));
    }
    for(int i=0;i<info.size3;i++){
        lines3->append(QPointF(i, info.line3[i]));
    }
    for(int i=0;i<info.size4;i++){
        lines4->append(QPointF(i, info.line4[i]));
    }

    chart->addSeries(lines1);
    chart->addSeries(lines2);
    chart->addSeries(lines3);
    chart->addSeries(lines4);


    chart->setAxisX(axisX, lines1);
    chart->setAxisY(axisY, lines1);
    chart->setAxisX(axisX, lines2);
    chart->setAxisY(axisY, lines2);
    chart->setAxisX(axisX, lines3);
    chart->setAxisY(axisY, lines3);
    chart->setAxisX(axisX, lines4);
    chart->setAxisY(axisY, lines4);

    if(chartType == 1){
        chart->removeSeries(lines1);
    }


}


