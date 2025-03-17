#include "cirtainutils.h"
#include "ui_cirtainutils.h"
#include "mybutton.h"
#include <QDateTime>
#include <QTimer>

cirtainUtils::cirtainUtils(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cirtainUtils)
{
    ui->setupUi(this);

    //定义一个定时器，每个100ms刷新以下系统时间显示
    QTimer *getSysTimeTimer = new QTimer(this);
    connect(getSysTimeTimer,SIGNAL(timeout()),this,SLOT(time_reflash()));
    getSysTimeTimer->start(100);



    animationUp = new QPropertyAnimation(ui->labelcur,"pos");
    animationDown = new QPropertyAnimation(ui->labelcur,"pos");
    data_timer = new QTimer(this);
    connect(data_timer,SIGNAL(timeout()),this, SLOT(slot_dataChanged()));

    connect(ui->btn,&MyButton::isClickedWithParams,this,&cirtainUtils::btnClicked);
}

cirtainUtils::~cirtainUtils()
{
    delete ui;
}

void cirtainUtils::btnClicked(bool value)
{
    if(value){
        ui->labelcur->setPixmap(QPixmap(":/curtain/curtain/l1.png"));
        ui->btnUp->setStyleSheet("border-image: url(:/curtain/curtain/s2.png);");
        ui->btnDown->setStyleSheet("border-image: url(:/curtain/curtain/d2.png);");
    }else{
        ui->labelcur->setPixmap(QPixmap(":/curtain/curtain/l2.png"));
        ui->btnUp->setStyleSheet("border-image: url(:/curtain/curtain/s1.png);");
        ui->btnDown->setStyleSheet("border-image: url(:/curtain/curtain/d1.png);");
    }
}

void cirtainUtils::slot_dataChanged()
{
    int scale = abs(ui->labelcur->pos().y()) * 1.0 /ui->labelcur->height() * 100;
    ui->labelpercent->setText(QString::number(scale) + " %");
}


void cirtainUtils::on_btnDown_pressed()
{
    int start_x = ui->labelcur->pos().x(); //动画开始x坐标
    int start_y = ui->labelcur->pos().y(); //动画开始y坐标
    int end_x = ui->labelcur->pos().x();   //动画结束x坐标
    int end_y = 0;                          //动画结束y坐标

    animationDown->setStartValue(QPoint(start_x,start_y));
    animationDown->setEndValue(QPoint(end_x,end_y));
    animationDown->setDuration(1000);
    animationDown->start();
    data_timer->start(100);
}

void cirtainUtils::on_btnDown_released()
{
    animationDown->stop();
    data_timer->stop();

    QString sendData = "<*06,107," + ui->labelpercent->text().replace(" %","") + "*>";
}

void cirtainUtils::on_btnUp_pressed()
{

    int start_x = ui->labelcur->pos().x();//动画开始x坐标
    int start_y = ui->labelcur->pos().y();//动画开始y坐标
    int end_x = ui->labelpercent->pos().x();//动画结束x坐标
    int end_y = 0 - ui->labelcur->height();//动画结束y坐标

    animationUp->setStartValue(QPoint(start_x,start_y));
    animationUp->setEndValue(QPoint(end_x,end_y));
    animationUp->setDuration(1000);
    animationUp->start();

    data_timer->start(100);
}

void cirtainUtils::on_btnUp_released()
{
    animationUp->stop();
    data_timer->stop();

    QString sendData = "<*06,107," + ui->labelpercent->text().replace(" %","") + "*>";
}

void cirtainUtils::time_reflash()
{
    getSysTime();
    ui->labelCurrentTime->setText(myTime);
}

void cirtainUtils::getSysTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    //处理日期
    QDate date = currentTime.date();
    int year = date.year();
    int month = date.month();
    int day = date.day();
    //处理时间
    QTime time = currentTime.time();
    int hour = time.hour();
    int minite = time.minute();
    int second = time.second();
    //格式化成 2024-01-01  23:12:05
    myTime = QString("%1-%2-%3  %4:%5:%6")
            .arg(year, 2, 10, QChar('0'))
            .arg(month, 2, 10, QChar('0'))
            .arg(day, 2, 10, QChar('0'))
            .arg(hour, 2, 10, QChar('0'))
            .arg(minite, 2, 10, QChar('0'))
            .arg(second, 2, 10, QChar('0'));
}
