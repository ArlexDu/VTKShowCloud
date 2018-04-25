//
// Created by 杜盛瑀 on 2018/4/23.
//
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointPicker.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPropPicker.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <QTableWidget.h>
#include <QString.h>

#include <fstream>
#include <iostream>
#include <string>

class nvtkPointPickerInteractorStyle : public vtkInteractorStyleTrackballCamera {

public:

vtkTypeMacro(nvtkPointPickerInteractorStyle, vtkInteractorStyleTrackballCamera);

    static nvtkPointPickerInteractorStyle *New();

    void OnLeftButtonDown() override;

    QTableWidget *table;

    //  文件名称
    void SetFileName(std::string filename);

protected:
    nvtkPointPickerInteractorStyle();

    ~nvtkPointPickerInteractorStyle();

//    获取信息
    char *GetInformation(int linecount);

    std::string FileName;
};
