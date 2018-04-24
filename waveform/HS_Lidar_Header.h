#ifndef HS_Lidar_Header_H
#define HS_Lidar_Header_H

#include <stdint.h>
class HS_Lidar_Header
{
public:
	uint16_t nFill;
	uint16_t nGPSWeek;
	double dGPSSecond;
	uint32_t nGPSBreakdownTime;
	double dAzimuth;
	double dPitch;
	double dRoll;
	double dX;
	double dY;
	double dZ;
	uint32_t nCodeDiscResolution;
	uint32_t nCodeNumber;
	uint32_t nWaveNumber;
	uint32_t nWaveLen;
    HS_Lidar_Header(){};
    ~HS_Lidar_Header(){};
};


#endif
