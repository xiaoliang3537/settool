#ifndef THREADTASK_H
#define THREADTASK_H

#include <string>
#include <list>

#include "define.h"
#include "pubvar.h"

using namespace std;

static QMutex g_mutexTask;
static QList<ST_Task_Info*> g_listTaskInfo;
extern int addTask(ST_Task_Info* stTaskInfo);
extern ST_Task_Info* getNextTask(bool bRemove = false);
extern int removeTask(int iTaskId);
extern int clearTask();


class CThreadTask
{
public:
    CThreadTask(int id);
    ~CThreadTask();

    int                             m_iStatus;
public:
    int init(ST_Task_Info &stTaskInfo, CPubVar* pubvar);
    void run();

private:
    HANDLE                          m_threadHandel[2];
    int                             m_iID;
    ST_Task_Info                    m_stTaskInfo;
    CPubVar*                        m_pubvar;
};

#endif // THREADTASK_H
