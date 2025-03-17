#ifndef IMAGESLIDER_H
#define IMAGESLIDER_H

#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <QPropertyAnimation>

class ImageSlider : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity) // 用于动画的透明度属性

public:
    explicit ImageSlider(QWidget *parent = nullptr);
    void setImages(const QStringList &imagePaths); // 设置图片路径
    void setChangeInterval(int msec); // 设置图片切换间隔

protected:
    void paintEvent(QPaintEvent *event) override; // 重写绘图事件

private slots:
    void nextImage(); // 切换到下一张图片
    void startFadeIn(); // 开始淡入动画
    void startFadeOut(); // 开始淡出动画

private:
    QList<QPixmap> imagePixmaps; // 存储加载的图片
    int currentImageIndex; // 当前图片索引
    int nextImageIndex; // 下一张图片索引
    QTimer *changeTimer; // 定时器，用于触发图片切换
    qreal opacity; // 当前图片的透明度
    QPropertyAnimation *fadeInAnimation; // 淡入动画
    QPropertyAnimation *fadeOutAnimation; // 淡出动画
    int displayDuration; // 单位：毫秒，图片显示的持续时间

    qreal getOpacity() const;       // 获取当前透明度
    void setOpacity(qreal value);   // 设置透明度
};

#endif // IMAGESLIDER_H
