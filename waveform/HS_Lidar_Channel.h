#ifndef HS_Lidar_Channel_H
#define HS_Lidar_Channel_H

#include <stdint.h>

class HS_Lidar_Channel
{
public:
	uint32_t nHeader;
	uint16_t nChannelNo;
	uint16_t nS0;
	uint16_t nL0;
	uint16_t nD0[320];
	uint16_t nS1;
	uint16_t nL1;
	uint16_t nD1;
	uint32_t nTest;
    HS_Lidar_Channel(){};
    ~HS_Lidar_Channel(){};
};


#endif
