#ifndef APK_GENERAL_H
#define APK_GENERAL_H

#include <string>
#include "apk.h"
#include "pubvar.h"
#include "define.h"

class CApkGeneral : public CApk
{
public:
    CApkGeneral();
    ~CApkGeneral();

public:
    virtual int initTask(ST_Task_Info* stTaskInfo, CPubVar* pubvar);
    int doModule();
    int doEnd();
protected:

private:
    int setBackInfo();
    int writeToFile(std::string &strFileName, CStringList &strList);

    // 获取目录下所有文件列表，不包括文件夹
    int getFileList(std::string &strdir, std::vector<std::string> &vecFile);
private:
    std::string m_strConfigFile;
    CStringList m_strList;
    std::list<I_FILE_TYPE*> m_listFileInfo;
};

// 导出函数声明
//EXPORT_DEF(CApkGeneral)
//{
//    return (void*)(new CApkGeneral);
//}

#endif // APH_HIJACK_H
