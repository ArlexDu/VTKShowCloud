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

int main()
{
    FILE *fp = NULL;
    if ((fp=fopen("/Users/arlex/Desktop/111.txt","r"))== NULL)
    {
        printf("Error in open file mbr.asc\n");
        return 1;
    }
    vtkSmartPointer<vtkRenderer> ren=vtkSmartPointer<vtkRenderer>::New();
    double arr[3];

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    int n=0;
    while(!feof(fp))//首先读取点云数据到点表points同时指定点对应的id:
    {
        int ret=fscanf(fp,"%lf %lf %lf",&arr[0],&arr[1],&arr[2]);
        if(ret!=3)
            break;
        points->InsertPoint(n,arr[0],arr[1],arr[2]);
        n++;
    }
    printf("%d\n", n);
    fclose(fp);

    vtkSmartPointer<vtkPolyVertex> polyvertex = vtkSmartPointer<vtkPolyVertex>::New();
    polyvertex->GetPointIds()->SetNumberOfIds(n);
    int i=0;
    for(i=0;i<n;i++)//建立拓扑关系
    {
        polyvertex->GetPointIds()->SetId(i,i);
    }

    vtkSmartPointer<vtkUnstructuredGrid> grid=vtkSmartPointer<vtkUnstructuredGrid>::New();
    grid->SetPoints(points);
    grid->InsertNextCell(polyvertex->GetCellType(),
                         polyvertex->GetPointIds());

    vtkSmartPointer<vtkDataSetMapper> map1 = vtkSmartPointer<vtkDataSetMapper>::New();
    map1->SetInputData(grid);

    vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
    actor1->SetMapper(map1);
    actor1->GetProperty()->SetColor(0.194,0.562, 0.75);

    ren->AddActor(actor1);
    ren->SetBackground(1, 1, 1);

    vtkSmartPointer<vtkRenderWindow> win=vtkSmartPointer<vtkRenderWindow>::New();
    win->AddRenderer(ren);
    win->SetSize(400,400);
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