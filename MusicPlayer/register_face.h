#ifndef REGISTER_FACE_H
#define REGISTER_FACE_H

#include <QWidget>
#include <QDebug>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QImage>
#include <QPixmap>
#include<QPushButton>
#include<QDir>
#include<QMessageBox>
#include <QMouseEvent>
#include <QPainter>

namespace Ui {
class RegisterFace;
}

class RegisterFace : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterFace(QWidget *parent = nullptr);
    ~RegisterFace();

    //==================变量=====================
    //点坐标
    QPoint z;
    //实例化摄像头对象
    QCamera *camera  = NULL;
    QCameraViewfinder *viewfinder  = NULL;
    QCameraImageCapture *imageCapture  = NULL;
    QImage faceImg;
    QString tmpText;


    //=========================函数==============
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    //绘图事件
    void paintEvent(QPaintEvent *event);
    //事件过滤器
    bool eventFilter(QObject *obj, QEvent *event);


private:
    Ui::RegisterFace *ui;
};

#endif // REGISTER_FACE_H
