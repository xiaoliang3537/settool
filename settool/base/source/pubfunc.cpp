#include "pubfunc.h"
#include "loger.h"

CPubfunc::CPubfunc()
{

}

bool CPubfunc::checkIsParam(char* info)
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

const char* CPubfunc::tr(const char* p, std::string &str)
{
#ifdef I_OS_WINDOWS
    str = p;
    str = KS_UTF8_to_ANSI(p);
    return str.c_str();
#else
    str = p;
    return p;
//    size_t inlen = strlen(p);
//    size_t outlen = 1024;
//    char temp[1024] = {0};
//    iconv_t t = iconv_open("UTF-8", "GB2312//IGNORE");
//    if(t == (iconv_t)-1)
//    {
//        cout << "iconv_open faild" << endl;
//        return "";
//    }
//    int iRet = 0;
//    char *in = (char*)p;
//    char *out = temp;
//    do
//    {
//        iRet = iconv(t, &in , &inlen, &out, &outlen);
//        cout << "iRet = " << iRet << "  " << "error " << errno << "temp = "   << temp << endl;
//        if(-1 == iRet )
//            break;
//        str = temp;
//        cout << "str = " << str << endl;
//        if( inlen == 0 )
//            break;
//    }while(0);

//    iconv_close(t);

//    if(iRet != 0)
//    {
//        return "";
//    }
//    return temp;
#endif
}

const char* CPubfunc::tr(unsigned long p, std::string &str)
{
    char sz[64] = {0};
    sprintf(sz, "%ld", p);
    str = sz;
    return sz;
}

#ifdef I_OS_WINDOWS
char* CPubfunc::QXUtf82Unicode(const char* utf, size_t *unicode_number)
{
    if(!utf || !strlen(utf))
    {
        *unicode_number = 0;
        return NULL;
    }
    int dwUnicodeLen = MultiByteToWideChar(CP_UTF8,0,utf,-1,NULL,0);
    size_t num = dwUnicodeLen*sizeof(wchar_t);
    wchar_t *pwText = (wchar_t*)malloc(num);
    memset(pwText,0,num);
    MultiByteToWideChar(CP_UTF8,0,utf,-1,pwText,dwUnicodeLen);
    *unicode_number = dwUnicodeLen - 1;
    return (char*)pwText;
}

char* CPubfunc::QXUnicode2Utf8(const char* unicode)
{
    int len;
    len = WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, NULL, 0, NULL, NULL);
    char *szUtf8 = (char*)malloc(len + 1);
    memset(szUtf8, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, szUtf8, len, NULL,NULL);
    return szUtf8;
}

std::string CPubfunc::KS_ANSI_to_UTF8 (const char* szAnsi)
{
    if (szAnsi == NULL)
        return NULL ;

    _bstr_t   bstrTmp (szAnsi) ;
    int  nLen = ::WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)bstrTmp, -1, NULL, 0, NULL, NULL) ;
    char * pUTF8 = new char[nLen+1] ;
    ZeroMemory (pUTF8, nLen + 1) ;
    ::WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)bstrTmp, -1, pUTF8, nLen, NULL, NULL) ;
    std::string strUtf8 = pUTF8;
    delete [] pUTF8;
    return strUtf8 ;
}

std::string CPubfunc::KS_UTF8_to_ANSI (const char* szUTF8)
{
    if (szUTF8 == NULL)
        return "" ;
    int     nLen = ::MultiByteToWideChar (CP_UTF8, 0, szUTF8, -1, NULL, 0) ;
    WCHAR   * pWstr = new WCHAR[nLen+1] ;
    ZeroMemory (pWstr, sizeof(WCHAR) * (nLen+1)) ;
    ::MultiByteToWideChar (CP_UTF8, 0, szUTF8, -1, pWstr, nLen) ;
    std::string  strAnsi (_bstr_t((wchar_t*)pWstr)) ;
    delete[] pWstr ;
    return strAnsi ;
}
#endif


