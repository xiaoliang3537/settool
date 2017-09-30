#include "fileinfo.h"

CFileInfo::CFileInfo(std::string &path)
{
    m_strFilePath = path;
}


CFileInfo::~CFileInfo()
{

}


bool CFileInfo::exists()
{
    if(m_strFilePath.length() == 0 )
        return false;
    int ret =access(m_strFilePath.c_str(), 0);
    if(0 == ret )
        return true;
    return false;
}

std::string CFileInfo::fileDir()
{
    return FileUtils::ExtractFileDir(m_strFilePath);
}

std::string CFileInfo::fileName()
{
    return FileUtils::ExtractFileRemoveExt(FileUtils::ExtractFileName(m_strFilePath));
}

std::string CFileInfo::fileFullName()
{
    return FileUtils::ExtractFileName(m_strFilePath);
}

std::string CFileInfo::filePath()
{
    return m_strFilePath;
}



CDir::CDir(std::string &path)
{
    m_strPath = path;
}

CDir::~CDir()
{

}


bool CDir::exists()
{
    if(m_strPath.length() == 0 )
        return false;
    if(0 != access(m_strPath.c_str(), 0) )
        return false;
    return true;
}

bool CDir::mkdir(const char* str)
{
    if(strlen(str) == 0 )
        return false;
    FileUtils::MakeSureDirExsitsW(str);
    return false;
}

bool CDir::deleteDir(const char* str)
{
    return false;
}




