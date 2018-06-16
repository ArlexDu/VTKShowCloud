#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkVectorText.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPoints.h>
#include <vtkPolyVertex.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkSmartPointer.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>
#include <vtkVertexGlyphFilter.h>
#include <liblas/liblas.hpp>
#include <QtCharts/QLineSeries>
#include <QValueAxis>
#include <QFileDialog.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <QMessageBox>


#include "ui_mainwindow.h"
#include "waveform/HS_Lidar.h"
#include "pointcloud/nvtkDataReader.h"
#include "pointcloud/nvtkPointPickerInteractorStyle.h"
#include "waveform/chart.h"
#include "waveform/drawdata.h"
#include "waveform/drawchartthread.h"
#include "waveform/chartview.h"
#include "pointcloud/nvtkDataSimplify.h"
#include "waveform/listItem.h"
#include "vtkBoxWidget.h"

#include <iostream>
#include <vector>

namespace Ui {
    class MainWindow;
}

// Forward Qt class declarations
class Ui_MainWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() override;
    Chart *chart;
    ChartView *chartView;
    DrawChartThread *drawChartThread;

private slots:
    void slotExit();
    void openLas();
    void showWidght();
    void on_actionMoveRight_triggered();
    void on_actionMoveLeft_triggered();
    void on_actionOceanStart_triggered();
    void on_actionOpenOceanFile_triggered();
    void on_showWave_clicked();
public slots:
    void onDrawChanged(DrawData info);

private:
    vector<vtkBoxWidget*> boxs;
    Ui_MainWindow *ui;
};

#endif // MAINWINDOW_H
