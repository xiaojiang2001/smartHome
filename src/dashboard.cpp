#include "dashboard.h"
#include "ui_dashboard.h"
#include <QtMath>

#include <QPainter>

DashBoard::DashBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DashBoard)
{
    ui->setupUi(this);
}

DashBoard::~DashBoard()
{
    delete ui;
}

void DashBoard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);

    painter.translate(rect().center());
    drawCrown(painter);
    drawScale(painter,75);
    drawScaleText(painter,60);

}

void DashBoard::drawScaleText(QPainter &painter, int radius)
{
    //写刻度文字
    QFont font("Arial",9);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::gray);
    int r = radius ;
    for(int i=0; i<=40; i++){
        if(i % 5 == 0){
            //保存坐标系
            painter.save();
            //算出平移点，弧度=角度*3.1415/180
            int delX = qCos( (280-angle*i)*M_PI/180) * r;//QT中sin认的是弧度
            int delY = qSin( qDegreesToRadians(280.0-angle*i)) * r;
            //平移坐标系
            painter.translate(QPoint(delX,-delY));
            //旋转坐标系
            painter.rotate(-135+angle*i);//angle=4,30*4=120的时候，实参是0，120
            //写上文字
            painter.drawText(-25,-25,50,30,Qt::AlignCenter,QString::number(i));
            //恢复坐标系
            painter.restore();
        }
    }
}

void DashBoard::drawScale(QPainter &painter, int radius)
{
    //画刻度
    //1. 算出一个刻度需要旋转的角度
    angle = 280*1.0 / 40;//270*1.0的作用是扩大变量类型，把int型阔成double,保留小数
    //保存当前坐标位置，此时此刻是在原点，x在3点钟方向
    painter.save();
    painter.setPen(QPen(Qt::white,1));
    //2.  设置第一个刻度的位置
    painter.rotate(130);
    for(int i=0;i<=40;i++){

        if(i % 5 == 0){
            //画长的刻度线
            painter.drawLine(radius - 8, 0, radius +2 ,0);
        }else{//画短的刻度线
            painter.drawLine(radius - 8, 0, radius - 3 ,0);
        }
        //画完后旋转
        painter.rotate(angle);
    }
    painter.restore();
    //painter.setPen(QPen(Qt::white,5));
}

void DashBoard::drawCrown(QPainter &painter)
{
    painter.save();
    int radius = 110;
    painter.setBrush(m_background = QColor(20, 20, 20));
    QLinearGradient lg1(0, -100, 0, 100);
    lg1.setColorAt(0, Qt::gray);
    lg1.setColorAt(1, QColor(20, 20, 20));
    painter.setBrush(lg1);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPoint(0,0), radius, radius);
    painter.setBrush(m_background = QColor(20, 20, 20));
    painter.drawEllipse(QPoint(0,0),103,103);
    painter.restore();
}
