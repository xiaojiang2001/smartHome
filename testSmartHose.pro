QT       += core gui
QT       += multimedia
QT       += multimediawidgets
QT       += network
QT       += serialport
#QT       += webengine
#QT       += webenginewidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SerialPortUtil.cpp \
    cam.cpp \
    cirtainutils.cpp \
    citycodeutils.cpp \
    contrlutils.cpp \
    custontheme.cpp \
    imageslider.cpp \
    leftcircle.cpp \
    main.cpp \
    musicutils.cpp \
    mybutton.cpp \
    mycolor.cpp \
    tcpserver.cpp \
    videowidget.cpp \
    weatherwdiget.cpp \
    widget.cpp

HEADERS += \
    ColorSingleton.h \
    SerialPortUtil.h \
    cam.h \
    cirtainutils.h \
    citycodeutils.h \
    contrlutils.h \
    custontheme.h \
    global.h \
    imageslider.h \
    leftcircle.h \
    musicutils.h \
    mybutton.h \
    mycolor.h \
    mywebenginepage.h \
    tcpserver.h \
    videowidget.h \
    weatherwdiget.h \
    widget.h


FORMS += \
    cam.ui \
    cirtainutils.ui \
    contrlutils.ui \
    custontheme.ui \
    musicutils.ui \
    weatherwdiget.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    cityCode.qrc \
    res.qrc

DISTFILES +=
