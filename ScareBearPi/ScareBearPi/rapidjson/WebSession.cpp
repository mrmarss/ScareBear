//
//  WebSession.cpp
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#include "WebSession.h"


WebSession::WebSession(std::string aSID, unsigned int aRC, time_t aStartTime)
{
    _sid = aSID;
    this->rc = aRC;
    this->start = aStartTime;
}

std::string WebSession::getSID()
{
    return _sid;
}