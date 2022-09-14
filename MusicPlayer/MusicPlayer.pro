QT       += core gui multimedia multimediawidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    face_recognition.cpp \
    main.cpp \
    main_page.cpp \
    login.cpp \
    register_face.cpp

HEADERS += \
    face_recognition.h \
    jsoninfo.h \
    main_page.h \
    login.h \
    ArcSoft_ArcFace/inc/amcomdef.h \
    ArcSoft_ArcFace/inc/arcsoft_face_sdk.h \
    ArcSoft_ArcFace/inc/asvloffscreen.h \
    ArcSoft_ArcFace/inc/merror.h \
    register_face.h

FORMS += \
    face_recognition.ui \
    main_page.ui \
    login.ui \
    register_face.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

win32: LIBS += -L$$PWD/ArcSoft_ArcFace/lib/Win32/ -llibarcsoft_face_engine

INCLUDEPATH += $$PWD/ArcSoft_ArcFace/inc
DEPENDPATH += $$PWD/ArcSoft_ArcFace/inc

win32 {
INCLUDEPATH += C:\Users\Bunny\Downloads\OpenCV-MinGW-Build-OpenCV-3.3.1-master\include\
INCLUDEPATH += C:\Users\Bunny\Downloads\OpenCV-MinGW-Build-OpenCV-3.3.1-master\include\opencv
INCLUDEPATH += C:\Users\Bunny\Downloads\OpenCV-MinGW-Build-OpenCV-3.3.1-master\include\opencv2
LIBS += C:\Users\Bunny\Downloads\OpenCV-MinGW-Build-OpenCV-3.3.1-master\bin\libopencv_*.dll

}
