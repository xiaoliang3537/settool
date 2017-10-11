#include "apk.h"
#include "pubvar.h"
#include "config.h"
#include "pubfunc.h"
#include "fileinfo.h"
#include "stringlist.h"
#include "include/Utils.h"
#include "FileUtils.h"
#include "zipfile.h"

CApk::CApk()
{
    m_bDecompressAll = false;
    m_pubvar = NULL;
    m_iProcessRet = -1;
    m_iProgramId = 0;
    m_iApkCount = 0;
    m_iProcessType = -1;
    m_stModuleInfo = new ST_Module();
    m_strWorkPath = CPubVar::GetInstance().getWorkPath();

    m_mapDexToSmali.insert(pair<string,string>("classes.dex", "smali"));
    m_mapDexToSmali.insert(pair<string,string>("classes2.dex", "smali_2"));
    m_mapDexToSmali.insert(pair<string,string>("classes3.dex", "smali_3"));
    m_mapDexToSmali.insert(pair<string,string>("classes4.dex", "smali_4"));
    m_mapDexToSmali.insert(pair<string,string>("classes5.dex", "smali_5"));

    m_mapSmaliToDex.insert(pair<string,string>("smali", "classes.dex"));
    m_mapSmaliToDex.insert(pair<string,string>("smali_2", "classes2.dex"));
    m_mapSmaliToDex.insert(pair<string,string>("smali_3", "classes3.dex"));
    m_mapSmaliToDex.insert(pair<string,string>("smali_4", "classes4.dex"));
    m_mapSmaliToDex.insert(pair<string,string>("smali_5", "classes5.dex"));

}

CApk::~CApk()
{

}

int CApk::initTask(ST_Task_Info* stTaskInfo, CPubVar* pubvar )
{
    g_loger = pubvar->m_loger;

#ifdef _DEBUG
    LOG(INFO) << tr("初始化APK信息") << endl;
#endif

    if(NULL == stTaskInfo)
    {
#ifdef _DEBUG
        LOG(INFO) << tr("任务信息为空") << endl;
#endif
        return -1;
    }

    // 目前支持自定义工具的命令条目长度只能为1
    std::list<ST_Param_Module*>::iterator it = stTaskInfo->listParamModule.begin();
    for( ; it != stTaskInfo->listParamModule.end(); it++ )
    {
        ST_Param_Module* info = *it;
        if( 3 == info->iType || 0 == info->iType || 2 == info->iType )                  // 经过改造的调用类型
        {
            m_iProcessType = 2;
        }
        else                                    // 默认类型只支持一次调用一个模块
        {
            if(stTaskInfo->listParamModule.size() == 0 )
            {
#ifdef _DEBUG
                LOG(INFO) << "mission count error!" << endl;
#endif
                return -2;
            }
            m_iProcessType = 1;
        }
    }


    m_stTaskInfo = stTaskInfo;
    m_config = pubvar->m_config;
    CPubVar::GetInstance().m_config = m_config;
    list<ST_Param_Module*>::iterator iter = m_stTaskInfo->listParamModule.begin();
    ST_Param_Module* param = *iter;
    this->m_stModuleInfo = m_config->getModuleInfo(param->iModuleID);
    this->m_strListArg = param->strParam;
    this->m_stTaskInfo = stTaskInfo;
    this->m_pubvar = pubvar;
    m_iProgramId = m_stModuleInfo->iModuleID;

    // 参数列表 检查参数以及对应的值
    CFileInfo fileInfo(m_stTaskInfo->strApkPath);
    if( !fileInfo.exists() )
    {
        LOG(ERROR) << "input apk file incorrect:" << m_stTaskInfo->strApkPath << endl;
        return 1;
    }

    m_strFilaName = fileInfo.fileName();
    this->m_pubvar = pubvar;
    this->m_strApkPath = fileInfo.fileName();

    m_strApkPath = m_stTaskInfo->strApkPath;
    m_strWorkPath = pubvar->getWorkPath();

    pubvar->generateRandFileName(m_strFilaName, m_strTempFilaName);

    this->m_strTempPath =  m_strWorkPath + _SYMBOL_PATH_ + "temp" + _SYMBOL_PATH_ + m_strTempFilaName;                       // 处理缓存文件目录
    CDir dir(m_strTempPath);
    if( !dir.exists() )
    {
        dir.mkdir(m_strTempPath.c_str());
    }

    // 设置解压文件
    return 0;
}

