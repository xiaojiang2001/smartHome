#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include <QPropertyAnimation>

class MyButton : public QWidget
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = nullptr);

signals:
    void isClicked();

    void isClickedWithParams(bool);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:

    bool isOff = true;

    QBrush offBgBrush = Qt::black;
    QBrush offRBrush  = QColor(174,94,134);

    QBrush onBgBrush  = Qt::gray;
    QBrush onRBrush   = QColor(0,128,128);

    QString offText = "";
    QString onText  = "";

    QPropertyAnimation *animation;
    int posX = height()/2;

};

#endif // MYBUTTON_H
