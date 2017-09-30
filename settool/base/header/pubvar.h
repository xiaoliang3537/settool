#ifndef PUBVAR_H
#define PUBVAR_H

#ifdef WIN32
# ifndef I_OS_WINDOWS
# define I_OS_WINDOWS
# endif
#elif unix
# ifndef I_OS_LINUX
# define I_OS_LINUX
# endif
#endif

#include <stdio.h>
#include <string>

#include <stdlib.h>
#include <iostream>
#include <cstring>
#include "config.h"
#include "stringlist.h"

#ifdef I_OS_WINDOWS
#include <windows.h>
#include <WinSock.h>
#include <io.h>
#else
//#include
#endif

#ifdef I_OS_WINDOWS

#define I_TO_CSTRING(p)                     iToTChar(p)
#define STD_TO_CSTRING(p)                   stringToTChar(p)
#define CSTRING_TO_STD(p)                   TCharToString(p)
#endif


#define TOOL_MYZIP              1
#define TOOL_AXMLPRINTER        2
#define TOOL_SMALI              3
#define TOOL_BAKSMALI           4
#define TOOL_AXMLMODIFY         6
#define TOOL_SIGNAPK            7
#define TOOL_BAKSMALI2          8
#define TOOL_SMALI2             9
#define TOOL_KEYTOOL            11


using namespace std;
class loger;

// 公共变量
class CPubVar
{
public:
    CPubVar();
    ~CPubVar();
public:
    static CPubVar & GetInstance()
    {
        static CPubVar instance;
        return instance;
    }
public:
    int init();
    // 获取随机数
    unsigned long getRandomCount();
    int generateRandFileName(std::string str, std::string &strFileName);
    int getNextIndex(){return m_iCurrIndex++;}
    std::string getModulePath(int moduleID);
    std::string getWorkPath();
    std::string getZipPath();
    std::string getSmaliPath();
    std::string getBackSmaliPath();
    std::string getAxmlModifyPath();
    std::string getAXMLPrinter2Path();
    /**
     * @brief getGernealToolList 获取通用工具列表 配置中iToolType = 1 类型的工具
     * @param [out] strlist 工具列表 通过 name=path 标识
     * @return 0 正常 非0 失败
     */
    int getGernealToolList(CStringList& strlist);
public:
    CConfig* m_config;
    loger* m_loger;
private:
    int m_iCurrIndex;
public:
#ifdef I_OS_WINDOWS
    HANDLE                  g_hHandel[10];
    int initHandle()
    {
        g_hHandel[0] = CreateEvent(NULL,TRUE,FALSE,NULL);
        g_hHandel[1] = CreateEvent(NULL,TRUE,FALSE,NULL);
        g_hHandel[2] = CreateEvent(NULL,TRUE,FALSE,NULL);
        g_hHandel[3] = CreateEvent(NULL,TRUE,FALSE,NULL);
        g_hHandel[4] = CreateEvent(NULL,TRUE,FALSE,NULL);
        g_hHandel[5] = CreateEvent(NULL,TRUE,FALSE,NULL);
        g_hHandel[6] = CreateEvent(NULL,TRUE,FALSE,NULL);
        g_hHandel[7] = CreateEvent(NULL,TRUE,FALSE,NULL);
        g_hHandel[8] = CreateEvent(NULL,TRUE,FALSE,NULL);
        g_hHandel[9] = CreateEvent(NULL,TRUE,FALSE,NULL);
        return 0;
    }
#else
#endif

private:
    std::string             m_strWorkPath;
    std::string             m_strZipToolPath;
    std::string             m_strSmaliToolPath;
    std::string             m_strBackSmaliToolPath;
    std::string             m_strAxmlModifyToolPath;
    std::string             m_strAXMLPrinter2ToolPath;

};

#endif
