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
        int vtkNotUsed(port), vtkInformation *info) {
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "nvtkDataObject");
    return 1;
}


int nvtkDataReader::RequestDataObject(vtkInformation * vtkNotUsed(request),
                                      vtkInformationVector ** vtkNotUsed(inputVector),
                                      vtkInformationVector *outputVector) {
    printf("create output object");
    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    nvtkDataObject *output = nvtkDataObject::SafeDownCast(
            outInfo->Get(vtkDataObject::DATA_OBJECT()));


    if (!output) {
        output = nvtkDataObject::New();
        outInfo->Set(vtkDataObject::DATA_OBJECT(), output);
        output->FastDelete();
        outInfo->Set(vtkDataObject::DATA_OBJECT(), output);

        this->GetOutputPortInformation(0)->Set(
                vtkDataObject::DATA_EXTENT_TYPE(), output->GetExtentType());
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

    char temp[256];

    strcpy(temp, FileName);

    char *delim = ".";

    FileType = strtok(temp, delim);

    FileType = strtok(NULL, delim);

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

    if (strcmp(FileType, "las") == 0) {
        ReadLas(cloud);
    } else if (strcmp(FileType, "txt") == 0) {
        ReadNormalTxt(cloud);
    } else {
        return VTK_ERROR;
    }

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

//读取有额外信息的txt文件
int nvtkDataReader::ReadInfoTxt(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
    std::ifstream ifs;
    ifs.open(FileName, std::ios_base::binary | std::ios_base::in);
    if (!ifs.is_open()) {
        vtkErrorMacro (<< "Unable to open File for reading in binary mode");
        return VTK_ERROR;
    }
    int count = 0;
    char line[512];
    while (!ifs.eof()) {
        ifs.getline(line, 512, '\n');
        count++;
    }
    printf("read number is %d \n", count + 1);
    cloud->height = 1;
    cloud->is_dense = false;
    cloud->width = count + 1;
    cloud->points.resize(cloud->width * cloud->height);
    int i = 0;
//    返回文件顶部
    ifs.clear();
    ifs.seekg(0);
    while (!ifs.eof()) {
        ifs.getline(line, 512, '\n');
        char* result[10];
        this->Split(line, " ",result);
        cloud->points[i].x = atof(result[1]);
        cloud->points[i].y = atof(result[2]);
        cloud->points[i].z = atof(result[3]);
        cloud->points[i].data[3] = i;
        i++;
    }
    ifs.close();
    return VTK_OK;
}

//读取普通的txt文件：只有x,y,z
int nvtkDataReader::ReadNormalTxt(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
    std::ifstream ifs;
    ifs.open(FileName, std::ios_base::binary | std::ios_base::in);
    if (!ifs.is_open()) {
        vtkErrorMacro (<< "Unable to open File for reading in binary mode");
        return VTK_ERROR;
    }
    int count = 0;
    char line[512];
    while (!ifs.eof()) {
        ifs.getline(line, 512, '\n');
        count++;
    }
    printf("read number is %d \n", count + 1);
    cloud->height = 1;
    cloud->is_dense = false;
    cloud->width = count + 1;
    cloud->points.resize(cloud->width * cloud->height);
    int i = 0;
//    返回文件顶部
    ifs.clear();
    ifs.seekg(0);
    while (!ifs.eof()) {
        ifs.getline(line, 512, '\n');
        char* result[3];
        this->Split(line, ",",result);
        if(result[0]==NULL||result[1]==NULL||result[2]==NULL){
            cout<<"index is "<<i<<endl;
            continue;
        }
        cloud->points[i].x = atof(result[0]);
        cloud->points[i].y = atof(result[1]);
        cloud->points[i].z = atof(result[2]);
        i++;
    }
    ifs.close();
    return VTK_OK;
}

//读取LAS文件
int nvtkDataReader::ReadLas(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
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
    ifs.close();
    return VTK_OK;
}

void nvtkDataReader::Split(char sentence[], char *delim, char *result[]) {
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


