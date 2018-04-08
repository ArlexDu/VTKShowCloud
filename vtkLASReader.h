#ifndef __vtkLASReader_h
#define __vtkLASReader_h

// VTK Includes
#include <vtk-8.1/vtkPolyDataAlgorithm.h>
#include <vtk-8.1/vtkPolyData.h>
#include <vtk-8.1/vtkCellArray.h>
#include <vtk-8.1/vtkPoints.h>
#include <vtk-8.1/vtkPointData.h>
#include <vtk-8.1/vtkVertexGlyphFilter.h>
#include <vtk-8.1/vtkInformation.h>
#include <vtk-8.1/vtkInformationVector.h>
#include <vtk-8.1/vtkObjectFactory.h>
//libLAS Includes
#include <liblas/liblas.hpp>

// C++ includes
#include <fstream>
#include <iostream>

class vtkLASReader : public vtkPolyDataAlgorithm {
public:
    static vtkLASReader *New();

vtkTypeMacro(vtkLASReader, vtkPolyDataAlgorithm)

    virtual void PrintSelf(ostream &os, vtkIndent indent);

    // Decription:
    // All the Visualisation Types have been listed here
    enum VisualisationTypeConstants {
        None = 0,
        Color,
        Classification
    };

    // Description:
    // All the Classification Types according to LAS spec are listed here
    enum ClassificationType {
        Created_NotClassified = 0,
        Unclassified,     // 1
        Ground,           // 2
        LowVegetation,    // 3
        MediumVegetation, // 4
        HighVegetation,   // 5
        Building,         // 6
        LowPoint,         // 7
        ModelKeyPoint,    // 8
        Water             // 9
    };

    // Decription:
    // Accessor for name of the file that will be opened
    vtkSetStringMacro(FileName)

    vtkGetStringMacro(FileName)

    // Decription:
    // Accessor for Visualisation Type
    vtkSetMacro(VisualisationType, VisualisationTypeConstants)

    vtkGetMacro(VisualisationType, VisualisationTypeConstants)

    // Decription:
    // Accessor for the LAS Header file
    vtkGetMacro(Header, liblas::Header *)

    // Description:
    // Set User specified color values in the Classification Color Map instead of the default values
    void SetClassificationColor(ClassificationType type, unsigned char color[3]);

    void SetClassificationColor(ClassificationType type, unsigned char red, unsigned char green, unsigned char blue);


protected:
    vtkLASReader();

    virtual ~vtkLASReader();

    // Decription:
    // Core implementation of the data set reader
    int RequestData(vtkInformation *request, vtkInformationVector **inputVector,
                    vtkInformationVector *outputVector);

    // Description:
    // Read point record data i.e. position and visualisation data
    void ReadPointRecordData(liblas::Reader &reader, vtkPolyData *pointsPolyData);

    // Description:
    // Map from Class Number to Corresponding Color
    static unsigned char ClassificationColorMap[10][3];

    int pointRecordsCount;
    VisualisationTypeConstants VisualisationType;
    liblas::Header *Header;
    char *FileName;

private:
    vtkLASReader(const vtkLASReader &);  // Not implemented
    void operator=(const vtkLASReader &);    // Not implemented
};

// To Do:
// Look for better Visualisation Methods if possible?
// Ex: Converting it to a Polygon mesh?
//

#endif // __vtkLASReader_h
