#ifndef HS_Lidar_H
#define HS_Lidar_H

#include "HS_Lidar_Channel.h"
#include "HS_Lidar_Header.h"
#include "iostream"
#include "fstream"

class HS_Lidar
{
public:

	HS_Lidar_Header header;
	HS_Lidar_Channel CH1;
	HS_Lidar_Channel CH2;
	HS_Lidar_Channel CH3;
	HS_Lidar_Channel CH4;

	HS_Lidar();
	~HS_Lidar();

	void initData(FILE *fp);
    void getHeader(FILE *fp);
    void getChannel(FILE *fp,HS_Lidar_Channel &CH);

};

#endif
