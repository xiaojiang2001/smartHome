#include "ImageSlider.h"
#include <QPainter>

ImageSlider::ImageSlider(QWidget *parent)
    : QWidget(parent), currentImageIndex(0), nextImageIndex(1), opacity(1.0), displayDuration(3000)
{
 /*
    // 设置显示时间为3秒
    changeTimer = new QTimer(this);
    connect(changeTimer, &QTimer::timeout, this, &ImageSlider::startFadeOut);

    fadeInAnimation = new QPropertyAnimation(this, "opacity");
    fadeInAnimation->setDuration(1000);         // 淡入动画持续时间为 1 秒
    fadeInAnimation->setStartValue(0.0);
    fadeInAnimation->setEndValue(1.0);
    connect(fadeInAnimation, &QPropertyAnimation::finished, this, [this]() {
        changeTimer->start(displayDuration);    // 淡入完成后，开始计时显示持续时间
    });

    fadeOutAnimation = new QPropertyAnimation(this, "opacity");
    fadeOutAnimation->setDuration(1000);        // 淡出动画持续时间为 1 秒
    fadeOutAnimation->setStartValue(1.0);
    fadeOutAnimation->setEndValue(0.0);
    connect(fadeOutAnimation, &QPropertyAnimation::finished, this, &ImageSlider::nextImage);

    // 初始立即开始淡入动画
    fadeInAnimation->start();
*/
}


void ImageSlider::setImages(const QStringList &imagePaths) {
    imagePixmaps.clear();
    for (const QString &path : imagePaths) {
        QPixmap pixmap;
        if (pixmap.load(path)) {
            imagePixmaps.append(pixmap);
        }
    }
    if (!imagePixmaps.isEmpty()) {
        currentImageIndex = 0;
        nextImageIndex = 1 % imagePixmaps.size();
        update();
    }
}

void ImageSlider::setChangeInterval(int msec) {
    changeTimer->start(msec); // 启动定时器
}

void ImageSlider::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setOpacity(opacity); // 设置透明度

    if (!imagePixmaps.isEmpty()) {
        if (currentImageIndex < imagePixmaps.size()) {
            // 绘制当前图片
            painter.drawPixmap(rect(), imagePixmaps[currentImageIndex]);
        }
        if (fadeInAnimation->state() == QPropertyAnimation::Running &&
                nextImageIndex < imagePixmaps.size()) {
            // 在淡入动画进行时绘制下一张图片
            painter.setOpacity(1.0 - opacity); // 设置透明度以产生淡入效果
            painter.drawPixmap(rect(), imagePixmaps[nextImageIndex]);
        }
    }
}
void ImageSlider::nextImage() {
    currentImageIndex = nextImageIndex;
    nextImageIndex = (nextImageIndex + 1) % imagePixmaps.size();
    fadeInAnimation->start(); // 立即开始淡入动画
}


void ImageSlider::startFadeIn() {
    fadeInAnimation->start(); // 开始淡入动画
}

void ImageSlider::startFadeOut() {
    if (fadeOutAnimation->state() == QPropertyAnimation::Running) {
        return; // 如果淡出动画已经在运行，则不重复启动
    }

    currentImageIndex = nextImageIndex;
    nextImageIndex = (nextImageIndex + 1) % imagePixmaps.size();
    fadeOutAnimation->start(); // 开始淡出动画
}
qreal ImageSlider::getOpacity() const {
    return opacity;
}

void ImageSlider::setOpacity(qreal value) {
    opacity = value;
    update(); // 更新窗口以应用新的透明度
}
