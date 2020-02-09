/*
 * CData.hpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */

#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include "IObserver.hpp"

using namespace std;



class CData
{
 public:
      CData():m_dLastValue(0),m_dTotalAverage(0),m_bTotalAverageAvailable(false)
      {
        cout<<"CData()"<<endl;
      };
      virtual ~CData(){};
      virtual void setNewData(char* Buffer,int bufferSize);
      virtual int getData(char* Buffer,int bufferSize);
      double getAverage() {return m_dLastValue;};
      virtual void setVectorSize(int vectorSize) {m_iVectorSize=vectorSize;};
      void setTotalAverage (double dTotalAverage);
      double getTotalAverage ();
      double calculateAverage();
      bool chekReceivedData();
      void clearData() {m_vecThreadData.clear();};
      void registerObserver(IObserver *_ptr) ;
      void unregisterObserver(IObserver *_ptr);
 protected:      
      vector<double> m_vecThreadData;
      std::thread* m_thread;
      int m_iVectorSize;
      double m_dLastValue;
      double m_dTotalAverage;
      IObserver *m_pObserver;
      bool m_bTotalAverageAvailable;
       
};



double CData::getTotalAverage () 
{
    m_bTotalAverageAvailable= false;
    return m_dTotalAverage;
}

void CData::setTotalAverage (double dTotalAverage) 
{
    if (m_dTotalAverage!=dTotalAverage)
        m_bTotalAverageAvailable= true;
    m_dTotalAverage=dTotalAverage;
}

void CData::setNewData(char* Buffer,int bufferSize)
{
    cout << "CData::setNewData"<<endl;
    double* pBuffer = (double*)Buffer;
    int i=0;
    while (pBuffer < (double*)&Buffer[bufferSize])
    {
        m_vecThreadData.push_back(*pBuffer);
        cout<< "V["<<i<<"]:"<< *pBuffer<<".ThreadID:"<<std::this_thread::get_id()<< endl;
        ++i;
        ++pBuffer;
    } 
    
}

int CData::getData(char* Buffer,int bufferSize)
{
    int restSize=0;
    int i=0;
    double* pBuffer = (double*)Buffer;
    for(auto it : m_vecThreadData)
    {
        *pBuffer=it;
        cout<< "Elem:"<<i<<"Value:"<< it<<".ThreadID:"<<std::this_thread::get_id()<< endl;
        ++pBuffer;
        restSize+= sizeof (double);
        if ((restSize + sizeof (double)) > bufferSize)
            break; 
    }
    return restSize;
}


double CData::calculateAverage()
{
    double average =0;
    if (m_vecThreadData.size()>0)
    {
        for(auto it : m_vecThreadData)
        {
            average += it; 
        }
        average = average / m_vecThreadData.size();
        if (m_pObserver!=nullptr)
            m_pObserver->notify((unsigned long) this,average);
    }
    m_dLastValue = average;    
    cout<< "Average:"<<average;
    return average; 
}



bool CData::chekReceivedData()
{
    return (m_iVectorSize== m_vecThreadData.size());  
}


void CData::registerObserver(IObserver *_ptr)
{
    if (_ptr == nullptr)
    {
        throw "EXCEPTION NULL POINTER!!";
    }
    m_pObserver= _ptr;
}


void CData::unregisterObserver(IObserver *_ptr)
{
    m_pObserver= nullptr;

}
