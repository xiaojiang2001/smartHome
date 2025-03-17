#include "tcpserver.h"
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <QTimer>

TcpServer::TcpServer(QWidget *parent) : QWidget(parent)
{
    m_isRun = false;

    //实例化客户端对象
    clientSocket = new QTcpSocket(this);
    timer = new QTimer(this);
    timer->setSingleShot(true);     //触发一次
    timer->setInterval(5000);       //设置超时时间 3s

    //initServer();
}

void TcpServer::initServer()
{
    qDebug() << "***********************in initServer";
    // 设置服务器IP地址和端口s号变量 启动监听
    // QT:    172.20.10.14
    // Android:  172.20.10.3
//    QHostAddress addr("172.27.160.1");
    QHostAddress addr("172.20.10.13");
    quint16 port = 8082;


    // 连接服务器
    clientSocket->connectToHost(addr, port);

   //绑定监听客户端接入的信号与槽函数
   connect(clientSocket, SIGNAL(connected()), this, SLOT(on_connect()));   //连接成功
   connect(timer, SIGNAL(timeout()),   this, SLOT(on_timer_out())); //连接失败

   //启动定时器
   timer->start();
   qDebug() << "***************启动连接";
}

void TcpServer::sendMsg(QString msg)
{
    clientSocket->write(msg.toStdString().c_str());
}

bool TcpServer::isRunning()
{
    return m_isRun;
}

void TcpServer::on_connect()
{
    QMessageBox::information(this, "成功", "网络连接成功");
    timer->stop();
    m_isRun = true;
    qDebug() << "***********************连接服务器成功******************";
    //绑定接收槽函数
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(on_readyRead_handler()));
}

void TcpServer::on_timer_out()
{
    QMessageBox::critical(this, "错误", "网络连接超时");
    qDebug() << "**********************连接超时************************";
    clientSocket->abort();        //中止客户端连接
}


void TcpServer::on_readyRead_handler()
{
    //因为数据由客户端的QTcpSocket进行发送，以此需要通过 `qobject_cast<>(sender())` 来获取信号发送方
    clientSocket = qobject_cast<QTcpSocket *>(sender());
    revData = clientSocket->readAll();
    qDebug() << revData;
    emit sendRevData(revData);
}
