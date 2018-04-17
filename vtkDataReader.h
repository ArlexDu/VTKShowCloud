#ifndef __vtkLASReader_h
#define __vtkLASReader_h

// VTK Includes
#include <vtk-8.1/vtkPolyDataAlgorithm.h>
#include <vtk-8.1/vtkUnstructuredGridAlgorithm.h>
#include <vtk-8.1/vtkUnstructuredGrid.h>
#include <vtk-8.1/vtkPolyData.h>
#include <vtk-8.1/vtkCellArray.h>
#include <vtk-8.1/vtkPoints.h>
#include <vtk-8.1/vtkPointData.h>
#include <vtk-8.1/vtkVertexGlyphFilter.h>
#include <vtk-8.1/vtkInformation.h>
#include <vtk-8.1/vtkInformationVector.h>
#include <vtk-8.1/vtkObjectFactory.h>
#include <vtk-8.1/vtkPolyVertex.h>

//libLAS Includes
#include <liblas/liblas.hpp>

//PCL Includes
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

// C++ includes
#include <fstream>
#include <iostream>

class vtkDataReader : public vtkUnstructuredGridAlgorithm {

public:

vtkTypeMacro(vtkDataReader, vtkUnstructuredGridAlgorithm)

    static vtkDataReader *New();

    void PrintSelf(ostream &os, vtkIndent indent) override;

//  文件名称
    vtkSetStringMacro(FileName)

    vtkGetStringMacro(FileName)

//  LAS Header
    vtkGetMacro(Header, liblas::Header *)


protected:
    vtkDataReader();

    virtual ~vtkDataReader();

//  Filter主要执行函数
    int RequestData(vtkInformation *request, vtkInformationVector **inputVector,
                    vtkInformationVector *outputVector) override;

//  读取LAS文件函数
    void ReadPointRecordData(pcl::PointCloud<pcl::PointXYZ> &cloud, vtkUnstructuredGrid *unstructuredGrid);

    int pointRecordsCount;
    liblas::Header *Header;
    char *FileName;

private:
    vtkDataReader(const vtkDataReader &);  // Not implemented
    void operator=(const vtkDataReader &);    // Not implemented
    //  las文件转化为pcd格式
    void Las2Pcd(liblas::Reader &reader, pcl::PointCloud<pcl::PointXYZ> &cloud);

//  数据处理
    void statisticalOutlierRemove(pcl::PointCloud<pcl::PointXYZ> &cloud);
};

#endif // __vtkLASReader_h