int CApk::start()
{
    int iRet = 0;

    if(NULL != m_pubvar)
    {
        m_iProgramId = m_pubvar->getNextIndex();
    }
    if(m_iProcessType != 2 )            // 直接执行
    {
        iRet = this->doModule();
        if(0 != iRet )
        {
            LOG(ERROR) << "doModule failure !" << endl;
            return -2;
        }
    }
    else
    {
        int iType = m_stModuleInfo->iType;
        if( iType == 3 || 0 == iType || 2 == iType )
        {
            int iRet = 0;
            LOG(INFO) << tr("start initApk") << endl;
            iRet = initApk();
            if(0 != iRet )
            {
                LOG(ERROR) << tr("initApk failure!") << " iRet = " << iRet << endl;
                return -1;
            }
            LOG(INFO) << tr("start doModule!") << endl;
            iRet = this->doModule();
            if(0 != iRet )
            {
                LOG(ERROR) << tr("doModule failure") << " iRet = " << iRet << endl;
                return -2;
            }
            LOG(INFO) << tr("start Compile dex") << endl;
            iRet = backPackDexFile();
            if(0 != iRet )
            {
                LOG(ERROR) << tr("Compile dex file failure!") << " iRet = " << iRet << endl;
                return -3;
            }
            LOG(INFO) << tr("do backPack") << endl;
            iRet = backPack();
            if(0 != iRet )
            {
                LOG(ERROR) << tr("backPack failure!") << " iRet = " << iRet << endl;
                return -4;
            }
            LOG(INFO) << tr("do end!") << endl;
            iRet = doEnd();
            if(0 != iRet )
            {
                return -5;
            }
        }
        else
        {
            return -99;
        }
    }
    return iRet ;
}

int CApk::doEnd()
{
    int iRet = 0;
    return iRet;
}

int CApk::initApk()
{
    int iRet = 0;
    // 解压文件
    if( m_listStDecompressFile.size() != 0 )
    {
        if( 0 != this->zipDeCompress() )
        {
            LOG(ERROR) << "DeCompress file failure" << endl;
            return -1;
        }
    }

    // 反编译dex文件
    if( m_strListDex.length() != 0 )
    {
        if(0 != this->decompileDexFile() )
        {
            LOG(ERROR) << "backSmali dex file failure" << endl;
            return -1;
        }
    }

    return iRet;
}


int CApk::checkParam(CStringList& strList)
{
    int iRet = 0;
    return iRet;
}

// 解压文件
int CApk::zipDeCompress(CStringList strListFile)
{
    int iRet = 0;
//    std::string strFilePath;
//    m_strFileList = strListFile;
//    for(int i = 0; i < strListFile.length(); i++ )
//    {
//        // 解压文件
//        strFilePath = this->m_strTempPath + "\\" + strListFile.at(i);
//        iRet = this->zipDeCompress(this->m_strApkPath, strListFile.at(i), strFilePath);
//        if(0 != iRet )
//        {
//            // 解压文件失败
//            break;
//        }
//    }
    return iRet;
}

