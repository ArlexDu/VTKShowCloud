#ifndef __CHARTVIEW_H__
#define __CHARTVIEW_H__

#include <QChartView>
#include <QRubberBand>

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
public:
    ChartView(QChart *chart, QWidget *parent = 0);
    int zoomLevel;
    int scrolledX[100];
    int scrolledY[100];
    void getToZoomLevel();

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);


private:
    bool isClicking;

    int xOld;
    int yOld;
};

#endif /* __CHARTVIEW_H__ */
