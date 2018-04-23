//
// Created by 杜盛瑀 on 2018/4/23.
//
#include "PointPickerInteractorStyle.h"

vtkStandardNewMacro(PointPickerInteractorStyle);

void PointPickerInteractorStyle::OnLeftButtonDown() {
    //打印鼠标左键像素位置
    std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0]
              << " " << this->Interactor->GetEventPosition()[1] << std::endl;
}
