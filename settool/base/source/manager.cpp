#include "apk.h"
#include "config.h"
#include "manager.h"
#include "define.h"
#include "task.h"
#include "pubvar.h"
#include "pubfunc.h"
#include "include/Utils.h"
#include "fileinfo.h"
#include "library.h"
#include "loger.h"

#include "apkgeneral.h"
#include "Utils.h"

#ifdef I_OS_WINDOWS
#include <windows.h>
#else
#endif

typedef  void *(*GET_INSTANCE)();
loger *g_loger = new loger();


/***************************************************************************************************************************************/

CManager::CManager()
{
    m_iRunStatus = 0;
    //m_pubvar = new CPubVar();

}

CManager::~CManager()
{


}

int CManager::init()
{
    int iRet = 0;

    //iRet = m_pubvar->init();
    CPubVar::GetInstance().init();
    g_loger->init();
    CPubVar::GetInstance().m_loger = g_loger;
    m_pubvar = &(CPubVar::GetInstance());

    return iRet;
}

int CManager::execCmd(int argc, char* argv[])
{
    LOG(INFO) << "execCmd" << endl;
    int iRet = 0;
    CConfig *config = CPubVar::GetInstance().m_config;

    if(argc == 1 )
    {
        cout << tr("input -help show the help info!") << endl;
        return 1;
    }

    if(argc == 2 )
    {
        if(strcmp(argv[1],"-help") != 0 )
        {
            LOG(ERROR)  << tr("param abnormal! ") << endl;
            return 1;
        }
        else
        {
            config->showHelp();
            return 0;
        }
    }

    list<ST_Module_Param*> listParam;
    list<ST_Module_Param*> listParamGeneral;
    list<int> listModuleID;
    std::string strApkPath;// = argv[1];
    std::string strApkParam;
    CStringList strListParam;
    int iModuleParamId = -1;                // apk文件入参的时候调用
    for(int i = 1; i < argc; i++)
    {
        if( CPubfunc::checkIsParam(argv[i]) )
        {
            ST_Module_Param * stParam= config->m_mapParam[argv[i]];
            if(NULL == stParam)
            {
                if(strcmp(argv[i],"-d") != 0)
                {
                    // 参数错误
                    iRet = -1;
                    LOG(ERROR)  << tr("Unrecognized param !") << "[" << argv[i] << "]" << endl;
                    break;
                }
                // 特别参数
                strListParam.append(argv[i]);
                break;
            }
            ST_Module* module = config->getModuleInfo(stParam->iModuleId);
            if(NULL == module )
            {
                iRet = -1;
                LOG(ERROR) << tr("Unrecognized param !") << "[" << argv[i] << "]" << endl;
                break;
            }
            if(1 != module->iAvailable )
            {
                iRet = -1;
                LOG(ERROR) << tr("Unrecognized param !") << "[" << argv[i] << "]" << endl;
                break;
            }
            ST_Module_Param* param = new ST_Module_Param;
            // copy param value
            *param = *stParam;
            param->listSubParam.clear();

            if( 1 == stParam->iValueNeed )
            {
                if( i+1 >= argc)
                {
                    // 参数错误
                    iRet = -2;
#ifdef I_OS_WINDOWS
                    LOG(INFO) << CPubfunc::KS_UTF8_to_ANSI(stParam->strParamName.c_str()) << tr(" cannot find value") << endl;
#else
#endif
                    break;
                }
                // no sub param
                if( !stParam->bSubParam )
                {
                    if( CPubfunc::checkIsParam(argv[i+1]) )
                    {
                        // 参数错误
    #ifdef I_OS_WINDOWS
                        LOG(ERROR) << CPubfunc::KS_UTF8_to_ANSI(stParam->strParamName.c_str()) << tr(" cannot find value") << endl;
    #else
    #endif
                        iRet = -3;
                        break;
                    }

                    if(strcmp("-infi", argv[i]) == 0 || 0 == strcmp("-hifi", argv[i]) ||
                            0 == strcmp("-fcfi", argv[i]) || 0 == strcmp("-zpfi", argv[i]) ||
                            0 == strcmp("-fifi", argv[i]) || 0 == strcmp("-fift", argv[i]) ||
                            0 == strcmp("-mofi", argv[i]) )
                    {
                        strApkPath = argv[i+1];
                        strApkParam = argv[i];
                        //addModuleId(listModuleID, stParam->iModuleId );
                        iModuleParamId = stParam->iModuleId ;
                        continue;
                    }
                    else
                    {
                        param->strParamValue = argv[i+1];
                    }
                    i++;

                }
                else
                {
                    // check next param
                    ST_Module_Param_Sub* sub = checkSubParamExsit(argv[i+1], stParam->listSubParam);
                    if( NULL == sub )
                    {
                        LOG(ERROR) << "sub param incorrect" << endl;
                        return -1;
                    }

                    int len = 0;
                    ST_Module_Param_Sub* subParam = new ST_Module_Param_Sub;
                    subParam->iValueNeed = sub->iValueNeed;
                    subParam->strParamDesc = sub->strParamDesc;
                    subParam->strParamName = sub->strParamName;
                    subParam->strParamNameL = sub->strParamNameL;
                    subParam->strParamDepend = sub->strParamDepend;
                    subParam->strFileParam = sub->strFileParam;

                    if(sub->iValueNeed)
                    {
                        if(i+2 >= argc )
                        {
                            LOG(ERROR) << "param too small" << endl;
                            return 0;
                        }
                        if( CPubfunc::checkIsParam(argv[i+2]) )
                        {
                            LOG(ERROR) << stParam->strParamName << tr(" cannot find value") << endl;
                            delete subParam;
                            return -1;
                        }
                        len = 2;
                        subParam->strParamValue = argv[i+2];
                    }
                    else
                    {
                        len = 1;
                    }
                    param->listSubParam.push_back(subParam);
                    i += len;
                }


            }
            else
            {
                if( i+1 < argc)
                {
                    if( !CPubfunc::checkIsParam(argv[i+1]) )
                    {
                        param->strParamValue = argv[i+1];
                        i++;
                    }
                }
            }

            // 专用参数加入模块列表
            if(param->iAttributes == 1)
            {
                listParam.push_back(param);
                addModuleId(listModuleID, stParam->iModuleId );
            }
            else        // 通用参数列表
            {
                listParamGeneral.push_back(param);
            }
        }
    }

    if(0 != iRet )
    {
        return 1;
    }

    // 参数路径转换
    strApkPath = abs_path(strApkPath.c_str());

    ST_Task_Info *stTaskInfo = new ST_Task_Info();
    stTaskInfo->strListParamCommon = strListParam;
    ST_Module* moduleR = NULL;
    if(listModuleID.size() == 0 )   // 只有一个入参的情况
    {
        ST_Module* module = config->getModuleInfo(iModuleParamId);
        ST_Param_Module* paramModule = new ST_Param_Module();
        paramModule->iModuleID = iModuleParamId;
        paramModule->iType = module->iType;
        paramModule->strToolName = CPubVar::GetInstance().getWorkPath() + _SYMBOL_PATH_ + "module" + _SYMBOL_PATH_ + module->strToolName;

        ST_Module_Param * param = config->m_mapParam[strApkParam.c_str()];

        string strCmd;
        if(module->iType == 1)
        {
            strCmd = "java -jar \"" + CPubVar::GetInstance().getModulePath(module->iModuleID) + "\" " + param->strParamNameL + " \"" + strApkPath + "\" " ;
#ifdef _DEBUG
            LOG(INFO) << "path =>" << CPubVar::GetInstance().getWorkPath().c_str() << endl;
#endif
        }
        else if(module->iType == 2)
        {
#ifdef I_OS_WINDOWS
            strCmd = "\"" + CPubVar::GetInstance().getModulePath(module->iModuleID) + "\" " + param->strParamNameL + " \"" + strApkPath  + "\" " ;
#else
            strCmd = "\"" + CPubVar::GetInstance().getModulePath(module->iModuleID) + "\" " + param->strParamNameL + " \"" + strApkPath  + "\" " ;

#endif
            LOG(INFO) << "path =>  =>" << CPubVar::GetInstance().getWorkPath().c_str() << endl;
        }
        else if(module->iType == 3 )
        {
            strCmd = "java -jar \"" + CPubVar::GetInstance().getModulePath(module->iModuleID) + "\" " + param->strParamNameL ;
        }

        paramModule->listParam.push_back(param);
        paramModule->strListParam = strCmd;
        stTaskInfo->listParamModule.push_back(paramModule);
        moduleR = module;
    }
    else
    {

        int iPre = -1;           // 默认执行方式
        int iCurr = -1;           // 默认执行方式
        bool b = true;
        list<int>::iterator iterM = listModuleID.begin();
        for( ; iterM != listModuleID.end(); iterM++ )
        {
            int iModuleID = *iterM;
            // 循环执行任务
            ST_Module* module = config->getModuleInfo(iModuleID);
            // 组合模块参数
            list<ST_Module_Param*>::iterator iter = listParam.begin();
            list<ST_Module_Param*>::iterator iterG = listParamGeneral.begin();
            ST_Param_Module* paramModule = new ST_Param_Module();
            paramModule->iModuleID = iModuleID;
            paramModule->iType = module->iType;
            paramModule->strToolName = CPubVar::GetInstance().getWorkPath() + _SYMBOL_PATH_ + "module" + _SYMBOL_PATH_ + module->strToolName;

            string strCmd;

            // 通用参数
            for(; iterG != listParamGeneral.end(); iterG++)
            {
                ST_Module_Param *paramG = *iterG;
                list<ST_Module_Param*>::iterator iter = module->listModuleParam.begin();
                for( ; iter != module->listModuleParam.end(); iter++ )
                {
                    ST_Module_Param *param = *iter;
                    if(paramG->strParamName == param->strParamName)
                    {
                        strCmd += param->strParamNameL;
                        if(paramG->strParamValue.length() != 0 )
                        {
                            strCmd += " " + paramG->strParamValue;
                        }
                        strCmd += " ";

                        paramModule->listParam.push_back(param);

                    }
                }
            }
            // 专属参数
            for(; iter != listParam.end(); iter++)
            {
                ST_Module_Param* param = *iter;
                if( param->iModuleId == module->iModuleID )
                {
                    strCmd += param->strParamNameL;
                    // check depend param
                    if(param->strParamDepend.length() > 0 )
                    {
                        if( !checkParamDepend(param->strParamDepend, listParam) )
                        {
                            LOG(INFO) << "param depend cannot find:" << param->strParamDepend << endl;
                            return 0;
                        }
                    }
                    if(param->bSubParam)
                    {
                        if(param->listSubParam.size() > 0 )
                        {
                            ST_Module_Param_Sub* sub = *(param->listSubParam.begin());
                            strCmd += " " + sub->strParamNameL;

                            if( sub->strParamDepend.length() > 0 )
                            {
                                if( !checkParamDepend(sub->strParamDepend, listParam) )
                                {
                                    LOG(INFO) << "param depend cannot find:" << sub->strParamDepend << endl;
                                    return 0;
                                }
                            }

                            if( sub->strParamValue.length() != 0 )
                            {
                                if(sub->iParamValueType == 1)
                                {
                                    sub->strParamValue = abs_path(sub->strParamValue.c_str());
                                }
                                strCmd += " \"" + sub->strParamValue + "\"";
                            }
                        }
                    }
                    else
                    {
                        if( param->strParamValue.length() != 0 )
                        {
                            if(param->iParamValueType == 1)
                            {
                                param->strParamValue = abs_path(param->strParamValue.c_str());
                            }
                            //strCmd += " " + param->strParamValue;
                            strCmd += " \"" + param->strParamValue + "\"";
                        }
                    }
                    strCmd += " ";
                    paramModule->listParam.push_back(param);
                }
            }
            stTaskInfo->listParamModule.push_back(paramModule);

            // 调用任务
            ST_Module_Param* param = config->getParamInModule(module->iModuleID, strApkParam);
            if(NULL == param )
            {
                LOG(ERROR) << tr("input param type error !") << endl;
                return -1;
            }

            if(module->iType == 1)
            {

                iCurr = 1;
                strCmd = "java -jar \"" + CPubVar::GetInstance().getModulePath(module->iModuleID) + "\" " + param->strParamNameL + " \"" + strApkPath + "\" " + strCmd ;
                LOG(INFO) << "cmd = " << strCmd << endl;
            }
            else if(module->iType == 2)
            {
                iCurr = 1;
#ifdef I_OS_WINDOWS
                strCmd = "\"" + CPubVar::GetInstance().getModulePath(module->iModuleID) + "\" " + strCmd;
#else
                strCmd = "\"" + CPubVar::GetInstance().getModulePath(module->iModuleID) + "\" " + strCmd;
#endif
                LOG(INFO) << "cmd = " << strCmd << endl;
            }
            else if(module->iType == 0 )
            {
                iCurr = 0;
            }
            else if(module->iType == 3 )
            {
                strCmd = "java -jar \"" + CPubVar::GetInstance().getModulePath(module->iModuleID) + "\" " + strCmd ;
                LOG(INFO) << "cmd = " << strCmd << endl;
            }

            if(iPre != -1)
            {
                if(iPre != iCurr)
                {
                    b = false;
                    break;
                }
                iPre = iCurr;
            }
            else
            {
                iPre = iCurr;
                moduleR = module;
            }

            paramModule->strListParam = strCmd;
        }

        if(!b)
        {
            LOG(ERROR) << tr("unsupported instruction !") <<  endl;
            return 1;
        }

    }
    stTaskInfo->strApkPath = strApkPath;

    // 直接执行指令
    {
#ifdef _DEBUG
        LOG(INFO) << "module id = " << moduleR->iModuleID << "  module info = " << moduleR->strSymbol << endl;
#endif
//        loadModule(moduleR->iModuleID);
//        GET_INSTANCE fun = CObjectFactory::getObject(moduleR->strSymbol);
//        if( NULL == fun )
//            return -1;
//        CApk* apk = (CApk*)fun();
        CApk* apk = loadModuleInner(moduleR->iModuleID);
        if( NULL != apk)
        {
            iRet = apk->initTask(stTaskInfo, m_pubvar);
            if(0 != iRet )
            {
                LOG(ERROR) << tr("init task error! ") << endl;
                return -1;
            }
            iRet = apk->start();
            if(0 != iRet )
            {
                LOG(ERROR) << tr("do task error !") << endl;
                return -1;
            }
            else
            {
                LOG(INFO) << tr("done!") << endl;
            }
        }
        else
        {
            LOG(ERROR) << tr("mission data error! ") << endl;
            return 1;
        }
        if(NULL != apk)
            delete apk;
    }

    return iRet;
}