// 反编译
int CApk::decompileDexFile()
{
    int iRet = 0;
    string strCmd, strToolPath, strOutPath, strOut;
    strToolPath = m_pubvar->getBackSmaliPath();

    //　确定需要进行反编译的dex
    for(int i = 0; i < m_strListDex.length(); i++ )
    {
        if( /*name.compare(strDex.c_str()) == 0 */1)
        {
            std::string name = m_strListDex.at(i);
            CFileInfo file(name);

            strOutPath = file.fileDir() + _SYMBOL_PATH_ +  m_mapDexToSmali[file.fileFullName()]; //file.fileName();
            FileUtils::MakeSureDirExsits(strOutPath);
            strCmd = "java -jar \"" + strToolPath + "\" \"" + m_strListDex.at(i) + "\" -o \"" + strOutPath + "\"";
#ifdef _DEBUG
            LOG(INFO) << strCmd << endl;
#endif
            mysystem(strCmd.c_str(), strOut);

            // 检查反编译结果
            checkOutPut(strOut);

            // 设置需要进行回编的Dex
            setBackSmaliFile(strOutPath, "" );
        }
    }
    return iRet;
}

// 编译dex
int CApk::backPackDexFile()
{
    int iRet = 0;
    string strCmd, strToolPath, strOut;
    strToolPath = m_pubvar->getSmaliPath();
    list<ST_FIleAddInfo>::iterator iter = m_listStDexFile.begin();
    for(; iter != m_listStDexFile.end(); iter++ )
    {
        ST_FIleAddInfo stInfo = *iter;

        strCmd = "java -jar \"" + strToolPath + "\" \"" + stInfo.strFilePath + "\" -o \"" + stInfo.strPathInZip + "\"";
#ifdef _DEBUG
        LOG(INFO) << "smali dex file :" << strCmd << endl;
#endif
        if( checkFileExsit(stInfo.strPathInZip.c_str()) )
        {
            if( !FileUtils::DelFileW(stInfo.strPathInZip) )
            {
                LOG(ERROR) << "delete file failure :" << stInfo.strPathInZip << endl;
                return -1;
            }
        }
        mysystem(strCmd.c_str(), strOut);

        // 检查编译结果 检查文件 stInfo.strPathInZip 是否存在
        if( !checkFileExsit(stInfo.strPathInZip.c_str()) )
        {
            LOG(ERROR) << "smali dex file failure:" << stInfo.strPathInZip << endl;
            return 1;
        }

        // 删除缓存目录
        if(FileUtils::DirExists(stInfo.strFilePath))
        {
            FileUtils::DeleteDir(stInfo.strFilePath);
        }
    }

    return iRet;
}

