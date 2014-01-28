//     Copyright (c) 2014 spacelan1993@gmail.com All rights reserved.

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCore>
#include <QMessageBox>
#include <qextserialport.h>
//#include <qextserialport_p.h>
//#include <qextserialport_global.h>
#include <QTimer>
#include <windows.h>

//串口读写延时
#define TIME_OUT 10

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
    //全局初始化
    void globalInit();

    void setComboxEnabled(bool);

private:
    Ui::Widget *ui;
    bool isFirst;
    QextSerialPort *myCom;

private slots:
    void readMyCom();
    void on_openclosebtn_clicked();
    void on_sendbtn_clicked();
    void anchorClickedSlot(const QUrl&);
    void on_clearbtn_clicked();
};

#endif // WIDGET_H
