#include "register_face.h"
#include "ui_register_face.h"

RegisterFace::RegisterFace(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterFace)
{
    ui->setupUi(this);

    //取消标题栏
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->close->setAttribute(Qt::WA_Hover,true);//开启悬停事件
    ui->close->installEventFilter(this);       //安装事件过滤器
    ui->captureBtn->setAttribute(Qt::WA_Hover,true);//开启悬停事件
    ui->captureBtn->installEventFilter(this);       //安装事件过滤器
    ui->confirm->setAttribute(Qt::WA_Hover,true);//开启悬停事件
    ui->confirm->installEventFilter(this);       //安装事件过滤器

    //初始化软件运行目录，新建用于保存人脸注册图片的路径

    QString dirName1 = "D:/MusicPlayer/";
    QDir dir1(dirName1);
    if(!dir1.exists())
    {
        dir1.mkdir(dirName1);
    }

    QString dirName2 = "D:/MusicPlayer/FaceId/";
    QDir dir2(dirName2);
    if(!dir2.exists())
    {
        dir2.mkdir(dirName2);
    }

    camera = new QCamera(); // 摄像头对象
    viewfinder = new QCameraViewfinder(this); // 用于实时显示摄像头图像
    imageCapture = new QCameraImageCapture(camera); // 用于截取摄像头图像
    camera->setViewfinder(viewfinder); // camera 使用 viewfinder 实时显示图像

    viewfinder->move(5,50);
    viewfinder->setFixedSize(400,400);

    camera->start();

    // 点击拍照
    QObject::connect(ui->captureBtn, &QPushButton::clicked, [=] {
        imageCapture->capture("capture.jpg"); // 如果不传入截图文件的路径，则会使用默认的路径，每次截图生成一个图片
    });
    // 拍照完成后的槽函数，可以把 image 保存到自己想要的位置
    QObject::connect(imageCapture, &QCameraImageCapture::imageCaptured, [=](int id, const QImage &image) {
        Q_UNUSED(id)
        faceImg = image;

        QImage scaledImage = image.scaled(ui->preview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->preview->setPixmap(QPixmap::fromImage(scaledImage));
    });

    connect(ui->confirm, &QPushButton::clicked, this, [=]{
        if(!ui->lineEdit->text().isEmpty() && !faceImg.isNull()){
            tmpText = ui->lineEdit->text();
            faceImg.save("D:/MusicPlayer/FaceId/" + tmpText +".jpg");
            this->close();
            camera->stop();
        }else{
            QMessageBox musicPlayMsgBox;
            musicPlayMsgBox.setText("请输入账户名和拍照人脸");
            musicPlayMsgBox.setStandardButtons(QMessageBox::Ok);
            musicPlayMsgBox.setButtonText(QMessageBox::Ok,"好的");
            musicPlayMsgBox.setDefaultButton(QMessageBox::Ok);
            int ret = musicPlayMsgBox.exec();
            switch (ret) {
            case QMessageBox::Ok:
                break;
            default:
                break;
            }
        }
    });
    connect(ui->close, &QPushButton::clicked, this, [=]{
        this->close();
        camera->stop();
    });
}

RegisterFace::~RegisterFace()
{
    delete ui;
}

//鼠标移动事件
void RegisterFace::mouseMoveEvent(QMouseEvent *event)
{
    QPoint y = event->globalPos();
    QPoint x = y - z;
    move(x);
}
//鼠标按下事件
void RegisterFace::mousePressEvent(QMouseEvent *event)
{
    QPoint y = event->globalPos();
    QPoint x = this->geometry().topLeft();
    this->z = y - x;
}
//绘图事件
void RegisterFace::paintEvent(QPaintEvent *)
{
    QPainter p1(this);
    QPixmap pix;
    pix.load(":/res/blue.jpg");
    p1.drawPixmap(0,0,this->width(),this->height(),pix);

}
//事件过滤器
bool RegisterFace::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->close) {
        if(event->type() == QEvent::HoverEnter) {
            ui->close->setIcon(QIcon(":/res/close_on.png"));//鼠标悬停改变按钮颜色
            return true;
        }
    }
    if(obj == ui->close) {
        if(event->type() == QEvent::HoverLeave) {
            ui->close->setIcon(QIcon(":/res/close.png"));//鼠标离开恢复原始按钮
            return true;
        }
    }
    if(obj == ui->captureBtn) {
        if(event->type() == QEvent::HoverEnter) {
            ui->captureBtn->setIcon(QIcon(":/res/registerFaceCapture_on.png"));//鼠标悬停改变按钮颜色
            return true;
        }
    }
    if(obj == ui->captureBtn) {
        if(event->type() == QEvent::HoverLeave) {
            ui->captureBtn->setIcon(QIcon(":/res/registerFaceCapture.png"));//鼠标离开恢复原始按钮
            return true;
        }
    }
    if(obj == ui->confirm) {
        if(event->type() == QEvent::HoverEnter) {
            ui->confirm->setIcon(QIcon(":/res/addFacePageBtn_on.png"));//鼠标悬停改变按钮颜色
            return true;
        }
    }
    if(obj == ui->confirm) {
        if(event->type() == QEvent::HoverLeave) {
            ui->confirm->setIcon(QIcon(":/res/addFacePageBtn.png"));//鼠标离开恢复原始按钮
            return true;
        }
    }
    return QWidget::eventFilter(obj,event);
}
