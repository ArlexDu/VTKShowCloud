#include "drawchartthread.h"
#include "chart.h"
#include "HS_Lidar.h"
#include "drawdata.h"
#include <QDebug>
#include <string>
#include <geos/platform.h>


using namespace std;
bool isHeaderRight(uint8_t header[8]) {

    uint8_t headerSign[] = { 1, 35, 69, 103, 137, 171, 205, 239 };
    bool returnVal = true;

    for (size_t i = 0; i < 8; i++)
    {
        header[i]!= headerSign[i]?returnVal = false:returnVal= true;
    }

    return returnVal;
}


void  moveToOffset(int offset,FILE *fp) {
    fseek(fp, 0, SEEK_SET);
    for (int nHead_coll = 0; nHead_coll < offset; nHead_coll++) {
        uint8_t header[8];
        memset(header, 0, sizeof(uint8_t) * 8);
        fread(header, sizeof(uint8_t), 8, fp);

        if (isHeaderRight(header)) {
//            _fseeki64(fp, 2681, SEEK_CUR);
            fseek(fp, 2680, SEEK_CUR);
        }
        else {
            nHead_coll = nHead_coll - 1;
//            _fseeki64(fp, -7, SEEK_CUR);
            fseek(fp, -7, SEEK_CUR);
        }
    }
}



DrawChartThread::DrawChartThread()
{
    i = 0;
}



