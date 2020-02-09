/*
 * CThread.hpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */

#pragma once

#include <thread>
#include <iostream>

using namespace std;


class CThread 
{
 public:
    CThread(){
        m_pThread=nullptr;
        cout<<"CThread()"<<endl;
        };
    virtual ~CThread(){};
    
    static void doJob(CThread*);
    
    virtual void createThread()
    {
        cout<<"thread was created"<<endl;
        m_pThread=new std::thread(CThread::doJob , this);
    };
    virtual void setObserver(CThread* _pThread) {m_pObserver=_pThread;};
    virtual void unsetThreadServer(CThread* pObj){};
      
 protected:
    virtual void task();
    virtual bool processData();
    std::thread* m_pThread;            
    CThread* m_pObserver;
};



void CThread::doJob(CThread* pObj)
{
    pObj->task();
} 


void CThread::task()
{
    bool bFinish=false;
    char a;
    cout << "CThread::task():start task!ThreadID:"<<std::this_thread::get_id()<< endl;
    while (!bFinish)
    {
        processData();
    }
    cout << "Task was finish!!! ThreadID:"<<std::this_thread::get_id()<< endl;
}
    
    
bool CThread::processData()
{
    return true;
}    
    
