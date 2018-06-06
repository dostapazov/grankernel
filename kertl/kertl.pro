#-------------------------------------------------
#
# Project created by QtCreator 2018-06-02T15:23:36
#
#-------------------------------------------------


CONFIG   -= app_bundle
CONFIG   -= qt
CONFIG   += staticlib
DESTDIR   = ../lib

TARGET = kertl
TEMPLATE = lib
INCLUDEPATH += ../inc


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        kertl.cpp

HEADERS +=
unix {
    target.path = /usr/lib
    INSTALLS += target
}
