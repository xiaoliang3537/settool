#ifndef MODIFYDEX_H
#define MODIFYDEX_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <list>
#include <map>
#include "FileUtils.h"
#include "Utils.h"

#ifdef  _WIN32
#include <io.h>
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

using namespace std;

typedef unsigned char u1;
typedef unsigned int  u4;

enum { kSHA1DigestLen = 20,
       kSHA1DigestOutputLen = kSHA1DigestLen*2 +1 };

/*
* Direct-mapped "header_item" struct.
*/
typedef struct DexHeader {
    u1  magic[8];           /* includes version number */
    u4  checksum;           /* adler32 checksum */
    u1  signature[kSHA1DigestLen]; /* SHA-1 hash */
    u4  fileSize;           /* length of entire file */
    u4  headerSize;         /* offset to start of next section */
    u4  endianTag;
    u4  linkSize;
    u4  linkOff;
    u4  mapOff;
    u4  stringIdsSize;
    u4  stringIdsOff;
    u4  typeIdsSize;
    u4  typeIdsOff;
    u4  protoIdsSize;
    u4  protoIdsOff;
    u4  fieldIdsSize;
    u4  fieldIdsOff;
    u4  methodIdsSize;   /*鏂规硶涓暟*/
    u4  methodIdsOff;
    u4  classDefsSize;
    u4  classDefsOff;
    u4  dataSize;
    u4  dataOff;
} DexHeader;

#ifdef  _WIN32
#define     _myZip_         "windows\\myzip.exe"
#define     _AXMNAME_       "windows\\AxmlModify.exe"
#define     _SYMBOL_PATH_   "\\"
#define     _AXMLPRINTER2_  "smali_jar\\AXMLPrinter2.jar"
#define     _baksmali2      "smali_jar\\baksmali2.jar"
#define     _smali2         "smali_jar\\smali2.jar"
#else
#define     _myZip_         "myzip"
#define     _AXMNAME_       "AxmlModify"
//#define     _SYMBOL_PATH_   "/"
#define     _AXMLPRINTER2_  "../smali_jar/AXMLPrinter2.jar"
#define     _baksmali2      "../smali_jar/baksmali2.jar"
#define     _smali2         "../smali_jar/smali2.jar"
#endif

#ifdef WIN32
#define I_FILE_TYPE _finddata_t
#else
#define I_FILE_TYPE dirent
#endif

#ifdef WIN32
# ifndef FILE_ATTRIB
# define FILE_ATTRIB 0x20
# endif

# ifndef DIR_ATTRIB
# define DIR_ATTRIB 0x10
# endif
#else
# ifndef FILE_ATTRIB
# define FILE_ATTRIB 0x08
# endif

# ifndef DIR_ATTRIB
# define DIR_ATTRIB 0x04
# endif
#endif


typedef struct MAX_DEXCOUNT
{
    MAX_DEXCOUNT()
    {
        strFileName = "";
        bMax = false;
    }

    std::string strFileName;
    bool bMax;
}maxDexCount;

typedef std::vector<MAX_DEXCOUNT>   OUT_MAX_DEXCOUNT;
typedef std::vector<std::string>    FILES_CACHE_LIST;
static const int MAX_COUNT = 40000;

inline bool mySort(const MAX_DEXCOUNT& T1, const MAX_DEXCOUNT& T2){return strcmp(T1.strFileName.c_str(), T2.strFileName.c_str()) < 0 ? true : false;}

class ModifyDex
{
public:
    ModifyDex();

public:
    virtual int initTask(const char* strPath, const char * strConfig, int maxCount, int iCopyCount);
    int doModule();

    bool ModifyApk(const char *pchApkName, unsigned int nMaxValue, int nCopytCount, bool bR);

private:
    bool GetWorkPath();
    void InitVariable();
    void SearchDexFile(FILES_CACHE_LIST &listFile_Dex);
    bool FileSearch(std::string strFileName, FILES_CACHE_LIST &listFile_Dex);
    bool ModifyActiveFile(FILES_CACHE_LIST &vcOptList, int nCopytCount,
                          int nDexCount, std::string strMainActivity, int &iCopyNumber);
    std::string obianMainActivity();
    bool getFileList(char* pszDestPath, FILES_CACHE_LIST &listFile_Foder);
    bool copyActiveToNewSmali(string newSmaliFoder, FILES_CACHE_LIST listFile_Foder, int nCopytCount, string strMainActivity, int &iCopyNumber, string sSrcFoder);
    bool parseXmlActivity(string strMainActivity, FILES_CACHE_LIST &listFile_Activity, string sSrcFoder);
    int  ReplaceStr(char* sSrc, const char* sMatchStr, const char* sReplaceStr);
    int  ExtraNumFrStr(std::string str);

private:
    bool beginDeApk(const char *pchApkName);
    int  obianDexheader(OUT_MAX_DEXCOUNT &vcMethodCountOutMax, unsigned int nMaxCount);
    bool beginParseDex(OUT_MAX_DEXCOUNT vcDexFile, FILES_CACHE_LIST &vcOptList);
    bool ModifyDexFile(FILES_CACHE_LIST &vcOptList, int nCopytCount, int nDexCount, int &iCopyNumber);
    bool startBackDex(FILES_CACHE_LIST vcOptList, int iCopyNumber);
    bool startBackApk(OUT_MAX_DEXCOUNT vcDexInfo, FILES_CACHE_LIST vcOptList);

private:
    bool parseMaxDexFile(OUT_MAX_DEXCOUNT vcDexFile, FILES_CACHE_LIST &vcModifyFile);
    bool decomDexFileToSmali(FILES_CACHE_LIST vcModifyFile, FILES_CACHE_LIST &vcOptList);
    bool deleteRepeatFile(OUT_MAX_DEXCOUNT vcDexInfo, FILES_CACHE_LIST vcOptList);
    bool getMainApplication(string &strMainApplication);
    bool getMainApplicationOrMainActivity(std::string strXmlFile, FILES_CACHE_LIST &vcCallActivity, FILES_CACHE_LIST vcXmlRegisterFiles);
    bool getMainSmaliBaseClass(string sSrcFoder, std::string strSmaliPath, FILES_CACHE_LIST& vcBaseClassSmali, bool bApp);

    // 获取目录下所有文件
    int getFileInDir(std::list<I_FILE_TYPE*> &listFileInfo);
    int clearFileInfo(std::list<I_FILE_TYPE*> &listFileInfo);

private:
    std::string m_sTempFilePath;
    std::string m_sExecFilePath;
    std::string m_sSrcsFilePath;
    std::string m_strNewApkPath;
    std::string m_sApkName;
    int m_nMaxValue;
    int m_nCopytCount;
    DexHeader *dexData;
private:
    std::string m_strZipPath;
    std::string m_strSmali;
    std::string m_strBackSmali;
    std::string m_strAxmlModify;
    std::string m_strTempPath;
    std::list<I_FILE_TYPE*> m_listInfo;
};

#endif // MODIFYDEX_H
