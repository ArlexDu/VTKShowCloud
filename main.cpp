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


int main()
{

    unsigned int red[3] = {255,0,0};
    unsigned int green[3] = {0,255,0};
    unsigned int blue[3] = {0,0,255};
    //数据的导入
    FILE *fp = NULL;
    if ((fp=fopen("/Users/arlex/Desktop/111.txt","r"))== NULL)
    {
        printf("Error in open file mbr.asc\n");
        return 1;
    }
    double arr[3];

    //空间结构的处理
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    //颜色值
    vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName("Colors");
    int n=0;
    while(!feof(fp))//首先读取点云数据到点表points同时指定点对应的id:
    {
        int ret=fscanf(fp,"%lf %lf %lf",&arr[0],&arr[1],&arr[2]);
        if(ret!=3)
            break;
        points->InsertPoint(n,arr[0],arr[1],arr[2]);
        if(arr[2]<17){
            colors->InsertNextTuple3(blue[0],blue[1],blue[2]);
        }else{
            colors->InsertNextTuple3(green[0],green[1],green[2]);
        }
        n++;
    }
    printf("%d\n", n);
    fclose(fp);

    //组织结构/拓扑结构的处理
    vtkSmartPointer<vtkPolyVertex> polyvertex = vtkSmartPointer<vtkPolyVertex>::New();
    polyvertex->GetPointIds()->SetNumberOfIds(n);
    int i=0;
    for(i=0;i<n;i++)//建立拓扑关系
    {
        polyvertex->GetPointIds()->SetId(i,i);
    }

    //
    vtkSmartPointer<vtkUnstructuredGrid> grid=vtkSmartPointer<vtkUnstructuredGrid>::New();
    grid->SetPoints(points);
    grid->InsertNextCell(polyvertex->GetCellType(),
                         polyvertex->GetPointIds());
    grid->GetPointData()->AddArray(colors);
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(grid);
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