// 回包
int CApk::backPack()
{
    int iRet = 0;
    // 检查需要回包的文件是否正常
    list<ST_FIleAddInfo>::iterator iter = m_listStAddFile.begin();
    // 保留原包
    if(m_strTempApkPath.length() == 0 )
    {
        m_strTempApkPath.append(FileUtils::ExtractFileRemoveExt(m_strApkPath)).append(m_strTempFilaName).append("_New.zip");
        LOG(INFO) << "temp apk path=" << m_strTempApkPath << endl;
    }

    if( !m_bDecompressAll )
    {
        // 删除原来的文件
        for(; iter != m_listStAddFile.end(); iter++)
        {
            ST_FIleAddInfo info = *iter;
            if(0 != access(info.strFilePath.c_str(), 0 ) )
            {
                LOG(ERROR) << "file not exsit [" << info.strFilePath <<"]" << endl;
                return -1;
            }
        }

        if(!FileUtils::copyFile((char*)m_strApkPath.c_str(), (char*)m_strTempApkPath.c_str(), true) )
        {
            LOG(INFO) << "copyFile apk fail ,errno = [" << errno << "] " << m_strTempApkPath << endl;
        }

        iter = m_listStAddFile.begin();
        string strFileList;
        // 删除原来的文件
        for(; iter != m_listStAddFile.end(); iter++)
        {
            ST_FIleAddInfo info = *iter;
            strFileList += info.strPathInZip +";";
        }

        // 删除用户指定的文件
        list<ST_FIleAddInfo>::iterator iter1 = m_listStDeleteFile.begin();
        for(; iter1 != m_listStDeleteFile.end(); iter1++)
        {
            ST_FIleAddInfo info = *iter1;
            strFileList += info.strPathInZip +";";
        }

        //
        if(m_stTaskInfo->strListParamCommon.length() > 0)
        {
            if (0 <= m_stTaskInfo->strListParamCommon.findIndex("-d") )
            {
                strFileList += "META-INF/*.SF;META-INF/*.RSA;META-INF/*.MF;";
            }
        }


        strFileList = strFileList.substr(0, strFileList.length()-1 );

    #ifdef _DEBUG
        LOG(INFO) << "delete file list: " << strFileList << endl;
    #endif

        iRet = deleteFileInApkEx(m_strTempApkPath.c_str(),strFileList.c_str() );
        if( iRet != 1 )
        {
            LOG(INFO) << "delete file failure !" << endl;
            iRet = -2;
            return iRet;
        }

        // 删除文件夹
        if(m_listStDecompressDir.size() > 0 )
        {
            list<ST_FIleAddInfo>::iterator iter2 = m_listStDecompressDir.begin();
            for(; iter2 != m_listStDecompressDir.end(); iter2++ )
            {
                ST_FIleAddInfo info = *iter2;
                iRet = DeleteInZipFileEx(m_strTempApkPath.c_str(), info.strPathInZip.c_str(), true );
                if(1 != iRet )
                {
                    LOG(ERROR) << "delete file failure [" << info.strFilePath << "]" << endl;
                    iRet = -2;
                    return iRet;
                }
            }
        }
    }

    bool bCreate = false;
    if(m_bDecompressAll)
        bCreate = true;

    iRet = 0;
    iter = m_listStAddFile.begin();
    for(; iter != m_listStAddFile.end(); iter++)
    {
        // 进行压缩
        ST_FIleAddInfo info = *iter;
#ifdef _DEBUG
    LOG(INFO) << "add file to apk: " << info.strFilePath << "->" << info.strPathInZip << endl;
#endif
        iRet = addDataOrFileToZip(m_strTempApkPath.c_str(), info.strPathInZip.c_str(), info.strFilePath.c_str(), 0, bCreate );
        bCreate = false;

        if(1 != iRet )
        {
            // 压缩出错
            iRet = -2;
            LOG(ERROR) << tr("compress file failure !") << " iRet = " << iRet << endl;
            return iRet;
        }
    }
    // 压缩文件夹
    if( m_listStDecompressDir.size() > 0 )
    {
        list<ST_FIleAddInfo>::iterator iter2 = m_listStDecompressDir.begin();
        for(; iter2 != m_listStDecompressDir.end(); iter2++ )
        {
            ST_FIleAddInfo info = *iter2;
#ifdef _DEBUG
            LOG(INFO) << "add dir to apk: " << info.strFilePath << "->" << info.strPathInZip << endl;
#endif
            iRet = AddDirToZip(info.strFilePath.c_str(), info.strPathInZip.c_str(), m_strTempApkPath.c_str() ,bCreate);
            bCreate = false;
            if(1 != iRet )
            {
                LOG(ERROR) << "compress file failure ! [" << info.strFilePath << "]" << endl;
                iRet = -2;
                return iRet;
            }
        }
    }
    iRet = 0;
    // 如果流程正确 则将缓存包替换原包

    return iRet;
}



