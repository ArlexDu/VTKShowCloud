//
// Created by 杜盛瑀 on 2018/4/23.
//

#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkCommand.h>
#include <vtkDataObject.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPolyVertex.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>

#include "nvtkDataSimplify.h"
#include "nvtkDataObject.h"

vtkStandardNewMacro(nvtkDataSimplify);

nvtkDataSimplify::nvtkDataSimplify()
{
    this->SetNumberOfInputPorts( 1 );
    this->SetNumberOfOutputPorts( 1 );
}

nvtkDataSimplify::~nvtkDataSimplify()
{
}

void nvtkDataSimplify::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}


int nvtkDataSimplify::FillInputPortInformation(
        int vtkNotUsed(port), vtkInformation* info)
{
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "nvtkDataObject");
    return 1;
}

//----------------------------------------------------------------------------
int nvtkDataSimplify::RequestData(
        vtkInformation* vtkNotUsed(request),
        vtkInformationVector **inputVector,
        vtkInformationVector* outputVector )
{
    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    vtkUnstructuredGrid* output = vtkUnstructuredGrid::SafeDownCast(
            outInfo->Get( vtkDataObject::DATA_OBJECT() ) );

    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    nvtkDataObject *input = nvtkDataObject::SafeDownCast(
            inInfo->Get(vtkDataObject::DATA_OBJECT()));
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = input->Getcloud();
    //  精简点云与去噪
    pcl::PointCloud<pcl::PointXYZ>::Ptr filter_cloud = this->RadiusOutlierRemove(cloud);
    vtkUnstructuredGrid *grid = vtkUnstructuredGrid::New();
//    转化为grid结构
    this->ConvertToGrid(filter_cloud, grid);
    printf("grid number is %d \n", grid->GetNumberOfPoints());
    printf("grid cell is %d \n", grid->GetNumberOfCells());
    output->ShallowCopy(grid);
    return 1;
}

pcl::PointCloud<pcl::PointXYZ>::Ptr nvtkDataSimplify::RadiusOutlierRemove(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
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

void nvtkDataSimplify::ConvertToGrid(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, vtkUnstructuredGrid *grid) {
    unsigned int red[3] = {255, 0, 0};
    unsigned int green[3] = {0, 255, 0};
    unsigned int blue[3] = {0, 0, 255};
    vtkPoints *points = vtkPoints::New();
    vtkUnsignedCharArray *colors = vtkUnsignedCharArray::New();
    colors->SetName("Colors");
    colors->SetNumberOfComponents(3);
    vtkDoubleArray *indexs = vtkDoubleArray::New();
    indexs->SetName("Indexs");
    indexs->SetNumberOfComponents(1);
    vtkSmartPointer<vtkPolyVertex> vertex = vtkSmartPointer<vtkPolyVertex>::New();
    vertex->GetPointIds()->SetNumberOfIds(cloud->width);
    for (int i = 0; i < cloud->width; i++) {
        points->InsertNextPoint(cloud->points[i].x, cloud->points[i].y, cloud->points[i].z);
        if (cloud->points[i].z < 17) {
            colors->InsertNextTuple3(blue[0], blue[1], blue[2]);
        } else {
            colors->InsertNextTuple3(green[0], green[1], green[2]);
        }
//        std::cout<<i<<" : "<<cloud->points[i].data[3]<<std::endl;
        indexs->InsertNextTuple1(cloud->points[i].data[3]);
        vertex->GetPointIds()->SetId(i, i);
    }
    grid->SetPoints(points);
    grid->InsertNextCell(vertex->GetCellType(), vertex->GetPointIds());
    grid->GetPointData()->AddArray(indexs);
    grid->GetPointData()->AddArray(colors);
}