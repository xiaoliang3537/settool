#ifndef MANAGERTHREAD_H
#define MANAGERTHREAD_H

//#include "threadtask.h"
#include "pubvar.h"

#ifdef I_OS_WINDOWS
#include <windows.h>
#else
#endif

class CManagerThread
{
public:
    CManagerThread();
    ~CManagerThread();
    int init(CPubVar* pubvar);
public:
    void run();
    int addTask(ST_Task_Info* stTaskInfo);

protected:
    int onThreadFinised(int id);
private:
    //CThreadTask*                        m_threadTask[5];
    CPubVar*                            m_pubvar;
};



#endif // MANAGERTHREAD_H
