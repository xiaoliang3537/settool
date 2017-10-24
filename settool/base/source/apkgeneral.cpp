#include "apkgeneral.h"
#include "define.h"
#include "register.h"
#include "loger.h"
#include "Utils.h"
#include "FileUtils.h"
#include "StringUtils.h"

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
        LOG(ERROR) << tr("initTask failure !") << endl;
        return iRet;
    }
    // 设置解压文件
    setDecompressFile("","classes*.dex");
    setDecompressFile("","AndroidManifest.xml");

    std::list<ST_Param_Module*>::iterator it = m_stTaskInfo->listParamModule.begin();
    for(; it != m_stTaskInfo->listParamModule.end(); it++ )
    {
        ST_Param_Module* module = *it;
        // 更新包 加密部分
        list<ST_Module_Param*>::iterator itP = module->listParam.begin();
        for(; itP != module->listParam.end(); itP++ )
        {
            ST_Module_Param* param = * itP;
            if(NULL == param )
            {
                LOG(ERROR) << "param info error!" << endl;
                return -1;
            }

            if(param->strFileParam.compare("*") == 0 )
            {
                m_bDecompressAll = true;
                break;
            }
            // 资源隐藏工具配置信息
            if(param->bSubParam)
            {
                list<ST_Module_Param_Sub*>::iterator it = param->listSubParam.begin();
                for(; it != param->listSubParam.end(); it++)
                {
                    ST_Module_Param_Sub* sub = *it;
                    if( sub->strParamValue.length() == 0 )
                    {
                        if(sub->strParamValue.length() == 0 )
                        {
                            if( sub->strFileParam.length() == 0 )
                            {
                                break;
                            }
                            else
                            {
                                if(sub->strFileParam.compare("*") == 0 )
                                {
                                    m_bDecompressAll = true;
                                    break;
                                }
                                setDecompressFile("", sub->strFileParam );
                                break;
                            }
                        }
                        else
                        {
                            setDecompressFile("", sub->strParamValue);
                        }
                        break;
                    }
                    setDecompressFile("", sub->strFileParam );
                }

            }
            if(param->strParamName.compare("-gaso") == 0 )
            {
                if( param->strParamValue.length() == 0 )
                {
                    LOG(ERROR) << "param info error!" << endl;
                    return -1;
                }
                if(0 == param->strParamValue.compare("*") || 0 == param->strParamValue.compare("*.so"))
                {
                    setDecompressFile("", "*.so" );
                    continue;
                }
                else
                {
                    setDecompressFile("", param->strParamValue );
                }
            }
            if(param->strParamName.compare("-gdllpath") == 0 )
            {
                if( param->strParamValue.length() == 0 )
                {
                    LOG(ERROR) << "param info error!" << endl;
                    return -1;
                }
                if(0 == param->strParamValue.compare("*") || 0 == param->strParamValue.compare("*.dll"))
                {
                    setDecompressFile("", "*.dll" );
                    continue;
                }
                else
                {
                    std::vector<std::string> vec = StringUtils::split(param->strParamValue, ";");
                    std::vector<std::string>::iterator it = vec.begin();
                    for(; it != vec.end(); it++ )
                    {
                        string str = *it;
                        str = "*/" + str;
                        setDecompressFile("", str );
                    }
                }
            }
            if( param->strFileParam.length() > 0 )
                setDecompressFile("", param->strFileParam );
        }
        if(m_bDecompressAll)
        {
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
                LOG(ERROR) << tr("cannot location module : ") << param->iModuleID << endl;
                return -1;
            }

#ifdef _DEBUG
            LOG(INFO) << tr("mission: ") << param->iModuleID << endl;
            LOG(INFO) << tr("commond: =  ") << param->strListParam << endl;
#endif

            string strOutPut;
            mysystem(param->strListParam.c_str(), strOutPut);
            if(strOutPut.find( module->strCheck ) == -1 )
            {
                iRet = -1;
                LOG(ERROR) << tr("domodule error, cannot location sucess flag: ") << module->strCheck << endl;
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
                LOG(ERROR) << tr("get tool list error !") << endl;
                return -1;
            }

            m_strConfigFile = m_strWorkPath + _SYMBOL_PATH_ + "temp" + _SYMBOL_PATH_ + m_strTempFilaName + ".ini";
            // 构造文件输入值
            if( 0 != this->writeToFile(m_strConfigFile, m_strList) )
            {
                LOG(ERROR) << tr("construct config error !") << endl;
                return -1;
            }

            // 调用模块处理
            string strOutPut;
            string strparam = moduleParam->strListParam + "-f \"" + m_strTempPath  + "\" -tf \"" + m_strConfigFile + "\"";
//#ifdef _DEBUG
            LOG(INFO) << "strCmd = " << strparam << endl;
//#endif


            mysystem( strparam.c_str(), strOutPut);
            if(strOutPut.find( module->strCheck ) == -1 )
            {
                iRet = -1;
                LOG(ERROR) << tr("domodule error, cannot location sucess flag: ") << module->strCheck << endl;
                return iRet;
            }
        }

        // 清除中间数据
        m_listStDexFile.clear();

        // 设置回编回包信息
        setBackInfo();