int CApk::zipDeCompress()
{
    int iRet = 0;

    // 解压全部
    if(m_bDecompressAll)
    {
        iRet = DecompressionZip(m_strTempPath.c_str(), m_strApkPath.c_str() );
        if(1 != iRet )
        {
            // 解压文件失败
            LOG(ERROR) << "Decompress all file failure !" << endl;
            iRet = -3;
            return iRet;
        }

        iRet = 0;
        // 检查是否包含 dex文件
        std::vector<std::string> vecFiles;
        FileUtils::GetAllFilesEntry(m_strTempPath, vecFiles);
        std::vector<std::string>::iterator iterD = vecFiles.begin();
        for(; iterD != vecFiles.end(); iterD++ )
        {
            string strFile = (*iterD);
            strFile = strFile.substr();
            if( strFile.length() <= 4 )
            {
                continue;
            }

            if( strFile.compare(strFile.length() - 4, 4, ".dex") == 0 )
            {
                m_strListDex.append(strFile);
#ifdef _DEBUG
                LOG(INFO) << strFile << endl;
#endif
            }
        }
    }
    else
    {
        vector<char*> vecDecFiles;
        std::string strFileList;
        list<ST_FIleAddInfo>::iterator iter = m_listStDecompressFile.begin();
        for(; iter != m_listStDecompressFile.end(); iter++ )
        {
            ST_FIleAddInfo info = *iter;
            strFileList += info.strPathInZip + ";";
            vecDecFiles.push_back((char*)info.strPathInZip.c_str());
        }
        strFileList = strFileList.substr(0,strFileList.length()-1);

        LOG(INFO) << "save path:" << m_strTempPath << endl;
#ifdef _DEBUG
        LOG(INFO) << "decompress file:" << strFileList << endl;
#endif
        // 解压文件
        iRet = DecompressionFilesEx(m_listStDecompressFile.size(), strFileList.c_str(), m_strTempPath.c_str(), m_strApkPath.c_str(), true);
        if(1 != iRet )
        {
            // 解压文件失败
            LOG(ERROR) << "Decompress file failure [" << strFileList << "]" << endl;
            iRet = -3;
            return iRet;
        }
        iRet = 0;
        // 解压文件夹
        if( m_listStDecompressDir.size() > 0 )
        {
            list<ST_FIleAddInfo>::iterator iterT = m_listStDecompressDir.begin();
            for(; iterT != m_listStDecompressDir.end(); iterT++ )
            {
                ST_FIleAddInfo info = *iterT;
                iRet = DecompressionDir( m_strApkPath.c_str(), m_strTempPath.c_str(), info.strPathInZip.c_str() );
                if(1 != iRet )
                {
                    iRet = -3;
                    LOG(ERROR) << "Decompress file failure [" << info.strPathInZip << "]" << endl;
                    return iRet;
                }
            }
        }

        iRet = 0;
        // 检查是否包含 dex文件
        std::vector<std::string> vecFiles;
        FileUtils::GetAllFilesEntry(m_strTempPath, vecFiles);
        std::vector<std::string>::iterator iterD = vecFiles.begin();
        for(; iterD != vecFiles.end(); iterD++ )
        {
            string strFile = (*iterD);
            strFile = strFile.substr();
            if( strFile.length() <= 4 )
            {
                continue;
            }

            if( strFile.compare(strFile.length() - 4, 4, ".dex") == 0 )
            {
                m_strListDex.append(strFile);
#ifdef _DEBUG
                LOG(INFO) << strFile << endl;
#endif
            }
        }
    }


    return iRet;
}


int CApk::zipCompress(string strZipFile, string strFile)
{
    int iRet = 0;

    return iRet;
}

int CApk::zipRemoveFile(string strZipFile,string strFile)
{
    int iRet = 0;

    return iRet;
}

int CApk::zipAddFile(string &strZipFile, string &strFile, string &strFileInZip)
{
    int iRet = 0;
    return iRet;
}


int CApk::xmlPrinter(string strXml, string strXmlTag)
{
    int iRet = 0;
    return iRet;
}

int CApk::xmlModefy(string strXml, string strParam)
{
    int iRet = 0;
    return iRet;
}

// 反编译
int CApk::smali()
{
    int iRet = 0;

    return iRet;
}

int CApk::backSmali()
{
    int iRet = 0;
    return iRet;
}

bool CApk::checkFileExsit(const char* file)
{
    if(NULL == file )
        return false;
    if( 0 == access(file, 0) )
    {
        return true;
    }
    return false;
}


