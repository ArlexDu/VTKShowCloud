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

// C++ includes
#include <fstream>
#include <iostream>

class vtkLASReader : public vtkUnstructuredGridAlgorithm {

public:

vtkTypeMacro(vtkLASReader, vtkUnstructuredGridAlgorithm)

    static vtkLASReader *New();

    void PrintSelf(ostream &os, vtkIndent indent) override;

//  文件名称
    vtkSetStringMacro(FileName)

    vtkGetStringMacro(FileName)

//  LAS Header
    vtkGetMacro(Header, liblas::Header *)


protected:
    vtkLASReader();

    virtual ~vtkLASReader();

//  Filter主要执行函数
    int RequestData(vtkInformation *request, vtkInformationVector **inputVector,
                    vtkInformationVector *outputVector) override;

//  读取LAS文件函数
    void ReadPointRecordData(liblas::Reader &reader, vtkUnstructuredGrid *unstructuredGrid);

    int pointRecordsCount;
    liblas::Header *Header;
    char *FileName;

private:
    vtkLASReader(const vtkLASReader &);  // Not implemented
    void operator=(const vtkLASReader &);    // Not implemented
};

#endif // __vtkLASReader_h
