#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QWidget>
#include <qtcpsocket.h>

class TcpServer : public QWidget
{
    Q_OBJECT
public:
    explicit TcpServer(QWidget *parent = nullptr);
    void initServer();
    void sendMsg(QString);
    bool isRunning();

private:
    QTcpSocket *clientSocket;     //通信套接字
    QTimer *timer;                //连接定时器
    QByteArray revData;           // 接收到的数据
    bool m_isRun;

signals:
    void sendRevData(QString);

private slots:
    void on_connect();              // 接收到新的连接
    void on_timer_out();            //连接失败
    void on_readyRead_handler();    // 接收数据槽函数
};

#endif // TCPSERVER_H