int CManager::execCmdEx(int argc, char* argv[])
{
    int iRet = 0;

//    m_managerThread = new CManagerThread();

//    m_managerThread->init(m_pubvar);
//    m_managerThread->start();

//    m_iRunStatus = 1;

//    // 返回当前任务编号 便于其他操作
//    //iRet = stTaskInfo.iTaskId;
//    Sleep(3000);
//    do
//    {
//        if(strListArg.length() < 1 )   // 参数数量不对
//            return 1;

//        // 获取模块信息
//        ST_Module* module = NULL;
//        for(int i = 1; i < strListArg.length(); i++ )
//        {
//            module = m_pubvar->m_config->getParamModule(strListArg.at(i));
//            if(NULL != module )
//            {
//                break;
//            }
//        }

//        // apk包路径
//        QString strPath = strListArg.at(0);
//        QStringList listParam = strListArg;
//        // 参数列表
//        listParam.removeAt(0);

//        // 获取处理对象
//        GET_INSTANCE fun = m_obj->getObject(module->strSymbol.toStdString());
//        CApk* apk = (CApk*)fun();

//        ST_Task_Info st;
//        st.iBlock = 1;
//        st.iTaskId = m_pubvar->getNextIndex();
//        st.strListParam = listParam;
//        st.strApkPath = strPath;
//        st.vObject = (void*)apk;
//        m_managerThread->addTask(&st);

//    }while(0);

    return iRet ;
}

