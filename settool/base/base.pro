CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG -= C++11

DEFINES += "_FILE_OFFSET_BITS=64"
DEFINES -= UNICODE
DEFINES += MARKUP_STL



win32{
    DEFINES += I_OS_WINDOWS
    DEFINES -= UNICODE
    CONFIG += -static-runtime

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG
        TARGET = settool_d
        DESTDIR = $$PWD/../../evsubs
        LIBS += -L$$PWD/../lib/debug -llibzip_d
        LIBS += -L$$PWD/../lib/debug -lzlib_d
        LIBS += -L$$PWD/../lib/debug -llibutils_d
    }
    else
    {
        TARGET = settool
        DESTDIR = $$PWD/../../evsubs
        LIBS += -L$$PWD/../lib/release -llibutils
        LIBS += -L$$PWD/../lib/release -lzlib
        LIBS += -L$$PWD/../lib/release -llibzip
    }

}
else:unix{
    DEFINES += I_OS_LINUX
    DEFINES += UTF-8
    QMAKE_LFLAGS = -static -fPIC

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG
        TARGET = settool_d
        DESTDIR = $$PWD/../../evsubs
        LIBS += -L$$PWD/../lib/debug -llibzip_d
        LIBS += -L$$PWD/../lib/debug -lzlib_d
        LIBS += -L$$PWD/../lib/debug -llibutils_d
        LIBS += -ldl
    }
    else
    {
        TARGET = settool
        DESTDIR = $$PWD/../../evsubs
        LIBS += -L$$PWD/../lib/release -llibutils
        LIBS += -L$$PWD/../lib/release -llibzip
        LIBS += -L$$PWD/../lib/release -lzlib
        LIBS += -ldl
    }
}



INCLUDEPATH += \
                $$PWD/include \
                $$PWD/header \
                $$PWD/zlib

HEADERS += \
    header/apk.h \
    header/config.h \
    header/define.h \
    header/fileinfo.h \
    header/library.h \
    header/list.h \
    header/manager.h \
    header/Markup.h \
    header/pubfunc.h \
    header/pubvar.h \
    header/register.h \
    header/stringlist.h \
    header/apkgeneral.h \
    zlib/zipfile.h \
    zlib/private.h \
    zlib/zip.h \
    zlib/ioapi.h \
    zlib/zlib.h \
    zlib/zconf.h \
    header/loger.h \
    include/3Des.h \
    include/CryptoDes.h \
    include/Debug.h \
    include/DebugMem.h \
    include/FileUtils.h \
    include/Md5.h \
    include/PathUtils.h \
    include/StringUtils.h \
    include/Utils.h


SOURCES += \
    source/apk.cpp \
    source/config.cpp \
    source/fileinfo.cpp \
    source/library.cpp \
    source/list.cpp \
    source/manager.cpp \
    source/Markup.cpp \
    source/pubfunc.cpp \
    source/pubvar.cpp \
    source/stringlist.cpp \
    source/loger.cpp \
    source/main.cpp \
    source/apkgeneral.cpp


