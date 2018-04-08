/*
 * Copyright 2007 Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the
 * U.S. Government. Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */


#include "ui_SimpleView.h"
#include "SimpleView.h"

#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include "vtkGenericOpenGLRenderWindow.h"
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include "vtkSmartPointer.h"
#include <vtkVectorText.h>
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkPoints.h"
#include "vtkPolyVertex.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkSmartPointer.h"
#include "vtkUnsignedCharArray.h"
#include "vtkPointData.h"
#include "vtkVertexGlyphFilter.h"
#include <liblas/liblas.hpp>



// Constructor
SimpleView::SimpleView()
{
  this->ui = new Ui_SimpleView;
  this->ui->setupUi(this);

  // Qt Table View
  this->TableView = vtkSmartPointer<vtkQtTableView>::New();

  // Place the table view in the designer form
  this->ui->tableFrame->layout()->addWidget(this->TableView->GetWidget());

  // Geometry
  unsigned int red[3] = {255,0,0};
  unsigned int green[3] = {0,255,0};
  unsigned int blue[3] = {0,0,255};
  //数据的导入
  std::ifstream ifs;
  ifs.open("/Users/arlex/Downloads/111.las",std::ios::in|std::ios::binary);
  liblas::ReaderFactory f;
  liblas::Reader reader = f.CreateWithStream(ifs);
  double arr[3];
  //空间结构的处理
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  //颜色值
  vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
  colors->SetNumberOfComponents(3);
  colors->SetName("Colors");
  int n=0;
  while(reader.ReadNextPoint()){
    liblas::Point const& p = reader.GetPoint();
    arr[0] = p.GetX();
    arr[1] = p.GetY();
    arr[2] = p.GetZ();
    points->InsertPoint(n,arr[0],arr[1],arr[2]);
//        if(arr[2]<17){
//            colors->InsertNextTuple3(blue[0],blue[1],blue[2]);
//        }else{
//            colors->InsertNextTuple3(green[0],green[1],green[2]);
//        }
    colors->InsertNextTuple3(red[0],red[1],red[2]);
    n++;
  }
  printf("%d\n", n);

  //组织结构/拓扑结构的处理
  vtkSmartPointer<vtkPolyData> grid=vtkSmartPointer<vtkPolyData>::New();
  grid->SetPoints(points);
  grid->GetPointData()->AddArray(colors);
  vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
  vertexFilter->SetInputData(grid);
  vertexFilter->Update();
  vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
//  printf("\n filter number is %d",vertexFilter->GetOutput()->GetNumberOfCells());
  mapper->SetInputData(vertexFilter->GetOutput());
  //设置点的颜色
  mapper->SetScalarModeToUsePointFieldData();
  mapper->SelectColorArray("Colors");

  // Actor in scene
  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  // VTK Renderer
  vtkNew<vtkRenderer> ren;

  // Add Actor to renderer
  ren->AddActor(actor);

  // VTK/Qt wedded
  vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
  this->ui->qvtkWidget->SetRenderWindow(renderWindow);
  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(ren);

  // Just a bit of Qt interest: Culling off the
  // point data and handing it to a vtkQtTableView
  vtkNew<vtkDataObjectToTable> toTable;
  toTable->SetInputData(vertexFilter->GetOutput());
  toTable->SetFieldType(vtkDataObjectToTable::POINT_DATA);

  // Here we take the end of the VTK pipeline and give it to a Qt View
  this->TableView->SetRepresentationFromInputConnection(toTable->GetOutputPort());

  // Set up action signals and slots
  connect(this->ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));

};

SimpleView::~SimpleView()
{
  // The smart pointers should clean up for up

}

// Action to be taken upon file open
void SimpleView::slotOpenFile()
{

}

void SimpleView::slotExit() {
  qApp->exit();
}
