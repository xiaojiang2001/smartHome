#include "mybutton.h"

#include <QMouseEvent>
#include <QPainter>

MyButton::MyButton(QWidget *parent) : QWidget(parent)
{
    setFixedSize(60,20);//10  50
    animation = new QPropertyAnimation(this);
    animation->setTargetObject(this);

    animation->setStartValue(height()/2);
    animation->setEndValue(width()-height()/2);
    animation->setEasingCurve(QEasingCurve::InOutElastic);
    animation->setDuration(200);

    connect(animation, &QPropertyAnimation::valueChanged,this,[=](const QVariant &value){
        posX = value.toInt();
        update();
    });
}

void MyButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    int radius = height()/2;
    painter.setPen(Qt::NoPen);
    painter.setBrush( isOff ? offBgBrush : onBgBrush);
    painter.drawRoundedRect(this->rect(), radius, radius);

    painter.setBrush( isOff ? offRBrush : onRBrush);
    QPoint center;
    center.setX(posX);
    center.setY(radius);
    painter.drawEllipse(center,radius-radius/10,radius-radius/10);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial",radius/2));
    painter.drawText(this->rect(),Qt::AlignCenter, isOff ? offText : onText);
}

void MyButton::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton){
        isOff ? animation->setDirection(QAbstractAnimation::Forward) : animation->setDirection(QAbstractAnimation::Backward);
        animation->start();
        emit isClicked();
        isOff ? emit isClickedWithParams(true) : emit isClickedWithParams(false);
        isOff = !isOff;


    }

}

void MyButton::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    animation->setStartValue(height()/2);
    animation->setEndValue(width()-height()/2);
}
