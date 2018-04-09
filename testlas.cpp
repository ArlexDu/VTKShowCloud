//VTK includes
#include <vtk-8.1/vtkSmartPointer.h>
#include <vtk-8.1/vtkRenderer.h>
#include <vtk-8.1/vtkRenderWindowInteractor.h>
#include <vtk-8.1/vtkRenderWindow.h>
#include <vtk-8.1/vtkPolyDataMapper.h>
#include "vtk-8.1/vtkInteractorStyleTrackballCamera.h"
#include "vtk-8.1/vtkDataSetMapper.h"

//Reader includes
#include "vtkLASReader.h"

int TestGeoJSON(int argc, char **argv);

int TestLASLidar(int argc, char **argv);

int main(int argc, char **argv) {
    return TestLASLidar(argc, argv);
//   return TestGeoJSON(argc, argv);
}

int TestLASLidar(int argc, char **argv) {
    //Reader
    vtkLASReader *reader = vtkLASReader::New();

    //Select source file
    reader->SetFileName("/Users/arlex/Downloads/111.las");
    reader->SetVisualisationType(vtkLASReader::None);

    //Read the output
    reader->Update();

    vtkPolyData *outputData = reader->GetOutput();
    printf("\n filter number is %d",outputData->GetNumberOfCells());
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(outputData);
    //设置点的颜色
    mapper->SetScalarModeToUsePointFieldData();
    mapper->SelectColorArray("Colors");

    vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
//    actor->GetProperty()->SetColor(0.194,0.562, 0.75);

    ren->AddActor(actor);
    ren->SetBackground(0, 0, 0);

    vtkSmartPointer<vtkRenderWindow> win = vtkSmartPointer<vtkRenderWindow>::New();
    win->AddRenderer(ren);
    win->SetSize(800, 800);
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

    return EXIT_SUCCESS;
}