#include "custontheme.h"
#include "ui_custontheme.h"

CustonTheme::CustonTheme(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustonTheme)
{
    ui->setupUi(this);

}

CustonTheme::~CustonTheme()
{
    delete ui;
}

void CustonTheme::on_btnThemeGreen_clicked()
{
    emit onThemeGreenSet();
}

void CustonTheme::on_btnThemeBlue_clicked()
{
    emit onThemeBlueSet();
}

void CustonTheme::on_btnThemePink_clicked()
{
    emit onThemePinkSet();
}

void CustonTheme::on_btnThemeBlack_clicked()
{
    emit onThemeBlackSet();
}
