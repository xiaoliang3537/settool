#ifndef TASK_H
#define TASK_H

#include <string>
#include <list>
#include "pubvar.h"
#include "define.h"
#include "managerthread.h"


using namespace std;
class CThreadTask;

class CTask
{
public:
    CTask();
    ~CTask();

public:
    int init();
    int addMission(ST_Task_Info& pstTaskInfo);

    // 获取任务状态
    int getStatus(int missionId);
    // 结束任务
    int finishMission(int missionId);
protected :
    int onMissionFinished(int id);
private:

private:
    // 任务列表
    list<ST_Task_Info>                              m_listTask;
    CThreadTask*                                    m_threadTask;
    CPubVar*                                        m_pubvar;
    //CManagerThread*                                 m_managerThread;
};


#endif
