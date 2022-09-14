#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include<QSettings>
#include "main_page.h"
#include "face_recognition.h"
#include "register_face.h"
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    //==================变量=====================
    //点坐标
    QPoint z;
    //MainPage类对象
    MainPage *mainPage;
    //FaceRecognition类对象
    FaceRecognition *faceRecognitionPage;
    //RegisterFace类对象
    RegisterFace *registerFacePage;
    //软件配置信息，用于临时登录信息传递
    QSettings *configIniWrite;

    //=================函数========================
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    //绘图事件
    void paintEvent(QPaintEvent *event);
    //事件过滤器
    bool eventFilter(QObject *obj, QEvent *event);



private:
    Ui::Login *ui;
};

#endif // LOGIN_H
