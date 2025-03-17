#ifndef MUSICUTILS_H
#define MUSICUTILS_H

#include <QMediaPlayer>
#include <QPushButton>
#include <QSlider>

#include <QWidget>

namespace Ui {
class MusicUtils;
}

class MusicUtils : public QWidget
{
    Q_OBJECT

public:
    explicit MusicUtils(QWidget *parent = nullptr);

    ~MusicUtils();

private slots:
    void on_btnplaypause_clicked(bool checked);

    void on_btnPre_clicked();
    void updateProgress(qint64 position);
    void on_btnNext_clicked();

    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::MusicUtils *ui;
    QMediaPlayer *player;
    QStringList playlist;
    int currentIndex;
};

#endif // MUSICUTILS_H
