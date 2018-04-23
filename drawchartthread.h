#ifndef DRAWCHARTTHREAD_H
#define DRAWCHARTTHREAD_H
#include <QThread>
#include "chart.h"
#include "drawdata.h"


class DrawChartThread:public QThread
{
    Q_OBJECT
public:
    Chart *chart;
    DrawChartThread();
    int i;
    void run();
signals:
    void draw(DrawData info);
};

#endif // DRAWCHARTTHREAD_H
