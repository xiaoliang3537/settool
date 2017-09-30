#ifndef PUBFUNC_H
#define PUBFUNC_H

#ifdef WIN32
# ifndef I_OS_WINDOWS
# define I_OS_WINDOWS
# endif
#elif unix
# ifndef I_OS_LINUX
# define I_OS_LINUX
# endif
#endif


#include "define.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cstring>

#ifdef I_OS_WINDOWS
#include <comdef.h>
#include <windows.h>
#else
#include <iconv.h>
#endif

using namespace std ;


class CPubfunc
{
public:
    CPubfunc();

public:
    // 检查是入参是否为参数
    static bool checkIsParam(char* info);
    static const char* tr(const char* p, std::string &str);
    static const char* tr(unsigned long p, std::string &str);
    // windows 下面字符转换
#ifdef I_OS_WINDOWS
    static char* QXUtf82Unicode(const char* utf, size_t *unicode_number);
    static char* QXUnicode2Utf8(const char* unicode);
    static std::string KS_ANSI_to_UTF8 (const char* szAnsi);
    static std::string KS_UTF8_to_ANSI (const char* szUTF8);
#else
#endif
};

#endif // PUBFUNC_H
