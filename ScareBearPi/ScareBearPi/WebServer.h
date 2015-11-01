//
//  WebServer.h
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#ifndef __ScareBearPi__WebServer__
#define __ScareBearPi__WebServer__

#include <stdio.h>
#include <list>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "microhttpd.h"

class WebPage;
class WebSession;

class WebServer
{
private:
    std::list<WebPage *> _pages;
    std::list<WebSession *> _sessions;
    
    struct MHD_Daemon *d;
    struct timeval tv;
    struct timeval *tvp;
    fd_set rs;
    fd_set ws;
    fd_set es;
    MHD_socket max;
    MHD_UNSIGNED_LONG_LONG mhd_timeout;
    int _port;
    
    WebPage *_notFoundPage;
    
public:
    
    WebServer(int aPort);
    
    void addPage(WebPage * aPage);
    
    void start();
    void run();
    void stop();
    
    int createResponse (struct MHD_Connection *connection,
                                   const char *url,
                                   const char *method,
                                   const char *version,
                                   const char *upload_data,
                                   size_t *upload_data_size,
                                   void **ptr);
    WebSession * getSession(struct MHD_Connection *connection);
    
    void addSessionCookie (WebSession *session, struct MHD_Response *response);
    void expireSessions();
};

#endif /* defined(__ScareBearPi__WebServer__) */
