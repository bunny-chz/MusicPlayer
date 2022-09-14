#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    //取消标题栏

    this->setWindowFlags(Qt::FramelessWindowHint);

    this->setStyleSheet("QWidget{border-top-left-radius:4px;border-top-right-radius:4px;}");

    ui->close->setAttribute(Qt::WA_Hover,true);//开启悬停事件
    ui->close->installEventFilter(this);       //安装事件过滤器
    ui->minimize->setAttribute(Qt::WA_Hover,true);//开启悬停事件
    ui->minimize->installEventFilter(this);       //安装事件过滤器
    ui->faceRecognitionBtn->setAttribute(Qt::WA_Hover,true);//开启悬停事件
    ui->faceRecognitionBtn->installEventFilter(this);       //安装事件过滤器
    ui->addFace->setAttribute(Qt::WA_Hover,true);//开启悬停事件
    ui->addFace->installEventFilter(this);       //安装事件过滤器

    //初始化配置信息文件
    configIniWrite = new QSettings("id.ini", QSettings::IniFormat);

    //关闭窗口
    connect(ui->close,&QToolButton::clicked,this,&Login::close);
    //最小化
    connect(ui->minimize,&QToolButton::clicked,this,[=]{
        this->showMinimized();
    });

    connect(ui->pushButton,&QPushButton::clicked,this,[=](){

        if(!ui->passwd->text().isEmpty() && !ui->id->text().isEmpty()){
            if(ui->id->text() == "chz" && ui->passwd->text() == "123")
            {
                this->mainPage = new MainPage();
                mainPage->show();
                this->close();
            }else{
                QMessageBox musicPlayMsgBox;
                musicPlayMsgBox.setText("账户名或密码错误！！！");
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
        }else{
            QMessageBox musicPlayMsgBox;
            musicPlayMsgBox.setText("请输入完全账户名和密码！");
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

    connect(ui->faceRecognitionBtn,&QPushButton::clicked,this,[=](){
        if(!ui->id->text().isEmpty()){
            configIniWrite->setValue("id",ui->id->text());
            this->faceRecognitionPage = new FaceRecognition();
            this->close();
            faceRecognitionPage->show();
        }else{
            QMessageBox musicPlayMsgBox;
            musicPlayMsgBox.setText("请输入人脸注册的账号名，才能进行人脸识别！");
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

    connect(ui->addFace,&QPushButton::clicked,this,[=](){
        this->registerFacePage = new RegisterFace();
        registerFacePage->show();
    });
}

Login::~Login()
{
    delete ui;
    configIniWrite->clear();
}

//事件过滤器
bool Login::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->close) {
        if(event->type() == QEvent::HoverEnter) {
            ui->close->setIcon(QIcon(":/res/close_on.png"));//鼠标悬停改变按钮颜色
            return true;
        }
    }
    if(obj == ui->minimize) {
        if(event->type() == QEvent::HoverEnter) {
            ui->minimize->setIcon(QIcon(":/res/minimize_on.png"));
            return true;
        }
    }
    if(obj == ui->close) {
        if(event->type() == QEvent::HoverLeave) {
            ui->close->setIcon(QIcon(":/res/close.png"));//鼠标离开恢复原始按钮
            return true;
        }
    }
    if(obj == ui->minimize) {
        if(event->type() == QEvent::HoverLeave) {
            ui->minimize->setIcon(QIcon(":/res/minimize.png"));
            return true;
        }
    }
    if(obj == ui->faceRecognitionBtn) {
        if(event->type() == QEvent::HoverEnter) {
            ui->faceRecognitionBtn->setIcon(QIcon(":/res/faceRecognition_on.png"));
            return true;
        }
    }
    if(obj == ui->faceRecognitionBtn) {
        if(event->type() == QEvent::HoverLeave) {
            ui->faceRecognitionBtn->setIcon(QIcon(":/res/faceRecognition.png"));//鼠标离开恢复原始按钮
            return true;
        }
    }
    if(obj == ui->addFace) {
        if(event->type() == QEvent::HoverEnter) {
            ui->addFace->setIcon(QIcon(":/res/faceAdd_on.png"));
            return true;
        }
    }
    if(obj == ui->addFace) {
        if(event->type() == QEvent::HoverLeave) {
            ui->addFace->setIcon(QIcon(":/res/faceAdd.png"));//鼠标离开恢复原始按钮
            return true;
        }
    }
    return QWidget::eventFilter(obj,event);
}
//鼠标移动事件
void Login::mouseMoveEvent(QMouseEvent *event)
{
    QPoint y = event->globalPos();
    QPoint x = y - z;
    move(x);
}
//鼠标按下事件
void Login::mousePressEvent(QMouseEvent *event)
{
    QPoint y = event->globalPos();
    QPoint x = this->geometry().topLeft();
    this->z = y - x;
}
//绘图事件
void Login::paintEvent(QPaintEvent *event)
{
    QPainter p1(this);
    QPixmap pix;
    pix.load(":/res/blue.jpg");
    p1.drawPixmap(0,0,this->width(),this->height(),pix);

}
