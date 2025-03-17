#include "weatherwdiget.h"
#include "ui_weatherwdiget.h"
#include "citycodeutils.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMessageBox>
#include <QNetworkReply>

WeatherWdiget::WeatherWdiget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeatherWdiget)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);


    ui->labelTips->setWordWrap(true);


    initTypeMap();

    //发起网络请求
    m_strUrl = "http://v1.yiketianqi.com/api?unescape=1&version=v9&appid=89361488&appsecret=K6tTmCT0";
    QUrl url(m_strUrl);
    QNetworkRequest res(url);
    manager->get(res);
    connect(manager, &QNetworkAccessManager::finished, this, &WeatherWdiget::readHttpReply);

}

WeatherWdiget::~WeatherWdiget()
{
    delete ui;
}

void WeatherWdiget::readHttpReply(QNetworkReply *reply)
{
    //状态码
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "statusCode: " << statusCode;
    //判断是否请求成功
    if (reply->error() != QNetworkReply::NoError || statusCode != 200){
        QMessageBox::warning(this, "天气", reply->errorString(), QMessageBox::Ok);
    }
    else{
        QByteArray requestData = reply->readAll();
        //qDebug() << requestData.data();
        parseJson(requestData);                     // 解析json数据
    }
}

int WeatherWdiget::parseString(QString str)
{
    str = str.split(" ").at(1);//以空格为分割符,去除前面的文字,取出第二个X℃,
    str = str.left(str.length() - 1);//去除后面的摄氏度符号
    return str.toInt();
}

void WeatherWdiget::parseJson(QByteArray data)
{
    //解析错误时会触发的
    QJsonParseError err;
    //创建QJsonDocument
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError)
    {
        qDebug("%s(%d): %s", __FUNCTION__, __LINE__, err.errorString().toLatin1().data());
        return;
    }
    //Json对象,包括了请求的全部数据
    QJsonObject rootObj = doc.object();
    // qDebug() << rootObj;

    // 开始解析数据
    day[0].m_city = rootObj["city"].toString();
    if(rootObj.contains("data") && rootObj["data"].isArray())
    {
        QJsonArray dayArray = rootObj["data"].toArray();
        int i = 0;
        for(QJsonValue val : dayArray)
        {
            day[i].m_date = val["date"].toString();
            day[i].m_week = val["week"].toString();
            day[i].m_wea = val["wea"].toString();
            //天气不要转
            if(day[i].m_wea.contains("转"))
            {
                QStringList weaList = day[i].m_wea.split("转");
                day[i].m_wea = weaList[0];
            }

            day[i].m_tem  = val["tem"].toString();
            day[i].m_tem1 = val["tem1"].toString();
            day[i].m_tem2 = val["tem2"].toString();
            day[i].m_date = val["date"].toString();
            day[i].m_air_tips = val["air_tips"].toString();
            day[i].m_win = val["win"][0].toString();
            day[i].m_win_speed = val["win_speed"].toString();
            day[i].m_air = val["air"].toString();
            day[i].m_air_level = val["air_level"].toString();
            day[i].m_humidity = val["humidity"].toString();
            i++;
        }
    }
    updateUi();
}

void WeatherWdiget::updateUi()
{
    // 01
    ui->labelWeekDay->setText( day[0].m_date + " " + day[0].m_week);

    // 02
    ui->labelweaIcon->setPixmap(weatherPicMap[day[0].m_wea]);
    ui->labelTodayTemp->setText(day[0].m_tem1);
    ui->labelTodyaLow->setText(" / "+day[0].m_tem2+"℃");
    ui->labelTips->setText(day[0].m_air_tips);
    ui->labelCity->setText(day[0].m_city);

    // 三天天气
    ui->labelday0Icon->setPixmap(weatherPicMap[day[1].m_wea]);
    ui->labelday0High->setText(day[1].m_tem1);
    ui->labelday0Low->setText(" / "+day[1].m_tem2+"℃");
    QStringList dateList = day[1].m_date.split("-");        // 2024-07-09
    ui->labelday0Date->setText(dateList[1] + "-" + dateList[2]);
    ui->labelday0wea->setText(day[1].m_wea);

    ui->labelday1Icon->setPixmap(weatherPicMap[day[2].m_wea]);
    ui->labelday1High->setText(day[2].m_tem1);
    ui->labelday1Low->setText(" / "+day[2].m_tem2+"℃");
    dateList = day[2].m_date.split("-");
    ui->labelday1Date->setText(dateList[1] + "-" + dateList[2]);
    ui->labelday1wea->setText(day[2].m_wea);

    ui->labelday3Icon->setPixmap(weatherPicMap[day[3].m_wea]);
    ui->labelday3High->setText(day[3].m_tem1);
    ui->labelday3Low->setText(" / "+day[3].m_tem2+"℃");
    dateList = day[3].m_date.split("-");
    ui->labelday3Date->setText(dateList[1] + "-" + dateList[2]);
    ui->labelday3wea->setText(day[3].m_wea);

    update();
}

