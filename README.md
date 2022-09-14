# MusicPlayer
C++ QT版音乐播放器，可以实现基本的音乐播放操作，加入百度手势识别API控制音乐播放，加入虹软的人脸识别用于人脸登录,准备识别前，用OpenCV处理图像





**电脑环境**

Windows x64

QT_5_14_2

**1.基本的音乐播放操作：**

项目配置文件需加入QT       += multimedia multimediawidgets


播放暂停，添加歌曲，上一首，下一首，调节进度条，实时显示进度时间，调节音量，显示歌曲歌手名字等信息。



**2.百度手势识别API  QT接入**


仓库中API的KEY,SECRET已删除，请自行获取

请查看百度官方文档

https://ai.baidu.com/tech/body/gesture

由于要用到网络，项目配置文件需加入QT       += network

由于用到OpenSSL，需要在QT对应的mingw目录加入两个dll文件，才可以使用百度手势识别。仓库中已经给出，我用的版本是1_1的。

如果版本不符合或没有SSL，请查看本地支持的版本是多少，然后在网上寻找解决方案，注意版本必须要严格对应。

注意软件打包的时候，不要忘了把SSL的dll加进打包目录




**3.虹软人脸识别QT接入**

仓库中SDK的激活信息已删除，请自行获取

参考的是这个仓库

https://github.com/cocowts/Qt_ArcSoft_ArcFace_Windows_x86_App

--------------------------------------------------------------------------------

虹软官网：https://ai.arcsoft.com.cn/product/arcface.html

版本：Windows x86 V2.0

-----------------------------------------------------------------------------------

OpenCV版本：V3.3.1

OpenCV开源地址：

https://github.com/opencv/opencv/releases

OpenCV的dll:

由于官方下载安装后的dll不完整，可以查看这个仓库

https://github.com/huihut/OpenCV-MinGW-Build/tree/OpenCV-3.3.1




3-1.人脸识别头文件导入

![image](https://user-images.githubusercontent.com/57706599/190042771-6bbf8dba-20e2-467d-9896-9db71b2e35d4.png)

-----------------------------------------------------------------------------------------------------------------

![image](https://user-images.githubusercontent.com/57706599/190043767-17fe6840-bf55-4bbb-a2a6-21eec546f023.png)



3-2. 在工程设置文件中加入本地安装的OpenCV的include和bin目录，示例如下

win32 {

    INCLUDEPATH += C:\Users\Bunny\Downloads\OpenCV-MinGW-Build-OpenCV-3.3.1-master\include\

    INCLUDEPATH += C:\Users\Bunny\Downloads\OpenCV-MinGW-Build-OpenCV-3.3.1-master\include\opencv

    INCLUDEPATH += C:\Users\Bunny\Downloads\OpenCV-MinGW-Build-OpenCV-3.3.1-master\include\opencv2

    LIBS += C:\Users\Bunny\Downloads\OpenCV-MinGW-Build-OpenCV-3.3.1-master\bin\libopencv_*.dll

}

3-3. 将 "ArcSoft_ArcFace"（虹软官方给的SDK包含有） 与 "OpenCV"（OpenCV安装后的目录包含有） 相关的 dll 文件复制到工程生成的exe应用程序同文件夹下
  
（如build-MusicPlayer-Desktop_Qt_5_14_2_MinGW_32_bit-Debug）

其中仓库中相关的 dll 文件已经全部给出，全部复制过去即可（前提是版本对应）



![image](https://user-images.githubusercontent.com/57706599/190067897-a85b27cd-fc06-4a9b-9437-9cf9536b0f9f.png)


----------------------------------------------------------

![image](https://user-images.githubusercontent.com/57706599/190067990-17d8cb95-50f8-49ad-9085-9e323f011644.png)

---------------------------------------------------------

![image](https://user-images.githubusercontent.com/57706599/190068543-6c3b5db9-30c6-4f74-a646-a9ef62515dc1.png)

