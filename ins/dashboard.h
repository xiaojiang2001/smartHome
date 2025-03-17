#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QPainter>
#include <QWidget>

namespace Ui {
class DashBoard;
}

class DashBoard : public QWidget
{
    Q_OBJECT

public:
    explicit DashBoard(QWidget *parent = nullptr);
    ~DashBoard();
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::DashBoard *ui;
    void drawCrown(QPainter &painter);
    int angle;
    int pointAngle;
    QColor m_background;
    QColor m_foreground;

    void drawScale(QPainter &painter, int radius);
    void drawScaleText(QPainter &painter, int radius);
};

#endif // DASHBOARD_H
