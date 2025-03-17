#ifndef CONTRLUTILS_H
#define CONTRLUTILS_H

#include <QWidget>

namespace Ui {
class ContrlUtils;
}

class ContrlUtils : public QWidget
{
    Q_OBJECT

public:
    explicit ContrlUtils(QWidget *parent = nullptr);
    ~ContrlUtils();

private slots:
    void on_btnLight_2_clicked(bool checked);
    void on_btnShan_clicked(bool checked);
    void on_btnAir_clicked(bool checked);

    void on_btnMode_clicked();
    void on_btnWinStyle_clicked();

signals:
    void lightCheckClicked(bool);
    void shanCheckClicked(bool);
    void airCheckClicked(bool);
    void winModeClicked();
    void winStyleClicked();

private:
    Ui::ContrlUtils *ui;
};

#endif // CONTRLUTILS_H
