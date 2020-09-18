#-------------------------------------------------
#
# Project created by QtCreator 2019-02-09T20:33:58
#
#-------------------------------------------------

QT       += core gui
QT += svg
RC_LANG = 0x0004

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_TARGET_PRODUCT=LZWCompressor
QMAKE_TARGET_COMPANY=Fengwei Zhang
QMAKE_TARGET_DESCRIPTION=The program is designed by Fengwei Zhang.
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dialog_create.cpp \
    dialog_compressing.cpp \
    archive_thread.cpp \
    dialog_expanding.cpp \
    dialog_unzipping.cpp \
    unzip_thread.cpp

HEADERS += \
        mainwindow.h \
    dialog_create.h \
    dialog_compressing.h \
    archive_thread.h \
    dialog_expanding.h \
    dialog_unzipping.h \
    unzip_thread.h
FORMS += \
        mainwindow.ui \
    dialog_create.ui \
    dialog_compressing.ui \
    dialog_expanding.ui \
    dialog_unzipping.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    svgs.qrc
