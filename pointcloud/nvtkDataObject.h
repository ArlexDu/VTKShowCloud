#include "vtkDataObject.h"
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/radius_outlier_removal.h>

class nvtkDataObject : public vtkDataObject
{

public:
	static nvtkDataObject* New();
	vtkTypeMacro(nvtkDataObject,vtkDataObject);

	void PrintSelf( ostream& os, vtkIndent indent );
	void ShallowCopy(nvtkDataObject* t);

	void Setcloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cp);
	pcl::PointCloud<pcl::PointXYZ>::Ptr Getcloud();
	nvtkDataObject();
	~nvtkDataObject();

private:
	nvtkDataObject( const nvtkDataObject& ); // Not implemented.
	void operator = ( const nvtkDataObject& ); // Not implemented.
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
};

