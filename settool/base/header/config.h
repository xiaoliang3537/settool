#ifndef CONFIG_H
#define CONFIG_H

#ifdef WIN32
# ifndef I_OS_WINDOWS
# define I_OS_WINDOWS
# endif
#elif unix
# ifndef I_OS_LINUX
# define I_OS_LINUX
# endif
#endif

#include <string>
#include <iostream>
#include <map>
#include <list>
#include "register.h"
#include "stringlist.h"

using namespace std;

struct ST_Module;
struct ST_Module_Param;

class CConfig
{
public:
    CConfig();
    ~CConfig();

public:
    int init();

    // 通过模块id查找模块信息
    ST_Module* getModuleInfo(int moduleId);

    // 通过参数查找对应模块
    ST_Module* getParamModule(string strParam);

    // 获取模块下面对应参数信息
    ST_Module_Param* getParamInModule(int moduleId, string strParam);

    // 获取通用模块数据列表
    int getGernealToolList(CStringList& strlist);

    // 打印帮助信息
    int showHelp();
    std::map <std::string, int>                                       m_mapParamModule;               // 参数模块对照表
    std::map<std::string, ST_Module_Param*>                           m_mapParam;                     // 参数表
private:
    int loadConfig();
private:
    std::map<int, ST_Module*>                                         m_mapModule;                    // 模块列表
    std::map<int, ST_Module*>                                         m_mapGeneralModule;             // 模块列表
};

#endif // CONFIG_H
