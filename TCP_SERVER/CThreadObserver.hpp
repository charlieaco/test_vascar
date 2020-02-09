/*
 * CThreadObserver.hpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */

#pragma once

#include <thread>
#include <iostream>
#include <list>
#include <map>
#include "CThread.hpp"
#include "IObserver.hpp"
#include "CThreadServer.hpp"
#include <condition_variable>


using namespace std;


class CThreadObserver:public IObserver,public CThread 
{
 public:
    CThreadObserver(){
        cout<<"CThreadObserver()"<<endl;
        m_state=ePrepareInfo;
        };
    virtual ~CThreadObserver(){};
    
    virtual void notify(unsigned long Id,double average);
    virtual void notify() {};
    virtual void setNewThreadServer(CThreadServer* pObj);
    virtual void unsetThreadServer(CThread* pObj)override;

      
 protected:
    virtual void task()override;
    void SendTotalAverageToThreads();
    void CalculateNewAverage();
    void saveNewAverage(unsigned long Id,double average);
    
    typedef enum typeState{idle,ePrepareInfo,eSendTotalAverageToThreads}etypeState;
    etypeState m_state;
    list <CThread*> m_listThread;
    std::condition_variable m_cv;
    std::mutex m_mtx;
    map <unsigned long ,double > m_MapAverage;
    std::mutex m_Map_mtx;
    double m_dTotalAverage;
    double m_dLastTotalAverage;

};


void CThreadObserver::task()
{
    int l_ReadQtty=0;
    bool finish=false;
    char a;
    cout << "CThreadObserver::task(): start task!ThreadID:"<<std::this_thread::get_id()<< endl;
    std::unique_lock<std::mutex> lck(m_mtx);
    while (!finish)
    {
        switch(m_state)
        {
          case ePrepareInfo:
//              cout << "CThreadObserver::task(): It s going to sleep!ThreadID:"<<std::this_thread::get_id()<< endl;
              //m_cv.wait(lck,[&]{ return m_AddressQueue.size();});// Wait until event
              m_cv.wait(lck);// Wait until event
//              cout << "CThreadObserver::task(): Wake up!ThreadID:"<<std::this_thread::get_id()<< endl;
              CalculateNewAverage();
              m_state = eSendTotalAverageToThreads;
              break;
          case eSendTotalAverageToThreads:
              SendTotalAverageToThreads();
              m_state=ePrepareInfo;
              break;
        }

    }
    cout << "Task was finish!!! ThreadID:"<<std::this_thread::get_id()<< endl;
}
    
    
    
void CThreadObserver::SendTotalAverageToThreads()
{
 
//    cout<<"CThreadObserver::SendTotalAverageToThreads().ThreadID:"<<std::this_thread::get_id()<< endl;
    std::lock_guard<std::mutex> lock(m_Map_mtx);
    map <unsigned long ,double >::iterator it;
    for (it=m_MapAverage.begin();it!=m_MapAverage.end();++it)
    {
        CData* pObj= (CData*)it->first;
        pObj->setTotalAverage(m_dTotalAverage);
    }
}    

    
void CThreadObserver::CalculateNewAverage()
{
    cout<<"CThreadObserver::CalculateNewAverage().ThreadID:"<<std::this_thread::get_id()<< endl;
    map <unsigned long ,double >::iterator it;
    std::lock_guard<std::mutex> lock(m_Map_mtx);
    double dAverage=0;    
    for (it=m_MapAverage.begin();it!=m_MapAverage.end();++it)
    {
        dAverage+= it->second;
    }
    m_dTotalAverage = dAverage  / m_MapAverage.size();
    
}    

void CThreadObserver::saveNewAverage(unsigned long Id,double average)
{
    //cout<<"Try to lock Map mutex.ThreadID:"<<std::this_thread::get_id()<< endl;        
    std::lock_guard<std::mutex> lock(m_Map_mtx);
    //cout<<"mutex was locket.ThreadID:"<<std::this_thread::get_id()<< endl;
    if (m_MapAverage.find(Id)!=m_MapAverage.end())
    {
        // The element already exist!
        m_MapAverage[Id]=average;
        //cout<<"The element already exist in Map ! Copy new value.Id"<<Id<<".Average:"<<average<<"ThreadID:"<<std::this_thread::get_id()<< endl;        
    }
    else
    {
        m_MapAverage.insert( std::pair<unsigned long ,double >(Id,average));
        //cout<<"New element in Map ! Insert new value.Id"<<Id<<".Average:"<<average<<"ThreadID:"<<std::this_thread::get_id()<< endl;        
    } 
}    
    
void CThreadObserver::notify(unsigned long Id,double average)
{
    //cout<<"CThreadObserver::notify(unsigned long Id) - begin.ThreadID:"<<std::this_thread::get_id()<< endl;
    saveNewAverage(Id,average);
    std::unique_lock<std::mutex> lck(m_mtx);
    m_cv.notify_all();
    //cout<<"CThreadObserver::notify(unsigned long Id) - end.ThreadID:"<<std::this_thread::get_id()<< endl;
}


void CThreadObserver::setNewThreadServer(CThreadServer* pObj)
{
    //cout << "CThreadObserver::setNewThreadServer(CThreadServer* pObj)"<<endl;
    m_listThread.push_back(pObj);
    pObj->getDataPrt()->registerObserver(this);
}



void CThreadObserver::unsetThreadServer(CThread* pThread)
{
    std::lock_guard<std::mutex> lock(m_Map_mtx);
    CThreadServer* pObj = dynamic_cast<CThreadServer*>(pThread);
    list <CThread*>::iterator itList;
    for (itList=m_listThread.begin();itList!=m_listThread.end();++itList)
    {
        if (*itList== pObj)
        {
            unsigned long Id = (unsigned long)pObj->getDataPrt();
            map <unsigned long ,double >::iterator it=m_MapAverage.find(Id);
            if (it!=m_MapAverage.end())
            {
                m_MapAverage.erase(it);
            }
            //cout<< "CThreadObserver::unsetThreadServer():CThreadServer:Ready!"<<endl;
        }
    }
}