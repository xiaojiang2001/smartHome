#include "cam.h"
#include "ui_cam.h"

#include <QVBoxLayout>
//#include <QWebEngineSettings>
//#include "mywebenginepage.h"

Cam::Cam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cam)
{
    ui->setupUi(this);

//    //创建 QWebEngineView
//    QWebEngineView *view = new QWebEngineView(ui->widgetWeb);
//    view->setGeometry(222,30,650,500);
//    QWebEngineSettings *settings = view->settings();
//    settings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
//    settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
//    view->setPage(new MyWebEnginePage);
//    view->load(QUrl("https://v.ifeng.com/c/8WUeT1Pv0go"));

//    connect(view, &QWebEngineView::loadFinished, [view](bool) {
//        QString script = "document.body.style.backgroundColor = 'rgb(44, 46, 67)';";

//        view->page()->runJavaScript(script);
//    });
}

Cam::~Cam()
{
    delete ui;
}

