TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += static

SOURCES += main.cpp \
    ModifyDex.cpp \
    getopt.c \
    Markup.cpp \
    loger.cpp \

HEADERS += \
    ModifyDex.h \
    FileUtils.h \
    getopt.h \
    PathUtils.h \
    StringUtils.h \
    Utils.h \
    Markup.h \
    loger.h \


win32{
    DEFINES += I_OS_WINDOWS
    DEFINES -= UNICODE
    QMAKE_LFLAGS = -static
}
else:unix{
    DEFINES += I_OS_LINUX
    DEFINES += UTF-8
    QMAKE_LFLAGS = -static -fPIC
    DEFINES += "_FILE_OFFSET_BITS=64"
}
unix:!macx:DEFINES += "_FILE_OFFSET_BITS=64"


CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
    TARGET = IJMModifydex_d
    DESTDIR = $$PWD/../evsubs/module/Modifydex
}
else
{
    TARGET = IJMModifydex
    DESTDIR = $$PWD/../evsubs/module/Modifydex
}

#TARGET = ModifyDexCt
