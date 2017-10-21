#include "config.h"
#include "define.h"
#include "Markup.h"
#include "pubvar.h"
#include "pubfunc.h"
#include <stdio.h>
#include <math.h>
#include <cstring>
#include <iomanip>
#include "PathUtils.h"
#include "loger.h"

#ifdef I_OS_WINDOWS
#include <comdef.h>
#include <windows.h>
#else
#endif

std::string g_strWorkPath;

CConfig::CConfig()
{

}

CConfig::~CConfig()
{

}

int CConfig::init()
{
    int iRet = 0;
    iRet = this->loadConfig();

    return iRet;
}



ST_Module* CConfig::getModuleInfo(int paramId)
{
    std::map<int, ST_Module*> tt = m_mapModule;
    std::map<int, ST_Module*>::iterator iter = m_mapModule.find(paramId);
    if(iter != m_mapModule.end() )
    {
        return (ST_Module*)(iter->second);
    }
    return NULL;
}


int CConfig::loadConfig()
{
    int iRet = 0;
    string strConfigPath = CPubVar::GetInstance().getWorkPath() + _SYMBOL_PATH_ + "config" + _SYMBOL_PATH_ + "config.xml" ;
    CMarkup objXml;
    do
    {
        LOG(INFO) << strConfigPath << endl;
        if(!objXml.Load(strConfigPath.c_str()))
        {
            LOG(ERROR) << "load xml fail" << endl;
            iRet = -1;
            break;
        }
        if(!objXml.FindElem("resources"))
        {
            iRet = -2;
            LOG(ERROR) << "FindElem resources xml fail" << endl;
            break;
        }
        objXml.IntoElem();
        while(1)
        {    
            if(!objXml.FindElem("module"))
            {
                objXml.OutOfElem();
                break;
            }
            ST_Module *module = new ST_Module();
            module->iModuleID = atoi( objXml.GetAttrib("id").c_str() );
            module->strModuleName = objXml.GetAttrib(("name") ) ;
            module->strSymbol = objXml.GetAttrib("symbol");
            module->iType = atoi(objXml.GetAttrib("type").c_str());
            module->iAvailable = atoi(objXml.GetAttrib("available").c_str());
            module->strToolName = objXml.GetAttrib("toolname");
            module->strHelp = objXml.GetAttrib("help");
            module->strCheck = objXml.GetAttrib("check");
            module->strVersion = objXml.GetAttrib("version");
            if(module->strCheck.length() == 0 )
            {
                module->strCheck = "sucess";
            }

            objXml.IntoElem();
            while(1)
            {
                if(!objXml.FindElem(("param")))
                {
                    objXml.OutOfElem();
                    break;
                }
                ST_Module_Param* stparam = new ST_Module_Param();
                stparam->iModuleId = module->iModuleID;
                stparam->strParamName = objXml.GetAttrib("name");
                stparam->strParamNameL = objXml.GetAttrib("name1");
                stparam->iValueNeed = atoi( objXml.GetAttrib("valueneed").c_str() );
                stparam->strParamDesc = objXml.GetAttrib("desc");
                stparam->iDefault = atoi( objXml.GetAttrib("default").c_str() );
                stparam->iAttributes = atoi( objXml.GetAttrib("attributes").c_str() );
                stparam->strParamDepend = objXml.GetAttrib("depend").c_str();
                // 参数类型
                stparam->iParamValueType = atoi(objXml.GetAttrib("valuetype").c_str()) ;
                stparam->strFileParam = objXml.GetAttrib("file");
                if( objXml.IntoElem() )
                {
                    while(1)
                    {
                        if(!objXml.FindElem(("subparam")))
                        {
                            objXml.OutOfElem();
                            break;
                        }

                        stparam->bSubParam = true;
                        ST_Module_Param_Sub* sub = new ST_Module_Param_Sub;
                        sub->strParamName = objXml.GetAttrib("name");
                        sub->strParamNameL = objXml.GetAttrib("name1");
                        sub->iValueNeed = atoi( objXml.GetAttrib("valueneed").c_str() );
                        sub->strParamDesc = objXml.GetAttrib("desc");
                        sub->strParamDepend = objXml.GetAttrib("depend");
                        // 参数类型
                        sub->iParamValueType = atoi(objXml.GetAttrib("valuetype").c_str()) ;
                        sub->strFileParam = objXml.GetAttrib("file");
                        stparam->listSubParam.push_back(sub);
                    }
                }

                module->listModuleParam.push_back(stparam);

                m_mapParamModule.insert(pair<string,int>(stparam->strParamName, stparam->iModuleId));
                m_mapParam.insert(pair<string,ST_Module_Param*>(stparam->strParamName, stparam));
            }
            m_mapModule.insert(pair<int,ST_Module*>(module->iModuleID,module));
            if(module->iModuleID < 100 )
            {
                m_mapGeneralModule.insert(pair<int, ST_Module*>(module->iModuleID, module));
            }
        }
    }while(0);
    return iRet;
}

