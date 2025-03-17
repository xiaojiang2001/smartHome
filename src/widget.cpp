#include "widget.h"
#include "ui_widget.h"
//#include "imageslider.h"
#include "custontheme.h"
#include "SerialPortUtil.h"
#include "cam.h"
#include "ColorSingleton.h"
#include <QMouseEvent>
#include <QPainter>
#include "global.h"

#define BLACK   0
#define BLUE    1
#define GREEN   2
#define PINK    3


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 设置无标题栏
    setWindowFlags(Qt::FramelessWindowHint);
    // 设置背景透明以支持圆角
    //setAttribute(Qt::WA_TranslucentBackground);

    setFixedSize(1300,762);
    // 右键退出
     quitMenu = new QMenu(this);
     //实例化动作
     QAction *quitAct = new QAction(QIcon(":/res/close.png"),tr("退出"),this);
     quitMenu->addAction(quitAct);                   //添加动作
     connect(quitMenu, &QMenu::triggered, [=](){     //绑定动作触发槽函数
         this->close();
     });

    themeWidget = new CustonTheme(this);
    themeWidget->hide();                    //设置主题颜色窗口
    buttonGroup = new QButtonGroup(this);
    contrlUtil = new ContrlUtils(this);
    contrlUtil->hide();
    camWidget = new Cam(this);
    camWidget->hide();
    server = new TcpServer(this);
    server->initServer();

    SerialPortUtil::initSerialPort("COM4", 9600);

    Mycolor::setThemeIndex(BLACK);
    Mycolor::setColorList();

    m_airFlag = false;
    QStringList imagePaths = {
        ":/res/backg01.png",
        ":/res/backg02.png",
        ":/res/backg03.png",
        ":/res/backg04.png",
        ":/res/bg005.png"
    };

    //ui->widgetShowCen01->setChangeInterval(3000); // 每 3000 毫秒切换图片
    //ui->widgetShowCen01->setImages(imagePaths);


    player = new QMediaPlayer(this);
    playerList = new QMediaPlaylist(this);
    videoWidget = new QVideoWidget(ui->btnVideo);
    // 设置播放器播放队列
    player->setPlaylist(playerList);
    // 播放窗口
    videoWidget->resize(400,278);
    ui->btnVideo->setStyleSheet("border-image: url(:/res/park1.jpg);");

    //设置按键组互斥
    buttonGroup->addButton(ui->btnCam);
    buttonGroup->addButton(ui->btnHome);
    buttonGroup->addButton(ui->btnTheme);
    buttonGroup->addButton(ui->btnDev);
    buttonGroup->setExclusive(true);
    
    //程序左侧按钮控件的信号与槽
    connect(ui->widgetCen02,&ContrlUtils::lightCheckClicked,this,&Widget::lightCon);
    connect(ui->widgetCen02,&ContrlUtils::shanCheckClicked,this,&Widget::shanCon);
    connect(ui->widgetCen02,&ContrlUtils::airCheckClicked,this,&Widget::airCon);
    connect(ui->widgetCen02,&ContrlUtils::winModeClicked,this,&Widget::winModeCon);
    connect(ui->widgetCen02,&ContrlUtils::winStyleClicked,this,&Widget::winStyleCon);


//    ui->widget0301->installEventFilter(this);
//    ui->widget0302->installEventFilter(this);
//    ui->widget0303->installEventFilter(this);

    update();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addRoundedRect(rect(), 35, 35);            // 15是圆角半径
    painter.fillPath(path, QColor(201,201,201));    // 您可以选择所需的颜色
}

bool Widget::eventFilter(QObject *watered, QEvent *event)
{
//    if(watered == ui->widget0301 && event->type() == QEvent::Paint)
//    {
//        drawDataLine();
//        return  true;
//    }
    //return  Widget::eventFilter(watered, event);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    //右键退出
    if(event->button() == Qt::RightButton){
        quitMenu->exec(QCursor::pos());
    }

    //左键移动窗口
    //event->pos()          鼠标位置 相对于窗口的位置
    //event->globalPos();   鼠标位置 相对于电脑的位置
    //this->pos()           窗口当前位置 相对于电脑位置
    if(event->button() == Qt::LeftButton)
    {
        //鼠标按下的时候 算出当前位置与窗口位置的坐标差
        delPoint = event->globalPos() - this->pos();
    }
}

