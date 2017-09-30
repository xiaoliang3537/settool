TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

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

DEFINES -= UNICODE

QMAKE_LFLAGS += -static

unix:!macx:DEFINES += "_FILE_OFFSET_BITS=64"

#TARGET = ModifyDexCt
