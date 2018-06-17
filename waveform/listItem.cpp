//
// Created by 杜盛瑀 on 2018/6/16.
//

#include "listItem.h"

listItem::listItem(QWidget *parent):QWidget(parent) {
    checkBox = new QCheckBox(this);
    checkBox->setGeometry(5,0,30,30);

    text = new QPlainTextEdit(this);
    text->setGeometry(40,0,100,30);
    text->setDisabled(true);
}

listItem::~listItem() {}

void listItem::setActor(vtkActor *a) {
    this->actor = a;
}

vtkActor* listItem::getActor() {
    return this->actor;
}

void listItem::setFileName(string name) {
    QString qname = QString::fromLocal8Bit(name.c_str());
    this->text->setPlainText(qname);
}