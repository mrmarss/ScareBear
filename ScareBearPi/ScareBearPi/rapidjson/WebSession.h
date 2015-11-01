//
//  WebSession.h
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#ifndef __ScareBearPi__WebSession__
#define __ScareBearPi__WebSession__

#include <stdio.h>
#include <string>

class WebSession
{
    /**
     * Unique ID for this session.
     */
    std::string _sid;
    
public:
    
    /**
     * Reference counter giving the number of connections
     * currently using this session.
     */
    unsigned int rc;
    
    /**
     * Time when this session was last active.
     */
    time_t start;
    
    WebSession(std::string aSID, unsigned int aRC, time_t aStartTime);
    
    std::string getSID();
};

#endif /* defined(__ScareBearPi__WebSession__) */
