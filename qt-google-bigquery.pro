#-------------------------------------------------
#
# Project created by QtCreator 2012-05-28T18:43:34
#
#-------------------------------------------------

QT       += core gui webkit network

TARGET = qt-google-bigquery
TEMPLATE = app

INCLUDEPATH += ../qjson/include

linux* {
LIBS += ../qjson/build/lib/libqjson.so
}

win* {
LIBS += ../qjson/build/lib/qjson0.dll

    debug {
        LIBS += -L../qt-oauth-lib/debug
    }
    release {
        LIBS += -L../qt-oauth-lib/release
    }
}

macx* {
LIBS += -F../qjson/build/lib -framework qjson
}


SOURCES += main.cpp\
        mainwindow.cpp \
    oauth2.cpp \
    logindialog.cpp \
    settings_api.cpp \
    manager_bigquery.cpp

HEADERS  += mainwindow.h \
    oauth2.h \
    logindialog.h \
    settings_api.h \
    manager_bigquery.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    settings_api.ui
