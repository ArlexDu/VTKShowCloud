#ifndef CHART_H
#define CHART_H
#include <QChartView>
#include <QLineSeries>
#include "drawdata.h"
class Chart
{
public:

    QtCharts::QChart *chart;
    QtCharts::QLineSeries *lines1;
    QtCharts::QLineSeries *lines2;
    QtCharts::QLineSeries *lines3;
    QtCharts::QLineSeries *lines4;
    int frameSize;
    int offset;
    int chartType;
    QString filename;
    bool isPlay;
    Chart();
    void newFrame(DrawData info);

};

#endif // CHART_H
