#ifndef DEFINE_H
#define DEFINE_H

#include <string>
#include <list>
#include <vector>
#include "stringlist.h"

#ifdef WIN32
# ifndef I_OS_WINDOWS
# define I_OS_WINDOWS
# endif
#elif unix
# ifndef I_OS_LINUX
# define I_OS_LINUX
# endif
#endif


using namespace std;

#define CONN(a,b) a##b
#ifdef I_OS_WINDOWS
#define EXPORT_DEF(name) extern "C" __declspec(dllexport) void* getInstance##name()
#else
#define EXPORT_DEF(name) extern "C" void* getInstance##name()
#endif


// 子参数列表
struct ST_Module_Param_Sub
{
    ST_Module_Param_Sub()
    {
        iValueNeed = -1;
    }

    string                              strParamName;               // 参数名称
    string                              strParamNameL;              // 参数名称1 实际入参
    string                              strParamDesc;               // 参数描述
    string                              strParamValue;              // 参数值
    string                              strParamDepend;             // 依赖参数
    unsigned int                        iValueNeed;                 // 是否必须带参数
};

// 参数列表
struct ST_Module_Param
{
    ST_Module_Param()
    {
        bSubParam = false;
        iDefault = -1;
        iModuleId = -1;
        iValueNeed = -1;
        iAttributes = -1;
    }
    bool                                bSubParam;                  // 是否含有子参数
    string                              strParamName;               // 参数名称
    string                              strParamNameL;              // 参数名称1 实际入参
    string                              strParamDesc;               // 参数描述
    string                              strParamValue;              // 参数值
    string                              strParamDepend;             // 依赖参数
    list<ST_Module_Param_Sub*>          listSubParam;               // 子参数列表
    unsigned int                        iDefault;                   // 默认参数
    int                                 iModuleId;                  // 参数对应模块编号
    unsigned int                        iValueNeed;                 // 是否必须带参数
    unsigned int                        iAttributes;                // 参数属性 0 通用 1 专用
};

// 模块结构信息
struct ST_Module
{
    ST_Module()
    {
        iModuleID = -1;
        iType = -1;
        iToolType = -1;
    }

    int                                 iModuleID;                  // 模块ID
    int                                 iType;                      // 模块类型 0 默认自定义模块 1 外部java可执行文件 2 可执行文件 3 默认经过处理的自定义模块
    int                                 iToolType;                  // 工具类型 1 基础工具 0 其他工具类型
    int                                 iAvailable;                 // 可用标识 0 不可用 1 正常可用(so exe jar 被用作单独模块) 2 不能直接被调用(只是基础组件)
    string                              strModuleName;              // 模块名称
    string                              strSymbol;                  // 模块标识
    string                              strToolName;                // 工具路径 工具路径为相对相对应用程序目录
    string                              strToolGeneralName;         // 通用参数工具名称
    string                              strListParam;               // 参数列表 (为listModuleParam参数集合)
    list<ST_Module_Param*>              listModuleParam;            // 参数列表
    string                              strHelp;                    // 实例代码
    string                              strCheck;                   // 执行成功校验字符串
    string                              strVersion;                 // 版本号
};

// 参数模块信息
struct ST_Param_Module
{
    int                                 iModuleID;                  // 模块ID
    int                                 iType;                      // 模块类型 0 默认自定义模块 1 外部java可执行文件 2 可执行文件
    std::string                         strParam;                   // 工具启动选项
    std::string                         strToolName;                // 工具路径
    std::string                         strListParam;               // 参数列表
    list<ST_Module_Param*>              listParam;                  // 参数详细信息
};

// 任务管理列表
struct ST_Task_Info
{
    ST_Task_Info()
    {
        iTaskId = -1;
        iBlock = 0;
        vObject = NULL;
    }
    int                                 iTaskId;
    int                                 iBlock;                     // 是否阻塞执行 0 否  1 是
    void*                               vObject;                    // 处理对象
    CStringList                         strListParam;
    string                              strApkPath;                 // 待操作对象
    std::list<ST_Param_Module*>         listParamModule;            // 参数任务 入参任务列表
    CStringList                         strListParamCommon;         // 通用参数列表
};


#ifndef MAX_DELAY_TIME
#define MAX_DELAY_TIME                      100*1000*5
#endif

# ifndef I_XML_PRINTER
# define I_XML_PRINTER "AXMLPrinter2.jar"
# endif

# ifndef I_SMALI
# define I_SMALI "smali-2.1.0.jar"
# endif

# ifndef I_SMALI_BACK
# define I_SMALI_BACK "baksmali-2.1.0.jar"
# endif


# ifndef I_XML_MODIFY
# define I_XML_MODIFY "AxmlModify.exe"
# endif

# ifndef I_ZIP_FILE
# define I_ZIP_FILE "myzip.exe"
# endif

#ifdef I_OS_WINDOWS

# ifndef I_DLL_SYMBOL
# define I_DLL_SYMBOL ".dll"
# endif

# ifndef I_SYMBOL_ENTER
# define I_SYMBOL_ENTER "\r\n"
# endif

# ifndef _SYMBOL_PATH_
# define _SYMBOL_PATH_ "\\"
# endif

# ifndef EXTRA_NAME
# define EXTRA_NAME ""
# endif

# ifndef FILE_ATTRIB
# define FILE_ATTRIB 0x20
# endif

# ifndef DIR_ATTRIB
# define DIR_ATTRIB 0x10
# endif

#else
# ifndef I_DLL_SYMBOL
# define I_DLL_SYMBOL ".so"
# endif

# ifndef _SYMBOL_PATH_
# define _SYMBOL_PATH_ "/"
# endif

# ifndef I_SYMBOL_ENTER
# define I_SYMBOL_ENTER "\n"
# endif

# ifndef EXTRA_NAME
# define EXTRA_NAME "lib"
# endif

# ifndef FILE_ATTRIB
# define FILE_ATTRIB 0x08
# endif

# ifndef DIR_ATTRIB
# define DIR_ATTRIB 0x04
# endif

#endif

#endif



