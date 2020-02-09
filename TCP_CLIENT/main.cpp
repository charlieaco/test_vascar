/*
 * main.cpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */

#include <stdexcept>   

#include "CClientConnectionMgr.hpp"

using namespace std;
   
int main(int argc, char** argv) 
{
    int iRet=0; 
    CClientConnectionMgr m_ClientConnMgr;
    bool finish=false;
    try
    {
        m_ClientConnMgr.initClient(argc, argv);
        while (!finish)
        {
            finish = m_ClientConnMgr.run();
        }
        cout << "Client is going to finish!!!"<< endl;
    }


    catch(const std::exception& e) 
    {
        cout << ("exception:")<< e.what()<<endl;
        iRet = 5;
    }
    catch (...)
    {
        cout << ("Unhandled exception !!!")<<endl;
        iRet = 10;
    }

    return 0; 
} 



