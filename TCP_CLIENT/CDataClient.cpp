/*
 * CDataClient.hpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */


#include <iostream>
#include <vector>

#include "CDataClient.hpp"

using namespace std;


CDataClient::CDataClient():m_unif((double)0,(double)10000),m_DataSize(0)
{
    cout<<"CDataClient()"<<endl;
}

void CDataClient::setNewData(char* Buffer,int bufferSize)
{
    double* pBuffer = (double*)Buffer;
    while (pBuffer <= (double*)&Buffer[bufferSize])
    {
        m_vecData.push_back(*pBuffer);
        cout << "CThreadServer::processData() Read:"<<*pBuffer<<endl;
        ++pBuffer;
    } 
}

int CDataClient::getData(char* Buffer,int bufferSize)
{
    int restSize=0;
    double* pBuffer = (double*)Buffer;
    for(auto it : m_vecData)
    {
        *pBuffer=it;
        cout<< "Dato:"<< it <<".";
        ++pBuffer;
        restSize+= sizeof (double);
        if ((restSize + sizeof (double)) > bufferSize)
        {
            cout<< "Size of Bufffer:"<<bufferSize << "restSize:"<< restSize<< endl;;
            break;
        }
    }
    cout<< endl;
    return restSize;
}


double CDataClient::generateRamdomValue()
{
    double a_random_double = m_unif(m_re);
    cout <<"New Random value:"<<a_random_double <<endl;
    return a_random_double;
}


void CDataClient::generateVectorInformation()
{
    cout <<"CDataClient::generateVectorInformation()"<<endl;
    for (int i=0;i<m_DataSize;++i)
    {
        m_vecData.push_back(generateRamdomValue());
    }
}
