//
//  GetMusicFilesPage.h
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#ifndef __ScareBearPi__GetMusicFilesPage__
#define __ScareBearPi__GetMusicFilesPage__

#include <stdio.h>

#include "WebPage.h"

class GetScareFilesPage : public WebPage
{
public:
    GetScareFilesPage();
    
    std::string fillRequest(WebSession * aSession);
};

#endif /* defined(__ScareBearPi__GetMusicFilesPage__) */
