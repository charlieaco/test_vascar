/*
 * CThreadServer.hpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */

#pragma once

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <chrono>
#include "CData.hpp"
#include "CThread.hpp"
//#include "CThreadObserver.hpp"


#define _DATA_SIZE 20 

using namespace std;

#define BUFFER_SIZE 1024*sizeof(double)

class CThreadServer: public CThread 
{
 public:
    CThreadServer():m_iDataSize(_DATA_SIZE),m_state(idle)
        {
            cout<<"CThreadServer()"<<endl;
        };
    ~CThreadServer(){};
    
    void init(int socket,int iDataSize) {m_iSocket=socket,m_iDataSize=iDataSize;};
    void registerObserver(IObserver *_ptr);
    void unregisterObserver(IObserver *_ptr);
    CData* getDataPrt() {return &m_Data;}

 protected:
    bool sendInformation();
    virtual void prepareInformation();      
    virtual bool processData()override;
    virtual void task()override;
    void prepareTotalAverage();
    
    int m_iSocket;
    CData m_Data;
    int m_iDataSize; 
    char m_buffer[BUFFER_SIZE];
    int m_iBufferSize;
    
    typedef enum typeState{idle,ePrepareInfo,eSendInformation,eReceiveVector,eCalculateNewAverage,eSendAverage}etypeState;
    etypeState m_state;
};


void CThreadServer::task()
{
    bool bFinish=false;
    try
    {
        cout << "CThreadServer::task().start task!ThreadID:"<<std::this_thread::get_id()<< endl;
        while (!bFinish)
        {
            switch(m_state)
            {
            case ePrepareInfo:
                prepareInformation();
                m_state = eSendInformation;
            case eSendInformation:
                sendInformation();
                m_state = eReceiveVector;
                break; 
            case eReceiveVector:
                bFinish=processData();
                m_state = eCalculateNewAverage;
                break; 
            case eCalculateNewAverage:
                m_Data.calculateAverage();
                m_Data.clearData();
                cout << "New Average:"<<m_Data.getAverage()<<endl;
                m_state = eSendAverage;
                break; 
            case eSendAverage:
                prepareTotalAverage();
                m_state = eSendInformation;
                break; 
            case idle:
            default:
                m_state = ePrepareInfo;
                break; 
            }
        }
        close(m_iSocket);
        m_pObserver->unsetThreadServer(this);
    }
    catch(...)
    {
        cout << "unhandle exception!!! ThreadID:"<<std::this_thread::get_id()<< endl;
    }
    cout << "Task was finish!!! ThreadID:"<<std::this_thread::get_id()<< endl;
}
    

void CThreadServer::prepareTotalAverage()
{
    
    prepareInformation();
    cout << "CThreadServer::prepareTotalAverage().TotalAverage:"<< m_Data.getTotalAverage()<<".ThreadID:"<<std::this_thread::get_id()<< endl;
    // Copy size information
    double *pbuffer = (double*)m_buffer;
    *pbuffer = m_Data.getTotalAverage();
    m_iBufferSize = sizeof (double);
}
    
void CThreadServer::prepareInformation()
{
    //cout << "CThreadServer::prepareInformation().ThreadID:"<<std::this_thread::get_id()<< endl;
    // Clear buffer
    memset(&m_buffer, 0, sizeof(m_buffer));
    // Copy size information
    int *pbuffer = (int*)m_buffer;
    *pbuffer = m_iDataSize;
    m_Data.setVectorSize( m_iDataSize);
    m_iBufferSize = sizeof (m_iDataSize);
}

bool CThreadServer::sendInformation()
{
    int Qtty; 
    cout << "CThreadServer::sendInformation().m_iBufferSize:"<<m_iBufferSize<<".ThreadID:"<<std::this_thread::get_id()<< endl;
    if ((Qtty = send(m_iSocket, m_buffer, m_iBufferSize,0 ))>=0)
    {
        // Send Size of data
        cout<<"CThreadServer::sendInformation() Information was sent >> Client Bytes["<<Qtty<<"].ThreadID:"<<std::this_thread::get_id()<< endl;
        return true;
    }
    return false;
}

bool CThreadServer::processData()
{
    bool ret = false;
    int iReadQtty=0;
    
    //cout << "CThreadServer::processData(). Receive Information from client....ThreadID:"<<std::this_thread::get_id()<< endl;
    iReadQtty = read( m_iSocket, m_buffer, BUFFER_SIZE);
    //cout << "CThreadServer::processData().Receive new data:"<< iReadQtty << "bytes.ThreadID:"<<std::this_thread::get_id()<< endl;

    // Checking the size of the recieved data!
    if (m_iDataSize*sizeof(double) ==iReadQtty)    
    {
        m_Data.setNewData(m_buffer,iReadQtty);
    }
    else 
    {
        // Data size is wrong! -> Finish!
        ret = true;
    }
    if (iReadQtty == 0)
    {
        // Data size is wrong! -> Finish!
        std::this_thread::sleep_for (std::chrono::seconds(1));
        ret = true;
    }       
    return ret; 
}




