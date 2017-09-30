#include "pubvar.h"
#include "FileUtils.h"
#include "PathUtils.h"
#include "loger.h"

#ifdef I_OS_WINDOWS
#include <Windows.h>
#else
#include <time.h>
#endif

int g_outinfo = 1;

CPubVar::CPubVar()
{
    m_iCurrIndex = 0;
}


CPubVar::~CPubVar()
{
    
}

int CPubVar::init()
{
    int iRet = 0;
    m_config = new CConfig();
    iRet = m_config->init();
    return iRet;
}

unsigned long CPubVar::getRandomCount()
{
#ifdef I_OS_WINDOWS
    return GetTickCount();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
}

int CPubVar::generateRandFileName(std::string str, std::string &strFileName)
{
    char szTemp[256] = {0};
    sprintf(szTemp, "%s_%ld", str.c_str(), getRandomCount() );
    strFileName = szTemp;
    return 0;
}

std::string CPubVar::getWorkPath()
{
    if(m_strWorkPath.length() == 0 )
    {
#ifdef WIN32
        TCHAR szPath[MAX_PATH];
        if( !GetModuleFileName(NULL,szPath,MAX_PATH) )
        {
            printf("GetModuleFileName failed (%d)\n", GetLastError());
            return "";
        }

//        int iLen = WideCharToMultiByte(CP_ACP, 0,szPath, -1, NULL, 0, NULL, NULL);
//        char* chRtn =new char[iLen*sizeof(char)];
//        WideCharToMultiByte(CP_ACP, 0, szPath, -1, chRtn, iLen, NULL, NULL);
//        m_strWorkPath = chRtn;
//        delete [] chRtn;
        m_strWorkPath = szPath;
        m_strWorkPath = FileUtils::ExtractFileDir(m_strWorkPath);
#else
        char current_absolute_path[260];
        if (NULL == realpath("./", current_absolute_path))
        {
            LOG(ERROR) << "get path fail" << endl;
            exit(-1);
        }
        m_strWorkPath = current_absolute_path;
#endif
    }
    return m_strWorkPath;
}

std::string CPubVar::getModulePath(int moduleID)
{
    std::string strPath;
    ST_Module *info = m_config->getModuleInfo(moduleID);
    if(NULL == info)
        return "";
    strPath = m_strWorkPath + _SYMBOL_PATH_ + info->strToolName;
    if(info->iType == 1 || info->iType == 3 )
    {
        strPath+=".jar";
    }
    else if(info->iType == 2 )
    {
#ifdef I_OS_WINDOWS
        strPath+=".exe";
#else
#endif
    }
#ifdef I_OS_WINDOWS
    return strPath;
#else
    return FileUtils::BslToSl(strPath);
#endif
}

std::string CPubVar::getZipPath()
{
    if(m_strZipToolPath.length() == 0 )
    {
        ST_Module *info = m_config->getModuleInfo(TOOL_MYZIP);
        m_strZipToolPath = m_strWorkPath + _SYMBOL_PATH_ + info->strToolName;
        if(info->iType == 1)
        {
            m_strZipToolPath+=".jar";
        }
        else if(info->iType == 2 )
        {
#ifdef I_OS_WINDOWS
            m_strZipToolPath+=".exe";
#endif
        }
    }

#ifdef I_OS_WINDOWS
    return m_strZipToolPath;
#else
    return FileUtils::BslToSl(m_strZipToolPath);
#endif
}

std::string CPubVar::getSmaliPath()
{
    if(m_strSmaliToolPath.length() == 0 )
    {
        ST_Module *info = m_config->getModuleInfo(TOOL_SMALI);
        m_strSmaliToolPath = m_strWorkPath + _SYMBOL_PATH_ + info->strToolName;
        if(info->iType == 1)
        {
            m_strSmaliToolPath+=".jar";
        }
        else if(info->iType == 2 )
        {
#ifdef I_OS_WINDOWS
            m_strSmaliToolPath+=".exe";
#endif
        }
    }

#ifdef I_OS_WINDOWS
    return m_strSmaliToolPath;
#else
    return FileUtils::BslToSl(m_strSmaliToolPath);
#endif
}

std::string CPubVar::getBackSmaliPath()
{
    if(m_strBackSmaliToolPath.length() == 0 )
    {
        ST_Module *info = m_config->getModuleInfo(TOOL_BAKSMALI);
        m_strBackSmaliToolPath = m_strWorkPath + _SYMBOL_PATH_ + info->strToolName;
        if(info->iType == 1)
        {
            m_strBackSmaliToolPath+=".jar";
        }
        else if(info->iType == 2 )
        {
#ifdef I_OS_WINDOWS
            m_strBackSmaliToolPath+=".exe";
#endif
        }
    }
#ifdef I_OS_WINDOWS
    return m_strBackSmaliToolPath;
#else
    return FileUtils::BslToSl(m_strBackSmaliToolPath);
#endif
}

std::string CPubVar::getAxmlModifyPath()
{
    if(m_strAxmlModifyToolPath.length() == 0 )
    {
        ST_Module *info = m_config->getModuleInfo(TOOL_AXMLMODIFY);
        m_strAxmlModifyToolPath = m_strWorkPath + _SYMBOL_PATH_ + info->strToolName;
        if(info->iType == 1)
        {
            m_strAxmlModifyToolPath+=".jar";
        }
        else if(info->iType == 2 )
        {
#ifdef I_OS_WINDOWS
            m_strAxmlModifyToolPath+=".exe";
#endif
        }
    }
#ifdef I_OS_WINDOWS
    return m_strAxmlModifyToolPath;
#else
    return FileUtils::BslToSl(m_strAxmlModifyToolPath);
#endif
}

std::string CPubVar::getAXMLPrinter2Path()
{
    if(m_strAXMLPrinter2ToolPath.length() == 0 )
    {
        ST_Module *info = m_config->getModuleInfo(TOOL_AXMLPRINTER);
        m_strAXMLPrinter2ToolPath = m_strWorkPath + _SYMBOL_PATH_ + info->strToolName;
        if(info->iType == 1)
        {
            m_strAXMLPrinter2ToolPath+=".jar";
        }
        else if(info->iType == 2 )
        {
#ifdef I_OS_WINDOWS
            m_strAXMLPrinter2ToolPath+=".exe";
#endif
        }
    }
#ifdef I_OS_WINDOWS
    return m_strAXMLPrinter2ToolPath;
#else
    return FileUtils::BslToSl(m_strAXMLPrinter2ToolPath);
#endif
}


int CPubVar::getGernealToolList(CStringList& strlist)
{
    return m_config->getGernealToolList(strlist);
}



