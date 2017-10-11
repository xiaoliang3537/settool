#include "apkgeneral.h"
#include "define.h"
#include "register.h"
#include "loger.h"
#include "Utils.h"
#include "FileUtils.h"

CApkGeneral::CApkGeneral()
{


}

CApkGeneral::~CApkGeneral()
{
    this->clearFileInfo(m_listFileInfo);
}

int CApkGeneral::initTask(ST_Task_Info* stTaskInfo, CPubVar* pubvar)
{
    int iRet = 0;
    iRet = CApk::initTask(stTaskInfo, pubvar);
    if(0 != iRet )
    {
        LOG(ERROR) << tr("初始化失败") << endl;
        return iRet;
    }
    // 设置解压文件
    setDecompressFile("","classes*.dex");
    setDecompressFile("test","AndroidManifest.xml");

    std::list<ST_Param_Module*>::iterator it = m_stTaskInfo->listParamModule.begin();
    for(; it != m_stTaskInfo->listParamModule.end(); it++ )
    {
        ST_Param_Module* module = *it;
        if( 123 == module->iModuleID )
        {
            setDecompressDir("", "assets" );
            setDecompressDir("", "lib" );
            break;
        }
    }
    return iRet;
}


int CApkGeneral::doModule()
{
    int iRet = 0;

    if( m_iProcessType == 1 )        // 直接调用
    {
        list<ST_Param_Module*>::iterator iter = m_stTaskInfo->listParamModule.begin();
        for(; iter != m_stTaskInfo->listParamModule.end(); iter++)
        {
            ST_Param_Module* param = *iter;
            ST_Module* module = m_pubvar->m_config->getModuleInfo( param->iModuleID );
            if(NULL == module )
            {
                LOG(ERROR) << tr("当前处理模块异常，无法定位到模块 : ") << param->iModuleID << endl;
                return -1;
            }

#ifdef _DEBUG
            LOG(INFO) << tr("执行任务: ") << param->iModuleID << endl;
            LOG(INFO) << tr("指令: =  ") << param->strListParam << endl;
#endif

            string strOutPut;
            mysystem(param->strListParam.c_str(), strOutPut);
            if(strOutPut.find( module->strCheck ) == -1 )
            {
                iRet = -1;
                LOG(ERROR) << tr("模块处理异常结束，未定位到成功标志 : ") << module->strCheck << endl;
                return iRet;
            }
        }
    }
    else
    {
        std::list<ST_Param_Module*>::iterator it = m_stTaskInfo->listParamModule.begin();
        for( ; it != m_stTaskInfo->listParamModule.end(); it++ )
        {
            ST_Param_Module* moduleParam = *it;
            ST_Module* module = m_pubvar->m_config->getModuleInfo( moduleParam->iModuleID );
            CFileInfo fileInfo(m_stTaskInfo->strApkPath);
            string str = m_pubvar->getWorkPath() + "\\temp\\" + fileInfo.fileName();

            if( 0 != m_pubvar->getGernealToolList(m_strList) )
            {
                LOG(ERROR) << tr("获取工具列表错误") << endl;
                return -1;
            }

//            string tempdir = "tempdir=" + m_strTempPath;
//            m_strList.append(tempdir);
            m_strConfigFile = m_strWorkPath + _SYMBOL_PATH_ + "temp" + _SYMBOL_PATH_ + m_strTempFilaName + ".ini";
            // 构造文件输入值
            if( 0 != this->writeToFile(m_strConfigFile, m_strList) )
            {
                LOG(ERROR) << tr("构造输入配置错误") << endl;
                return -1;
            }

            // 调用模块处理
            string strOutPut;
            string strparam = moduleParam->strListParam + "-f \"" + m_strTempPath  + "\" -tf \"" + m_strConfigFile + "\"";
//            string strparam = "java -jar \"" + CPubVar::GetInstance().getModulePath(module->iModuleID) + "\" -f \"" + m_strTempPath
//                    + "\" " + "-nc";
#ifdef _DEBUG
            LOG(INFO) << "strCmd = " << strparam << endl;
#endif


            mysystem( strparam.c_str(), strOutPut);
            if(strOutPut.find( module->strCheck ) == -1 )
            {
                iRet = -1;
                LOG(ERROR) << tr("模块处理异常结束，未定位到成功标志 : ") << module->strCheck << endl;
                return iRet;
            }

            // 清除中间数据
            m_listStDexFile.clear();

            // 设置回编回包信息
            setBackInfo();
#ifdef _DEBUG
            list<ST_FIleAddInfo>::iterator itA = m_listStAddFile.begin();
            for(; itA != m_listStAddFile.end(); itA++ )
            {
                ST_FIleAddInfo info = *itA;
                LOG(INFO) << info.strFilePath << "  "  << info.strPathInZip << endl;
            }
#endif

        }
        return iRet;
    }

}

