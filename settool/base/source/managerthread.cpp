#include "managerthread.h"
#include "pubvar.h"

CManagerThread::CManagerThread()
{
//    for(int i = 0; i < 5; i++)
//    {
//        m_threadTask[i] = new CThreadTask(i+1);
//    }

}


CManagerThread::~CManagerThread()
{
//    for(int i = 0; i < 5; i++)
//    {
//        delete m_threadTask[i];
//    }
}

int CManagerThread::init(CPubVar* pubvar)
{
//    ST_Task_Info stInfo;
//    m_pubvar = pubvar;
//    for(int i = 0; i < 5; i++)
//    {
//        m_threadTask[i]->start();
//        m_threadTask[i]->init(stInfo, m_pubvar);
//    }
    return 0;
}

int CManagerThread::addTask(ST_Task_Info* stTaskInfo)
{
//    for(int i = 0; i < 5; i++)
//    {
//        if(m_threadTask[i]->m_iStatus == 0 )
//        {
//            if( !::SetEvent(m_pubvar->g_hHandel[i+1]) )
//            {
//            }
//            break;
//        }
//    }
        return 0;
}

int CManagerThread::onThreadFinised(int id)
{
        return 0;
}

void CManagerThread::run()
{

#ifdef I_OS_WINDOWS     // windwos 下对多线程处理
    // 等待线程退出
    //WaitForSingleObject(handel, INFINITE);
#else
    // linux
#endif
}



