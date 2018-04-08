#include "vtkLASReader.h"

vtkStandardNewMacro(vtkLASReader)

//----------------------------------------------------------------------------
unsigned char vtkLASReader::ClassificationColorMap[][3] = {
        //{Red,  Green,   Blue}
        {0,   0,   0},    //0     Created, Never Classified   Black
        {255, 0,   0},    //1     Unclassified                Red
        {145, 100, 45},    //2     Ground                      Brown
        {0,   255, 0},    //3     Low Vegetation              Light Green
        {0,   160, 0},    //4     Medium Vegetation           Medium Green
        {0,   90,  255},    //5     High Vegetation             Dark Green
        {255, 255, 0},    //6     Building                    Yellow
        {255, 140, 0},    //7     Low Point                   Orange
        {255, 0,   255},    //8     Model Key-Point             Purple
        {0,   255, 255},    //9     Water                       Blue
};

//----------------------------------------------------------------------------
vtkLASReader::vtkLASReader() {
    this->FileName = NULL;
    this->pointRecordsCount = 0;
    this->VisualisationType = None;
    this->Header = NULL;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

//----------------------------------------------------------------------------
vtkLASReader::~vtkLASReader() {
    if (!FileName)
        delete[] FileName;

    if (!Header)
        delete Header;
}

//----------------------------------------------------------------------------
int vtkLASReader::RequestData(vtkInformation * vtkNotUsed(request),
                              vtkInformationVector ** vtkNotUsed(request),
                              vtkInformationVector *outputVector) {
    // Get the info object
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    // Get the ouptut
    vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    // Open LAS File for reading
    std::ifstream ifs;
    ifs.open(FileName, std::ios_base::binary | std::ios_base::in);

    if (!ifs.is_open()) {
        vtkErrorMacro (<< "Unable to open File for reading in binary mode");
        return VTK_ERROR;
    }

    // Read header data
    liblas::ReaderFactory readerFactory;
    liblas::Reader reader = readerFactory.CreateWithStream(ifs);
    Header = new liblas::Header(reader.GetHeader());
    pointRecordsCount = Header->GetPointRecordsCount();
    printf("pointRecordCount is %d",pointRecordsCount);
    vtkPolyData *pointsPolyData = vtkPolyData::New();
    ReadPointRecordData(reader, pointsPolyData);

    // Convert points to verts in output polydata
    vtkVertexGlyphFilter *vertexFilter = vtkVertexGlyphFilter::New();
    vertexFilter->SetInputData(pointsPolyData);
    vertexFilter->Update();
    printf("\n filter number is %d",vertexFilter->GetOutput()->GetNumberOfCells());
    output->ShallowCopy(vertexFilter->GetOutput());

    return VTK_OK;
}

//----------------------------------------------------------------------------
void vtkLASReader::SetClassificationColor(ClassificationType type,
                                          unsigned char red,
                                          unsigned char green,
                                          unsigned char blue) {
    if (type <= 0 || type > Water) {
        vtkErrorMacro (<< "Invalid type of Class Specified!");
        return;
    }

    this->ClassificationColorMap[type][0] = red;
    this->ClassificationColorMap[type][1] = green;
    this->ClassificationColorMap[type][2] = blue;
}

//----------------------------------------------------------------------------
void vtkLASReader::SetClassificationColor(ClassificationType type, unsigned char color[]) {
    this->SetClassificationColor(type, color[0], color[1], color[2]);
}

//----------------------------------------------------------------------------
void vtkLASReader::ReadPointRecordData(liblas::Reader &reader, vtkPolyData *pointsPolyData) {
    vtkPoints *points = vtkPoints::New();
    vtkUnsignedCharArray *colors = vtkUnsignedCharArray::New();
    colors->SetName("Colors");
    colors->SetNumberOfComponents(3);

    for (int i = 0; i < pointRecordsCount && reader.ReadNextPoint(); i++) {
        liblas::Point const &p = reader.GetPoint();
        points->InsertNextPoint(p.GetX(),//* Header->GetScaleX() * 2 + Header->GetOffsetX(),
                                p.GetY(),// * Header->GetScaleY() * 2 + Header->GetOffsetY(),
                                p.GetZ());// * Header->GetScaleZ() * 2 + Header->GetOffsetZ());

        unsigned char *color;
        switch (VisualisationType) {
            case None:
                colors->InsertNextTuple3(255, 0, 0);
            case Color: {
                char color[3];
                color[0] = p.GetColor().GetRed() / 256;
                color[1] = p.GetColor().GetGreen() / 256;
                color[2] = p.GetColor().GetBlue() / 256;
                colors->InsertNextTuple3(color[0], color[1], color[2]);
            }
//      delete
                break;
            case Classification:
                char color[3];
                color[0] = vtkLASReader::ClassificationColorMap[p.GetClassification().GetClass()][0];
                color[1] = vtkLASReader::ClassificationColorMap[p.GetClassification().GetClass()][1];
                color[2] = vtkLASReader::ClassificationColorMap[p.GetClassification().GetClass()][2];
                colors->InsertNextTuple3(color[0], color[1], color[2]);
                break;
            default:
                break;
        }
    }

    pointsPolyData->SetPoints(points);
    pointsPolyData->GetPointData()->AddArray(colors);
}

//----------------------------------------------------------------------------
void vtkLASReader::PrintSelf(ostream &os, vtkIndent indent) {
    Superclass::PrintSelf(os, indent);
    os << "vtkLASReader" << std::endl;
    os << "Filename: " << this->FileName << std::endl;
}
