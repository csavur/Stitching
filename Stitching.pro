#-------------------------------------------------
#
# Project created by QtCreator 2016-09-15T19:34:49
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Stitching
TEMPLATE = app


SOURCES += main.cpp\
        camerastitchingpage.cpp \
    graphicscameraitem.cpp \
    scene.cpp

HEADERS  += camerastitchingpage.h \
    graphicscameraitem.h \
    scene.h

FORMS    += camerastitchingpage.ui

RESOURCES += \
    images.qrc
