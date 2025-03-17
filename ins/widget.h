#ifndef WIDGET_H
#define WIDGET_H

#include <QButtonGroup>
#include <QMap>
#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QMenu>
#include "contrlutils.h"
#include "custontheme.h"
#include "mycolor.h"
#include "cam.h"
#include "tcpserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watered, QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void on_btnDev_clicked();
    void on_btnHome_clicked();
    void on_btnTheme_clicked();
    void lightCon(bool);
    void shanCon(bool);
    void airCon(bool);
    void winModeCon();
    void winStyleCon();

    void on_btnCam_clicked();

    void on_btnVideo_clicked();

private:
    Ui::Widget *ui;
    QMenu *quitMenu;
    QPoint delPoint;
    QWidget *widgetCover = nullptr;          // 设置主题界面窗口
    ContrlUtils *contrlUtil = nullptr;
    QButtonGroup *buttonGroup;
    CustonTheme *themeWidget;
    Cam *camWidget;
    TcpServer *server;

    QMediaPlayer *player;
    QMediaPlaylist *playerList;
    QVideoWidget *videoWidget;

    void setTheme();
    void setBtnColor(QWidget *btn);
    void showCover();
    void drawDataLine(void);

    bool m_airFlag;
};
#endif // WIDGET_H
