#ifndef LEFTCIRCLE_H
#define LEFTCIRCLE_H

#include <QWidget>

class LeftCircle : public QWidget
{
    Q_OBJECT
public:
    explicit LeftCircle(QWidget *parent = nullptr);

signals:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTimer *timer;
    int mark = 0;
    double pointAngle;

    int currentValue = 20;
    int startAngle = 75;
    double angle;
    void drawMiddleCircle(QPainter &painter, int radius, QPen pen);
    void drawScale(QPainter &painter, int radius);
    void drawScaleText(QPainter &painter, int radius);
    void DrawCircle_bom(QPainter& painter,int radius);
    void drawPointLine(QPainter &painter,int lenth);
    void drawTextMiddleCircle(QPainter& painter);
    void drawSpeedPie(QPainter &painter, int radius);

};

#endif // LEFTCIRCLE_H
