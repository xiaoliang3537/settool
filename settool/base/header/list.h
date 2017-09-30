#ifndef LIST_H
#define LIST_H

#include <list>

using namespace std;

template <typename T>
class CIList
{
public:
    CIList();
    CIList(const CIList<T> &t);
public:
    int append(const T &t);
    void push_back(const T &t);
    void push_front(const T &t);
    void insert(int i, const T &t);

    T itemAt(int i);
    int clear();
    bool contains(const T &t) ;
    bool empty() ;


public:

};

#endif // LIST_H
