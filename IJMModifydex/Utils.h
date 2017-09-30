#ifndef UTILS_H_
#define UTILS_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
/*****************************************************************************
* arg_to_int - Convert argument string to integer.
*
* min - Minimum allowed value, inclusive.
* max - Maximum allowed value, inclusive.
* defalt - The default value, in case of an error.
* opt - Option string of this argument.  (ex., "-h");
*/

inline int arg_to_int(const char* arg, int min, int max, int defalt, const char* opt)
{
#ifdef _WIN32
    int i = defalt;
    int rv;

    /* no argument means we use the default value */
    if(!arg) goto done;

    /* make sure we got an integer argument */
    rv = sscanf(arg, "%d", &i);
    if(rv != 1) {
        fprintf(stderr, "%s: integer argument required.\n", opt);
        i = defalt;
        goto done;
    }

    /* make sure the integer argument is within the desired range */
    if(i < min || max < i) {
        fprintf(stderr, "%s: argument out of integer range.\n", opt);
        i = defalt;
        goto done;
    }

done:
    return i;

#else
int i = defalt;
int rv;

/* no argument means we use the default value */
if(!arg) goto done;

/* make sure we got an integer argument */
rv = sscanf(arg, "%d", &i);
if(rv != 1) {
    fprintf(stderr, "%s: integer argument required.\n", opt);
    i = defalt;
    goto done;
}

/* make sure the integer argument is within the desired range */
if(i < min || max < i) {
    fprintf(stderr, "%s: argument out of integer range.\n", opt);
    i = defalt;
    goto done;
}

done:
return i;
#endif
}
inline bool mysystem( const char * pszCmd,std::string &strOutput)
{
#ifdef _WIN32
    bool bRet = TRUE;
    HANDLE hRead,hWrite;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL; //?????????????????
    sa.bInheritHandle = TRUE; //??????????о???
    do
    {
        if (!CreatePipe(&hRead,&hWrite,&sa,0)) //???????????
        {
            bRet = FALSE;
            break;
        }
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si,sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        GetStartupInfo(&si);
        si.hStdError = hWrite;
        si.hStdOutput = hWrite;    //????????????????????д??????
        si.wShowWindow = SW_HIDE; //???????
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

        TCHAR cmdline[1024] = {0};
        char buffer[1024] = {0};
        DWORD bytesRead;
        sprintf(cmdline,"cmd /C \"%s\"",pszCmd);
        if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) //?????????
        {
            bRet = FALSE;
            break;
        }
        CloseHandle(hWrite); //?????????
        Sleep(10);
        strOutput.clear();
        while (true)
        {
            memset(buffer,0,1024);
            if (ReadFile(hRead,buffer,1024,&bytesRead,NULL) == NULL) //??????
                break;
            strOutput.append(buffer,bytesRead);
        }
        CloseHandle(hRead);
    }while (0);
    return bRet;
#else
    FILE *pFile = popen(pszCmd, "r"); //???????
    if (!pFile)
    {
        return 0;
    }
    char buf [1024] = {0};
    fread(buf,1,1024,pFile);
    strOutput  = buf;
    pclose(pFile); //?????
    return 1;
#endif
}
inline bool  WindowsExec(const char* cmdline, bool uCmdShow)
{
#ifdef _WIN32
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    si.cb = sizeof(STARTUPINFO);
    si.lpReserved = NULL;
    si.lpDesktop = NULL;
    si.lpTitle = NULL;
    si.dwFlags = 0;
    si.cbReserved2 = 0;
    si.lpReserved2 = NULL;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = uCmdShow?SW_SHOWNORMAL :SW_HIDE;

 // PVOID OldValue = NULL;
//	 Wow64DisableWow64FsRedirection(&OldValue);

 std::string cmdline_fixed;
    cmdline_fixed.append("cmd /c \"").append(cmdline).append("\"");


    BOOL bres = CreateProcess(NULL,(LPSTR)cmdline_fixed.c_str(),NULL,NULL,false,
        NORMAL_PRIORITY_CLASS,
        NULL,NULL,&si,&pi);


   //?????????
 //  Wow64RevertWow64FsRedirection(&OldValue);

    if(bres==false)
    {
        std::cout << "??????????" << std::endl;
        return false;
    }
    else
    {
        CloseHandle(pi.hThread);
        DWORD dwret=WaitForSingleObject(pi.hProcess, INFINITE);
        switch(dwret)
        {
        case WAIT_OBJECT_0:
            {
                DWORD dwexitcode;
                bres = GetExitCodeProcess(pi.hProcess,&dwexitcode);
                TCHAR exitmsgbuf[1024] = {0};
                if(bres)
                {
                    //	wsprintf(exitmsgbuf,"exit code:%d",dwexitcode);
                }
                else
                {
                    wsprintf(exitmsgbuf,"exit code failed to return");
                    return false;
                }
                std::wcout << exitmsgbuf << std::endl;
                break;

            }

        default:
            std::wcout << "exit for other reason" << std::endl;
            return false;
        }
        CloseHandle(pi.hProcess);
    }
    return true;
#else
    if(cmdline == NULL || strlen(cmdline) <= 0)
    {
        return false;
    }
    char szbuf[1024] = {0};
    sprintf(szbuf,"%s",cmdline);
    std::cout<<szbuf<<std::endl;
    int status = system(szbuf);
    if(-1 != status && WIFEXITED(status)&&0 == WEXITSTATUS(status))
    {
        return true;
    }
    return false;
#endif
}


#define MAX_BUFFER  2048
inline static std::string abs_path(const char* srcpath)
{
    std::string strAbsolutionpath = "";
    char abspath[MAX_BUFFER] = {0};
#ifdef _WIN32
    if(_fullpath(abspath, srcpath, MAX_BUFFER) != NULL)
    {
        if(strstr(srcpath, abspath))
            strAbsolutionpath.append(srcpath);
        else
            strAbsolutionpath.append(abspath, strlen(abspath));
    }
    else
    {
        printf("invalid path!\n");
    }
#else
    if(getcwd(abspath, MAX_BUFFER) != NULL)
    {
        char *pszfull = (char*)malloc(MAX_BUFFER*sizeof(char*));
        memcpy(pszfull, abspath, strlen(abspath));

        if(pszfull[0] == '/')
            strAbsolutionpath.append(srcpath);
        else
        {
            strcat(pszfull, srcpath);
            strAbsolutionpath.append(pszfull).append("/").append(srcpath);
        }
        if(pszfull)
            free(pszfull);
    }
    else
    {
        printf("invailed path!\n");
    }

#endif
    return strAbsolutionpath;
}


#endif

