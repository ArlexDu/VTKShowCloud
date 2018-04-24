#include "nvtkDataReader.h"
#include "nvtkDataObject.h"

vtkStandardNewMacro(nvtkDataReader);

nvtkDataReader::nvtkDataReader() {
    this->FileName = NULL;
    this->pointRecordsCount = 0;
    this->Header = NULL;
    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

nvtkDataReader::~nvtkDataReader() {
    if (!FileName)
        delete[] FileName;

    if (!Header)
        delete Header;
}

int nvtkDataReader::FillOutputPortInformation(
        int vtkNotUsed(port), vtkInformation* info)
{
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "nvtkDataObject");
    return 1;
}


int nvtkDataReader::RequestDataObject(vtkInformation* vtkNotUsed(request),
                                          vtkInformationVector** vtkNotUsed(inputVector),
                                          vtkInformationVector* outputVector )
{
    printf("create output object");
    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    nvtkDataObject* output = nvtkDataObject::SafeDownCast(
            outInfo->Get( vtkDataObject::DATA_OBJECT() ) );


    if ( ! output )
    {
        output = nvtkDataObject::New();
        outInfo->Set( vtkDataObject::DATA_OBJECT(), output );
        output->FastDelete();
        outInfo->Set(vtkDataObject::DATA_OBJECT(),output);

        this->GetOutputPortInformation(0)->Set(
                vtkDataObject::DATA_EXTENT_TYPE(), output->GetExtentType() );
    }

    return 1;
}

int nvtkDataReader::RequestData(vtkInformation * vtkNotUsed(request),
                               vtkInformationVector ** vtkNotUsed(request),
                               vtkInformationVector *outputVector) {
    printf("fill data to ouput object");
    // Get the info object
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    // Get the ouptut
    nvtkDataObject *output = nvtkDataObject::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

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
    const pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
//    las转化为pcd格式
    Las2Pcd(reader, cloud);

    vtkSmartPointer<nvtkDataObject> data = vtkSmartPointer<nvtkDataObject>::New();
    data->Setcloud(cloud);

    output->ShallowCopy(data);
    // Convert points to verts in output polydata
//  printf("return number is %d \n", output->GetNumberOfCells());
    return VTK_OK;
}

void nvtkDataReader::PrintSelf(ostream &os, vtkIndent indent) {
    Superclass::PrintSelf(os, indent);
    os << "nvtkDataReader" << std::endl;
    os << "Filename: " << this->FileName << std::endl;
}

void nvtkDataReader::Las2Pcd(liblas::Reader &reader, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
    Header = new liblas::Header(reader.GetHeader());
    pointRecordsCount = Header->GetPointRecordsCount();
    printf("read number is %d \n", pointRecordsCount);
    cloud->width = pointRecordsCount;
    cloud->height = 1;
    cloud->is_dense = false;
    cloud->points.resize(cloud->width * cloud->height);
    int i = 0;
    while (reader.ReadNextPoint()) {
        liblas::Point const &p = reader.GetPoint();
        cloud->points[i].x = p.GetX();
        cloud->points[i].y = p.GetY();
        cloud->points[i].z = p.GetZ();
        i++;
    }
}
