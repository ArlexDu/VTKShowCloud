#include "vtk-8.1/vtkActor.h"
#include "vtk-8.1/vtkRenderer.h"
#include "vtk-8.1/vtkRenderWindow.h"
#include "vtk-8.1/vtkRenderWindowInteractor.h"
#include "vtk-8.1/vtkProperty.h"
#include "vtk-8.1/vtkInteractorStyleTrackballCamera.h"
#include "vtk-8.1/vtkPoints.h"
#include "vtk-8.1/vtkPolyVertex.h"
#include "vtk-8.1/vtkUnstructuredGrid.h"
#include "vtk-8.1/vtkDataSetMapper.h"
#include "vtk-8.1/vtkSmartPointer.h"
#include "vtk-8.1/vtkUnsignedCharArray.h"
#include "vtk-8.1/vtkPointData.h"
#include "vtk-8.1/vtkVertexGlyphFilter.h"
#include <liblas/liblas.hpp>
int main()
{

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
    printf("\n filter number is %d",vertexFilter->GetOutput()->GetNumberOfCells());
    mapper->SetInputData(vertexFilter->GetOutput());
    //设置点的颜色
    mapper->SetScalarModeToUsePointFieldData();
    mapper->SelectColorArray("Colors");

    vtkSmartPointer<vtkRenderer> ren=vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
//    actor->GetProperty()->SetColor(0.194,0.562, 0.75);

    ren->AddActor(actor);
    ren->SetBackground(0, 0, 0);

    vtkSmartPointer<vtkRenderWindow> win=vtkSmartPointer<vtkRenderWindow>::New();
    win->AddRenderer(ren);
    win->SetSize(800,800);
    win->BordersOn();
    //win->FullScreenOn();
    //win->HideCursor();

    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    iren->SetRenderWindow(win);
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
            vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    iren->SetInteractorStyle(style);

    iren->Start();

    return 0;
}