int CApk::setAddFile(string strFilePath, string strPathInZip)
{
    int iRet = 0;
    ST_FIleAddInfo st;
    st.strFilePath = strFilePath;
    st.strPathInZip = strPathInZip;
    this->m_listStAddFile.push_back(st);
    return iRet;
}

int CApk::setDecompressFile(string strFilePath, string strPathInZip)
{
    int iRet = 0;
    ST_FIleAddInfo st;
    st.strFilePath = strFilePath;
    st.strPathInZip = strPathInZip;
    this->m_listStDecompressFile.push_back(st);
    return iRet;
}

int CApk::setDecompressDir(string strFilePath, string strPathInZip)
{
    int iRet = 0;
    ST_FIleAddInfo st;
    st.strFilePath = strFilePath;
    st.strPathInZip = strPathInZip;
    this->m_listStDecompressDir.push_back(st);
    return iRet;
}

int CApk::setBackSmaliFile(string strFilePath, string strPathInZip)
{
    int iRet = 0;
    ST_FIleAddInfo st;
    st.strFilePath = strFilePath;
    st.strPathInZip = strPathInZip;
    this->m_listStDexFile.push_back(st);
    return iRet;
}

int CApk::setPackDeleteFile(string strFilePath, string strPathInZip)
{
    int iRet = 0;
    ST_FIleAddInfo st;
    st.strFilePath = strFilePath;
    st.strPathInZip = strPathInZip;
    this->m_listStDeleteFile.push_back(st);
    return iRet;
}

int CApk::checkOutPut(string &strOut, string su)
{
    int iLen, count, iRet = 0;
    iLen = strOut.length();
    do
    {
        break;
    }while(1);

    return iRet;
}



// 获取目录下所有文件
int CApk::getFileInDir(std::list<I_FILE_TYPE*> &listFileInfo)
{
    int iRet = 0;
#ifdef I_OS_WINDOWS
    //文件句柄
    long   hFile   =   0;
    //文件信息
    struct _finddata_t fileinfo;
    string p;
    if((hFile = _findfirst(p.assign(m_strTempPath).append("\\*").c_str(),&fileinfo)) !=  -1)
    {
        do
        {
            _finddata_t* f = new _finddata_t;
            memcpy(f, &fileinfo, sizeof(_finddata_t) );
            //std::string strfilename = p.assign(m_strTempPath).append("\\").append(fileinfo.name);
            listFileInfo.push_back( f );
        }while(_findnext(hFile, &fileinfo)  == 0);
        _findclose(hFile);
    }
    //排序，按从小到大排序
//    sort(files.begin(), files.end());
//    return files;

#elif I_OS_LINUX
    DIR *dir;
    struct dirent *ptr;
    if (( dir = opendir(m_strTempPath.c_str())) == NULL)
    {
        LOG(ERROR) << "open dir fail " << m_strTempPath << endl;
        return -1;
    }

    while ( ( ptr = readdir(dir)) != NULL )
    {
        if( strcmp(ptr->d_name,"." ) == 0 || strcmp( ptr->d_name,".." ) == 0 )    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 10)    ///link file
            continue;
        else if(ptr->d_type == FILE_ATTRIB || ptr->d_type == DIR_ATTRIB )    ///file or dir
        {
            I_FILE_TYPE* f = new I_FILE_TYPE;
            memcpy(f, ptr, sizeof(I_FILE_TYPE));
            listFileInfo.push_back(f);
        }
    }
    closedir(dir);
    //排序，按从小到大排序
    //sort(files.begin(), files.end());
#endif

    return iRet;
}

int CApk::clearFileInfo(std::list<I_FILE_TYPE*> &listFileInfo)
{
    if(listFileInfo.size() == 0 )
        return 0;

    std::list<I_FILE_TYPE*>::iterator it = listFileInfo.begin();
    for(; it != listFileInfo.end(); it++ )
    {
        I_FILE_TYPE* file = *it;
        if( NULL != file )
        {
            delete file;
        }
    }
    listFileInfo.clear();
    return 0;
}



