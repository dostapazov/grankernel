#-------------------------------------------------
#
# Project created by QtCreator 2018-05-31T00:28:49
#
#-------------------------------------------------



TARGET   = otdhlpr
TEMPLATE = lib
CONFIG   -= app_bundle
CONFIG   -= qt
DESTDIR  = ../bin
DEFINES += KERTL_DLL
INCLUDEPATH += ../inc


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        otdhlpr.cpp \
    otd_base.cpp \
    otd_addr.cpp



unix {
    target.path = /usr/lib
    INSTALLS += target
}
