//
//  WebPage.cpp
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#include "WebPage.h"

WebPage::WebPage()
{
    _mime = "text/html";
}

WebPage::WebPage(std::string aURL)
{
    _url = aURL;
}

std::string WebPage::getURL()
{
    return _url;
}

std::string WebPage::getMime()
{
    return _mime;
}

std::string WebPage::fillRequest(WebSession * aSession)
{
    return "HELLO!";
}

void WebPage::handlePost(WebSession * aSession, const char * aData, size_t aDataLength)
{
    
}