ST_Module* CConfig::getParamModule(string strParam)
{
    ST_Module* module = NULL;
//    do
//    {
//        if(0 == strParam.length())
//        {
//            break;
//        }
//        list<ST_Module*> listModule = m_mapModule.values();

//        if(listModule.count() == 0 )
//            break;
//        for(int i = 0; i < listModule.count();i++)
//        {
//            if(listModule.at(i)->strListParam.contains(strParam))
//            {
//                module = m_mapModule.value(listModule.at(i)->iModuleID);
//            }
//        }
//    }while(0);

    return module;
}

ST_Module_Param* CConfig::getParamInModule(int moduleId, string strParam)
{
    ST_Module* module = m_mapModule[moduleId];
    if(NULL == module )
        return NULL;

    list<ST_Module_Param*>::iterator it = module->listModuleParam.begin();
    for(; it != module->listModuleParam.end(); it++ )
    {
        ST_Module_Param* param = *it;
        if(NULL == param )
            return NULL;
        if(param->strParamName.compare(strParam) == 0 )
        {
            return param;
        }
    }
    return NULL;
}


int CConfig::getGernealToolList(CStringList& strlist)
{
    int iRet = 0;
    std::map<int, ST_Module*>::iterator iter = m_mapGeneralModule.begin();
    for(; iter != m_mapGeneralModule.end(); iter++)
    {
        ST_Module* module = (ST_Module*)(iter->second);
        if(module->iModuleID < 100 && module->iAvailable != 0 )
        {
            string strPath = CPubVar::GetInstance().getWorkPath() + _SYMBOL_PATH_ + module->strToolName;
#ifdef I_OS_WINDOWS
#else
            strPath =FileUtils::BslToSl(strPath);
#endif
            if(module->iType == 1)
            {
                strPath += ".jar";
            }
            else if(module->iType == 2 )
            {
    #ifdef I_OS_WINDOWS
                strPath+=".exe";
    #endif
            }
            std::string str;
            str = module->strSymbol + "=" + strPath;
            strlist.append(str);
        }
    }
    return iRet;
}

int CConfig::showHelp()
{
    std::map<int, ST_Module*>::iterator iter = m_mapModule.begin();
    for(; iter != m_mapModule.end(); iter++)
    {
        ST_Module* module = (ST_Module*)(iter->second);
        if(module->iAvailable == 1 )
        {
#ifdef I_OS_WINDOWS
            cout << CPubfunc::KS_UTF8_to_ANSI(module->strModuleName.c_str()) << tr(" version:") << module->strVersion << endl;
#else
            cout << module->strModuleName.c_str() << tr("param list") << endl;
#endif
        }
        if( module->iAvailable == 1 )
        {
            std::list<ST_Module_Param*>::iterator it = module->listModuleParam.begin();
            for(; it != module->listModuleParam.end(); it++ )
            {
                ST_Module_Param* param = (ST_Module_Param*)(*it);
#ifdef I_OS_WINDOWS
                cout << setw(10) << param->strParamName << "  " << CPubfunc::KS_UTF8_to_ANSI(param->strParamDesc.c_str()) << endl;
                if(param->bSubParam)
                {
                    list<ST_Module_Param_Sub*>::iterator sub = param->listSubParam.begin();
                    for(; sub != param->listSubParam.end(); sub++ )
                    {
                        ST_Module_Param_Sub* subParam = *sub;
                        cout << setw(10) << subParam->strParamName << "      " << CPubfunc::KS_UTF8_to_ANSI(subParam->strParamDesc.c_str()) << endl;
                    }
                }
#else
                cout << setw(10) << param->strParamName << "  " << param->strParamDesc.c_str() << endl;
                if(param->bSubParam)
                {
                    list<ST_Module_Param_Sub*>::iterator sub = param->listSubParam.begin();
                    for(; sub != param->listSubParam.end(); sub++ )
                    {
                        ST_Module_Param_Sub* subParam = *sub;
                        cout << setw(10) << subParam->strParamName << "      " << subParam->strParamDesc.c_str() << endl;
                    }
                }
#endif
            }
        }
    }
    return 0;
}

