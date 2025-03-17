#ifndef CUSTONTHEME_H
#define CUSTONTHEME_H

#include <QWidget>

namespace Ui {
class CustonTheme;
}

class CustonTheme : public QWidget
{
    Q_OBJECT

public:
    explicit CustonTheme(QWidget *parent = nullptr);
    ~CustonTheme();

private slots:
    void on_btnThemeGreen_clicked();

    void on_btnThemeBlue_clicked();

    void on_btnThemePink_clicked();

    void on_btnThemeBlack_clicked();

signals:
    void onThemeGreenSet();
    void onThemeBlueSet();
    void onThemePinkSet();
    void onThemeBlackSet();

private:
    Ui::CustonTheme *ui;

};

#endif // CUSTONTHEME_H
