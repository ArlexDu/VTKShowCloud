#include "mineDataObject.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(mineDataObject);

mineDataObject::mineDataObject()
{
	this->cloud = NULL;
}

mineDataObject::~mineDataObject()
{
}

void mineDataObject::PrintSelf( ostream& os, vtkIndent indent )
{
	this->Superclass::PrintSelf( os, indent );
}

void mineDataObject::ShallowCopy(mineDataObject* t)
{
	this->cloud = t->Getcloud();
}

pcl::PointCloud<pcl::PointXYZ>::Ptr mineDataObject::Getcloud()
{
	return this->cloud;
}

void mineDataObject::Setcloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cp)
{
	this->cloud = cp;
}

