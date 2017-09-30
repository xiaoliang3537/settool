TEMPLATE = lib
CONFIG   += staticlib

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG -= C++11

DEFINES += "_FILE_OFFSET_BITS=64"
DEFINES -= UNICODE
DEFINES += MARKUP_STL


win32{
    DEFINES += I_OS_WINDOWS
}
else:unix{
    DEFINES += I_OS_LINUX
}

debug {
    DEFINES += _DEBUG
    TARGET = base_d
    DESTDIR = $$PWD/../lib/debug
}
else:release {
    TARGET = base
    DESTDIR = $$PWD/../lib/release
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
#    header/managerthread.h \
    header/Markup.h \
    header/pubfunc.h \
    header/pubvar.h \
    header/register.h \
    header/stringlist.h \
    zlib/zipfile.h \
    zlib/private.h \
    zlib/zip.h \
    zlib/ioapi.h \
    zlib/zlib.h \
    zlib/zconf.h \
#    header/task.h \
#    header/threadtask.h
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
#    source/managerthread.cpp \
    source/Markup.cpp \
    source/pubfunc.cpp \
    source/pubvar.cpp \
    source/stringlist.cpp \
#    source/task.cpp \
#    source/threadtask.cpp
    source/loger.cpp


