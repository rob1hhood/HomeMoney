#-------------------------------------------------
#
# Project created by QtCreator 2016-12-24T10:50:10
#
#-------------------------------------------------

QT       += core gui sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HomeMoney
TEMPLATE = app

win32:RC_ICONS += HomeMoney.ico
macx:ICON = HomeMoney.icns

SOURCES += main.cpp\
        MainWindow.cpp \
    DialogAbout.cpp \
    DBTools.cpp \
    DialogEdit.cpp \
    TestClass.cpp

HEADERS  += MainWindow.h \
    DialogAbout.h \
    DBTools.h \
    DialogEdit.h \
    TestClass.h

RESOURCES += \
    HomeMoney.qrc

