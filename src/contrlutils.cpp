#include "contrlutils.h"
#include "ui_contrlutils.h"
#include <QDebug>

ContrlUtils::ContrlUtils(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContrlUtils)
{
    ui->setupUi(this);

}

ContrlUtils::~ContrlUtils()
{
    delete ui;
}


void ContrlUtils::on_btnLight_2_clicked(bool checked)
{
    emit lightCheckClicked(checked);
}
void ContrlUtils::on_btnShan_clicked(bool checked)
{
    emit shanCheckClicked(checked);
}
void ContrlUtils::on_btnAir_clicked(bool checked)
{
    emit airCheckClicked(checked);
}

void ContrlUtils::on_btnMode_clicked()
{
    emit winModeClicked();
}

void ContrlUtils::on_btnWinStyle_clicked()
{
    emit winStyleClicked();
}
