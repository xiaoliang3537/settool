#include "Utils.h"
#include <stdio.h>
#include <iostream>
#include <list>
#include <iomanip>

#include "stringlist.h"
#include "zipfile.h"

#ifdef WIN32
# ifndef I_OS_WINDOWS
# define I_OS_WINDOWS
# endif
#elif unix
# ifndef I_OS_LINUX
# define I_OS_LINUX
# endif
#endif

#include "manager.h"

using namespace  std;

int main(int argc, char* argv[])
{
    int iRet = 0;

#ifdef I_OS_WINDOWS
    setlocale(LC_ALL, "utf-8");
#else
    //setlocale(LC_ALL, "utf-8");
    //setuid(geteuid());
    //setgid(getegid());
#endif

    CManager manager;
    manager.init();
    manager.execCmd(argc, argv);


    return iRet;
}




