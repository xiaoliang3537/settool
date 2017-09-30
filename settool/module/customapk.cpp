#include "include/Utils.h"
#include "public/fileinfo.h"
#include "customapk.h"

// 定义导出函数
EXPORT_DEF(CCustomapk)
{
    return (void*)(new CCustomapk);
}

void* getinstanceCCustomapk()
{
    return (void*)(new CCustomapk());
}

CCustomapk::CCustomapk()
{
}

CCustomapk::~CCustomapk()
{
}


int CCustomapk::doModule()
{
    int iRet = 0;
//    std::string strCmd;
//#ifdef I_OS_WINDOWS
//    std::string strToolPath = m_strWorkPath + "\\" + m_stModuleInfo->strToolName;
//#else
//    std::string strToolPath = m_strWorkPath + "/" + m_stModuleInfo->strToolName;
//#endif
//    bool b = false;
//    if(m_stModuleInfo->iType == 1 )
//    {
//        b = true;
//        strToolPath += ".jar";
//    }
//    else if(2 == m_stModuleInfo->iType)
//    {
//#ifdef I_OS_WINDOWS
//        strToolPath += ".exe";
//#endif
//    }

//    do
//    {
//        CFileInfo file(strToolPath);
//        if(!file.exists())
//        {
//            // 工具不存在
//            cout < "工具不存在" << endl;
//            break ;
//        }

//        if(b)
//        {
//            strCmd += "java " ;
//            strCmd += "-jar ";
//        }
//        strCmd += strToolPath + " ";
//        strCmd += m_strApkPath + " ";

//        for(int i = 0; i < m_strListArg.count(); i++)
//        {
//            strCmd += m_strListArg.at(i) + " ";
//        }

//        m_process->start( strCmd);

//        if( !m_process->waitForStarted() )
//        {
//            break;
//        }
//        if(!m_process->waitForFinished(MAX_DELAY_TIME) )
//        {
//            break;
//        }
//    }while(0);

    return iRet;
}




