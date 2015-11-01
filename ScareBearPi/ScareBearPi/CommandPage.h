//
//  CommandPage.h
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#ifndef __ScareBearPi__CommandPage__
#define __ScareBearPi__CommandPage__

#include <stdio.h>

#include "WebPage.h"

class WebSession;

class CommandPage : public WebPage
{
    
public:
    CommandPage();
    
    void handlePost(WebSession * aSession, const char * aData, size_t aDataLength);
};

#endif /* defined(__ScareBearPi__CommandPage__) */
