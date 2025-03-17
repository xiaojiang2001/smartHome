#ifndef COLORSINGLETON_H
#define COLORSINGLETON_H

#include <QColor>


class ColorSingleton {
public:
    static ColorSingleton& getInstance() {
        static ColorSingleton instance;
        return instance;
    }

    void setColor(const QColor &color) { this->color = color; }
    QColor getColor() const { return color; }

    // 禁用拷贝和赋值
    ColorSingleton(const ColorSingleton&) = delete;
    ColorSingleton& operator=(const ColorSingleton&) = delete;

private:
    QColor color;
    ColorSingleton() {} // 私有构造函数
};

#endif // COLORSINGLETON_H
