#include "main_page.h"
#include "ui_main_page.h"


MainPage::MainPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainPage)
{
    ui->setupUi(this);

    this->setFixedSize(1200,750); //设置窗口大小
    setWindowFlags(Qt::FramelessWindowHint);

    progressTimer = new QTimer(this);
    gestureRecogTimer = new QTimer(this);

    ui->prevBtn->setAttribute(Qt::WA_Hover,true);//开启悬停事件
    ui->prevBtn->installEventFilter(this);       //安装事件过滤器
    ui->nextBtn->setAttribute(Qt::WA_Hover,true);//开启悬停事件
    ui->nextBtn->installEventFilter(this);       //安装事件过滤器
    ui->closeBtn->setAttribute(Qt::WA_Hover,true);//开启悬停事件
    ui->closeBtn->installEventFilter(this);       //安装事件过滤器
    ui->logout->setAttribute(Qt::WA_Hover,true);//开启悬停事件
    ui->logout->installEventFilter(this);       //安装事件过滤器

    this->player = new QMediaPlayer(this);
    this->playList = new QMediaPlaylist(this);
    this->player->setPlaylist(playList);//设置播放列表
    localList = new QListWidget(this);
    tmpListWidget = new QListWidget();

    //歌单列表设置透明
    localList->setStyleSheet
            ("QScrollArea{background-color:transparent;}");
    localList->viewport()->setStyleSheet
            ("background-color:transparent;");

    //初始化软件运行目录，新建用于保存歌曲解析出来的图片路径
    //初始化软件运行目录，新建用于保存人脸注册图片的路径

    QString dirName1 = "D:/MusicPlayer/";
    QDir dir1(dirName1);
    if(!dir1.exists())
    {
        dir1.mkdir(dirName1);
    }

    QString dirName2 = "D:/MusicPlayer/SongPicture/";
    QDir dir2(dirName2);
    if(!dir2.exists())
    {
        dir2.mkdir(dirName2);
    }

    //创建新的stack
    listStack = new QStackedWidget(this);
    //添加歌单
    ui->stackedWidget->addWidget(localList);
    ui->stackedWidget->setCurrentWidget(localList);

    //初始化音量配置
    QSettings *playerConfigIniWrite = new QSettings("setting.ini", QSettings::IniFormat);
    if(!playerConfigIniWrite->value("volume").isNull()){
        ui->volSlider->setValue(playerConfigIniWrite->value("volume").toInt());
    }else{
        ui->volSlider->setValue(50);
    }
    delete playerConfigIniWrite;

    QString volStr = QString("%1").arg(ui->volSlider->value());
    ui->volStatusBtn->setText(volStr);


    //=============================信号槽链接================
    connect(ui->openFileBtn,&QToolButton::clicked,this,&MainPage::addFile);//连接打开按钮信号 添加音乐文件槽函数
    connect(ui->closeBtn,&QToolButton::clicked,this,[=]{
        QSettings *playerConfigIniWrite = new QSettings("setting.ini", QSettings::IniFormat);
        playerConfigIniWrite->setValue("volume",ui->volSlider->value());
        delete playerConfigIniWrite;
        this->close();
    });
    connect(this->ui->volSlider,SIGNAL(sliderMoved(int)),this,SLOT(volMoved(int)));//拖动改变进度条
    connect(this->progressTimer,SIGNAL(timeout()),this,SLOT(progressUpdate()));//实时更新进度条
    connect(this->ui->progressSlider,SIGNAL(sliderMoved(int)),this,SLOT(progressMoved(int)));//拖动改变进度条
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));
    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(onStateChanged(QMediaPlayer::State)));
    connect(playList,SIGNAL(currentMediaChanged(const QMediaContent&)),this,SLOT(onCurrentMediaChanged(const QMediaContent&)));
    connect(localList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemPlay(QListWidgetItem*)));
}

