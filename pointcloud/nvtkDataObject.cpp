#include "nvtkDataObject.h"

vtkStandardNewMacro(nvtkDataObject);

nvtkDataObject::nvtkDataObject()
{
	this->cloud = NULL;
}

nvtkDataObject::~nvtkDataObject()
{
}

void nvtkDataObject::PrintSelf( ostream& os, vtkIndent indent )
{
	this->Superclass::PrintSelf( os, indent );
}

void nvtkDataObject::ShallowCopy(nvtkDataObject* t)
{
	this->cloud = t->Getcloud();
}

pcl::PointCloud<pcl::PointXYZ>::Ptr nvtkDataObject::Getcloud()
{
	return this->cloud;
}

void nvtkDataObject::Setcloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cp)
{
	this->cloud = cp;
}

