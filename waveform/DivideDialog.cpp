//
// Created by 杜盛瑀 on 2018/6/17.
//

#include "DivideDialog.h"

DivideDialog::DivideDialog(QWidget *parent) {
    titleLabel = new QLabel("请选择文件分隔符：");
    list = new QListWidget();
    list->addItem("空格");
    list->addItem("逗号");
    list->addItem("分号");
    okBtn=new QPushButton("确定");
    dlgLayout=new QVBoxLayout;
    dlgLayout->setMargin(10);
    dlgLayout->addWidget(titleLabel);
    dlgLayout->addWidget(list);
    dlgLayout->addWidget(okBtn);
    setLayout(dlgLayout);
    connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
    //设置对话框的标题和大小
    setWindowTitle("选择分隔符");
    resize(500,400);
}
DivideDialog::~DivideDialog(){
}

void DivideDialog::accept()
{
    QDialog::accept();
}

char DivideDialog::result() {
    QListWidgetItem *item =list->item(list->currentRow());
    string separater = string((const char *)item->text().toLocal8Bit());
    cout<<"分隔符是"<<separater<<endl;
    char c =' ';
    if(separater== "空格"){
        c=' ';
    }else if(separater == "逗号"){
        c=',';
    }else{
        c=';';
    }
    return c;
}