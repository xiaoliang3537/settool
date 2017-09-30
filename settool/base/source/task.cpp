#include "task.h"
#include <windows.h>

#include "apk.h"
#include "threadtask.h"


CTask::CTask()
{
    m_pubvar = NULL;
}

CTask::~CTask()
{

}

int CTask::init()
{
    int iRet = 0;
    return iRet;
}

int CTask::addMission(ST_Task_Info& pstTaskInfo)
{
    int iRet = 0;

//    CThreadTask* task = new CThreadTask();
//    connect(task, SIGNAL(onTaskFinished(int)), this, SLOT(onMissionFinished(int)));

//    task->init(pstTaskInfo,m_pubvar);
//    task->start();

    //WaitForMultipleObjects();

    return iRet;
}


int CTask::getStatus(int missionId)
{
    int iRet = 0;
    return iRet;
}

// ½áÊøÈÎÎñ
int CTask::finishMission(int missionId)
{
    int iRet = 0;
    return iRet;
}

// slots on finished
int CTask::onMissionFinished(int id)
{
    int iRet = 0;

    QObject* obj = sender();
    delete obj;

    return iRet;
}