//鼠标左键按下后的移动 设置窗口新位置
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    //鼠标偏移后 通过新的鼠标位置和距离偏差delPoint算出新的窗口位置,并移到到新位置
    this->move(event->globalPos() - delPoint);
}

void Widget::on_btnDev_clicked()
{
    if(contrlUtil){
        contrlUtil->setGeometry(450,160,contrlUtil->width(),contrlUtil->height());
        contrlUtil->show();
    }
    if(themeWidget) themeWidget->hide();
    if(camWidget) camWidget->hide();

}

void Widget::on_btnCam_clicked()
{
    camWidget->setGeometry(110,10,camWidget->width(),camWidget->height());
    camWidget->show();

    if(contrlUtil) contrlUtil->hide();
    if(themeWidget) themeWidget->hide();

}

void Widget::on_btnHome_clicked()
{
    //页面的显示和遮挡，最好用单例模式，这里就简单的控制，不太友好
    if(contrlUtil ) contrlUtil->hide();
    if(themeWidget) themeWidget->hide();
    if(camWidget  ) camWidget->hide();
}

void Widget::lightCon(bool value)
{
//    if(server->isRunning() == false)
//        return;

    qDebug() << "clicked: " << value;

    QWidget *contrl = ui->widgetCen02->findChild<QWidget*>("widgetTop");
    QWidget *contrlLight = contrl->findChild<QWidget*>("widgetLight");
    QPushButton *btn = contrlLight->findChild<QPushButton *>("btnLight_2");
    if(value)
    {
        server->sendMsg("openlight");
        contrlLight->setStyleSheet("background-color: rgb(200, 200, 200)");
        btn->setStyleSheet("border-image: url(:/con/contrl/dd2.png);");
        SerialPortUtil::lightCon();
    }
    else
    {
        server->sendMsg("closelight");
        btn->setStyleSheet("border-image: url(:/con/contrl/dd1.png);");
        setBtnColor(contrlLight);
        SerialPortUtil::lightCon();
    }
}

//主题相关API
void Widget::on_btnTheme_clicked()
{
    themeWidget->setGeometry(110,10,themeWidget->width(),themeWidget->height());
    themeWidget->show();
    connect(themeWidget,&CustonTheme::onThemeBlueSet,this,[=](){
        Mycolor::setThemeIndex(BLUE);
        ColorSingleton::getInstance().setColor(Mycolor::blueColorList.at(4));
        setTheme();
    });
    connect(themeWidget,&CustonTheme::onThemeBlackSet,this,[=](){
        Mycolor::setThemeIndex(BLACK);
        ColorSingleton::getInstance().setColor(Mycolor::blackColorList.at(4));
        setTheme();
    });
    connect(themeWidget,&CustonTheme::onThemePinkSet,this,[=](){
        Mycolor::setThemeIndex(PINK);
        ColorSingleton::getInstance().setColor(Mycolor::pinkColorList.at(4));
        setTheme();
    });
    connect(themeWidget,&CustonTheme::onThemeGreenSet,this,[=](){
        Mycolor::setThemeIndex(GREEN);
        ColorSingleton::getInstance().setColor(Mycolor::greenColorList.at(4));
        setTheme();
    });

    if(contrlUtil ) contrlUtil->hide();
    if(camWidget) camWidget->hide();

}

//主题相关API
void Widget::shanCon(bool value)
{
//    if(server->isRunning() == false)
//        return;

    QWidget *contrl = ui->widgetCen02->findChild<QWidget*>("widgetTop");
    QWidget *widWind = contrl->findChild<QWidget*>("widgetWind");
    QPushButton *btnShan = widWind->findChild<QPushButton *>("btnShan");
    if(value){
        server->sendMsg("openshan");
        widWind->setStyleSheet("background-color: rgb(200, 200, 200)");
        btnShan->setStyleSheet("border-image: url(:/con/contrl/fs2.png);");
    }else{
        server->sendMsg("closeshan");
        btnShan->setStyleSheet("border-image: url(:/con/contrl/fs1.png);");
        setBtnColor(widWind);
    }
}

