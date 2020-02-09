/*
 * CConnectionMgr.cpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */


#include <unistd.h> 
#include <string.h> 

#include "CConnectionMgr.hpp"
#include "CThread.hpp"
#include "CThreadServer.hpp"
#include "CThreadObserver.hpp"


#define PORT 5001 
using namespace std;

CConnectionMgr::CConnectionMgr():m_iConnectionQtty(2),m_opt(1),m_iArgSize(0) 
{
//    cout <<"CConnectionMgr()"<<endl; 
}

bool CConnectionMgr::checkCommandLine(int argc, char** argv)
{
    if (argc==1)
    {
        throw std::runtime_error("Please exectute : > ./TCP_server XX  -> XX is size of vector");
    }
    m_iArgSize = atoi(argv[1]);

    return true;
}


void CConnectionMgr::initServer(int argc, char** argv)
{
    cout <<"CConnectionMgr::initServer()"<<endl;
    
    checkCommandLine(argc, argv);
     
    if ((m_iServer_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        throw std::runtime_error("socket failed"); 
    } 
    cout<<"socket Success"<<endl;
       
    if (setsockopt(m_iServer_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &m_opt, sizeof(m_opt))) 
    { 
        throw std::runtime_error("setsockopt failed"); 
    } 
    cout<<"setsockopt Success"<<endl;
    m_address.sin_family = AF_INET; 
    m_address.sin_addr.s_addr = INADDR_ANY; 
    m_address.sin_port = htons( PORT ); 
       
    if (bind(m_iServer_fd, (struct sockaddr *)&m_address,  
                                 sizeof(m_address))<0) 
    { 
        throw std::runtime_error("bind failed"); 
    }
    cout<<"bind Success"<<endl;
    m_pObserver= new CThreadObserver;
    if (m_pObserver==nullptr)
    {
        throw std::runtime_error("ERROR new CThreadObserver");
    }
    m_pObserver->createThread();
}

void CConnectionMgr::waitingForNewClient()
{
    cout << "CConnectionMgr::waitingForNewClient()"<<endl;
    bool bFinish=false;
    int iNew_socket;
    int iAddrLen = sizeof(m_address);
    while (!bFinish)
    {
        if (listen(m_iServer_fd, m_iConnectionQtty) < 0) 
        { 
            throw std::runtime_error("listen error!!!");
        } 
        if ((iNew_socket = accept(m_iServer_fd, (struct sockaddr *)&m_address,  
                         (socklen_t*)&iAddrLen))<0) 
        { 
            throw std::runtime_error("accept error!!!");
        }
        CThreadServer* pThreadObj= new CThreadServer;
        
        if (pThreadObj==nullptr)
        {
            throw std::runtime_error( "ERROR new CThreadServer!!");
        }
        cout <<"new CThreadServer was created!"<<endl; 
        
        m_pObserver->setNewThreadServer(pThreadObj);
        pThreadObj->init(iNew_socket,m_iArgSize);
        pThreadObj->createThread();
        pThreadObj->setObserver(m_pObserver);
    }
}


