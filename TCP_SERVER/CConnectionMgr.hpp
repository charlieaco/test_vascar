/*
 * CConnectionMgr.hpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */

 
#pragma once

#include <netinet/in.h> 


class CThreadObserver;
class CThread;

class CConnectionMgr
{
public:
    CConnectionMgr();
    virtual ~CConnectionMgr(){};
    virtual void initServer(int argc, char** argv);
    virtual void waitingForNewClient();

protected:
    bool checkCommandLine(int argc, char** argv);
    
    int m_iConnectionQtty;
    int m_iServer_fd;
    struct sockaddr_in m_address;
    int m_opt ;
    CThreadObserver* m_pObserver; 
    int m_iArgSize;
    
};
