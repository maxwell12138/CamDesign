#-------------------------------------------------
#
# Project created by QtCreator 2017-04-18T23:44:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = CamDesign
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    data.cpp \
    platemovecam.cpp \
    rpn.cpp \
    basiccam.cpp \
    inputtable.cpp \
    dialog.cpp \
    figure.cpp

HEADERS  += mainwindow.h \
    data.h \
    def.h \
    platemovecam.h \
    rpn.h \
    basiccam.h \
    inputtable.h \
    dialog.h \
    figure.h

FORMS    += mainwindow.ui \
    dialog.ui

DISTFILES += \
    res.rc
RC_FILE+=CamDesign.rc \
        res.rc
RESOURCES += \
    res.qrc
