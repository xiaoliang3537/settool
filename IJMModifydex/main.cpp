#include <iostream>
#include <stdio.h>
#ifdef _WIN32
#include "getopt.h"
#else
#include <getopt.h>
#endif
#include "ModifyDex.h"
#include "Utils.h"

#include "loger.h"

using namespace std;

void help()
{
    std::cout << "\r\n IJMModifydex\r\n"
              << " ----------------------------------------------------------------------------\n"
              << "   [using]: [-f FILE][-n METHOD_NUMBERS][-c ACTIVITY_COUNTS][-r IS_REPLACE]"
              << "\r\n   -f     setting input \"APK\" File \r\n"
              << "\r\n   -n     setting input method Values(An integer greater than 0 less than 65535) \r\n"
              << "\r\n   -c     setting input modify activity Counts(An integer greater than 0) \r\n"
              << "\r\n   -r     setting Compulsory coverage \r\n"
              << "\r\n   -v     gets the application version number\r\n"
              << " ----------------------------------------------------------------------------\n"
              << " example:\n"
              << "   ./IJMModify -f 1.apk -n 30000 -c 10\n"
              << " ----------------------------------------------------------------------------\n"
              << std::endl;
}

void printVersion()
{
    printf(" IJMModifydex Version 3.3 \r\n");
}

bool checkIsParam(char* info)
{
    if(NULL == info )
        return false;

    if( 1 >= strlen(info) )
    {
        return false;
    }

    if(info[0] == '-')
    {
        return true;
    }
    return false;
}

struct ST_Param
{
    std::string strName;
    std::string strValue;
};

int loadParam(int argc, char** argv, std::vector<ST_Param> &vec)
{
    int iRet = 0;
    if(argc == 1 )
    {
        cout << "input -help show the help info!" << endl;
        return 1;
    }

    if(argc == 2 )
    {
        if(strcmp(argv[1],"-help") == 0  )
        {
            help();
            return 1;
        }
        else if(strcmp(argv[1],"-v") == 0  || strcmp(argv[1],"-V") )
        {
            printVersion();
            return 1;
        }
        else
        {
            cout  << "param abnormal! " << endl;
            return 1;
        }
    }

    for(int i = 1; i < argc; i++)
    {
        if( checkIsParam(argv[i]) )
        {
            ST_Param stInfo;
            stInfo.strName = argv[i];
            if(i + 1 < argc )
            {
                if( !checkIsParam(argv[i+1]) )
                {
                    stInfo.strValue = argv[i+1];
                    i++;
                    vec.push_back(stInfo);
                }
                else
                {
                    if(strcmp("-mv", argv[i] ) == 0)
                    {
                        vec.push_back(stInfo);
                        continue;
                    }
                    else
                        return -1;
                }
            }
        }
    }

    return iRet;
}

loger *g_loger = new loger();

int main(int argc, char** argv)
{
    g_loger->init();

    std::string pszApkPath;
    std::string strConfig;

    bool bBackUp = false;

    ModifyDex ojbModify;

    char cformat[] = "-H::h::F:f:tf:TF:N:n:C:c:R::r::V::v::";

    bool bVersion = false;
    unsigned int nMaxValue = 0;
    unsigned int nCount = 0;
    std::string strMaxValue = "", strCount = "";
    std::string strMovePath ;
    std::vector<ST_Param> vec;
    if(0 != loadParam(argc, argv, vec))
    {
        return 1;
    }

    bool bR = false;

    std::vector<ST_Param>::iterator it = vec.begin();
    for(; it != vec.end(); it++ )
    {
        ST_Param st = *it;
        if( 0 == st.strName.compare("-f") || 0 == st.strName.compare("-F") )
        {
            pszApkPath = st.strValue;
        }
        if( 0 == st.strName.compare("-tf") || 0 == st.strName.compare("-TF") )
        {
            strConfig = st.strValue;
        }
        if( 0 == st.strName.compare("-n") || 0 == st.strName.compare("-N") )
        {
            strMaxValue = st.strValue;
        }
        if( 0 == st.strName.compare("-c") || 0 == st.strName.compare("-C") )
        {
            strCount = st.strValue;
        }
        if( 0 == st.strName.compare("-mv") || 0 == st.strName.compare("-MV") )
        {
            strMovePath = st.strValue;
        }
        if( 0 == st.strName.compare("-v") || 0 == st.strName.compare("-v") )
        {
            bVersion = true;
            break;
        }
    }

    if(bVersion)
    {
        printVersion();
        return 0;
    }

//    while(1)
//    {
//        int c = getopt(argc, argv, cformat);
//        if (c == -1)
//        {
//            break;
//        }
//        switch (c)
//        {
//        case 'H':
//        case 'h':
//            help();
//            exit(0);
//            break;
//        case 'F':
//        case 'f':
//            pszApkPath.append(optarg);
//            break;
//        case 'TF':
//        case 'tf':
//            strConfig.append(optarg);
//            break;
//        case 'N':
//        case 'n':
//            strMaxValue.append(optarg);
//            break;
//        case 'C':
//        case 'c':
//            strCount.append(optarg);
//            break;
//        case 'R':
//        case 'r':
//            bR = true;
//            break;
//        case 'V':
//        case 'v':
//            bVersion = true;
//            break;
//        default:
//            std::cout << "Please enter the correct parameters" << std::endl;
//            help();
//            exit(0);
//            break;
//        }
//    }

//    if(bVersion)
//    {
//        printVersion();
//        return 0;
//    }

    stringstream ss;
    ss << strMaxValue;
    ss >> nMaxValue;
    ss.str("");

    stringstream ss2;
    ss2 << strCount;
    ss2 >> nCount;
    ss2.str("");

    if(pszApkPath.empty())
    {
        std::cout << "Please enter a file that needs to be split" << std::endl;
        help();
        return 0;
    }
    else if(nMaxValue <= 0 || nCount <= 0 )
    {
        std::cout << "please input \'-n\' or \'-c\' parameters " << std::endl;
        help();
        return 0;
    }
    else if(nMaxValue < nCount )//|| nMaxValue > 65535 || nCount > 100)
    {
        std::cout << "error!\n'n' value more than 'c' value;\nPlease enter the correct \'-n\' and \'-c\' parameters" << std::endl;
        help();
        return 0;
    }

    if(nMaxValue > 65535 ) {
        nMaxValue = 65535;
    }


    std::string strSrcFilePath = abs_path(pszApkPath.c_str());
    std::cout << "full path is : " << pszApkPath << std::endl;
    std::cout << "-n parameter is : " << strMaxValue << std::endl;
    std::cout << "-c parameter is : " << strCount << std::endl;

    ojbModify.initTask(pszApkPath.c_str(),strConfig.c_str(), nMaxValue, nCount, strMovePath.c_str() );
    bBackUp = ojbModify.doModule();

    if (0 != bBackUp)
    {
        std::cout << "ENCRYPTION FAILED" << std::endl;
        exit(0);
    }
    else
    {
        std::cout << "ENCRYPTION SUCCESS" << std::endl;
    }


    return 0;
}

