#ifndef REGISTER_H
#define REGISTER_H

#include <map>
#include <string>

#include "define.h"

class CApk;
using  namespace std;
typedef  void *(*GET_INSTANCE)();

static std::map<string, GET_INSTANCE> mapCls_;

class CObjectFactory
{
public:
    static void Register( const string &name, GET_INSTANCE func)
    {
        mapCls_.insert(pair<string, GET_INSTANCE>(name,func));
    }
    static GET_INSTANCE getObject(const string& name)
    {
        std::map<string, GET_INSTANCE>::iterator iter = mapCls_.find(name);
        if( iter == mapCls_.end() )
        {
            return NULL;
        }
        return (GET_INSTANCE)iter->second;
    }
public:

};
//__declspec(selectany) QMap<string, GET_INSTANCE> CObjectFactory::mapCls_;


class Register
{
public:
    Register( const string &name, GET_INSTANCE func)
    {
        CObjectFactory::Register(name, func);
    }
};

#define REGISTER_CLASS(class_name) \
class class_name##Register { \
public: \
     static  CApk* NewInstance() \
    { \
         return  new class_name; \
    } \
private: \
     static Register reg_; \
}; \
Register class_name##Register::reg_(#class_name, class_name##Register::NewInstance);



#endif // REGISTER_H