void DrawChartThread::run(){
    string filename = chart->filename.toStdString();

if(chart->chartType==1){

    qDebug()<< chart->filename;

    FILE *fp;
    fp = fopen(filename.c_str(), "rb");

    while(!isInterruptionRequested()){

        int x1 = 0;
        int x2 = 0;
        int x3 = 0;
        int x4 = 0;
        DrawData *drawData = new DrawData;



        //读取4个波段数据

        //每帧数据长6500
        int nLen = 6500;
        short *sData_CH0 = new short [nLen];
        short *sData_CH1 = new short [nLen];
        short *sData_CH2 = new short [nLen];
        short *sData_CH3 = new short [nLen];
        memset(sData_CH0, 0, sizeof(short)*nLen);
        memset(sData_CH1, 0, sizeof(short)*nLen);
        memset(sData_CH2, 0, sizeof(short)*nLen);
        memset(sData_CH3, 0, sizeof(short)*nLen);

        //LADM2-激光数据头大小，固定值
        static const int nLen_Header = 160;



        //每个Binshot长度是固定的
        double dLen_Binshot = 2*4*nLen;

        int nTragetShot = i;

        //指针跳的长度 nTragetShot是想要读取的帧号
        int64 nSeekL = nLen_Header + dLen_Binshot*nTragetShot;

        //常规fseek第二个参数为long 最大值为2147483647, 激光数据的字节更大，这里用_fseeki64
//        _fseeki64(fp, nSeekL, SEEK_SET);
        fseek(fp, nSeekL, SEEK_SET);

        fread(sData_CH0, sizeof(short), nLen, fp);
        fread(sData_CH1, sizeof(short), nLen, fp);
        fread(sData_CH2, sizeof(short), nLen, fp);
        fread(sData_CH3, sizeof(short), nLen, fp);



        drawData->maxY = 0;
        drawData->minY = 0;


        drawData->size1 = nLen;
        drawData->size2 = nLen;
        drawData->size3 = nLen;
        drawData->size4 = nLen;

        for (int j=0;j < nLen;j++) {
            if(sData_CH0[i]<(-2000)){
                sData_CH0[j]=0;
            }
            if(sData_CH0[j]<(-2000)){
                sData_CH0[j]=0;
            }
            if(sData_CH0[j]<drawData->minY){
                drawData->minY= sData_CH0[j];
            }
            if(sData_CH1[j]>drawData->maxY){
                drawData->maxY= sData_CH0[j];
            }
             drawData->line2[j]= sData_CH0[j];
             x1++;
        }
        for (int j=0;j < nLen;j++) {
            if(sData_CH1[j]<(-2000)){
                sData_CH1[j]=0;
            }
            if(sData_CH1[j]<(-2000)){
                sData_CH1[j]=0;
            }
            if(sData_CH1[j]<drawData->minY){
                drawData->minY= sData_CH1[j];
            }
            if(sData_CH1[j]>drawData->maxY){
                drawData->maxY= sData_CH1[j];
            }
             drawData->line2[j]= sData_CH1[j];
             x2++;
        }

        for (int j=0;j < nLen;j++) {
            if(sData_CH2[j]<(-2000)){
                sData_CH2[j]=0;
            }
            if(sData_CH2[j]<(-2000)){
                sData_CH2[j]=0;
            }
            if(sData_CH2[j]<drawData->minY){
               drawData-> minY= sData_CH1[j];
            }
            if(sData_CH2[j]>drawData->maxY){
                drawData->maxY= sData_CH2[j];
            }
             drawData->line3[j]= sData_CH2[j];
             x3++;
        }

        for (int j=0;j < nLen;j++) {
            if(sData_CH3[j]<(-2000)){
                sData_CH3[j]=0;
            }
            if(sData_CH3[j]<(-2000)){
                sData_CH3[j]=0;
            }
            if(sData_CH3[j]<drawData->minY){
                drawData->minY= sData_CH1[j];
            }
            if(sData_CH3[j]>drawData->maxY){
                drawData->maxY= sData_CH3[j];
            }
             drawData->line4[j]= sData_CH3[j];
             x4++;
        }

        drawData->x1 = x1;
        drawData->x2 = x2;
        drawData->x3 = x3;
        drawData->x4 = x4;
        i++;
        drawData->i = i;
      emit draw(*drawData);
        msleep(500);
    }
    fclose(fp);

}else{


    FILE *fp;
    fp = fopen(filename.c_str(), "rb");
    fseek(fp, 0, SEEK_END);
    int filesize = ftell(fp) / 2688 - 2;
    fseek(fp, 0, SEEK_SET);
    while(!isInterruptionRequested()){

        int x1 = 0;
        int x2 = 0;
        int x3 = 0;
        int x4 = 0;
        DrawData *drawData = new DrawData;
        HS_Lidar *HS = new HS_Lidar;
        msleep(500);
        uint8_t header[8];
        moveToOffset( i,fp);
        qDebug()<<"i"<<i;

        fread(header, sizeof(uint8_t), 8, fp);

        HS->initData(fp);

        drawData->maxY = 0;
        drawData->minY = 0;


        drawData->size1 = HS->CH1.nL0;
        drawData->size2 = HS->CH2.nL0;
        drawData->size3 = HS->CH3.nL0;
        drawData->size4 = HS->CH4.nL0;

        for(int j  = 0;j<HS->CH1.nL0;j++){
          drawData->line1[j] = (unsigned)HS->CH1.nD0[j];
          if((unsigned)HS->CH1.nD0[j]>drawData->maxY){
              drawData->maxY = (unsigned)HS->CH1.nD0[j];
          }
          x1++;
      }

        for(int j = 0;j<HS->CH2.nL0;j++){
          drawData->line2[j] = (unsigned)HS->CH2.nD0[j];

          if((unsigned)HS->CH2.nD0[j]>drawData->maxY){
              drawData->maxY = (unsigned)HS->CH2.nD0[j];
          }
          x2++;
      }

        for(int j = 0;j<HS->CH3.nL0;j++){
          drawData->line3[j] = (unsigned)HS->CH3.nD0[j];
          if((unsigned)HS->CH3.nD0[j]>1000){
          }
          if((unsigned)HS->CH3.nD0[j]>drawData->maxY){
              drawData->maxY = (unsigned)HS->CH3.nD0[j];
          }
          x3++;
      }

        for(int j = 0;j<HS->CH4.nL0;j++){
          drawData->line4[j] = (unsigned)HS->CH4.nD0[j];
          if((unsigned)HS->CH4.nD0[j]>1000){
          }
          if((unsigned)HS->CH4.nD0[j]>drawData->maxY){
              drawData->maxY = (unsigned)HS->CH4.nD0[j];
          }



          x4++;
      }

        drawData->x1 = x1;
        drawData->x2 = x2;
        drawData->x3 = x3;
        drawData->x4 = x4;
        i++;
        drawData->i = i;
      emit draw(*drawData);
    }
    fclose(fp);


}






}
