//
//  GetCareFilesPage.h
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#ifndef __ScareBearPi__GetCareFilesPage__
#define __ScareBearPi__GetCareFilesPage__

#include <stdio.h>

#include "WebPage.h"

class GetCareFilesPage : public WebPage
{
public:
    GetCareFilesPage();
    
    std::string fillRequest(WebSession * aSession);
};

#endif /* defined(__ScareBearPi__GetCareFilesPage__) */
