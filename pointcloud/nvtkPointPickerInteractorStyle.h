//
// Created by 杜盛瑀 on 2018/4/23.
//
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointPicker.h>
#include <vtkObjectFactory.h>

class nvtkPointPickerInteractorStyle : public vtkInteractorStyleTrackballCamera {

public:

vtkTypeMacro(nvtkPointPickerInteractorStyle, vtkInteractorStyleTrackballCamera);

    static nvtkPointPickerInteractorStyle *New();
    void OnLeftButtonDown() override;

protected:
    nvtkPointPickerInteractorStyle();
    ~nvtkPointPickerInteractorStyle();
};
