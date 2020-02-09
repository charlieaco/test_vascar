/*
 * main.cpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */

#include <iostream>
#include <string.h>
#include <stdexcept> 
#include "CConnectionMgr.hpp"



using namespace std;


int main(int argc, char** argv)
{
    int iRet = 0;
    CConnectionMgr m_ConnMgr;
    

    try
    {
        m_ConnMgr.initServer(argc, argv);
        m_ConnMgr.waitingForNewClient();
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

    return iRet;
}


