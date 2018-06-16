#ifndef __vtkLASReader_h
#define __vtkLASReader_h

// C++ includes
#include <fstream>
#include <iostream>
#include <vector>

// VTK Includes
#include <vtkPolyDataAlgorithm.h>
#include <vtkUnstructuredGridAlgorithm.h>
#include <vtkDataObjectAlgorithm.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPolyVertex.h>



//libLAS Includes
#include <liblas/liblas.hpp>

//PCL Includes
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/radius_outlier_removal.h>

using namespace std;

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
    int
    RequestDataObject(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector);

//  数据处理
    int RequestData(vtkInformation *request, vtkInformationVector **inputVector,
                    vtkInformationVector *outputVector) override;

//  读取LAS文件函数
    int FillOutputPortInformation(int port, vtkInformation *info) override;

    int pointRecordsCount;
    liblas::Header *Header;
    char *FileName;

private:
    nvtkDataReader(const nvtkDataReader &);  // Not implemented
    void operator=(const nvtkDataReader &);    // Not implemented
    char *FileType;

//  读取规定格式的txt文件并转化为pcd格式
    int ReadInfoTxt(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);

//  读取x,y,z的txt文件并转化为pcd格式
    int ReadNormalTxt(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);

//  读取las文件并转化为pcd格式
    int ReadLas(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);

//  分割字符串
    void Split(char sentence[], char *delim, char *result[]);
};

#endif // __vtkLASReader_h
