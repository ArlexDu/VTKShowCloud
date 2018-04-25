//
// Created by 杜盛瑀 on 2018/4/23.
//
#include "nvtkPointPickerInteractorStyle.h"

vtkStandardNewMacro(nvtkPointPickerInteractorStyle);

nvtkPointPickerInteractorStyle::nvtkPointPickerInteractorStyle() {}

nvtkPointPickerInteractorStyle::~nvtkPointPickerInteractorStyle() {}

void nvtkPointPickerInteractorStyle::SetFileName(std::string filename){
    this->FileName = filename;
}

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
    if (picker->GetPointId() != NULL) {
        int id = picker->GetPointId();
        vtkDataArray *line = picker->GetDataSet()->GetPointData()->GetArray("Indexs");
        int linecount = line->GetTuple1(id);
//        获取指定行的信息
        char *data;
        data = this->GetInformation(linecount);
//        分割字符串
        std::string result[9];
        char* p;
        p = strtok(data, " ");
        std::string s(p);
        result[0] = s;
        int i = 1;
        while (p != NULL) {
            p = strtok(NULL, " ");
            if (p == NULL) {
                break;
            }
            std::string s(p);
            result[i] = s;
            i++;
        }
        for (int i = 0; i < 9; i++) {
            std::string item = result[i];
//            std::cout<<item<<" ";
            table->setItem(i, 1,  new QTableWidgetItem(QString(item.c_str())));
        }

    }

    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

char *nvtkPointPickerInteractorStyle::GetInformation(int linecount) {
    std::ifstream ifs;
    ifs.open(FileName.c_str(), std::ios_base::binary | std::ios_base::in);
    if (!ifs.is_open()) {
        vtkErrorMacro (<< "Unable to open File for reading in binary mode");
    }
    int count = 0;
    char line[512];
    while (!ifs.eof()) {
        ifs.getline(line, 512, '\n');
        if (linecount == count) {
            break;
        }
        count++;
    }
    ifs.close();
    return line;
}