MainPage::~MainPage()
{
    delete ui;
    delete this->player;//软件关闭，释放player占用内存
    delete this->playList;//软件关闭，释放playList占用内存
}
//歌曲列表选项item点击后，对应item的歌曲播放
void MainPage::itemPlay(QListWidgetItem *item)
{
    tmpListWidget = item->listWidget();
    int currentItemIndex = tmpListWidget->currentRow();
    this->playList->setCurrentIndex(currentItemIndex);
    qDebug()<<currentItemIndex;
    qDebug()<<this->playList->currentIndex();
    this->player->play();

    QFile file("D:/MusicPlayer/PlayList/PlayList.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QByteArray playListLine = file.readAll();

    QString playListStr(playListLine);

    QStringList playList = playListStr.split("\n");
    qDebug()<< playList;
    if(!analyseMusicInfo(playList[this->playList->currentIndex()])){
        ui->songName->setText("无歌曲信息");
        ui->songAuthor->setText("无歌曲信息");
        ui->songAlbum->setText("无歌曲信息");
        ui->albumPic->setText("无图片");
    }
}
//当前媒体歌曲变化后,让歌曲列表对应的歌曲选项显示被选中
void MainPage::onCurrentMediaChanged(const QMediaContent &){
    int currentSongIndex = playList->currentIndex();
    localList->setCurrentRow(currentSongIndex,QItemSelectionModel::SelectCurrent);


    QFile file("D:/MusicPlayer/PlayList/PlayList.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QByteArray playListLine = file.readAll();

    QString playListStr(playListLine);

    QStringList playList = playListStr.split("\n");
    qDebug()<< playList;
    if(!analyseMusicInfo(playList[this->playList->currentIndex()])){
        ui->songName->setText("无歌曲信息");
        ui->songAuthor->setText("无歌曲信息");
        ui->songAlbum->setText("无歌曲信息");
        ui->albumPic->setText("无图片");
    }
}
//实现添加音乐文件槽函数
void MainPage::addFile(){

    //删除歌曲列表txt文件
    QString dirName1 = "D:/MusicPlayer/";
    QDir dir1(dirName1);
    if(!dir1.exists())
    {
        dir1.mkdir(dirName1);
    }

    bool exist;
    QString fileName;

    QDir *folder = new QDir;
    exist = folder->exists("D:/MusicPlayer/PlayList");

    if(!exist){
        folder->mkdir("D:/MusicPlayer/PlayList");
    }
    fileName = tr("D:/MusicPlayer/PlayList/PlayList.txt");

    QFile f(fileName);
    f.remove(fileName);
    f.close();

    //调用文件对话框，获取选中的文件名，并返回给musicFileNames
    QStringList musicFileNames = QFileDialog::getOpenFileNames(this,
                                                               tr("open music file"), "E:/music/", tr("music Files (*.mp3 )"));
    //判断musicFileNames内容是否为空
    if(!musicFileNames.isEmpty()){
        qDebug()<<musicFileNames;//调试窗口输出添加的文件名
        this->playList->clear();//每次载入播放列表，把原有的播放器列表清空
        localList->clear();//每次载入播放列表，把原有的本地列表清空
        foreach (const QString &musicFileName, musicFileNames) {
            QMediaContent media = QMediaContent(QUrl::fromLocalFile(musicFileName));
            this->playList->addMedia(media);//用foreach循环把fileNames的每个文件名添加进播放列表playList




            //设置本地歌单的每个选项的基本组件
            QWidget* localWidget = new QWidget();
            QVBoxLayout* vbl = new QVBoxLayout(localWidget);
            QStringList labelStr1 = musicFileName.split('/');
            QStringList labelStr2 = labelStr1.last().split('.');
            QLabel* songInfo = new QLabel(labelStr2[0]);
            QFont font;
            font.setBold(true);
            font.setPointSize(10);
            songInfo->setFont(font);
            vbl->addWidget(songInfo);

            vbl->setSizeConstraint(QLayout::SetFixedSize);
            localWidget->setLayout(vbl);

            QListWidgetItem* localWidgetItem = new QListWidgetItem;
            localWidgetItem->setSizeHint( localWidget->sizeHint());
            localList->addItem(localWidgetItem);
            localList->setItemWidget(localWidgetItem, localWidget);

            QString listStr = musicFileName + "\n";

            //初始化软件运行目录，新建用于保存人脸注册图片的路径
            QString dirName1 = "D:/MusicPlayer/";
            QDir dir1(dirName1);
            if(!dir1.exists())
            {
                dir1.mkdir(dirName1);
            }


            bool exist;
            QString fileName;

            QDir *folder = new QDir;
            exist = folder->exists("D:/MusicPlayer/PlayList");

            if(!exist){//不存在就创建
                folder->mkdir("D:/MusicPlayer/PlayList");
            }
            fileName = tr("D:/MusicPlayer/PlayList/PlayList.txt");

            QFile f(fileName);
            if(!f.open(QIODevice::WriteOnly | QIODevice::Append)){
                QMessageBox::warning(this,tr("错误"),tr("打开文件失败,数据保存失败"));
                return ;
            }else{
                f.write(listStr.toUtf8().data());
            }
            f.close();
        }
        this->playList->setCurrentIndex(0);//默认载入完列表，把当前列表索引置为第一个，软件会播放第一首歌

        if(!analyseMusicInfo(musicFileNames[this->playList->currentIndex()])){
            ui->songName->setText("无歌曲信息");
            ui->songAuthor->setText("无歌曲信息");
            ui->songAlbum->setText("无歌曲信息");
            ui->albumPic->setText("无图片");
        }
    }
    return;
}


//MP3IDV2格式解析函数
bool MainPage::analyseMusicInfo(QString path)  //music analyse api
{
    //初始化
    m_music_info.mic_songer="";
    m_music_info.mic_name="";
    m_music_info.mic_time="";
    m_music_info.pic_flag="0";
    m_music_info.pic_type="";
    m_music_info.pic_path="";

    //支持mp3IDV2解析
    QFile file(path);
    bool isok=file.open(QIODevice::ReadOnly);
    TAB_info tab_info;
    qint64 head_size=0;       //头部大小
    qint64 file_seek=0;       //文件指针
    quint64 len;
    if(isok==false)
    {
        qDebug()<<"open error";
        file.close();
        return false;
    }
    //文件打开成功
    m_music_info.mic_path=path;//记录mp3文件的路径
    file.read((char*)&tab_info,sizeof(tab_info));
    file_seek=file_seek+10;
    //判断是否为mp3的IDV2格式
    //qDebug()<<QString(tab_info.format);
    if(QString(tab_info.format)!="ID3\u0003"||(int)tab_info.version !=3)
    {
        qDebug()<<"mp3 is not ID3V2 error";
        return false;
    }

    head_size=(tab_info.header_size[0]&0xff)<<21 |
                                              (tab_info.header_size[1]&0xff)<<14 |
                                                                              (tab_info.header_size[2]&0xff)<<7  |
                                                                                                              (tab_info.header_size[3]&0xff);   //每8位只用前7位，第8位无效恒为0;

    HEAD_info head_info;
    quint32 size;

    while(file_seek<head_size)
    {
        //读取头部信息
        len=file.read((char*)&head_info,sizeof(head_info));
        file_seek=file_seek+len;
        size=(head_info.Size[0]&0xff) <<24|(head_info.Size[1]&0xff)<<16|(head_info.Size[2]&0xff)<<8|(head_info.Size[3]&0xff);
        //有"TIT2""TPE1""TALB""TXXX" "TRCK""TPOS""TCON""APIC"
        //qDebug()<<QString(head_info.FrameID);
        if(QString(head_info.FrameID)=="TIT2")        //曲名
        {
            QTextStream stream(&file);
            stream.seek(file.pos()+1);
            QString all= stream.readLine((int)(size/2-1)); //unicode编码中文是两个字节为一个中文，外加结束为零。
            QTextCodec *codec = QTextCodec::codecForName("GBK");
            QString name = codec->toUnicode(all.toLocal8Bit().data());
            ui->songName->setText(name);
            //qDebug()<<name;
            m_music_info.mic_name=name;
            file_seek=file_seek+size;
            file.seek(file_seek);
            continue;
        }
        if(QString(head_info.FrameID)=="TPE1")        //歌手
        {
            QTextStream stream(&file);
            stream.seek(file.pos()+1);
            QString all= stream.readLine((int)(size/2-1)); //unicode编码中文是两个字节为一个中文，外加结束为零。
            QTextCodec *codec = QTextCodec::codecForName("GBK");
            QString author = codec->toUnicode(all.toLocal8Bit().data());
            ui->songAuthor->setText(author);
            //qDebug()<<author;
            m_music_info.mic_songer=author;
            file_seek=file_seek+size;
            file.seek(file_seek);
            continue;
        }
        if(QString(head_info.FrameID)=="TALB")        //专辑
        {
            QTextStream stream(&file);
            stream.seek(file.pos()+1);
            QString all= stream.readLine((int)(size/2-1)); //unicode编码中文是两个字节为一个中文，外加结束为零。
            QTextCodec *codec = QTextCodec::codecForName("GBK");
            QString album = codec->toUnicode(all.toLocal8Bit().data());
            ui->songAlbum->setText(album);
            //qDebug()<<album;
            m_music_info.mic_album=album;
            file_seek=file_seek+size;
            file.seek(file_seek);
            continue;
        }
        if(QString(head_info.FrameID)=="APIC")        //图片
        {
            m_music_info.pic_flag="1";
            file_seek=file_seek+14;       //去掉14位为照片描述
            file.seek(file_seek);

            char *piture =(char *)malloc(size);
            file.read((char *)piture,size-14);
            file_seek=file_seek+size-14;

            //判断照片的存储格式jpg/png
            if(((uchar)piture[0]== 255) && ((uchar)piture[1]== 216)) //0xff 0xd8  ->jpg
            {
                m_music_info.pic_type="jpg";
            }else if(((uchar)piture[0]== 137) && ((uchar)piture[1]== 80)) //0x89 0x50  ->png
            {
                m_music_info.pic_type="png";
            }
            QString path =QString("D:/MusicPlayer/SongPicture/%1.%2").arg(m_music_info.mic_name).arg(m_music_info.pic_type);

            QPixmap p1;
            p1.load(path);
            QPixmap p2 = p1.scaled(200,200,Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->albumPic->setPixmap(p2);

            m_music_info.pic_path=path;
            QFile testpic(path);
            testpic.open(QIODevice::WriteOnly);
            testpic.write(piture,size-14);
            testpic.close();
            free(piture);
            continue;
        }
        //其他信息不需要
        file_seek=file_seek+size;
        file.seek(file_seek);
    }
    file.close();

    return true;
}

//实现按钮音乐播放函数
void MainPage::musicPlayBtn(){  

    //判断当前播放列表是否为空，空则提示用户添加
    if(!this->playList->isEmpty()){
        //判断当前播放状态，如果正在播放，点击按钮会暂停，并把按钮图标设置成"播放"，否则设置成"暂停"
        if(this->player->state() == QMediaPlayer::PlayingState){
            this->player->pause();
        }else{
            this->player->setVolume(ui->volSlider->value());
            this->player->play();
            this->progressTimer->start(50);

            QFile file("D:/MusicPlayer/PlayList/PlayList.txt");
            if(!file.open(QIODevice::ReadOnly)) {
                qDebug()<<"Can't open the file!"<<endl;
            }

            QByteArray playListLine = file.readAll();

            QString playListStr(playListLine);

            QStringList playList = playListStr.split("\n");
            qDebug()<< playList;
            if(!analyseMusicInfo(playList[this->playList->currentIndex()])){
                ui->songName->setText("无歌曲信息");
                ui->songAuthor->setText("无歌曲信息");
                ui->songAlbum->setText("无歌曲信息");
                ui->albumPic->setText("无图片");
            }
        }
    }else{
        QMessageBox musicPlayMsgBox;
        musicPlayMsgBox.setText("播放列表无歌曲");
        musicPlayMsgBox.setInformativeText("是否添加歌曲列表?");
        musicPlayMsgBox.setStandardButtons(QMessageBox::Open | QMessageBox::Cancel);
        musicPlayMsgBox.setButtonText(QMessageBox::Open,"添加歌曲");
        musicPlayMsgBox.setButtonText(QMessageBox::Cancel,"不添加");
        musicPlayMsgBox.setDefaultButton(QMessageBox::Open);
        int ret = musicPlayMsgBox.exec();
        switch (ret) {
        case QMessageBox::Open:
            this->addFile();
            break;
        case QMessageBox::Cancel:

            break;
        default:
            break;
        }
    }
}
//播放器状态改变后的槽函数，让播放按钮正常显示
void MainPage::onStateChanged(QMediaPlayer::State state){
    if(state == QMediaPlayer::PlayingState){
        ui->playMusicBtn->setIcon(QIcon(":/res/pause.png"));
    }else if(state == QMediaPlayer::PausedState || state == QMediaPlayer::StoppedState){
        ui->playMusicBtn->setIcon(QIcon(":/res/play.png"));
    }
}

//实现手势音乐播放函数
void MainPage::musicPlayGesture(){
    this->player->setVolume(ui->volSlider->value());
    this->player->play();
    this->progressTimer->start(50);

}
//实现手势音乐播放函数
void MainPage::musicPauseGesture(){
    this->player->pause();
}

//上一首
void MainPage::prevSong(){
    if(!this->playList->isEmpty()){
        if(this->playList->currentIndex() == 0){
            this->playList->setCurrentIndex(this->playList->mediaCount() - 1);
            this->player->play();

            QFile file("D:/MusicPlayer/PlayList/PlayList.txt");
            if(!file.open(QIODevice::ReadOnly)) {
                qDebug()<<"Can't open the file!"<<endl;
            }

            QByteArray playListLine = file.readAll();

            QString playListStr(playListLine);

            QStringList playList = playListStr.split("\n");
            qDebug()<< playList;
            if(!analyseMusicInfo(playList[this->playList->currentIndex()])){
                ui->songName->setText("无歌曲信息");
                ui->songAuthor->setText("无歌曲信息");
                ui->songAlbum->setText("无歌曲信息");
                ui->albumPic->setText("无图片");
            }
        }
        else {
            this->playList->previous();
            QFile file("D:/MusicPlayer/PlayList/PlayList.txt");
            if(!file.open(QIODevice::ReadOnly)) {
                qDebug()<<"Can't open the file!"<<endl;
            }

            QByteArray playListLine = file.readAll();

            QString playListStr(playListLine);

            QStringList playList = playListStr.split("\n");
            qDebug()<< playList;
            if(!analyseMusicInfo(playList[this->playList->currentIndex()])){
                ui->songName->setText("无歌曲信息");
                ui->songAuthor->setText("无歌曲信息");
                ui->songAlbum->setText("无歌曲信息");
                ui->albumPic->setText("无图片");
            }
        }
    }
    else{
        QMessageBox musicPlayMsgBox;
        musicPlayMsgBox.setText("播放列表无歌曲");
        musicPlayMsgBox.setInformativeText("是否添加歌曲列表?");
        musicPlayMsgBox.setStandardButtons(QMessageBox::Open | QMessageBox::Cancel);
        musicPlayMsgBox.setButtonText(QMessageBox::Open,"添加歌曲");
        musicPlayMsgBox.setButtonText(QMessageBox::Cancel,"不添加");
        musicPlayMsgBox.setDefaultButton(QMessageBox::Open);
        int ret = musicPlayMsgBox.exec();
        switch (ret) {
        case QMessageBox::Open:
            this->addFile();
            break;
        case QMessageBox::Cancel:

            break;
        default:
            break;
        }
    }
}

//下一首
void MainPage::nextSong(){
    if(!this->playList->isEmpty()){
        if(this->playList->currentIndex() == this->playList->mediaCount() - 1){
            this->playList->setCurrentIndex(0);
            this->player->play();
            QFile file("D:/MusicPlayer/PlayList/PlayList.txt");
            if(!file.open(QIODevice::ReadOnly)) {
                qDebug()<<"Can't open the file!"<<endl;
            }

            QByteArray playListLine = file.readAll();

            QString playListStr(playListLine);

            QStringList playList = playListStr.split("\n");
            qDebug()<< playList;
            if(!analyseMusicInfo(playList[this->playList->currentIndex()])){
                ui->songName->setText("无歌曲信息");
                ui->songAuthor->setText("无歌曲信息");
                ui->songAlbum->setText("无歌曲信息");
                ui->albumPic->setText("无图片");
            }
        }
        else {
            this->playList->next();
            QFile file("D:/MusicPlayer/PlayList/PlayList.txt");
            if(!file.open(QIODevice::ReadOnly)) {
                qDebug()<<"Can't open the file!"<<endl;
            }

            QByteArray playListLine = file.readAll();

            QString playListStr(playListLine);

            QStringList playList = playListStr.split("\n");
            qDebug()<< playList;
            if(!analyseMusicInfo(playList[this->playList->currentIndex()])){
                ui->songName->setText("无歌曲信息");
                ui->songAuthor->setText("无歌曲信息");
                ui->songAlbum->setText("无歌曲信息");
                ui->albumPic->setText("无图片");
            }
        }
    }
    else{
        QMessageBox musicPlayMsgBox;
        musicPlayMsgBox.setText("播放列表无歌曲");
        musicPlayMsgBox.setInformativeText("是否添加歌曲列表?");
        musicPlayMsgBox.setStandardButtons(QMessageBox::Open | QMessageBox::Cancel);
        musicPlayMsgBox.setButtonText(QMessageBox::Open,"添加歌曲");
        musicPlayMsgBox.setButtonText(QMessageBox::Cancel,"不添加");
        musicPlayMsgBox.setDefaultButton(QMessageBox::Open);
        int ret = musicPlayMsgBox.exec();
        switch (ret) {
        case QMessageBox::Open:
            this->addFile();
            break;
        case QMessageBox::Cancel:

            break;
        default:
            break;
        }
    }
}
//调节音量
void MainPage::volMoved(int vol){
    if(this->player->isMuted()){
        this->player->setMuted(false);
    }
    this->player->setVolume(vol);
    QString volStr = QString("%1").arg(vol);
    ui->volStatusBtn->setText(volStr);
    //在图形界面上，显示的信息更直观
    if(vol == 0){
        ui->volStatusBtn->setIcon(QIcon(":/res/volumeZero.png"));
    }else if(vol > 0 && vol < 33){
        ui->volStatusBtn->setIcon(QIcon(":/res/volumeLow.png"));
    }else if(vol > 32 && vol < 66){
        ui->volStatusBtn->setIcon(QIcon(":/res/volumeMiddle.png"));
    }else if(vol > 65){
        ui->volStatusBtn->setIcon(QIcon(":/res/volumeHigh.png"));
    }
}
//静音
void MainPage::muteMusic(){
    if(this->player->isMuted()){
        this->player->setMuted(false);
        int vol = this->player->volume();
        if(vol == 0){
            ui->volStatusBtn->setIcon(QIcon(":/res/volumeZero.png"));
        }else if(vol > 0 && vol < 33){
            ui->volStatusBtn->setIcon(QIcon(":/res/volumeLow.png"));
        }else if(vol > 32 && vol < 66){
            ui->volStatusBtn->setIcon(QIcon(":/res/volumeMiddle.png"));
        }else if(vol > 65){
            ui->volStatusBtn->setIcon(QIcon(":/res/volumeHigh.png"));
        }
    }else{
        this->player->setMuted(true);
        ui->volStatusBtn->setIcon(QIcon(":/res/volumeCross.png"));
    }
}
//拖动改变进度条
void MainPage::progressMoved(int value){
    if(!this->playList->isEmpty()){
        float percent = (value * 1.0)/ui->progressSlider->maximum();
        long position = this->player->duration() * percent;
        this->player->setPosition(position);
        positionTime = QTime(0, 0, 0).addMSecs(int(position)).toString(QString::fromLatin1("mm:ss"));
        ui->progressLabel->setText(positionTime+"/"+durationTime);
    }else{
        ui->progressSlider->setValue(0);
        QMessageBox musicPlayMsgBox;
        musicPlayMsgBox.setText("播放列表无歌曲");
        musicPlayMsgBox.setInformativeText("是否添加歌曲列表?");
        musicPlayMsgBox.setStandardButtons(QMessageBox::Open | QMessageBox::Cancel);
        musicPlayMsgBox.setButtonText(QMessageBox::Open,"添加歌曲");
        musicPlayMsgBox.setButtonText(QMessageBox::Cancel,"不添加");
        musicPlayMsgBox.setDefaultButton(QMessageBox::Open);
        int ret = musicPlayMsgBox.exec();
        switch (ret) {
        case QMessageBox::Open:
            this->addFile();
            break;
        case QMessageBox::Cancel:

            break;
        default:
            break;
        }
    }
}
//实时更新进度条
void MainPage::progressUpdate(){
    long pos = this->player->position();
    long duration = this->player->duration();
    int val = 100 * (pos * 1.0)/duration;
    this->ui->progressSlider->setValue(val);
}
//歌曲总时长
void MainPage::onDurationChanged(qint64 duration)
{
    durationTime = QTime(0, 0, 0).addMSecs(int(duration)).toString(QString::fromLatin1("mm:ss"));
    ui->progressLabel->setText(positionTime+"/"+durationTime);
}
//实时获取歌曲进度时间
void MainPage::onPositionChanged(qint64 position)
{
    if(ui->progressSlider->isSliderDown()){
        return;//如果手动调整进度条，则不处理
    }
    positionTime = QTime(0, 0, 0).addMSecs(int(position)).toString(QString::fromLatin1("mm:ss"));
    ui->progressLabel->setText(positionTime+"/"+durationTime);

    int pos = position/10;
    QMap<int, QString>::iterator iter = lrcMap.begin();
    while (iter != lrcMap.end())
    {
        if(pos - 50 <= iter.key() && pos + 50 >= iter.key())
        {
            int j=0;
            if(iter != lrcMap.begin())
            {
                iter--;
                ui->label_20->setText(iter.value());
                j++;
            }
            if(iter != lrcMap.begin())
            {
                iter--;
                ui->label_19->setText(iter.value());
                j++;
            }

            if(iter != lrcMap.begin())
            {
                iter--;
                ui->label_6->setText(iter.value());
                j++;
            }
            for(;j>0;j--)
            {
                iter++;
            }
            //中间
            ui->label_21->setText(iter.value());
            iter++;
            if(iter != lrcMap.end())
            {
                ui->label_22->setText(iter.value());
            }
            else
            {
                ui->label_22->setText("");
                return;
            }
            iter++;
            if(iter != lrcMap.end())
            {
                ui->label_23->setText(iter.value());
            }
            else
            {
                ui->label_23->setText("");
                return;
            }
            iter++;
            if(iter != lrcMap.end())
            {
                ui->label_24->setText(iter.value());
            }
            else
            {
                ui->label_24->setText("");
                return;
            }
            iter++;
            if(iter != lrcMap.end())
            {
                ui->label_25->setText(iter.value());
            }
            else
            {
                ui->label_25->setText("");
                return;
            }
            iter++;
            if(iter != lrcMap.end())
            {
                ui->label_26->setText(iter.value());
            }
            else
            {
                ui->label_26->setText("");
                return;
            }
            iter++;
            if(iter != lrcMap.end())
            {
                ui->label_27->setText(iter.value());
            }
            else
            {
                ui->label_27->setText("");
                return;
            }
        }
        iter++;
    }
}
/**
 * @brief MainWindow::getTextByApi
 * 通过百度api开始识别手势
 */
void MainPage::getResultByApi(){
    getAccessToken();
    recognition();
}
/**
 * @brief MainWindow::recognition
 * 手势识别
 */
void MainPage::recognition(){
    QString imgBase = originImgBase;
    qDebug() << "开始请求";
    netAccess = new QNetworkAccessManager(this);
    QUrl url;

    url.setUrl("https://aip.baidubce.com/rest/2.0/image-classify/v1/gesture?access_token="+accessToken);

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    QByteArray byteArr;
    byteArr.append("image=");
    byteArr.append(QUrl::toPercentEncoding(imgBase));
    connect(netAccess, &QNetworkAccessManager::finished,this, &MainPage::recognitionRequestFinished);
    QNetworkReply* reply = netAccess->post(request,byteArr);
    qDebug() << "结束请求";
}



/**
 *  MainWindow::getAccessToken
 * 获取百度Access Token
 */
void MainPage::getAccessToken(){
    QString key = "";//此处请填入百度手势识别API_KEY
    QString secret = "";//此处请填入百度手势识别API_SECRET
    QSettings *gestureConfig = new QSettings("AccessToken.ini", QSettings::IniFormat);
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t();   //将当前时间转为时间戳
    if((gestureConfig->value("tokenTime").toInt() + 30*24*3600) < timeT){
        netAccess = new QNetworkAccessManager(this);
        QString parm = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id="+key+"&client_secret="+secret+"&";
        QUrl url(parm);
        connect(netAccess, &QNetworkAccessManager::finished,this, &MainPage::getAccessTokenRequestFinished);
        QNetworkReply* reply = netAccess->get(QNetworkRequest(url));
        qDebug() << "获取Token";
    }else{
        accessToken = gestureConfig->value("accessToken").toString();
    }
}

/**
 *  MainWindow::requestFinished
 *  reply
 * 获取access_token后的信号槽
 */
void MainPage::getAccessTokenRequestFinished(QNetworkReply* reply) {
    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    }else {
        // 获取返回内容
        QJsonParseError json_error;
        QJsonDocument parse_doucment = QJsonDocument::fromJson(reply->readAll(), &json_error);
        if(parse_doucment.isObject()){
            QJsonObject obj = parse_doucment.object();
            accessToken = obj.take("access_token").toString();
            QSettings *gestureConfig = new QSettings("AccessToken", QSettings::IniFormat);
            //向ini文件中写入内容,setValue函数的两个参数是键值对
            gestureConfig->setValue("accessToken", accessToken);
            QDateTime time = QDateTime::currentDateTime();   //获取当前时间
            int timeT = time.toTime_t();   //将当前时间转为时间戳
            gestureConfig->setValue("tokenTime", timeT);
            //写入完成后删除指针
            delete gestureConfig;
        }
    }
    netAccess->deleteLater();
}

/**
 *  MainWindow::recognitionRequestFinished
 *  reply
 * 手势识别的信号槽
 */
void MainPage::recognitionRequestFinished(QNetworkReply* reply){
    QNetworkReply::NetworkError err = reply->error();
    //qDebug()<<reply->readAll();
    if(err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    }else {
        // 获取返回内容
        QJsonParseError json_error;
        QJsonDocument parse_doucment = QJsonDocument::fromJson(reply->readAll(), &json_error);
        if(parse_doucment.isObject()){
            QJsonObject obj = parse_doucment.object();
            int result_num = obj.take("result_num").toInt();
            if(result_num > 0){
                QString res;
                QJsonArray jsonArr = obj.take("result").toArray();

                for(int i=0;i<result_num;i++){
                    res.append(jsonArr[i].toObject().take("classname").toString());
                    res.append(" ");
                }
                qDebug()<<res.toUtf8().data();
                if(res.contains("Five",Qt::CaseInsensitive)){
                    musicPauseGesture();
                }else if(res.contains("Ok",Qt::CaseInsensitive)){
                    musicPlayGesture();
                }else if(res.contains("One",Qt::CaseInsensitive)){
                    prevSong();
                }else if(res.contains("Two",Qt::CaseInsensitive)){
                    nextSong();
                }
            }
        }
    }
    netAccess->deleteLater();
}
/**
 * 图片转Base64
 *
 */
QByteArray MainPage::imageToBase64(const QImage &image)
{
    QImage imageFile(image);
    QByteArray ba;
    QBuffer buf(&ba);
    imageFile.save(&buf, "png");
    return ba.toBase64();
}

//设置背景图片
void MainPage::paintEvent(QPaintEvent *){
    QPainter bgPainter(this);
    QPixmap bgPixmap;
    bgPixmap.load(":/res/lay1.jpg");
    bgPainter.drawPixmap(0,0,this->width(),this->height(),bgPixmap);
}

//鼠标按下事件
void MainPage::mousePressEvent(QMouseEvent *event)
{
    QPoint y = event->globalPos();//获取鼠标点击处的全局坐标
    QPoint x = this->geometry().topLeft();//获取窗口左上角到屏幕左上角的坐标
    this->z = y - x;//相减即为z坐标，鼠标点击处到窗口左上角的点坐标
}
//鼠标移动事件
void MainPage::mouseMoveEvent(QMouseEvent *event)
{
    QPoint y = event->globalPos();//获取鼠标点击处的全局坐标
    QPoint x = y - z;
    move(x);//鼠标移动，窗口就移动
}
//鼠标释放事件
void MainPage::mouseReleaseEvent(QMouseEvent *){
    this->z = QPoint();//鼠标释放后让z坐标重置为0
}

//键盘事件，设置快捷键
void MainPage::keyPressEvent(QKeyEvent* event)
{
    if ((event->modifiers() == Qt::CTRL) && (event->key() == Qt::Key_P)){
        musicPlayGesture();
    }else if((event->modifiers() == Qt::CTRL) && (event->key() == Qt::Key_S)){
        musicPauseGesture();
    }else if((event->modifiers() == Qt::CTRL) && (event->key() == Qt::Key_A)){
        prevSong();
    }else if((event->modifiers() == Qt::CTRL) && (event->key() == Qt::Key_D)){
        nextSong();
    }else if((event->modifiers() == Qt::CTRL) && (event->key() == Qt::Key_M)){
        muteMusic();
    }
}

//事件过滤器
bool MainPage::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->prevBtn) {
        if(event->type() == QEvent::HoverEnter) {
            ui->prevBtn->setIcon(QIcon(":/res/player_back_on.png"));//鼠标悬停改变按钮颜色
            return true;
        }
    }
    if(obj == ui->nextBtn) {
        if(event->type() == QEvent::HoverEnter) {
            ui->nextBtn->setIcon(QIcon(":/res/player_forward_on.png"));
            return true;
        }
    }
    if(obj == ui->prevBtn) {
        if(event->type() == QEvent::HoverLeave) {
            ui->prevBtn->setIcon(QIcon(":/res/player_back.png"));//鼠标离开恢复原始按钮
            return true;
        }
    }
    if(obj == ui->nextBtn) {
        if(event->type() == QEvent::HoverLeave) {
            ui->nextBtn->setIcon(QIcon(":/res/player_forward.png"));
            return true;
        }
    }
    if(obj == ui->closeBtn) {
        if(event->type() == QEvent::HoverEnter) {
            ui->closeBtn->setIcon(QIcon(":/res/close_on.png"));//鼠标悬停改变按钮颜色
            return true;
        }
    }
    if(obj == ui->closeBtn) {
        if(event->type() == QEvent::HoverLeave) {
            ui->closeBtn->setIcon(QIcon(":/res/close.png"));//鼠标离开恢复原始按钮
            return true;
        }
    }
    if(obj == ui->logout) {
        if(event->type() == QEvent::HoverEnter) {
            ui->logout->setIcon(QIcon(":/res/logout_on.png"));//鼠标悬停改变按钮颜色
            return true;
        }
    }
    if(obj == ui->logout) {
        if(event->type() == QEvent::HoverLeave) {
            ui->logout->setIcon(QIcon(":/res/logout.png"));//鼠标离开恢复原始按钮
            return true;
        }
    }

    return QWidget::eventFilter(obj,event);
}
//手势识别按钮点击后，开始识别
void MainPage::on_gestureSwitch_clicked()
{
    if(this->gestureRecogTimer->isActive()){//关闭手势识别
        this->gestureRecogTimer->stop();
        camera->stop();
        ui->gestureSwitch->setIcon(QIcon(":/res/switchOff.png"));
    }else{//开启手势识别
        ui->gestureSwitch->setIcon(QIcon(":/res/switchOn.png"));
        qDebug() << QStringLiteral("OpenSSL支持情况:") << QSslSocket::supportsSsl();
        qDebug()<<"QSslSocket="<<QSslSocket::sslLibraryBuildVersionString();

        this->camera = new QCamera(); // 摄像头对象
        QCameraImageCapture *imageCapture = new QCameraImageCapture(camera); // 用于截取摄像头图像

        camera->start();
        imageCapture->capture("capture.jpg"); // 初始化截一张图

        // 拍照完成后的槽函数
        QObject::connect(imageCapture, &QCameraImageCapture::imageCaptured, [=](int id, const QImage &image) {
            Q_UNUSED(id);
            this->originImgBase = imageToBase64(image);

        });

        connect(gestureRecogTimer,&QTimer::timeout,this,[=]{
            imageCapture->capture("capture.jpg"); // 每次截图生成一个图片，用于识别并控制
            QTimer::singleShot(200, this,[=](){//等待截图完再识别
                getResultByApi();
            });
        });
        this->gestureRecogTimer->start(2000);
    }
}

void MainPage::on_playMusicBtn_clicked()
{
    musicPlayBtn();
}

void MainPage::on_prevBtn_clicked()
{
    prevSong();
}

void MainPage::on_nextBtn_clicked()
{
    nextSong();
}
void MainPage::on_volStatusBtn_clicked()
{
    muteMusic();
}
//切换本地播放列表
void MainPage::on_localListBtn_clicked()
{
    if(ui->stackedWidget->currentWidget() != localList){
        ui->stackedWidget->setCurrentWidget(localList);
    }
}
//登出账号
void MainPage::on_logout_clicked()
{
    this->close();
}
