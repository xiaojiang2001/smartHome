#ifndef CIRTAINUTILS_H
#define CIRTAINUTILS_H

#include <QPropertyAnimation>
#include <QWidget>

namespace Ui {
class cirtainUtils;
}

class cirtainUtils : public QWidget
{
    Q_OBJECT

public:
    explicit cirtainUtils(QWidget *parent = nullptr);
    ~cirtainUtils();

public slots:
    void btnClicked(bool);
    void slot_dataChanged();

private slots:
    void time_reflash();

    void on_btnDown_pressed();

    void on_btnDown_released();

    void on_btnUp_pressed();

    void on_btnUp_released();

private:
    Ui::cirtainUtils *ui;

    QPropertyAnimation* animationUp;
    QPropertyAnimation* animationDown;
    QTimer *data_timer;
    QString myTime;


    void getSysTime();

};

#endif // CIRTAINUTILS_H
