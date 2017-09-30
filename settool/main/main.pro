TARGET = settool
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
#CONFIG += static
CONFIG -= C++11

DEFINES += "_FILE_OFFSET_BITS=64"
DEFINES += MARKUP_STL


INCLUDEPATH += \
                $$PWD/../base/include \
                $$PWD/../base/header \
                $$PWD/../base/zlib \


DEFINES += "_FILE_OFFSET_BITS=64"

win32{
    DEFINES += I_OS_WINDOWS
    DEFINES -= UNICODE
    DEFINES += _DEBUG
    debug {
        TARGET = settool_d
        DESTDIR = $$PWD/../../evsubs
        LIBS += -L$$PWD/../lib/debug -lbase_d
        LIBS += -L$$PWD/../lib/debug -llibzip_d
        LIBS += -L$$PWD/../lib/debug -lzlib_d
    }
    else:release {
        TARGET = settool
        DESTDIR = $$PWD/../../evsubs
        LIBS += -L$$PWD/../lib -lbase
    }
}
else:unix{
    DEFINES += I_OS_LINUX
    DEFINES += UTF-8
    QMAKE_LFLAGS = -static -fPIC
    debug {
        TARGET = settool_d
        DESTDIR = $$PWD/../../evsubs
        LIBS += -L$$PWD/../lib/debug -lbase_d
        LIBS += -L$$PWD/../lib/debug -llibzip_d
        LIBS += -L$$PWD/../lib/debug -lzlib_d
        LIBS += -ldl

    }
    else:release {
        TARGET = settool
        DESTDIR = $$PWD/../../evsubs
        LIBS += -L$$PWD/../lib/release -lbase
    }

}

SOURCES += \
    main.cpp \
    apkgeneral.cpp

HEADERS += \
    apkgeneral.h



