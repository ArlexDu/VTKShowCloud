#include "vtkLASReader.h"

vtkStandardNewMacro(vtkLASReader)

vtkLASReader::vtkLASReader() {
    this->FileName = NULL;
    this->pointRecordsCount = 0;
    this->Header = NULL;
    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

vtkLASReader::~vtkLASReader() {
    if (!FileName)
        delete[] FileName;

    if (!Header)
        delete Header;
}

int vtkLASReader::RequestData(vtkInformation * vtkNotUsed(request),
                              vtkInformationVector ** vtkNotUsed(request),
                              vtkInformationVector *outputVector) {
    // Get the info object
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    // Get the ouptut
    vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

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
    printf("pointRecordCount is %d \n", pointRecordsCount);
    vtkUnstructuredGrid *grid = vtkUnstructuredGrid::New();
    ReadPointRecordData(reader, grid);
    printf("grid number is %d \n", grid->GetNumberOfPoints());
//  printf("grid number is %d \n", grid->GetNumberOfCells());
    output->ShallowCopy(grid);
    // Convert points to verts in output polydata
//  printf("return number is %d \n", output->GetNumberOfCells());
    return VTK_OK;
}

void vtkLASReader::ReadPointRecordData(liblas::Reader &reader, vtkUnstructuredGrid *grid) {
    unsigned int red[3] = {255,0,0};
    unsigned int green[3] = {0,255,0};
    unsigned int blue[3] = {0,0,255};
    vtkPoints *points = vtkPoints::New();
    vtkUnsignedCharArray *colors = vtkUnsignedCharArray::New();
    colors->SetName("Colors");
    colors->SetNumberOfComponents(3);
    vtkSmartPointer<vtkPolyVertex> vertex = vtkSmartPointer<vtkPolyVertex>::New();
    vertex->GetPointIds()->SetNumberOfIds(pointRecordsCount);
    for (int i = 0; i < pointRecordsCount && reader.ReadNextPoint(); i++) {
        liblas::Point const &p = reader.GetPoint();
        points->InsertNextPoint(p.GetX(), p.GetY(), p.GetZ());
        if(p.GetZ()<17){
            colors->InsertNextTuple3(blue[0],blue[1],blue[2]);
        }else {
            colors->InsertNextTuple3(green[0], green[1], green[2]);
        }
        vertex->GetPointIds()->SetId(i,i);
    }
    grid->SetPoints(points);
    grid->InsertNextCell(vertex->GetCellType(),vertex->GetPointIds());
    grid->GetPointData()->AddArray(colors);
}

void vtkLASReader::PrintSelf(ostream &os, vtkIndent indent) {
    Superclass::PrintSelf(os, indent);
    os << "vtkLASReader" << std::endl;
    os << "Filename: " << this->FileName << std::endl;
}
