//     Copyright (c) 2014 spacelan1993@gmail.com All rights reserved.

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    globalInit();
    myCom = NULL;
    isFirst = true;
    ui->recieveTextBrowser->setOpenLinks(false);
    connect(ui->recieveTextBrowser, SIGNAL(anchorClicked(const QUrl&)),this, SLOT(anchorClickedSlot(const QUrl&)));

    QString date = QLocale(QLocale::C).toDate(QString(__DATE__).replace("  "," "),"MMM d yyyy").toString("yyyy-MM-dd");
    QString time = QString(__TIME__).left(5);
    setWindowTitle(tr("Spacelan's 串口调试助手   ")+date+" "+time);

    ui->recieveTextBrowser->moveCursor(QTextCursor::End);
    ui->recieveTextBrowser->setText(tr("欢迎使用Spacelan's 串口调试助手！\n\nSpacelan's 串口调试助手是Spacelan写的第一个也是目前唯一一个有那么一丢丢实用价值的QT程序\n\nSpacelan's 串口调试助手是某个项目的基础程序\n\nSpacelan's 串口调试助手的代码参考了开源程序QCom"));
    ui->recieveTextBrowser->moveCursor(QTextCursor::End);
    ui->recieveTextBrowser->insertHtml(tr("<a href=\"http://www.qter.org/?page_id=203\">参考链接</a><br><br>"));
    ui->recieveTextBrowser->moveCursor(QTextCursor::End);
    ui->recieveTextBrowser->insertPlainText("GitHub ");
    ui->recieveTextBrowser->moveCursor(QTextCursor::End);
    ui->recieveTextBrowser->insertHtml("<a href=\"https://github.com/spacelan/MyCom.git\">https://github.com/spacelan/MyCom.git</a><br><br>");
    ui->recieveTextBrowser->moveCursor(QTextCursor::End);
    ui->recieveTextBrowser->insertPlainText("Copyright (c) 2014 spacelan1993@gmail.com All rights reserved.");
    ui->recieveTextBrowser->moveCursor(QTextCursor::End);
}

Widget::~Widget()
{
    if(myCom != NULL)
    {
        if(myCom->isOpen())
            myCom->close();
        delete myCom;
    }
    delete ui;
}

void Widget::globalInit()
{
    ui->openclosebtn->setText(tr("打开串口"));
    ui->sendbtn->setText(tr("发送"));
    ui->sendbtn->setEnabled(false);
}

void Widget::setComboxEnabled(bool b)
{
    ui->portnameComboBox->setEnabled(b);
    ui->baudRateComboBox->setEnabled(b);
    ui->dataBitsComboBox->setEnabled(b);
    ui->parityComboBox->setEnabled(b);
    ui->stopbitsComboBox->setEnabled(b);
}

void Widget::readMyCom()
{
    QByteArray temp = myCom->readAll();
    QString buf;

    if(!temp.isEmpty())
    {
        ui->recieveTextBrowser->setTextColor(Qt::black);
        buf = temp;

//        ui->recieveTextBrowser->setText(ui->recieveTextBrowser->document()->toPlainText() + buf);
//        QTextCursor cursor = ui->recieveTextBrowser->textCursor();
//        cursor.movePosition(QTextCursor::End);
//        ui->recieveTextBrowser->setTextCursor(cursor);

        ui->recieveTextBrowser->moveCursor(QTextCursor::End);
        ui->recieveTextBrowser->insertPlainText(buf);
        ui->recieveTextBrowser->moveCursor(QTextCursor::End);
    }
}

void Widget::on_openclosebtn_clicked()
{
    if(isFirst)
    {
        ui->recieveTextBrowser->clear();
        isFirst = false;
    }
    if(myCom)
    {
        myCom->close();
        delete myCom;
        myCom =NULL;

        ui->appName->setText(tr("未打开..."));
        ui->sendbtn->setEnabled(false);
        setComboxEnabled(true);
        ui->openclosebtn->setText(tr("打开串口"));

        QMessageBox::information(this,tr("关闭串口成功"),tr("成功关闭串口")/* + portName*/,QMessageBox::Ok);
        return;
    }
    QString portName = ui->portnameComboBox->currentText();
    myCom = new QextSerialPort(portName);
    connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));

    myCom->setBaudRate((BaudRateType)ui->baudRateComboBox->currentText().toInt());

    switch(ui->dataBitsComboBox->currentIndex())
    {
    case 0:
        myCom->setDataBits(DATA_7);
        break;
    case 1:
        myCom->setDataBits(DATA_8);
        break;
    default:
        myCom->setDataBits(DATA_7);
    }

    switch(ui->parityComboBox->currentIndex())
    {
    case 0:
        myCom->setParity(PAR_NONE);
        break;
    case 1:
        myCom->setParity(PAR_ODD);
        break;
    case 2:
        myCom->setParity(PAR_EVEN);
        break;
    default:
        myCom->setParity(PAR_NONE);
    }

    switch(ui->stopbitsComboBox->currentIndex())
    {
    case 0:
        myCom->setStopBits(STOP_1);
        break;
    case 1:
        myCom->setStopBits(STOP_1_5);
        break;
    case 2:
        myCom->setStopBits(STOP_2);
        break;
    default:
        myCom->setStopBits(STOP_1);
    }

    myCom->setFlowControl(FLOW_OFF);

    myCom->setTimeout(TIME_OUT);

    if(myCom->open(QIODevice::ReadWrite))
    {

        //界面控制
        ui->appName->setText(tr("已打开..."));
        ui->openclosebtn->setText(tr("关闭串口"));
        ui->sendbtn->setEnabled(true);
        setComboxEnabled(false);
        ui->sendLineEdit->setFocus();

        QMessageBox::information(this,tr("打开串口成功"),tr("成功打开串口")+portName,QMessageBox::Ok);

    }
    else
    {
        QMessageBox::critical(this, tr("打开失败"), tr("未能打开串口 ") + portName + tr("\n该串口设备不存在或已被占用"), QMessageBox::Ok);
        return;
    }
}

void Widget::on_sendbtn_clicked()
{
    if(ui->sendLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("提示消息"), tr("没有需要发送的数据"), QMessageBox::Ok);
        return;
    }
    ui->sendLineEdit->setFocus();
    QByteArray buf;
    buf = ui->sendLineEdit->text().toAscii();
    myCom->write(buf);
    ui->recieveTextBrowser->setTextColor(Qt::lightGray);
}

void Widget::anchorClickedSlot(const QUrl &url)
{
    ShellExecuteA(NULL, "open", url.toString().toStdString().c_str(), "", "", SW_SHOW);
}

void Widget::on_clearbtn_clicked()
{
    ui->recieveTextBrowser->clear();
}
