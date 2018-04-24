#ifndef DRAWDATA_H
#define DRAWDATA_H

class DrawData{
public:
    short line1[6500];
    short line2[6500];
    short line3[6500];
    short line4[6500];
    int size1;
    int size2;
    int size3;
    int size4;
    int maxY;
    int minY;
    int x1;
    int x2;
    int x3;
    int x4;
    int i;
    DrawData(){};
};


Q_DECLARE_METATYPE(DrawData);
#endif // DRAWDATA_H
