#ifndef CAM_H
#define CAM_H
//#include <QWebEngineSettings>
//#include <QWebEngineView>
#include <QWidget>

namespace Ui {
class Cam;
}

class Cam : public QWidget
{
    Q_OBJECT

public:
    explicit Cam(QWidget *parent = nullptr);
    ~Cam();

private:
    Ui::Cam *ui;
     //QWebEngineView *view;
};

#endif // CAM_H
