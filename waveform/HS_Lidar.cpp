#include "HS_Lidar.h"
#include "iostream"
#include "fstream"



#define Swap16(v)  ( ((v & 0xff) << 8) | (v >> 8) )

#define Swap32(v)  ( (v >> 24)\
	                 | ((v & 0x00ff0000) >> 8)\
		             | ((v & 0x0000ff00) << 8)\
		             | (v << 24) )



union DoubleAndChar
{
	double dval;
	unsigned char ucval[8];
};



double  SwapDouble(double * dv)
{
	DoubleAndChar dac;
	unsigned char ucTmp;
	dac.dval = *dv;
	for (int i = 0; i<4; i++) {
		ucTmp = dac.ucval[i];
		dac.ucval[i] = dac.ucval[7 - i];
		dac.ucval[7 - i] = ucTmp;
	}
	*dv = dac.dval;
	return dac.dval;
}


void DataInt16Swap16(uint16_t *Data, int nNum)
{
	for (int i = 0; i < nNum; i++)
	{
        Data[i] = Swap16(Data[i]);
	}
}




HS_Lidar::HS_Lidar()
{
}


HS_Lidar::~HS_Lidar()
{
}



void HS_Lidar::initData(FILE *fp) {
	getHeader(fp);
	getChannel(fp, CH1);
	getChannel(fp, CH2);
	getChannel(fp, CH3);
	getChannel(fp, CH4);


}


void HS_Lidar::getHeader(FILE *fp) {
	fread(&header.nFill, sizeof(uint16_t), 1, fp);
	fread(&header.nGPSWeek, sizeof(uint16_t), 1, fp);
	fread(&header.dGPSSecond, sizeof(double), 1, fp);
	fread(&header.nGPSBreakdownTime, sizeof(uint32_t), 1, fp);
	fread(&header.dAzimuth, sizeof(double), 1, fp);
	fread(&header.dPitch, sizeof(double), 1, fp);
	fread(&header.dRoll, sizeof(double), 1, fp);
	fread(&header.dX, sizeof(double), 1, fp);
	fread(&header.dY, sizeof(double), 1, fp);
	fread(&header.dZ, sizeof(double), 1, fp);
	fread(&header.nCodeDiscResolution, sizeof(uint32_t), 1, fp);
	fread(&header.nCodeNumber, sizeof(uint32_t), 1, fp);
	fread(&header.nWaveNumber, sizeof(uint32_t), 1, fp);
	fread(&header.nWaveLen, sizeof(uint32_t), 1, fp);





	header.nFill = Swap16(header.nFill);
	header.nGPSWeek = Swap16(header.nGPSWeek);
	header.nGPSBreakdownTime = Swap32(header.nGPSBreakdownTime);
	header.nCodeDiscResolution = Swap32(header.nCodeDiscResolution);
	header.nCodeNumber = Swap32(header.nCodeNumber);
	header.nWaveNumber = Swap32(header.nWaveNumber);
	header.nWaveLen = Swap32(header.nWaveLen);
	header.dGPSSecond = SwapDouble(&(header.dGPSSecond));
	header.dAzimuth = SwapDouble(&(header.dAzimuth));
	header.dPitch = SwapDouble(&(header.dPitch));
	header.dRoll = SwapDouble(&(header.dRoll));
	header.dX = SwapDouble(&(header.dX));
	header.dY = SwapDouble(&(header.dY));
	header.dZ = SwapDouble(&(header.dZ));
}



void HS_Lidar::getChannel(FILE *fp, HS_Lidar_Channel &CH) {

	fread(&CH.nHeader, sizeof(uint32_t), 1, fp);
	CH.nHeader = Swap32(CH.nHeader);

	fread(&CH.nChannelNo, sizeof(uint16_t), 1, fp);
	CH.nChannelNo = Swap16(CH.nChannelNo);

	fread(&CH.nS0, sizeof(uint16_t), 1, fp);
	CH.nS0 = Swap16(CH.nS0);

	fread(&CH.nL0, sizeof(uint16_t), 1, fp);
	CH.nL0 = Swap16(CH.nL0);


	fread(&CH.nD0, CH.nL0 * sizeof(uint16_t), 1, fp);
	DataInt16Swap16(CH.nD0, CH.nL0);




	fread(&CH.nTest, sizeof(uint32_t), 1, fp);
	CH.nTest = Swap32(CH.nTest);

	if (CH.nTest == 3952125274)
	{
		fseek(fp, -4, SEEK_CUR);
		CH.nS1 = 0;
		CH.nL1 = 0;
		CH.nD1 = 0;
	}
	else
	{
		fseek(fp, -4, SEEK_CUR);

		fread(&CH.nS1, sizeof(uint16_t), 1, fp);
		CH.nS1 = Swap16(CH.nS1);

		fread(&CH.nL1, sizeof(uint16_t), 1, fp);
		CH.nL1 = Swap16(CH.nS1);

		uint16_t *Ch_Data1 = new uint16_t[CH.nL1];
		fread(Ch_Data1, CH.nL1 * sizeof(uint16_t), 1, fp);
		DataInt16Swap16(Ch_Data1, CH.nL1);


		if (Ch_Data1 != NULL)
		{
			delete[] Ch_Data1;
			Ch_Data1 = NULL;
		}
	}



}
