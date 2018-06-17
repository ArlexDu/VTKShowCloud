//
// Created by 杜盛瑀 on 2018/6/17.
//

#ifndef VTKSHOWCLOUD_DIVIDEDIALOG_H
#define VTKSHOWCLOUD_DIVIDEDIALOG_H
#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <iostream>

using namespace std;

class DivideDialog:public QDialog{
    Q_OBJECT
public:
//构造函数，表示自定义的对话框没有父窗口部件
    DivideDialog(QWidget *parent=0);
    ~DivideDialog();
public slots:
    virtual void accept();
    char result();
private:
    QLabel *titleLabel;
    QListWidget *list;
    QPushButton *okBtn;
    QVBoxLayout *dlgLayout;
};


#endif //VTKSHOWCLOUD_DIVIDEDIALOG_H
