/*
 * CDataClient.hpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */

#pragma once

#include <iostream>
#include <vector>
#include <random>

using namespace std;

class CDataClient
{
 public:
      CDataClient();
      virtual ~CDataClient(){};
      
      virtual void setNewData(char* Buffer,int bufferSize);
      virtual int  getData(char* Buffer,int bufferSize);
      virtual void generateVectorInformation();
      virtual void setDataSize(int DataSize){m_DataSize=DataSize;};
      virtual void clearData() {m_vecData.clear();};
 protected:
      virtual double generateRamdomValue();      
      int m_DataSize;
      int m_p;
      vector <double> m_vecData; 
      std::default_random_engine m_re;
      std::uniform_real_distribution<double> m_unif;
};

