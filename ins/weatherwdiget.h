#ifndef WEATHERWDIGET_H
#define WEATHERWDIGET_H
#include "citycodeutils.h"

#include <QNetworkAccessManager>
#include <QWidget>

namespace Ui {
class WeatherWdiget;
}

class Day
{
public:
    QString m_date;
    QString m_week;
    QString m_city;
    QString m_tem;
    QString m_tem1;
    QString m_tem2;
    QString m_wea;
    QString m_win;
    QString m_win_speed;
    QString m_air;
    QString m_air_level;
    QString m_humidity;
    QString m_air_tips;
};


class WeatherWdiget : public QWidget
{
    Q_OBJECT

public:
    explicit WeatherWdiget(QWidget *parent = nullptr);
    ~WeatherWdiget();
private slots:
    void readHttpReply(QNetworkReply *reply);

    void on_btnSearch_clicked();

private:
    Ui::WeatherWdiget *ui;
    QString m_strUrl;
    QNetworkAccessManager *manager;
    void parseJson(QByteArray data);
    void updateUi();
    int parseString(QString str);

    //未来几天的数据
    Day day[7];
    QMap<QString, QString> weatherPicMap;
    void initTypeMap();
    cityCodeUtils codeUtils;

};

#endif // WEATHERWDIGET_H
