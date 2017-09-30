#include "library.h"
#include "loger.h"

CLibrary::CLibrary(string &strFile)
{
    this->m_strFile= strFile;
#ifdef I_OS_WINDOWS
    LPCSTR l = m_strFile.c_str();
    m_hinstLib = LoadLibrary(l);
    if(NULL != m_hinstLib)
        m_bLoaded = true;
    else
        m_bLoaded = false;
#else
    void* p_Handle = dlopen(strFile.c_str(), RTLD_NOW);
    void* Error = dlerror();
    if( Error )
    {
        LOG(ERROR) << "open file error:" << strFile << "  " << (char*)Error << endl;
        m_bLoaded = false;
    }
    else
        m_bLoaded = true;
    m_hinstLib = p_Handle;
#endif
}



CLibrary::~CLibrary()
{

}

bool CLibrary::load()
{
    return m_bLoaded;
    return false;
}

iPoniter CLibrary::resolve(const char *symbol)
{
#ifdef I_OS_WINDOWS
    if(m_bLoaded)
    {
        void* p = GetProcAddress(m_hinstLib, symbol);
        return  (iPoniter)p;
    }
    else
        return 0;
#else
    if(m_bLoaded)
    {
        iPoniter f;
        void* p = dlsym(m_hinstLib, symbol );
        void* Error = dlerror();
        if( Error )
        {
            LOG(ERROR) << "open file error:" << m_strFile << endl;
            return 0;
        }
        return (iPoniter)p;
    }
    return 0;
#endif
}

bool CLibrary::unload()
{
#ifdef I_OS_WINDOWS
    if(m_bLoaded)
    {
        if(FreeLibrary(m_hinstLib) )
            return true;
        else
            return false;
    }
    else
        return false;
#else
    if(m_bLoaded)
    {
        int iRet = dlclose(m_hinstLib);
        if(iRet != 0 )
        {
            LOG(ERROR) << "close file error:" << m_strFile << endl;
            return false;
        }
        else
            return true;
    }
#endif
}
