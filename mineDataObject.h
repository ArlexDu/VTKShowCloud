#include "vtkDataObject.h"
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/radius_outlier_removal.h>

class mineDataObject : public vtkDataObject
{

public:
	static mineDataObject* New();
	vtkTypeMacro(mineDataObject,vtkDataObject);

	void PrintSelf( ostream& os, vtkIndent indent );
	void ShallowCopy(mineDataObject* t);

	void Setcloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cp);
	pcl::PointCloud<pcl::PointXYZ>::Ptr Getcloud();
	mineDataObject();
	~mineDataObject();

private:
	mineDataObject( const mineDataObject& ); // Not implemented.
	void operator = ( const mineDataObject& ); // Not implemented.
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
};

