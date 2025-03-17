#include "musicutils.h"
#include "ui_musicutils.h"
#include "global.h"

#include <QMediaMetaData>

MusicUtils::MusicUtils(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicUtils)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    currentIndex = 0;

    // 加入歌曲
//    QString song1Path = "E:/smartHome/songs/wake.mp3";
//    QString song2Path = "E:/smartHome/songs/bye bye bye.mp3";
//    QString song3Path = "E:/smartHome/songs/圈住你.mp3";
//    QString song4Path = "E:/smartHome/songs/LoveStory.mp3";

    playlist.append(song1Path);
    playlist.append(song2Path);
    playlist.append(song3Path);
    playlist.append(song4Path);


    player->setMedia(QUrl::fromLocalFile(playlist[currentIndex]));
    //player->play();

//    if (player->isMetaDataAvailable()) {
//        // 检查元数据
//        qDebug() << "有背景图";
//    }else{
//        qDebug() << "没背景图";
//    }

    connect(player, &QMediaPlayer::metaDataAvailableChanged, [=](bool available) {
        if (available)
        {
            // 获取歌曲名
            QStringList nameList = playlist[currentIndex].split("songs/");
            qDebug() << nameList[1];
            ui->labelSongName->setText(nameList[1]);
        }
    });

    connect(player, &QMediaPlayer::positionChanged, [&](qint64 pos){

        // 更新进度条
        ui->horizontalSlider->setValue(pos);
        int minutes = pos / 1000 / 60;
        int seconds = pos / 1000 % 60;
        ui->labelcurrentTime->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
        ui->labelTotalTime->setText(QString("%1:%2").arg(player->duration()/1000/60,
                                                         2, 10, QChar('0')).arg(player->duration()/1000%60, 2, 10, QChar('0')));

        ui->horizontalSlider->setRange(0, player->duration());

    });
}

MusicUtils::~MusicUtils()
{
    delete ui;
}

void MusicUtils::on_btnplaypause_clicked(bool checked)
{
    Q_UNUSED(checked);

    if (player->state() == QMediaPlayer::PlayingState)
    {
        player->pause();
        ui->btnplaypause->setStyleSheet(
                    "QPushButton{"
                    "    border-image: url(:/music/music/playsong.png);"
                    "}"
                    "QPushButton:hover{"
                    "    border-image: url(:/music/music/playhover.png);"
                    "}"
                    );

    }
    else
    {
        player->play();
        ui->btnplaypause->setStyleSheet(
                    "QPushButton{"
                    "    border-image: url(:/music/music/stop.png);"
                    "}"
                    "QPushButton:hover{"
                    "    border-image: url(:/music/music/stophover.png);"
                    "}"
                    );
    }
}

void MusicUtils::on_btnPre_clicked()
{
    currentIndex = (currentIndex - 1 + playlist.size()) % playlist.size();
    player->setMedia(QUrl::fromLocalFile(playlist[currentIndex]));
    player->play();
    ui->btnplaypause->setStyleSheet(
                "QPushButton{"
                "    border-image: url(:/music/music/stop.png);"
                "}"
                "QPushButton:hover{"
                "    border-image: url(:/music/music/stophover.png);"
                "}"
                );
}

void MusicUtils::updateProgress(qint64 position)
{
    ui->horizontalSlider->setValue(static_cast<int>(position));
}

void MusicUtils::on_btnNext_clicked()
{
    currentIndex = (currentIndex + 1) % playlist.size();
    player->setMedia(QUrl::fromLocalFile(playlist[currentIndex]));
    player->play();
    ui->btnplaypause->setStyleSheet(
                "QPushButton{"
                "    border-image: url(:/music/music/stop.png);"
                "}"
                "QPushButton:hover{"
                "    border-image: url(:/music/music/stophover.png);"
                "}"
                );
}

void MusicUtils::on_horizontalSlider_sliderMoved(int position)
{
    qDebug() << "test" << position;
    ui->horizontalSlider->setValue(position);
    int minutes = position / 1000 / 60;
    int seconds = position / 1000 % 60;
    ui->labelcurrentTime->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
    player->setPosition(position);
}


