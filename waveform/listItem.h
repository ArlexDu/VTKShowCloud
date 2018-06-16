//
// Created by 杜盛瑀 on 2018/6/16.
//

#ifndef VTKSHOWCLOUD_LISTITEM_H
#define VTKSHOWCLOUD_LISTITEM_H
#include <QWidget>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <vtkActor.h>
#include <string>
#include <QString>

using namespace std;

class listItem : public QWidget{
    Q_OBJECT
public:
    listItem(QWidget *parent);
    ~listItem();
    void setFileName(string name);
    void setActor(vtkActor *actor);
    vtkActor* getActor();
    QCheckBox *checkBox;
private:
    QPlainTextEdit *text;
    vtkActor *actor;
};


#endif //VTKSHOWCLOUD_LISTITEM_H
