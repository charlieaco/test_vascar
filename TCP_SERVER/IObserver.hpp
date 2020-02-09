/*
 * IObserver.hpp
 *
 *  Created on: 07 Feb. 2020
 *      Author: Acosta, 
 */

#pragma once

class IObserver {
public:
    virtual void notify(unsigned long Id,double) = 0;
    virtual void notify() = 0;
};
