#ifndef __vtkLASReader_h
#define __vtkLASReader_h

// VTK Includes
#include <vtk-8.1/vtkPolyDataAlgorithm.h>
#include <vtk-8.1/vtkUnstructuredGridAlgorithm.h>
#include <vtk-8.1/vtkDataObjectAlgorithm.h>
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
#include <pcl/filters/radius_outlier_removal.h>

// C++ includes
#include <fstream>
#include <iostream>

class nvtkDataReader : public vtkDataObjectAlgorithm {

public:

vtkTypeMacro(nvtkDataReader, vtkDataObjectAlgorithm)

    static nvtkDataReader *New();

    void PrintSelf(ostream &os, vtkIndent indent) override;

//  文件名称
    vtkSetStringMacro(FileName)

    vtkGetStringMacro(FileName)

//  LAS Header
    vtkGetMacro(Header, liblas::Header *)


protected:
    nvtkDataReader();

    virtual ~nvtkDataReader();

//  数据对象初始化
    int RequestDataObject( vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector);
//  数据处理
    int RequestData(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector) override;

//  读取LAS文件函数
    int FillOutputPortInformation(int port, vtkInformation *info) override;

    int pointRecordsCount;
    liblas::Header *Header;
    char *FileName;

private:
    nvtkDataReader(const nvtkDataReader &);  // Not implemented
    void operator=(const nvtkDataReader &);    // Not implemented
//  las文件转化为pcd格式
    void Las2Pcd(liblas::Reader &reader, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);
};

#endif // __vtkLASReader_h
