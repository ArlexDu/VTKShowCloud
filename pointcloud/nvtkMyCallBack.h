//
// Created by 杜盛瑀 on 2018/6/16.
//

#include "vtkActor.h"
#include "vtkCommand.h"
#include "vtkBoxWidget.h"
#include "vtkTransform.h"

class nvtkMyCallBack : public vtkCommand {
public:
    static nvtkMyCallBack *New()
    { return new nvtkMyCallBack; }
    void Execute(vtkObject *caller, unsigned long, void*) override
    {
        vtkTransform *t = vtkTransform::New();
        vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
        widget->GetTransform(t);
        widget->GetProp3D()->SetUserTransform(t);
        t->Delete();
    }
};
