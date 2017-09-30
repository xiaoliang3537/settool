#include "threadtask.h"
#include <QDebug>
#include <QWaitCondition>
#include "apk.h"
#include "managerthread.h"




int addTask(ST_Task_Info* stTaskInfo)
{
    if(NULL == stTaskInfo)
    {
        return -1;
    }
    g_mutexTask.lock();
    ST_Task_Info* st = new ST_Task_Info;
    *st = *stTaskInfo;
    g_listTaskInfo.append(st);
    g_mutexTask.unlock();
}

ST_Task_Info* getNextTask(bool bRemove )
{
    ST_Task_Info* st = NULL;
    g_mutexTask.lock();
    do
    {
        if(g_listTaskInfo.length() == 0 )
            break;
        st = new ST_Task_Info;

        if(bRemove)
        {
            st = (ST_Task_Info*)g_listTaskInfo.at(0);
            g_listTaskInfo.removeAt(0);
        }
        else
        {
            st = new ST_Task_Info;
            *st = *(g_listTaskInfo.at(0));
        }
    }while(0);
    qDebug() << "task count = " << g_listTaskInfo.count();
    g_mutexTask.unlock();
    return st;
}

int removeTask(int iTaskId)
{
    g_mutexTask.lock();
    g_mutexTask.unlock();
}

int clearTask()
{
    g_mutexTask.lock();
    g_mutexTask.unlock();
}

/**********************************************************************************************************/
CThreadTask::CThreadTask(int id)
{
    this->m_iID = id;
    m_pubvar = NULL;
    m_iStatus = -1;
}


CThreadTask::~CThreadTask()
{

}

int CThreadTask::init(ST_Task_Info &stTaskInfo, CPubVar* pubvar)
{
    //WaitForMultipleObjects();

    m_pubvar = pubvar;
    m_threadHandel[0] = m_pubvar->g_hHandel[0];
    m_threadHandel[1] = m_pubvar->g_hHandel[m_iID];

    int iRet = 0;
    m_stTaskInfo = stTaskInfo;
    return iRet;
}

void CThreadTask::run()
{
    qDebug() << "thread :[" << m_iID << "] start";
#ifdef I_OS_WINDOWS
    if(NULL == m_pubvar)
    {
        this->exit(-1);
        return ;
    }

    int iIndex = -9;
    while(1)
    {
        iIndex = 1;
        m_iStatus = 0;
        DWORD dwRet = ::WaitForSingleObject(m_pubvar->g_hHandel[m_iID], 1000*5);
        m_iStatus = 1;
        switch(dwRet)
        {
        case WAIT_TIMEOUT:              // 超时
            qDebug() << "curr work time out" << "[" << m_iID << "]";
            iIndex = 0;
            break;
        case WAIT_OBJECT_0:             // 启动任务
            iIndex = 1;
            qDebug() << "curr work" << "[" << m_iID << "]";
            break;
//        case WAIT_OBJECT_0+1:           // 启动任务
//            qDebug() << "curr work" << "[" << m_iID << "]";
//            iIndex = 1;
//            break;
        default:
            break;
        }

        if(-1 == iIndex)
        {
            break;
        }
        if(1 == iIndex)
        {
            ST_Task_Info* st = getNextTask(true);

            if(NULL == st)
            {
                qDebug() << "curr thread sleep" << m_iID;
                ResetEvent(m_pubvar->g_hHandel[m_iID]);
                continue;
            }
            void* obj = st->vObject;
            if(NULL == obj )
            {
                break;
            }

            if(NULL ==  dynamic_cast<CApk*>((CApk*)obj))
                break;
            CApk* apk = (CApk*)obj;

            ST_Module* module = NULL;
            for(int i = 0; i < st->strListParam.length(); i++ )
            {
                module = m_pubvar->m_config->getParamModule(st->strListParam.at(i));
                if(NULL != module )
                {
                    break;
                }
            }

            if(NULL == module)
            {
                break;
            }
            qDebug() << tr("模块").toLatin1();
            apk->setModuleInfo(module);
            int iRet = apk->start(st->strApkPath, st->strListParam, m_pubvar);

            // 发送当前任务执行完成信号
            emit this->onTaskFinished(iRet);
            SetEvent(m_pubvar->g_hHandel[m_iID]);
        }
    }
    qDebug() << "线程:[" << m_iID << "]结束";
#elif defined I_OS_LINUX
#endif

//    pthread_join;
//    sem_wait;
    //sem_timewait();
}


