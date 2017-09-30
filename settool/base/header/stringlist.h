#ifndef STRINGLIST_H
#define STRINGLIST_H

#include <string>
#include <list>

using namespace std;

class CStringList
{
public:
    CStringList();
    ~CStringList();

public:
    std::string at(int i);
    bool append(const char* p);
    bool append(std::string& str);
    bool insert(int i, std::string& str);
    bool clear();
    bool removeAt(int i);
    int length();
    int findIndex(const char* p);
private:
    int m_iListLen ;
    std::list<std::string> m_list;
    std::list<std::string>::iterator m_it;
};

#endif // STRINGLIST_H