//#ifdef _DEBUG
//        list<ST_FIleAddInfo>::iterator itA = m_listStAddFile.begin();
//        for(; itA != m_listStAddFile.end(); itA++ )
//        {
//            ST_FIleAddInfo info = *itA;
//            LOG(INFO) << info.strFilePath << "  "  << info.strPathInZip << endl;
//        }
//#endif

        return iRet;
    }
    return iRet;
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
    if( 0 != this->getFileInDir(m_listFileInfo, m_strTempPath) )
    {
        LOG(ERROR) << tr("get file list failure !") << endl;
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
            if( NULL != strstr(info->name, "smali") )
            {
                std::string p;
                std::string strFilePath;
                std::string strFileName;
                std::string strDexFileName = m_mapSmaliToDex[info->name];
                strFilePath.append(m_strTempPath).append(_SYMBOL_PATH_).append(strDexFileName );
                strFileName.append(strDexFileName);

                setBackSmaliFile(p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->name), strFilePath );
                setAddFile( p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(strDexFileName), strFileName);
            }
            else
            {
                std::string p;
                if( m_bDecompressAll )
                {
                    setDecompressDir( p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->name),info->name );
                }
                else
                {
                    std::vector<std::string> vcPath;
                    std::string strRelativelyPath;
                    std::string strRootDir;
                    strRootDir.assign(m_strTempPath).append(_SYMBOL_PATH_);
                    p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->name);
                    if( 0 != getFileList(p, vcPath) )
                    {
                        LOG(ERROR) << "getFileList error [" << p << "]" << endl;
                        return -1;
                    }

                    for(int i = 0; i < vcPath.size(); i++ )
                    {
                        std::string s = vcPath[i];
                        strRelativelyPath = s.substr(strRootDir.length(), s.length()-strRootDir.length() );
                        setAddFile( s, strRelativelyPath );
                    }
                }
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
            if( NULL != strstr(info->d_name, "smali") )
            {
                std::string p;
                std::string strFilePath;
                std::string strFileName;
                std::string strDexFileName = m_mapSmaliToDex[info->d_name];
                strFilePath.append(m_strTempPath).append(_SYMBOL_PATH_).append(strDexFileName );
                strFileName.append(strDexFileName);

                setBackSmaliFile(p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->d_name), strFilePath );
                setAddFile( p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(strDexFileName), strFileName);
            }
            else
            {
                std::string p;
                if( m_bDecompressAll )
                {
                    setDecompressDir( p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->d_name),info->d_name );
                    LOG(INFO) << "dir=>" << p << endl;
                }
                else
                {
                    std::vector<std::string> vcPath;
                    std::string strRelativelyPath;
                    std::string strRootDir;
                    strRootDir.assign(m_strTempPath).append(_SYMBOL_PATH_);
                    p.assign(m_strTempPath).append(_SYMBOL_PATH_).append(info->d_name);
                    if( 0 != getFileList(p, vcPath) )
                    {
                        LOG(ERROR) << "getFileList error [" << p << "]" << endl;
                        return -1;
                    }

                    for(int i = 0; i < vcPath.size(); i++ )
                    {
                        std::string s = vcPath[i];
                        strRelativelyPath = s.substr(strRootDir.length(), s.length()-strRootDir.length() );
                        setAddFile( s, strRelativelyPath );
                    }
                }
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

int CApkGeneral::getFileList(std::string &strdir, std::vector<std::string> &vecFile)
{

    // 获取目录下 class 文件夹
    int iRet = 0;
    std::list<I_FILE_TYPE*> listFileInfo;
    if( 0 != this->getFileInDir(listFileInfo, strdir) )
    {
        LOG(ERROR) << tr("get file list failure !") << endl;
        return -1;
    }

    std::list<I_FILE_TYPE*>::iterator it = listFileInfo.begin();
    for(; it != listFileInfo.end(); it++ )
    {
#ifdef I_OS_WINDOWS
        // 设置回编dex文件
        I_FILE_TYPE* info = *it;

        if(info->attrib &  DIR_ATTRIB /*_A_SUBDIR*/)
        {
            if(0 == strcmp(info->name, ".") || 0 == strcmp(info->name, "..") )
                continue;

            std::string p;
            p.assign(strdir).append(_SYMBOL_PATH_).append(info->name);
            getFileList(p, vecFile);
        }
        else if(info->attrib & FILE_ATTRIB /*_A_ARCH*/ )
        {
            std::string p;
            p.assign(strdir).append(_SYMBOL_PATH_).append(info->name);
            vecFile.push_back(p);
        }
#else
        // 设置回编dex文件
        I_FILE_TYPE* info = *it;

        if(info->d_type &  DIR_ATTRIB /*_A_SUBDIR*/)
        {
            if(0 == strcmp(info->d_name, ".") || 0 == strcmp(info->d_name, "..") )
                continue;

            std::string p;
            p.assign(strdir).append(_SYMBOL_PATH_).append(info->d_name);
            getFileList(p, vecFile);
        }
        else if(info->d_type & FILE_ATTRIB /*_A_ARCH*/ )
        {
            std::string p;
            p.assign(strdir).append(_SYMBOL_PATH_).append(info->d_name);
            vecFile.push_back(p);
        }

#endif
    }
    return iRet;
}



