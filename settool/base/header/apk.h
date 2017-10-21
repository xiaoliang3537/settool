#ifndef APK_H
#define APK_H

#ifdef WIN32
# ifndef I_OS_WINDOWS
# define I_OS_WINDOWS
# endif
#elif unix
# ifndef I_OS_LINUX
# define I_OS_LINUX
# endif
#endif

#include <iostream>
#include <string>
#include <string>
#include <list>
#include "stringlist.h"
#include "pubvar.h"
#include "define.h"
#include "loger.h"
#include "fileinfo.h"



#ifdef I_OS_WINDOWS
#include <direct.h>
#else
#include <unistd.h>
#endif

#ifdef I_OS_WINDOWS
#define I_FILE_TYPE _finddata_t
#else
#define I_FILE_TYPE dirent
#endif

struct Zipfile;

extern loger *g_loger;

using namespace std;

class CApk
{
public:
    CApk();
    virtual ~CApk();

    virtual int initTask(ST_Task_Info* stTaskInfo, CPubVar* pubva);
public:
    /**
     * @brief start 功能模块调用入口
     * @param pubvar 公共变量，操作过程中需要的一些数据，如果当前模块不需要外部数据处理，则忽略
     * @return 0 正常 != 异常
     */
    int start( );

    /**
     * @brief setApkInfo 提供功能模块设置特殊数据接口
     * @param apkInfo 包含数据的地址
     */
    virtual void setApkInfo(void* apkInfo){return;}

    /**
     * @brief 执行指令操作
     */
    int execCmd(std::string strCmd);
protected:
    /**
     * @brief checkParam 入参检查，检查入参是否合理合法
     * @param strList 输入参数
     * @return 0 正常 != 异常
     */
    virtual int checkParam(CStringList& strList);
    /**
     * @brief init 根据入参进行apk包信息初始化操作 目前处理解压文件列表 反编译文件
     * @return 0 正常 != 异常
     */
    virtual int initApk();
    /**
     * @brief doModule 抽象接口，定义子模块业务处理功能入口,派生类必须实现该接口进行业务处理
     * @return 0 正常 != 异常
     */
    virtual int doModule() = 0;

    /**
     * @brief doEnd;
     * @return 0 正常 != 异常
     */
    virtual int doEnd();

    /**
     * @brief 解压文件 参数列表中的文件到指定目录(m_strTempPath+文件相对路径)
     * @param strListFile 待解压文件列表
     * @return 0 成功 > 0 对应第几个文件解析失败 < 0 异常
     */
    virtual int zipDeCompress(CStringList strListFile);

    /**
     * @brief decompileDexFile 反编译dex文件
     * @return 0 正常 != 异常
     */
    virtual int decompileDexFile();
    /**
     * @brief backPackDexFile 回编dex文件
     * @return 0 正常 != 异常
     */
    virtual int backPackDexFile();
    /**
     * @brief backPack 回包
     * @return 0 正常 != 异常
     */
    virtual int backPack();

protected:
    /**
     * @brief zipDeCompress 解压文件
     * @return 0 成功 < 0 异常 > 0 其他问题
     */
    int zipDeCompress();

    /**
     * @brief zipCompress 压缩文件
     * @param strZipFile 待操作zip文件
     * @param strFile 待添加的压缩文件
     * @return
     */
    int zipCompress(string  strZipFile, string  strFile);

    /**
     * @brief zipRemoveFile 删除文件
     * @param strZipFile 待操作zip文件
     * @param strFile 待删除的文件 相对路径
     * @return
     */
    int zipRemoveFile(string  strZipFile,string  strFile);

    /**
     * @brief zipAddFile 添加压缩文件
     * @param strZipFile 待操作zip文件
     * @param strFile 待添加文件路径
     * @param strFileInZip  待添加文件在zip中的路径
     * @return
     */
    int zipAddFile(string &strZipFile, string &strFile, string &strFileInZip);

    /**
     * @brief xmlPrinter 对解压的xml文件进行解析
     * @param strXml 待解析xml文件
     * @param strXmlTag 解析文件存放路径
     * @return
     */
    int xmlPrinter(string  strXml, string  strXmlTag);
    /**
     * @brief xmlModefy 更改解压后的xml文件
     * @param strXml 待操作xml文件
     * @param strParam 操作入参
     * @return
     */
    int xmlModefy(string  strXml, string  strParam);
    int smali();
    int backSmali();
    bool checkFileExsit(const char* file);
    // 获取目录下所有文件
    int getFileInDir(std::list<I_FILE_TYPE*> &listFileInfo, std::string strDir);
    int clearFileInfo(std::list<I_FILE_TYPE*> &listFileInfo);
protected:
    // 解压 压缩文件列表设置
    int setAddFile(string strFilePath, string strPathInZip);
    int setDecompressFile(string strFilePath, string strPathInZip);
    int setDecompressDir(string strFilePath, string strPathInZip);
    // 设置进行回编的文件
    int setBackSmaliFile(string strFilePath, string strPathInZip);
    // 设置回包中需要删除的文件
    int setPackDeleteFile(string strFilePath, string strPathInZip);
    // 检查程序运行结果
    int checkOutPut(string &strOut, string su = "SUCESS");
protected:
    bool                                        m_bDecompressAll;           //
    int                                         m_iProgramId;               // 模块ID
    CPubVar*                                    m_pubvar;                   // 系统公共变量
    CConfig*                                    m_config;                   // 配置信息
    int                                         m_iParamID;                 // 业务处理ID 针对具体业务而定
    int                                         m_iApkCount;
    int                                         m_iProcessRet;              // 处理结果 0 正常 !=0异常
    int                                         m_iProcessType;             // 执行类型 1 普通直接调用 2 模块经过拆分后调用
    string                                      m_strWorkPath;              // 工作路径
    string                                      m_strArg;                   // 功能参数
    string                                      m_strApkPath;               // 待处理apk路径
    string                                      m_strTempApkPath;           // 临时apk路径
    string                                      m_strTempPath;
    string                                      m_strListArg;               // 操作参数
    string                                      m_strFilaName;              // apk 文件名
    string                                      m_strTempFilaName;          // apk 随机文件名称
    CStringList                                 m_strFileList;              // 待解压文件列表
    CStringList                                 m_strListDex;               // dex文件列表 需要解压dex文件 才会存在
    CStringList                                 m_strListAdd;               // 新增文件列表
    ST_Module*                                  m_stModuleInfo;             // 模块信息
    ST_Task_Info*                               m_stTaskInfo;               // 任务信息
    Zipfile*                                    m_zipFile;                  // zip文件信息
protected:
    typedef struct
    {
        string strFilePath;
        string strPathInZip;
    } ST_FIleAddInfo;
    list<ST_FIleAddInfo>                        m_listStAddFile;            // 待添加的文件列表
    list<ST_FIleAddInfo>                        m_listStDecompressFile;     // 待解压文件列表
    list<ST_FIleAddInfo>                        m_listStDecompressDir;      // 待解压文件夹 待压缩文件夹
    list<ST_FIleAddInfo>                        m_listStDexFile;            // Dex文件列表
    list<ST_FIleAddInfo>                        m_listStDeleteFile;         // 待删除的文件列表

    std::map<std::string, std::string>          m_mapDexToSmali;            // 反编译文件目录
    std::map<std::string, std::string>          m_mapSmaliToDex;            // 反编译文件目录
    std::map<std::string, void*>                m_mapZipFileInfo;           // 压缩文件信息
};

#endif      // APK_H
