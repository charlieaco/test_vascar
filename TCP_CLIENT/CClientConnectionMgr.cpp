/*
 * CClientConnectionMgr.cpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */

#include <arpa/inet.h> 
#include "CClientConnectionMgr.hpp"

using namespace std;


CClientConnectionMgr::CClientConnectionMgr():m_argSize(0)
{
    cout <<"CClientConnectionMgr()"<<endl; 
}

bool CClientConnectionMgr::checkCommandLine(int argc, char** argv)
{
    //host, port and time between messages in seconds
    if (argc<4)
    {
        std::string strMessage ("Please exectute : > ./TCP_client xxx.xxx.xxx.xxx ppp ss\n");
        strMessage += "where ->xxx.xxx.xxx.xxx is the host IP address,\n";
        strMessage += "        ppp is Port,\n";
        strMessage += "        ss is time between messages in seconds.\n";
        throw std::runtime_error(strMessage);    
    }
    // get host
    m_strIPAddress = argv[1];
    
    // get port
    m_iPort = std::atoi(argv[2]);
    if (m_iPort ==0)   
    {
        std::string strMessage ("Please exectute : > ./TCP_client xxx.xxx.xxx.xxx ppp ss\n");
        strMessage+= "ppp must no be ZERO\n";
        throw std::runtime_error(strMessage);    
    }
    
    // get time between messages in seconds
    m_iDelayBetweenMsg = std::atoi(argv[3]);
    if (m_iDelayBetweenMsg ==0)   
    {
        std::string strMessage ("Please exectute : > ./TCP_client xxx.xxx.xxx.xxx ppp ss\n");
        strMessage+= "ss must no be ZERO\n";
        throw std::runtime_error(strMessage);    
    }
    cout << "Client configuration IPAddress:"<<m_strIPAddress <<".Port:"<<m_iPort<<endl;
    cout << "Time between messages in seconds:"<<m_iDelayBetweenMsg<<endl;  
}


void CClientConnectionMgr::initClient(int argc, char** argv)
{
    cout <<"CClientConnectionMgr::initClient()"<<endl;

    checkCommandLine(argc, argv);
     
    char buffer[1024] = {0}; 
    if ((m_iSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        std::string strMessage ("Socket creation error\n");
        throw std::runtime_error(strMessage);    
    } 
   
    m_serv_addr.sin_family = AF_INET; 
    m_serv_addr.sin_port = htons(m_iPort); 

    cout << "It is going to connect to :"<<m_strIPAddress<<"-Port"<< m_iPort<< endl;        
    if(inet_pton(AF_INET, m_strIPAddress.c_str(), &m_serv_addr.sin_addr)<=0)  
    { 
        std::string strMessage ("Invalid address/ Address not supported \n");
        throw std::runtime_error(strMessage);    
    } 
   
    if (connect(m_iSock, (struct sockaddr *)&m_serv_addr, sizeof(m_serv_addr)) < 0) 
    { 
        std::string strMessage ("Connection Failed\n");
        throw std::runtime_error(strMessage);    
    }
    m_InfoMgr.init(m_iSock,m_iDelayBetweenMsg);

    cout <<"CClientConnectionMgr::initClient():Connected!"<<endl;



}

bool CClientConnectionMgr::run()
{
    return m_InfoMgr.stateMachine();
}
