#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);

signals:

private:
//    QMediaPlayer *player;
//    QMediaPlaylist *playerList;
//    QVideoWidget *videoWidget;

};

#endif // VIDEOWIDGET_H
