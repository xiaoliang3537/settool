#ifndef LIBRARY_H
#define LIBRARY_H

#ifdef WIN32
# ifndef I_OS_WINDOWS
# define I_OS_WINDOWS
# endif
#elif unix
# ifndef I_OS_LINUX
# define I_OS_LINUX
# endif
#endif

#include <stdio.h>
#include <string>
#include "define.h"

#ifdef I_OS_WINDOWS
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

using namespace std;


#ifdef I_OS_WINDOWS
# ifndef DLL_INSTANCE
# define DLL_INSTANCE HINSTANCE
# endif
typedef int iPoniter;
#else
# ifndef DLL_INSTANCE
# define DLL_INSTANCE void*
# endif
typedef void* iPoniter;
#endif

#define poniter void (*)()

class CLibrary
{
public:
    CLibrary(string &strFile);
    ~CLibrary();

public:
    bool load();
    iPoniter resolve(const char *symbol);
    bool unload();
private:
    bool                m_bLoaded;
    string              m_strFile;
    DLL_INSTANCE        m_hinstLib;
};

#endif // LIBRARY_H
