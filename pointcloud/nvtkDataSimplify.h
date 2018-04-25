//
// Created by 杜盛瑀 on 2018/4/23.
//

#ifndef VTKSHOWCLOUD_SIMPLIFICATION_H
#define VTKSHOWCLOUD_SIMPLIFICATION_H
#include <vtkUnstructuredGridAlgorithm.h>

//PCL Includes
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <iostream>

class vtkDataSet;
class nvtkDataObject;

class nvtkDataSimplify : public vtkUnstructuredGridAlgorithm
{
public:
    static nvtkDataSimplify *New();
vtkTypeMacro(nvtkDataSimplify,vtkUnstructuredGridAlgorithm);

    void PrintSelf(ostream& os, vtkIndent indent);

protected:
    nvtkDataSimplify();
    ~nvtkDataSimplify();

    virtual int RequestData(
            vtkInformation* request,
            vtkInformationVector** inputVector,
            vtkInformationVector* outputVector );

    virtual int FillInputPortInformation( int port, vtkInformation* info );
//  点云精简
    pcl::PointCloud<pcl::PointXYZ>::Ptr RadiusOutlierRemove(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);
//  转化为UnstructuredGrid结构
    void ConvertToGrid(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, vtkUnstructuredGrid *unstructuredGrid);

private:
    nvtkDataSimplify( const nvtkDataSimplify& ); // Not implemented.
    void operator = ( const nvtkDataSimplify& );  // Not implemented.
};


#endif //VTKSHOWCLOUD_SIMPLIFICATION_H
