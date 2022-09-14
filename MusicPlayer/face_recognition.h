#ifndef FACE_RECOGNITION_H
#define FACE_RECOGNITION_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QIcon>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QRect>
#include <QTime>
#include <QTimer>
#include<QCamera>
#include<QSettings>
#include <QMouseEvent>
#include <QPainter>
#include "main_page.h"

namespace Ui {
class FaceRecognition;
}

class FaceRecognition : public QWidget
{
    Q_OBJECT

public:
    explicit FaceRecognition(QWidget *parent = nullptr);
    ~FaceRecognition();

    struct Imag
    {
        QImage img;//图片
        QRect rect;//人脸识别矩形标记
        int age;//人脸信息年龄
        int gender;//人脸信息性别
    };


    //==================变量=====================
    //点坐标
    QPoint z;
    //人脸库结构体
    Imag faceBase;
    //实时人脸截图结构体
    Imag currentFace;

    MainPage *mainPage;
    QTimer *faceRecognitionTimer = NULL;
    //实例化摄像头对象
    QCamera *camera  = NULL;
    QCameraViewfinder *viewfinder  = NULL;
    QCameraImageCapture *imageCapture  = NULL;
    //软件配置信息，用于临时登录信息传递
    QSettings *faceRecognitionConfig= NULL;

    //=========================函数==============
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    //绘图事件
    void paintEvent(QPaintEvent *event);
    //事件过滤器
    bool eventFilter(QObject *obj, QEvent *event);
    //初始化连接信号 槽函数
    void initSLot();
    //封装SDK整个调用过程用到的接口函数
    bool doCompare(Imag& img_photo, Imag& img_idcard, float val);

protected slots:
    //开始识别
    void onCompare();
    //人脸识别初始化定时器，摄像头，截图方法
    void faceRecognitionLogin();

signals:


private:
    Ui::FaceRecognition *ui;
};

#endif // FACE_RECOGNITION_H
