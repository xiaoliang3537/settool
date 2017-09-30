TEMPLATE = lib
TARGET = CApkGeneral
CONFIG -= qt
DEFINES -= UNICODE
DEFINES += MARKUP_STL

INCLUDEPATH += \
                $$PWD/../base/include \
                $$PWD/../base/header \
                $$PWD/../base/zlib

debug {
    DEFINES += _DEBUG
    TARGET = CApkGeneral
    DESTDIR = $$PWD/../../evsubs/module
    LIBS += -L$$PWD/../lib/debug -lbase_d  \
            -L$$PWD/../lib -lzlib
}
else:release {
    TARGET = CApkGeneral
    DESTDIR = $$PWD/../../evsubs/module
    LIBS += -L$$PWD/../lib/release -lbase  \
            -L$$PWD/../lib -lzlib
}

win32{
    DEFINES += I_OS_WINDOWS
}
else:unix{
    DEFINES += I_OS_LINUX
}

HEADERS += \
    apkgeneral.h

SOURCES += \
    apkgeneral.cpp