void WeatherWdiget::initTypeMap()
{
    //根据keys,设置icon的路径
    //根据keys,设置icon的路径
    weatherPicMap.insert("暴雪",":/res/type/BaoXue.png");
    weatherPicMap.insert("暴雨",":/res/type/BaoYu. png");
    weatherPicMap.insert("暴雨到大暴雨",":/res/type/BaoYuDaoDaBaoYu.png");
    weatherPicMap.insert("大暴雨",":/res/type/DaBaoYu.png");
    weatherPicMap.insert("大暴雨到特大暴雨",":/res/type/DaBaoYuDaoTeDaBaoYu.png");
    weatherPicMap.insert("大到暴雪",":/res/type/DaDaoBaoXue.png");
    weatherPicMap.insert("大雪",":/res/type/DaXue.png");
    weatherPicMap.insert("大雨",":/res/type/DaYu.png");
    weatherPicMap.insert("冻雨",":/res/type/DongYu.png");
    weatherPicMap.insert("多云",":/res/type/DuoYun.png");
    weatherPicMap.insert("浮沉",":/res/type/FuChen.png");
    weatherPicMap.insert("雷阵雨",":/res/type/LeiZhenYu.png");
    weatherPicMap.insert("雷阵雨伴有冰雹",":/res/type/LeiZhenYuBanYouBingBao.png");
    weatherPicMap.insert("霾",":/res/type/Mai.png");
    weatherPicMap.insert("强沙尘暴",":/res/type/QiangShaChenBao.png");
    weatherPicMap.insert("晴",":/res/type/Qing.png");
    weatherPicMap.insert("沙尘暴",":/res/type/ShaChenBao.png");
    weatherPicMap.insert("特大暴雨",":/res/type/TeDaBaoYu.png");
    weatherPicMap.insert("undefined",":/res/type/undefined.png");
    weatherPicMap.insert("雾",":/res/type/Wu.png");
    weatherPicMap.insert("小到中雪",":/res/type/XiaoDaoZhongXue.png");
    weatherPicMap.insert("小到中雨",":/res/type/XiaoDaoZhongYu.png");
    weatherPicMap.insert("小雪",":/res/type/XiaoXue.png");
    weatherPicMap.insert("小雨",":/res/type/XiaoYu.png");
    weatherPicMap.insert("雪",":/res/type/Xue.png");
    weatherPicMap.insert("扬沙",":/res/type/YangSha.png");
    weatherPicMap.insert("阴",":/res/type/Yin.png");
    weatherPicMap.insert("雨",":/res/type/Yu.png");
    weatherPicMap.insert("雨夹雪",":/res/type/YuJiaXue.png");
    weatherPicMap.insert("阵雪",":/res/type/ZhenXue.png");
    weatherPicMap.insert("阵雨",":/res/type/ZhenYu.png");
    weatherPicMap.insert("中到大雪",":/res/type/ZhongDaoDaXue.png");
    weatherPicMap.insert("中到大雨",":/res/type/ZhongDaoDaYu.png");
    weatherPicMap.insert("中雪",":/res/type/ZhongXue.png");
    weatherPicMap.insert("中雨",":/res/type/ZhongYu.png");
}


void WeatherWdiget::on_btnSearch_clicked()
{
    QString cityName = ui->lineEditCity->text();
    QString cityCode = codeUtils.getCityCode(cityName);
    if(cityCode != NULL)
    {
        m_strUrl += "&cityid=" + cityCode;
        //qDebug() << m_strUrl;
        manager->get(QNetworkRequest(QUrl(m_strUrl)));
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误");
        msgBox.setText("请输入正确的城市名");
        msgBox.setStyleSheet("QPushButton{color:red}");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
}
