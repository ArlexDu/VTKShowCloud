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
#include "pointcloud/nvtkDataReader.h"
int main()
{
    vtkSmartPointer<vtkLASReader> reader = vtkSmartPointer<vtkLASReader>::New();
    reader->SetFileName("/Users/arlex/Downloads/111.las");
    reader->Update();
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(reader->GetOutput());
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