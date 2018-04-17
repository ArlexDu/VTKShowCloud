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

#include "ui_mainwindow.h"
#include "HS_Lidar.h"
#include "vtkDataReader.h"

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

private slots:
    void slotExit();
    void openLas();

private:
    Ui_MainWindow *ui;
};

#endif // MAINWINDOW_H
