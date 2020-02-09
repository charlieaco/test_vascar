/*
 * CInformationManager.hpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */

#include <sys/socket.h>
#include <unistd.h> 
#include <thread>

#include "CInformationManager.hpp"


CInformationManager::CInformationManager()
{
    m_state = eIdle;
    cout<<"CThreadServer()"<<endl;
    m_iDelayBetweenMsg=1;
}

CInformationManager::~CInformationManager()
{
}


void CInformationManager::init(int iSocket,int iDelayBetweenMsg) 
{
    m_socket=iSocket;
    m_iDelayBetweenMsg = iDelayBetweenMsg;
}

bool CInformationManager::receiveTotalAverage()
{
    bool ret = false;
    int l_ReadQtty=0;
    
    cout << "CInformationManager::receiveTotalAverage(). Receive Information from server...."<< endl; 
    l_ReadQtty = read( m_socket, m_buffer, BUFFER_SIZE);
    //cout << "CInformationManager::receiveTotalAverage().Receive new data:"<< l_ReadQtty << "bytes."<< endl;
    
    if (l_ReadQtty > 0)
    {
        
        ///double* pBuffer = (double*)m_buffer;
        double Average = *((double*)m_buffer);
        // Average
        cout << "CInformationManager::receiveTotalAverage().Average:"<< Average << endl; 
    }
    if (l_ReadQtty == 0)       ret = true;
}


bool CInformationManager::receiveVectorSize()
{
    bool ret = false;
    int l_ReadQtty=0;
    
    //cout << "CInformationManager::receiveVectorSize(). Receive Information from server...."<< endl; 
    l_ReadQtty = read( m_socket, m_buffer, BUFFER_SIZE);
    //cout << "CInformationManager::receiveVectorSize().Receive new data:"<< l_ReadQtty << "bytes."<< endl;
    
    if (l_ReadQtty > 0)
    {
        
        int* pBuffer = (int*)m_buffer;
        // Set VectorSize 
        m_Data.setDataSize(*pBuffer);
    }
    if (l_ReadQtty == 0)       ret = true;
}

bool CInformationManager::processData()
{
    bool ret = false;
    int l_ReadQtty=0;
    
    //cout << "CThreadServer::processData(). Receive Information from server...."<< endl; 
    l_ReadQtty = read( m_socket, m_buffer, BUFFER_SIZE);
    //cout << "CThreadServer::processData().Receive new data:"<< l_ReadQtty << "bytes."<< endl;
    
    if (l_ReadQtty > 0)
    {
        int* pBuffer = (int*)m_buffer;
        m_Data.setDataSize(*pBuffer);
    }
    if (l_ReadQtty == 0)       ret = true;
     
    return ret; 
}


void CInformationManager::sendData()
{
  
  m_bufferSize=BUFFER_SIZE;
  int size = m_Data.getData(m_buffer,m_bufferSize);
  cout << "CInformationManager::sendData(). send data to server.Bytes:"<<size<< endl;
  if (send(m_socket, m_buffer, size,0 )>=0)
  {
  }
  
}


bool CInformationManager::stateMachine()
{
    bool bFinish=false;
    int iCicle=0;
    cout << "bool CInformationManager::stateMachine() Start!"<< endl;
    while (!bFinish)
    {
        switch(m_state)
        {
        case eReceiveSize:
            receiveVectorSize();
            m_state = eGenerateData;
            break;
        case eGenerateData:
            m_Data.generateVectorInformation();       
            m_state = eSendVector;
            break; 
        case eSendVector:
            sendData();
            //m_state = eDoNothing;
            m_state = eReceiveAverage;
            break; 
        case eReceiveAverage:
            receiveTotalAverage();
            m_state = eDoNothing;
            break; 
        case eDoNothing:
            std::this_thread::sleep_for (std::chrono::seconds(m_iDelayBetweenMsg));
            cout<<"Do nothing...Cicle:"<<iCicle++<<endl;
            m_state = eGenerateData;
            m_Data.clearData();           
            break; 
        case eIdle:
        default:
            m_state = eReceiveSize;
            break; 
        }
    }
    cout << "bool CInformationManager::stateMachine() End!"<< endl;
    return bFinish;
}
