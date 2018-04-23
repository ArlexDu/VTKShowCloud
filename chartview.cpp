#include "chartview.h"
#include <QDebug>

ChartView::ChartView(QChart *chart, QWidget *parent) :
    QChartView(chart, parent),
    isClicking(false),
    xOld(0), yOld(0)
{
    zoomLevel = 0;
    for(int i=0;i<100;i++){
        scrolledX[i] = 0;
        scrolledY[i] = 0;
    }
    setRubberBand(QChartView::RectangleRubberBand);
}


void ChartView::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
        if(zoomLevel<100){
            chart()->zoom(1.1);
            zoomLevel ++;
        }

    } else {
        if(zoomLevel>0){
            chart()->zoom(10.0/11);
            zoomLevel --;
        }

    }

}

void ChartView::getToZoomLevel(){

        for(int i=0;i<zoomLevel+1;i++)
        {
            chart()->scroll(-scrolledX[i],scrolledY[i]);
            if(i!=0||i!=zoomLevel){
               chart()->zoom(1.1);
            }

        }



}


void ChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        isClicking = true;
    } else if (event->button() & Qt::RightButton) {
        chart()->zoomReset();
        zoomLevel = 0;
        for(int i=0;i<100;i++){
            scrolledX[i] = 0;
            scrolledY[i] = 0;
        }

    }

    QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    int x, y;

    if (isClicking) {
        if (xOld == 0 && yOld == 0) {

        } else {
            x = event->x() - xOld;
            y = event->y() - yOld;
            chart()->scroll(-x, y);



            scrolledX[zoomLevel] = x+scrolledX[zoomLevel];
            qDebug()<<"scrolledX[zoomLevel]"<<scrolledX[zoomLevel];
            scrolledY[zoomLevel] = y+scrolledY[zoomLevel];
            qDebug()<<"scrolledY[zoomLevel]"<<scrolledY[zoomLevel];


        }

        xOld = event->x();
        yOld = event->y();

        return;
    }

    QChartView::mouseMoveEvent(event);
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (isClicking) {
        xOld = yOld = 0;
        isClicking = false;
    }

    /* Disable original right click event */
    if (!(event->button() & Qt::RightButton)) {
        QChartView::mouseReleaseEvent(event);
    }
}

void ChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        chart()->scroll(-10, 0);
        scrolledX[zoomLevel] +=10;
        break;
    case Qt::Key_Right:
        chart()->scroll(10, 0);
        scrolledX[zoomLevel] -= 10;
        break;
    case Qt::Key_Up:
        chart()->scroll(0, 10);
        scrolledY[zoomLevel] += 10;
        break;
    case Qt::Key_Down:
        chart()->scroll(0, -10);
        scrolledY[zoomLevel] -= 10;
        break;
    default:
        keyPressEvent(event);
        break;
    }
}
