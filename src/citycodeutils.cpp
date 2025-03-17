#include "citycodeutils.h"

#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>

cityCodeUtils::cityCodeUtils()
{

}

void cityCodeUtils::initMap()
{
    // 打开Json文件
    // QFile file("D:/qtProject/00_QT_CLC/smartHome/citycode.json");
    QFile file(":/citycode.json");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "---------file open error----------";
    }
    QByteArray readData = file.readAll();
    file.close();

    // 将json转为QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(readData);
    if(jsonDoc.isArray())
    {
        QJsonArray citys = jsonDoc.array();
        for(QJsonValue val : citys)
        {
            if(val.isObject())
            {
                QString city_name = val["city_name"].toString();
                QString city_code = val["city_code"].toString();
                cityMap.insert(city_name, city_code);
            }
        }
    }
}

QString cityCodeUtils::getCityCode(QString cityName)
{
    if(cityMap.isEmpty())
        initMap();
    QMap<QString, QString>::iterator it =cityMap.find(cityName);
    if(it != cityMap.end())
        return it.value();
    else
    {
        it = cityMap.find(cityName + "市");
        if(it != cityMap.end())
            return it.value();
        else
        {
            it = cityMap.find(cityName + "县");
            if(it != cityMap.end())
                return it.value();
            else
            {
                it = cityMap.find(cityName + "区");
                if(it != cityMap.end())
                    return it.value();
            }
        }
    }
    return "";
}