//主题相关API
void Widget::airCon(bool value)
{
//    if(server->isRunning() == false)
//        return;

    QWidget *contrl = ui->widgetCen02->findChild<QWidget*>("widgetTop");
    QWidget *widAir = contrl->findChild<QWidget*>("widgetAir");
    QPushButton *btnAir = widAir->findChild<QPushButton *>("btnAir");
    if(value)
    {
        m_airFlag = true;
        widAir->setStyleSheet("background-color: rgb(200, 200, 200)");
        btnAir->setStyleSheet("border-image: url(:/con/contrl/kt2.png);");

    }else{
        m_airFlag = false;
        btnAir->setStyleSheet("border-image: url(:/con/contrl/kt1.png);");
        setBtnColor(widAir);
    }

    static int data = 0;
    data++;
    if(data % 2)
        server->sendMsg("open air");
    else
        server->sendMsg("close air");
}

void Widget::winModeCon()
{
//    if(server->isRunning() == false || m_airFlag == false)
//        return;

    static int mode = 1;
    //qDebug() << "mode: " << mode;

    QWidget *contrl = ui->widgetCen02->findChild<QWidget*>("widgetTop");
    QWidget *widAir = contrl->findChild<QWidget*>("widgetAir");

    QLabel *labelAuto = widAir->findChild<QLabel*>("labelAuto");
    QLabel *labelCold = widAir->findChild<QLabel*>("labelCold");
    QLabel *labelHu = widAir->findChild<QLabel*>("labelHu");
    QLabel *labelSendWind = widAir->findChild<QLabel*>("labelSendWind");
    switch (mode)
    {
    case 0:
        //labeAuto->setStyleSheet("background-color: rgb(200, 200, 200)");
        labelSendWind->setStyleSheet("border-image: url(:/con/contrl/sf-c.png);");
        labelAuto->setStyleSheet("border-image: url(:/con/contrl/zd-o.png);");
        break;

    case 1:
        labelAuto->setStyleSheet("border-image: url(:/con/contrl/zd-c.png);");
        labelCold->setStyleSheet("border-image: url(:/con/contrl/zl-o.png);");
        break;
    case 2:
        labelCold->setStyleSheet("border-image: url(:/con/contrl/zl-c.png);");
        labelHu->setStyleSheet("border-image: url(:/con/contrl/cs-o.png);");
        break;

    case 3:
        labelHu->setStyleSheet("border-image: url(:/con/contrl/cs-c.png);");
        labelSendWind->setStyleSheet("border-image: url(:/con/contrl/sf-o.png);");
        break;
    default:
        break;
    }
    mode = (mode + 1) % 4;
}

void Widget::winStyleCon()
{
//    if(server->isRunning() == false || m_airFlag == false)
//        return;

    static int style = 3;
    //qDebug() << "style: " << style;

    QWidget *contrl = ui->widgetCen02->findChild<QWidget*>("widgetTop");
    QWidget *widAir = contrl->findChild<QWidget*>("widgetAir");

    QLabel *labelSmallWind = widAir->findChild<QLabel*>("labelSmallWind");
    QLabel *labelLittleWind = widAir->findChild<QLabel*>("labelLittleWind");
    QLabel *labelBigWind = widAir->findChild<QLabel*>("labelBigWind");
    QLabel *labelSuperWind = widAir->findChild<QLabel*>("labelSuperWind");
    switch (style)
    {
    case 0:
        labelSuperWind->setStyleSheet("border-image: url(:/con/contrl/mf-c.png);");
        labelSmallWind->setStyleSheet("border-image: url(:/con/contrl/xf-o.png);");
        break;
    case 1:
        labelSmallWind->setStyleSheet("border-image: url(:/con/contrl/xf-c.png);");
        labelLittleWind->setStyleSheet("border-image: url(:/con/contrl/wf-o.png);");
        break;
    case 2:
        labelLittleWind->setStyleSheet("border-image: url(:/con/contrl/wf-c.png);");
        labelBigWind->setStyleSheet("border-image: url(:/con/contrl/df-o.png);");
        break;
    case 3:
        labelBigWind->setStyleSheet("border-image: url(:/con/contrl/df-c.png);");
        labelSuperWind->setStyleSheet("border-image: url(:/con/contrl/mf-o.png);");
        break;
    default:
        break;
    }
    style = (style + 1) % 4;
}

