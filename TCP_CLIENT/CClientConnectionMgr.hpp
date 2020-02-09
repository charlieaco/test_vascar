/*
 * CClientConnectionMgr.hpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */

 
#pragma once

#include <netinet/in.h> 
#include "CInformationManager.hpp"

 

class CClientConnectionMgr
{
public:
    CClientConnectionMgr();
    virtual ~CClientConnectionMgr(){};
    virtual void initClient(int argc, char** argv);
    virtual bool run();

protected:
    bool checkCommandLine(int argc, char** argv);
    
    struct sockaddr_in m_address;
    int m_argSize;
    std::string m_strIPAddress;// Server Address
    int  m_iPort;            // Server Port             
    int m_iDelayBetweenMsg;  // time between messages in seconds
    int m_iSock ; // Socket
    struct sockaddr_in m_serv_addr;
    typedef enum typeState{idle,eReceiveVectorSize,ePrepareVector,eSendVector}etypeState;
    etypeState m_state;
    CInformationManager m_InfoMgr;
};
