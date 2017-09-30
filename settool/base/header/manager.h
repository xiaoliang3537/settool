#ifndef MANAGER_H
#define MANAGER_H

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
#include "managerthread.h"
#include "config.h"
#include "apk.h"
#include "register.h"
#include "config.h"
#include "pubvar.h"
#include "stringlist.h"
#include "library.h"


class CManager
{
public:
    CManager();
    ~CManager();

public:
    int getRunStatus(){return m_iRunStatus;}
    /**
     * @brief init 初始化功能模块 包括加载配置，加载业务模块，初始化公共数据3个部分
     * @return 0 正常 != 0 异常
     */
    int init();

    /**
     * @brief execCmd 同步调用执行指令
     * @param strArg 指令信息 从终端输入或者其他形式输入
     * @param iModuleId 模块编号 要调用的具体业务模块，需要与config.xml配置对应
     * @return 0 正常 != 0 异常
     */
    int execCmd(int argc, char* argv[]);
    /**
     * @brief execCmdEx 异步步调用执行指令 处理过程信息以及结果 通过关联信号 onStatusChanged taskFinished 获取
     * @param strArg 指令信息 从终端输入或者其他形式输入
     * @param iModuleId 模块编号 要调用的具体业务模块，需要与config.xml配置对应
     * @return 0 正常 != 0 异常
     */
    int execCmdEx(int argc, char* argv[]);

    // 获取模块列表
//    QList<ST_Module*> getModuleList()
//    {
//        if(NULL != m_conifg)
//            return m_conifg->getModuleList();
//        return QList<ST_Module*>();
//    }
//    // 获取模块对应参数列表
//    std::list<ST_Module_Param*> getModuleParamList(int iModuleId)
//    {
//        if(NULL != m_conifg)
//            return m_conifg->getModuleParamList(iModuleId);
//        return std::list<ST_Module_Param*>();
//    }
protected :
    int onTaskFinished(int id);
    int onApkFinished(int id);
    int onTimer();
protected:
    int loadModule(int id);
    CApk* loadModuleInner(int id);
    int unLoadModule(int id);

    // 检查参数是否存在
    ST_Module_Param_Sub* checkSubParamExsit(string strParam, list<ST_Module_Param_Sub*> &list);
    bool checkParamDepend(string &strParam, list<ST_Module_Param*> &listParam);
private:
    void addModuleId(list<int> &listModuleID, int moduleID);
private:
    // 对象工厂
    CObjectFactory *                                m_obj;
    CConfig*                                        m_conifg;
    map<int, CLibrary*>                             m_mapLib;
    CPubVar*                                        m_pubvar;
    int                                             m_iModuleId;
    int                                             m_iRunStatus;   //0 空闲 1 繁忙

    CManagerThread*                                 m_managerThread;
};

#endif //MANAGER_H
