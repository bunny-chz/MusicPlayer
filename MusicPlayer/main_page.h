#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include<QDebug>
#include<QToolButton>
#include <QtMultimedia>
#include<QFileDialog>
#include<QPainter>
#include<QPoint>
#include<QMessageBox>
#include<QAbstractSlider>
#include<QTimer>
#include <QMainWindow>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMainWindow>
#include <QDebug>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QImage>
#include <QPixmap>
#include <QVBoxLayout>
#include <QPushButton>
#include<QLabel>
#include <QMainWindow>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QTime>
#include <QSettings>
#include <QMessageBox>
#include <QDateTime>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextCodec>
#include <QJsonArray>
#include<QByteArray>
#include<QBuffer>
#include<QImage>
#include<QTimer>
#include<QObject>
#include<QEvent>
#include<QMouseEvent>
#include<QListWidget>
#include<QStackedWidget>
#include<QListWidgetItem>
#include<QMediaContent>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>
#include <QMediaPlayer>
#include <QObject>
#include<QStringList>
#include<QFileDialog>
#include<QWidget>
#include<QSize>
#include<QVector>
#include<QDir>
#include<QPalette>
#include"jsoninfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainPage; }
QT_END_NAMESPACE

//定义歌曲文件信息结构体，歌曲文件本身就带有mp3 IDV2信息，用于存储歌手、歌名等信息
typedef struct music_info_st
{
    QString mic_name;           //曲名
    QString mic_songer;         //歌手
    QString mic_time;           //时长
    QString mic_album;          //专辑
    QString mic_path;           //mp3所在路径
    QString pic_flag="0";       //标记位，是否有图片
    QString  pic_path;          //歌曲图片读取后保存的路径
    QString  pic_type;          //图片类型 jpg,png
    QString showlist;           //列表显示
}MUSIC_info;

//mp3 IDV2格式：-> 标签头 + 标签帧1头帧 + 标签帧1内容1 + 标签帧2头帧 + 标签帧2内容 + .... + 最后音乐正式内容
//标签头：开始前10位
typedef struct TAB_info_st
{
    char format[3];           //格式
    char version;             //版本
    char unuse[2];
    char header_size[4];      //标签帧+标签头的size
}TAB_info;
//标签帧头帧：每帧前8位
typedef struct head_info_st
{
    char FrameID[4];    /*用四个字符标识一个帧，说明其内容，稍后有常用的标识对照表*/
    char Size[4];       /*帧内容的大小，不包括帧头，不得小于1*/
    char Flags[2];      /*存放标志，只定义了6位*/
}HEAD_info;


class MainPage : public QWidget
{
    Q_OBJECT

public:

    //==================变量=====================
    MainPage(QWidget *parent = nullptr);
    ~MainPage();
    //实例化一个指针player对象 播放对象
    QMediaPlayer *player = NULL;
    //实例化一个指针playList对象  播放列表对象
    QMediaPlaylist *playList = NULL;
    //实例化一个QPoint对象z,记录鼠标点击处到窗口左上角的点坐标
    QPoint z;
    //实例化进度条更新定时器对象
    QTimer *progressTimer;
    //实例化手势识别摄像头截取图片的定时器对象
    QTimer *gestureRecogTimer;
    //定义字节数组，用于接收摄像头截图转换后的Base64字符串
    QByteArray originImgBase;
    //获取 Access Token时，用于接收返回的Access Token
    QString accessToken;
    //实例化用于网络请求和接收的对象
    QNetworkAccessManager *netAccess;
    //实例化摄像头对象
    QCamera *camera;
    //进度更新的时间
    QString positionTime;
    //歌曲的总时长
    QString durationTime;
    //本地歌单列表
    QListWidget *localList;
    //临时列表，用于获取列表item的位置
    QListWidget *tmpListWidget;
    //stackWidget,用于切换歌单
    QStackedWidget *listStack;
    //结构体 记录当前mp3IDV2格式文件信息
    MUSIC_info m_music_info;


    QMap<int,QString> lrcMap;           //保存歌词
    QByteArray lyricsLine;




    //=========================函数==============
    //重写绘图事件函数
    void paintEvent(QPaintEvent *);
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *);
    //键盘事件，设置快捷键
    void keyPressEvent(QKeyEvent* event);
    //事件过滤器
    bool eventFilter(QObject *obj, QEvent *event);
    //获取AccessToken函数
    void getAccessToken();
    //获取到AccessToken后，开始识别
    void recognition();
    //通过调用getAccessToken() 和 recognition()，实现识别
    void getResultByApi();
    //图片转Base64
    QByteArray imageToBase64(const QImage &image);
    //解析歌曲文件的的歌曲信息
    bool analyseMusicInfo(QString path);

    JsonInfo parseJson(QString json);
    void search(QString str,int page,int pagesize);

    QString getcontains(QJsonObject Object, QString strInfo);
    void parseJsonSongInfo(QString json);


public slots:
    //定义添加音乐文件槽函数
    void addFile();
    //定义按钮音乐播放暂停函数
    void musicPlayBtn();
    //定义手势音乐播放函数
    void musicPlayGesture();
    //定义手势音乐播放函数
    void musicPauseGesture();
    //void addBgPic();//定义添加背景图片函数
    //上一首
    void prevSong();
    //下一首
    void nextSong();
    //调节音量
    void volMoved(int vol);
    //静音
    void muteMusic();
    //拖动改变进度条
    void progressMoved(int value);
    //实时更新进度条
    void progressUpdate();
    //获取到AccessToken后的槽函数
    void getAccessTokenRequestFinished(QNetworkReply* reply);
    //识别完后后的槽函数
    void recognitionRequestFinished(QNetworkReply* reply);
    //播放器状态改变后的槽函数
    void onStateChanged(QMediaPlayer::State state);
    //歌曲列表选项item点击后，对应item的歌曲播放
    void itemPlay(QListWidgetItem *item);
    //当前媒体歌曲变化后的槽函数
    void onCurrentMediaChanged(const QMediaContent &);
    //手势识别开关按钮点击后的槽函数
    void on_gestureSwitch_clicked();
    //播放按钮槽函数
    void on_playMusicBtn_clicked();
    //上一首按钮槽函数
    void on_prevBtn_clicked();
    //下一首按钮槽函数
    void on_nextBtn_clicked();
    //当前歌曲进度变化后的槽函数
    void onPositionChanged(qint64 position);
    //当前歌曲总时长变化后的槽函数
    void onDurationChanged(qint64 duration);
    //静音按钮
    void on_volStatusBtn_clicked();
    //本地歌单按钮槽函数
    void on_localListBtn_clicked();
    //登出账号
    void on_logout_clicked();





signals:

private:
    Ui::MainPage *ui;

};
#endif // MAINPAGE_H
