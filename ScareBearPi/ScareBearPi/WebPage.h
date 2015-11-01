//
//  WebPage.h
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#ifndef __ScareBearPi__WebPage__
#define __ScareBearPi__WebPage__

#include <stdio.h>
#include <string>
#include "WebSession.h"

class WebPage
{
    std::string _url;
    std::string _mime;
    
public:
    
    WebPage();
    WebPage(std::string aURL);
    
    std::string getURL();
    std::string getMime();
    
    virtual std::string fillRequest(WebSession * aSession);
    virtual void handlePost(WebSession * aSession, const char * aData, size_t aDataLength);
};

#endif /* defined(__ScareBearPi__WebPage__) */
