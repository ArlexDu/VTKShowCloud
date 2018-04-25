//
// Created by 杜盛瑀 on 2018/4/23.
//
#include "nvtkPointPickerInteractorStyle.h"

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
    if (picker->GetPointId() != NULL) {
        int id = picker->GetPointId();
        vtkDataArray *line = picker->GetDataSet()->GetPointData()->GetArray("Indexs");
        int linecount = line->GetTuple1(id);
        char *data;
        data = this->GetInformation(linecount);
        char *result[9];
        this->Split(data, " ", result);
        for (int i = 0; i < 9; i++) {
            printf(" %s",result[i]);
        }
    }


    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

char *nvtkPointPickerInteractorStyle::GetInformation(int linecount) {
    std::ifstream ifs;
    ifs.open(FileName, std::ios_base::binary | std::ios_base::in);
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

void nvtkPointPickerInteractorStyle::Split(char *sentence, char *delim, char *result[]) {
    char *p;
    p = strtok(sentence, delim);
    result[0] = p;
    int i = 1;
    while (p != NULL) {
        p = strtok(NULL, delim);
        if (p == NULL) {
            break;
        }
        result[i] = p;
        i++;
    }
}