//主题相关API
void Widget::setTheme()
{
    QStringList colors;
    switch (Mycolor::myTheme) {
    case BLACK:
        colors = Mycolor::blackColorList;
        break;
    case BLUE:
        colors = Mycolor::blueColorList;
        break;
    case PINK:
        colors = Mycolor::pinkColorList;
        break;
    case GREEN:
        colors = Mycolor::greenColorList;
        break;
    }

    ui->widgetALL->setStyleSheet(colors.at(1));
    ui->widgetCenter->setStyleSheet(colors.at(2));

    QWidget *contrl = ui->widgetCen02->findChild<QWidget*>("widgetTop");
    contrl->setStyleSheet(colors.at(3));
    contrl->findChild<QWidget*>("widgetLight")->setStyleSheet(colors.at(4));
    contrl->findChild<QWidget*>("widgetWind")->setStyleSheet(colors.at(4));
    contrl->findChild<QWidget*>("widgetAir")->setStyleSheet(colors.at(4));

    QWidget *wea = ui->widgetWea->findChild<QWidget*>("widgetTop");
    wea->setStyleSheet(colors.at(3));
    wea->findChild<QWidget*>("widgetdays")->setStyleSheet(colors.at(4));

    QWidget *music = ui->widgetMusic->findChild<QWidget*>("widgetTop");
    music->setStyleSheet(colors.at(3));

    QWidget *cur = ui->widgetCur->findChild<QWidget*>("widgetTop");
    cur->setStyleSheet(colors.at(3));

    on_btnHome_clicked();
    ui->btnHome->setChecked(true);

}
//主题相关API
void Widget::setBtnColor(QWidget *btn)
{
    switch (Mycolor::myTheme) {
    case BLACK:
        btn->setStyleSheet(Mycolor::blackColorList.at(4));
        break;
    case BLUE:
        btn->setStyleSheet(Mycolor::blueColorList.at(4));
        break;
    case PINK:
        btn->setStyleSheet(Mycolor::pinkColorList.at(4));
        break;
    case GREEN:
        btn->setStyleSheet(Mycolor::greenColorList.at(4));
        break;
    }
}

void Widget::drawDataLine()
{
//    QPainter painter(ui->widget0301);
//    // 定义温度数据10个点
//    QPoint points[10];

//    int preX = ui->widget0301->width() / 10;
//    for(int i = 0; i < 10; i++)
//    {
//        points[i].setX(preX * (i+1));
//        points[i].setY(ui->widget0301->height() / 2);
//        painter.drawEllipse(QPoint(points[i]),3,3);
//    }
//    for(int i = 0; i < 9; i++)
//        painter.drawLine(points[i],points[i+1]);

//    // 绘制温湿度、光照
//    painter.setPen(Qt::yellow);

//    painter.drawLine(QPoint(10,10),QPoint(20,20));
}

void Widget::on_btnVideo_clicked()
{
    static int i = 0;

    if(i == 0)
    {
        ui->btnVideo->setStyleSheet("");
        player->setVideoOutput(videoWidget);
        //static QString filePath = "D:/qtProject/00_QT_CLC/smartHome/video/test3.mp4";
        player->setMedia(QUrl::fromLocalFile(filePath));
        player->play();
    }
    else
    {
        if(i % 2 == 1)
            player->pause();
        else
            player->play();
    }
    i++;
}
