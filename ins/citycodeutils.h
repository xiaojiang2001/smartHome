#ifndef CITYCODEUTILS_H
#define CITYCODEUTILS_H
#include <QMap>
#include <QString>


class cityCodeUtils
{
public:
    cityCodeUtils();
    QMap<QString, QString> cityMap;
    void initMap();
    QString getCityCode(QString cityName);
};

#endif // CITYCODEUTILS_H
