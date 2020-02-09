/*
 * CInformationManager.hpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */

#pragma once

#include "CDataClient.hpp"


using namespace std;

#define BUFFER_SIZE sizeof(double)*1024

class CInformationManager
{
 public:
      CInformationManager();
      virtual~CInformationManager();

      void init(int iSocket,int iDelayBetweenMsg);
      bool stateMachine();

 protected:
      bool receiveVectorSize();
      bool receiveTotalAverage(); 
      virtual bool processData();
      void sendData();

      int m_socket;
      CDataClient m_Data;
      int  m_DataSize; 
      char m_buffer[BUFFER_SIZE];
      int m_bufferSize;
      typedef enum typeClientStates{eIdle,eReceiveSize,eGenerateData,eSendVector,eReceiveAverage,eDoNothing}_typeClientStates;
      _typeClientStates m_state ;
      int m_iDelayBetweenMsg; 
};

