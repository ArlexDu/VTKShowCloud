//
// Created by 杜盛瑀 on 2018/4/23.
//
#include <vtkPropPicker.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include "nvtkPointPickerInteractorStyle.h"
#include <vtkRenderWindow.h>
#include <vtkProperty.h>

vtkStandardNewMacro(nvtkPointPickerInteractorStyle);

nvtkPointPickerInteractorStyle::nvtkPointPickerInteractorStyle() {}

nvtkPointPickerInteractorStyle::~nvtkPointPickerInteractorStyle() {}

void nvtkPointPickerInteractorStyle::OnLeftButtonDown() {
    // Pick from this location.
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    vtkSmartPointer<vtkPointPicker> picker =
            vtkSmartPointer<vtkPointPicker>::New();
    picker->Pick(x, y, 0, this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
    double *pos = picker->GetPickPosition();
    std::cout << "Pick position (world coordinates) is: "
              << pos[0] << " " << pos[1]
              << " " << pos[2] << std::endl;

    std::cout << "Picked actor: " << picker->GetPointId() << std::endl;

    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}