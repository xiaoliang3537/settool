#include "stringlist.h"

CStringList::CStringList()
{
    m_iListLen = 0;
}


CStringList::~CStringList()
{
    m_list.clear();
}

std::string CStringList::at(int i)
{
    if(i < 0 )
        return "";
    int where = 0;
    m_it = m_list.begin();
    for(; m_it != m_list.end(); m_it++ )
    {
        if(i == where)
        {
            return *m_it;
            break;
        }
        where++;
    }
    return "";
}

bool CStringList::append(const char* p)
{
    m_list.push_back(p);
    return true;
}

bool CStringList::append(std::string& str)
{
    m_list.push_back(str);
    return true;
}

bool CStringList::insert(int i, std::string& str)
{
    m_it = m_list.begin();
    int where = 0;
    for(; m_it != m_list.end(); m_it++ )
    {
        if(i == where)
        {
            m_list.insert(m_it, str);
            break;
        }
        where++;
    }
    return true;
}

bool CStringList::clear()
{
    m_list.clear();
    return true;
}

bool CStringList::removeAt(int i)
{
    m_it = m_list.begin();
    int where = 0;
    for(; m_it != m_list.end(); m_it++ )
    {
        if(i == where)
        {
            m_list.erase(m_it);
            break;
        }
        where++;
    }
    return true;
}

int CStringList::length()
{
    return m_list.size();
}

int CStringList::findIndex(const char* p)
{
    if(NULL == p )
        return -1;
    std::string str = p;
    m_it = m_list.begin();
    int where = 0;
    for(; m_it != m_list.end(); m_it++ )
    {
        std::string s = (*m_it);
        if(str.compare( s ) == 0 )
            return where;
        where++;
    }

    return -1;
}

