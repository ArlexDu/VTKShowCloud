//
// Created by 杜盛瑀 on 2018/4/23.
//
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointPicker.h>
#include <vtkObjectFactory.h>

class PointPickerInteractorStyle : public vtkInteractorStyleTrackballCamera {

public:

vtkTypeMacro(PointPickerInteractorStyle, vtkInteractorStyleTrackballCamera);

    static PointPickerInteractorStyle *New();
    void OnLeftButtonDown() override;

protected:
    PointPickerInteractorStyle();
    ~PointPickerInteractorStyle();
};
