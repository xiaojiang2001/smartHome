#include "leftcircle.h"

#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QtMath>
#include "ColorSingleton.h"

LeftCircle::LeftCircle(QWidget *parent) : QWidget(parent)
{
    setFixedSize(QSize(280,280));
    timer = new QTimer(this);
    ColorSingleton::getInstance().setColor(QColor(34, 38, 46));
    currentValue = 30;
    connect(timer, &QTimer::timeout,[=](){
        if(mark == 0){
            currentValue++;
            if(currentValue >= 101){
                mark = 1;
            }

        }
        if(mark == 1){
            currentValue--;
            if(currentValue == 0){
                mark = 0;
            }
        }

        update();
    });
    timer->start(100);
}

void LeftCircle::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);


//    painter.setBrush(ColorSingleton::getInstance().getColor());
//    painter.setPen(Qt::NoPen);
//    painter.drawRoundedRect(0,0,308,319,20,20);


    painter.setRenderHint(QPainter::Antialiasing,true);
    //坐标系 平移到上方
    QPoint UP(rect().width()/2, rect().height()/2);
    painter.translate(UP);

    //画速度
    drawSpeedPie(painter, 105);
    //画中间小圆环
    painter.setBrush(QColor(0,0,0));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPoint(0,0),90,90);
    //画指针
    drawPointLine(painter, 105);
    //画中间小圆
    drawMiddleCircle(painter, 40, QPen(QColor(255,255,255),2));
    //画圈中小横线
    painter.setPen(QPen(QColor(200,200,200,55),0.5));
    painter.drawLine(QPoint(-37,0),QPoint(37,0));
    //画圈中文字
    drawTextMiddleCircle(painter);
    //画刻度
    drawScale(painter, 100);
    painter.setPen(Qt::NoPen);
    //画外圈
    DrawCircle_bom(painter,110);
    //画码字
    drawScaleText(painter, 125);
}

void LeftCircle::drawMiddleCircle(QPainter &painter, int radius, QPen pen)
{
    // 画小圆
    painter.setBrush(QColor(64,62,62));
    painter.setPen(pen);
    painter.drawEllipse(QPoint(0,0),radius,radius);
}

void LeftCircle::drawScale(QPainter &painter, int radius)
{
    //画刻度
    //1. 算出一个刻度需要旋转的角度
    angle = 165*1.0 / 11;//270*1.0的作用是扩大变量类型，把int型阔成double,保留小数
    pointAngle = 150*1.0 / 101;

    painter.setPen(QPen(Qt::white,3));

    int r = radius;
    for(int i=0; i<=10; i++){
        //保存坐标系
        painter.save();
        //算出平移点，弧度=角度*3.1415/180
        int delX = qCos( ((360-startAngle)-angle*i)*M_PI/180) * r;//QT中sin认的是弧度
        int delY = qSin(qDegreesToRadians((360-startAngle)-angle*i)) * r;
        //平移坐标系
        painter.translate(QPoint(delX,-delY));
        //旋转坐标系
        // painter.rotate(-120+angle*i);//angle=4,30*4=120的时候，实参是0，120

        painter.drawEllipse(QPoint(0,0),1,1);
        //恢复坐标系
        painter.restore();
    }
}

void LeftCircle::drawScaleText(QPainter &painter, int radius)
{
    //写刻度文字
    QFont font("Arial",7);
    font.setBold(false);
    painter.setFont(font);
    int r = radius;
    for(int i=0; i<=10; i++){

        //保存坐标系
        painter.save();
        //算出平移点，弧度=角度*3.1415/180
        int delX = qCos( (285-angle*i)*M_PI/180) * r;//QT中sin认的是弧度
        int delY = qSin(qDegreesToRadians(285-angle*i)) * r;
        //平移坐标系
        painter.translate(QPoint(delX,-delY));
        //旋转坐标系
        // painter.rotate(-120+angle*i);//angle=4,30*4=120的时候，实参是0，120
        //写上文字
        if(i == 0){
            painter.setPen(QPen(QColor(48,168,43),3));
        }else{
            painter.setPen(QPen(QColor(141,135,136),3));
        }
        painter.drawText(-10,-10,20,20,Qt::AlignCenter,QString::number(i*10));
        //恢复坐标系
        painter.restore();

    }
}

void LeftCircle::DrawCircle_bom(QPainter &painter, int radius)
{
    //保存绘图对象
    painter.save();
    //计算大小圆路径
    QPainterPath outRing;
    QPainterPath inRing;
    outRing.moveTo(0,0);
    inRing.moveTo(0,0);
    outRing.arcTo(-radius,-radius, 2*radius,2*radius,-startAngle,-155);
    inRing.addEllipse(-radius+1,-radius + 1,2*(radius-1),2*(radius-1));
    outRing.closeSubpath();

    //设置画刷
    painter.setBrush(Qt::white);
    //大圆减小圆
    painter.setPen(QPen(QColor(230,230,230),2));
    painter.drawPath(outRing.subtracted(inRing));
    painter.restore();
}

void LeftCircle::drawPointLine(QPainter &painter, int lenth)
{
    Q_UNUSED(lenth);
    //画指针，线
    painter.save();
    painter.setBrush(QColor(230,0,0,150));
    painter.setPen(Qt::NoPen);
    static const QPointF points[4] = {
        QPointF(0,0.0),
        QPointF(105.0,-1.0),
        QPointF(105.0,1.0),
        QPointF(0,8.0),
    };
    painter.rotate(startAngle + pointAngle * currentValue);
    painter.drawPolygon(points, 4);
    // painter.drawLine(60,0,lenth,0);
    //坐标轴先回到原点
    painter.restore();
}

void LeftCircle::drawTextMiddleCircle(QPainter &painter)
{
    //画圈中下方文字
    setFont(QFont("Consolas",13));
    painter.setPen(QPen(QColor(141,135,136),1));
    painter.drawText(QRect(-30,-40,60,40),Qt::AlignCenter,QString::number(currentValue));
    //写KPL
    QFont font = painter.font();
    font.setPixelSize(15);
    font.setFamily("Consolas");
    painter.setFont(font);
    //painter.drawText(QRect(-35,0,70,40),Qt::AlignCenter,"摄氏度");
}

void LeftCircle::drawSpeedPie(QPainter &painter, int radius)
{
    QRect rentangle(-radius,-radius,radius*2,radius*2);
    painter.setPen(Qt::NoPen);
    // 创建锥形渐变
    QRadialGradient gradient(QPoint(20,height()/2),200); // 第二个参数是起始角度
    gradient.setColorAt(0.0, QColor(255,255,255));
    gradient.setColorAt(0.02, Qt::yellow);
    gradient.setColorAt(1.0, QColor(255,30,35));

    // 设置画刷
    painter.setBrush(gradient);

    painter.drawPie(rentangle,(360-startAngle)*16,-pointAngle*currentValue*16);//angle前面取 负数，为了让它顺时针方向画
}
