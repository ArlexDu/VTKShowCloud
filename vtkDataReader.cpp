#include "vtkDataReader.h"

vtkStandardNewMacro(vtkDataReader);

vtkDataReader::vtkDataReader() {
    this->FileName = NULL;
    this->pointRecordsCount = 0;
    this->Header = NULL;
    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

vtkDataReader::~vtkDataReader() {
    if (!FileName)
        delete[] FileName;

    if (!Header)
        delete Header;
}

int vtkDataReader::RequestData(vtkInformation * vtkNotUsed(request),
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
    const pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
//    las转化为pcd格式
    Las2Pcd(reader, cloud);
//  精简点云与去噪
    pcl::PointCloud<pcl::PointXYZ>::Ptr filter_cloud = RadiusOutlierRemove(cloud);
    vtkUnstructuredGrid *grid = vtkUnstructuredGrid::New();
    ReadPointRecordData(filter_cloud, grid);
    printf("grid number is %d \n", grid->GetNumberOfPoints());
    printf("grid cell is %d \n", grid->GetNumberOfCells());
    output->ShallowCopy(grid);
    // Convert points to verts in output polydata
//  printf("return number is %d \n", output->GetNumberOfCells());
    return VTK_OK;
}

void vtkDataReader::ReadPointRecordData(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, vtkUnstructuredGrid *grid) {
    unsigned int red[3] = {255, 0, 0};
    unsigned int green[3] = {0, 255, 0};
    unsigned int blue[3] = {0, 0, 255};
    vtkPoints *points = vtkPoints::New();
    vtkUnsignedCharArray *colors = vtkUnsignedCharArray::New();
    colors->SetName("Colors");
    colors->SetNumberOfComponents(3);
    vtkSmartPointer<vtkPolyVertex> vertex = vtkSmartPointer<vtkPolyVertex>::New();
    vertex->GetPointIds()->SetNumberOfIds(cloud->width);
    for (int i = 0; i < cloud->width; i++) {
        points->InsertNextPoint(cloud->points[i].x, cloud->points[i].y, cloud->points[i].z);
        if (cloud->points[i].z < 17) {
            colors->InsertNextTuple3(blue[0], blue[1], blue[2]);
        } else {
            colors->InsertNextTuple3(green[0], green[1], green[2]);
        }
        vertex->GetPointIds()->SetId(i, i);
    }
    grid->SetPoints(points);
    grid->InsertNextCell(vertex->GetCellType(), vertex->GetPointIds());
    grid->GetPointData()->AddArray(colors);
}

void vtkDataReader::PrintSelf(ostream &os, vtkIndent indent) {
    Superclass::PrintSelf(os, indent);
    os << "vtkDataReader" << std::endl;
    os << "Filename: " << this->FileName << std::endl;
}

void vtkDataReader::Las2Pcd(liblas::Reader &reader, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
    Header = new liblas::Header(reader.GetHeader());
    pointRecordsCount = Header->GetPointRecordsCount();
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

pcl::PointCloud<pcl::PointXYZ>::Ptr vtkDataReader::RadiusOutlierRemove(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
    pcl::RadiusOutlierRemoval<pcl::PointXYZ> sor;
    pcl::PointCloud<pcl::PointXYZ>::Ptr filter_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    printf("cloud width is %d \n", cloud->width);
    sor.setInputCloud(cloud);
    sor.setRadiusSearch(3);
    sor.setMinNeighborsInRadius (3);
    sor.filter (*filter_cloud);
    printf("filter cloud width is %d \n", filter_cloud->width);
    return filter_cloud;
}