int CApkGeneral::doEnd()
{
    int iRet = 0;
    // 删除临时文件
    FileUtils::DeleteDir(m_strTempPath);
    FileUtils::DelFileW(m_strConfigFile);
    return iRet;
}

int CApkGeneral::setBackInfo()
{
    int iRet = 0;

    // 设置待压缩文件夹的时候情况原来的记录
    m_listStDecompressDir.clear();

    // 获取目录下 class 文件夹
    if( 0 != this->getFileInDir(m_listFileInfo) )
    {
        LOG(ERROR) << tr("获取文件列表失败") << endl;
        return -1;
    }

    std::list<I_FILE_TYPE*>::iterator it = m_listFileInfo.begin();
    for(; it != m_listFileInfo.end(); it++ )
    {
#ifdef I_OS_WINDOWS
        // 设置回编dex文件
        I_FILE_TYPE* info = *it;

        if(info->attrib &  DIR_ATTRIB /*_A_SUBDIR*/)
        {
            if(0 == strcmp(info->name, ".") || 0 == strcmp(info->name, "..") )
                continue;

            // 如果匹配到class文件夹 则为需要进行回编的dex文件， 否则为待压缩文件
            if( NULL != strstr(info->name, "classes") )
            {
                std::string p;
                std::string strFilePath;
                std::string strFileName;
                strFilePath.append(m_strTempPath).append(_SYMBOL_PATH_).append(info->name).append( ".dex" );
                strFileName.append(info->name).append(".dex");

                setBackSmaliFile(p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->name), strFilePath );
                setAddFile( p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->name)+".dex", strFileName);
            }
            else
            {
                std::string p;
                setDecompressDir( p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->name),info->name );
            }
        }
        else if(info->attrib & FILE_ATTRIB /*_A_ARCH*/ )
        {
            if(strlen(info->name) > 4 )
            {
                if( 0 == strcmp(info->name + strlen(info->name) - 4, ".dex") )
                {
                    continue;
                }
            }

            std::string p;
            setAddFile( p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->name),info->name );
        }
#else
        // 设置回编dex文件
        I_FILE_TYPE* info = *it;

        if(info->d_type &  DIR_ATTRIB /*_A_SUBDIR*/)
        {
            if(0 == strcmp(info->d_name, ".") || 0 == strcmp(info->d_name, "..") )
                continue;

            // 如果匹配到class文件夹 则为需要进行回编的dex文件， 否则为待压缩文件
            if( NULL != strstr(info->d_name, "classes") )
            {
                std::string p;
                std::string strFilePath;
                std::string strFileName;
                strFilePath.append(m_strTempPath).append(_SYMBOL_PATH_).append(info->d_name).append( ".dex" );
                strFileName.append(info->d_name).append(".dex");

                setBackSmaliFile(p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->d_name), strFilePath );
                setAddFile( p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->d_name)+".dex", strFileName);
            }
            else
            {
                std::string p;
                setDecompressDir( p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->d_name),info->d_name );
            }
        }
        else if(info->d_type & FILE_ATTRIB /*_A_ARCH*/ )
        {
            if(strlen(info->d_name) > 4 )
            {
                if( 0 == strcmp(info->d_name + strlen(info->d_name) - 4, ".dex") )
                {
                    continue;
                }
            }

            std::string p;
            setAddFile( p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->d_name),info->d_name );
        }
#endif
    }
    return iRet;
}

int CApkGeneral::writeToFile(std::string &strFileName, CStringList &strList)
{
    int iRet = 0;
    FILE* f = fopen(strFileName.c_str(), "wb");
    if(NULL == f )
        return -1;
    for(int i = 0; i < strList.length(); i++ )
    {
        fprintf(f, "%s%s", strList.at(i).c_str(), I_SYMBOL_ENTER);
    }
    if(f)
        fclose(f);
    return iRet;
}