int CManager::onTaskFinished(int id)
{
//    if(  NULL != dynamic_cast<CManagerThread*>(sender()))
//    {
//        qDebug() << "CManagerThread end";
//    }
        return 0;
}

int CManager::onApkFinished(int id)
{
//    CApk* apk = (CApk*)sender();
//    if(NULL != apk)
//        delete apk;
//    m_iRunStatus = 0;
//    qDebug() << "apk finished ...";
        return 0;
}

int CManager::onTimer()
{
    return 0;
}

int CManager::loadModule(int id)
{
    int iRet = 0;
    if( m_mapLib.find(id) != m_mapLib.end() )
    {
        return 0;
    }
    char szTemp[128];

    ST_Module* module = CPubVar::GetInstance().m_config->getModuleInfo(id);
    if(NULL == module)
        return -1;

    string strSymbol = module->strSymbol;
    do
    {
        memset(szTemp, 0x0, 128);
        sprintf(szTemp, "%s%smodule%s%s%s%s",CPubVar::GetInstance().getWorkPath().c_str(),_SYMBOL_PATH_,_SYMBOL_PATH_,EXTRA_NAME,strSymbol.c_str(), I_DLL_SYMBOL);
        // 模块文件不存在 无法加载
        string strFile = szTemp;
        CFileInfo file(strFile);
        if( !file.exists() )
        {
            LOG(ERROR) << tr("module file unexist !") << szTemp << endl;
            break;
        }
        strFile = szTemp;
        CLibrary *lib = new CLibrary( strFile );
        if( !lib->load() )
        {
            iRet = -1;
            LOG(ERROR) << tr("load file error :") << szTemp << endl;
            break;
        }
        memset(szTemp, 0x0, sizeof(szTemp));
        sprintf(szTemp, "getInstance%s", strSymbol.c_str());
        GET_INSTANCE getInstance = (GET_INSTANCE)lib->resolve( szTemp );
        // 注册组件
        if(NULL == getInstance)
        {
            iRet = -2;
            LOG(ERROR) << tr("load interface failure") << szTemp << endl;
            break;
        }
        CObjectFactory::Register(module->strSymbol,getInstance);

        m_mapLib.insert(pair<int, CLibrary*>(id,lib) );
    }while (0);

    return iRet;
}

CApk* CManager::loadModuleInner(int id)
{
    int iRet = 0;
    if( m_mapLib.find(id) != m_mapLib.end() )
    {
        return NULL;
    }
    char szTemp[128];

    ST_Module* module = CPubVar::GetInstance().m_config->getModuleInfo(id);
    if(NULL == module)
        return NULL;

    string strSymbol = module->strSymbol;
    CApk* apk = NULL;
    switch (module->iModuleID) {
    default:
        apk = new CApkGeneral;
        break;
    }

    return apk;
}

int CManager::unLoadModule(int id)
{
    int iRet = 0;
    map<int, CLibrary*>::iterator iter = m_mapLib.find(id);
    if(iter == m_mapLib.end() )
    {
        return 0;
    }
    CLibrary *lib = iter->second;
    if(NULL != lib)
    {
        m_mapLib.erase(iter);
        lib->unload();
        delete lib;
    }

    return iRet;
}


void CManager::addModuleId(list<int> &listModuleID, int moduleID)
{
    int iSize = listModuleID.size();
    if( iSize == 0 )
        return listModuleID.push_back(moduleID);
    bool b = false;
    list<int>::iterator iter = listModuleID.begin();
    for(; iter != listModuleID.end(); iter++)
    {
        int i = (int)(*iter);
        if(i == moduleID)
        {
            b = true;
            break;
        }
        if( i > moduleID )
        {
            if(iter == listModuleID.begin() )
            {
                listModuleID.push_front(moduleID);
            }
            else
            {
                listModuleID.insert(iter, moduleID);
            }
            b = true;
            break;
        }
    }

    if( !b )
    {
        listModuleID.insert(iter, moduleID);
        return ;
    }
}


ST_Module_Param_Sub* CManager::checkSubParamExsit(string strParam, std::list<ST_Module_Param_Sub*> &list)
{
    if(strParam.length() == 0 || list.size() == 0 )
        return NULL;

    std::list<ST_Module_Param_Sub*>::iterator it = list.begin();
    for(; it != list.end(); it++ )
    {
        ST_Module_Param_Sub *info = *it;
        if( info->strParamName.compare(strParam) == 0 )
        {
            return info;
        }
    }
    return NULL;
}


bool CManager::checkParamDepend(string &strParam, list<ST_Module_Param*> &listParam)
{
    list<ST_Module_Param*>::iterator it = listParam.begin();
    for(; it != listParam.end(); it++ )
    {
        ST_Module_Param* info = *it;
        if(0 == info->strParamName.compare(strParam))
        {
            return true;
        }
    }
    return false;
}
