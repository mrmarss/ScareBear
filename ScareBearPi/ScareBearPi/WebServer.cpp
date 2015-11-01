//
//  WebServer.cpp
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#include "WebServer.h"

#include "WebSession.h"
#include "WebPage.h"

#include <iostream>

/**
 * Invalid method page.
 */
#define METHOD_ERROR "<html><head><title>Illegal request</title></head><body>Go away.</body></html>"

/**
 * Name of our cookie.
 */
#define COOKIE_NAME "session"

/**
 * Data kept per request.
 */
struct Request
{
    
    /**
     * Associated session.
     */
    WebSession *session;
    
    /**
     * Post processor handling form data (IF this is
     * a POST request).
     */
    struct MHD_PostProcessor *pp;
    
    /**
     * URL to serve in response to this POST (if this request
     * was a 'POST')
     */
    const char *post_url;
    
};

/**
 * Return the session handle for this connection, or
 * create one if this is a new user.
 */
WebSession * WebServer::getSession (struct MHD_Connection *connection)
{
    WebSession *result = NULL;
    const char *cookie;
    
    cookie = MHD_lookup_connection_value (connection,
                                          MHD_COOKIE_KIND,
                                          COOKIE_NAME);
    if (cookie != NULL)
    {
        for (auto it = _sessions.begin(); it != _sessions.end(); it++)
        {
            WebSession *current = *it;
            if (strcmp (cookie, current->getSID().c_str()))
            {
                result = current;
                break;
            }
        }
    }
    
    if (result == NULL)
    {
        /* not a super-secure way to generate a random session ID,
         but should do for a simple example... */
        std::string sid;
        sid += (unsigned int) rand ();
        sid += (unsigned int) rand ();
        sid += (unsigned int) rand ();
        sid += (unsigned int) rand ();
        WebSession *newSession = new WebSession(sid, 1, time(NULL));
        _sessions.push_back(newSession);
        result = newSession;
    }
    return result;
}


/**
 * Iterator over key-value pairs where the value
 * maybe made available in increments and/or may
 * not be zero-terminated.  Used for processing
 * POST data.
 *
 * @param cls user-specified closure
 * @param kind type of the value
 * @param key 0-terminated key for the value
 * @param filename name of the uploaded file, NULL if not known
 * @param content_type mime-type of the data, NULL if not known
 * @param transfer_encoding encoding of the data, NULL if not known
 * @param data pointer to size bytes of data at the
 *              specified offset
 * @param off offset of data in the overall value
 * @param size number of bytes in data available
 * @return MHD_YES to continue iterating,
 *         MHD_NO to abort the iteration
 */
static int
post_iterator (void *cls,
               enum MHD_ValueKind kind,
               const char *key,
               const char *filename,
               const char *content_type,
               const char *transfer_encoding,
               const char *data, uint64_t off, size_t size)
{
    struct Request *request = (struct Request *)cls;
    WebSession *session = request->session;
    
    if (0 == strcmp ("DONE", key))
    {
        fprintf (stdout,
                 "Session `%s' submitted something",
                 session->getSID().c_str());
        return MHD_YES;
    }
    /*
    if (0 == strcmp ("v1", key))
    {
        if (size + off >= sizeof(session->value_1))
            size = sizeof (session->value_1) - off - 1;
        memcpy (&session->value_1[off],
                data,
                size);
        session->value_1[size+off] = '\0';
        return MHD_YES;
    }
     */
    fprintf (stderr, "Unsupported form value `%s'\n", key);
    return MHD_YES;
}


/**
 * Callback called upon completion of a request.
 * Decrements session reference counter.
 *
 * @param cls not used
 * @param connection connection that completed
 * @param con_cls session handle
 * @param toe status code
 */
static void
request_completed_callback (void *cls,
                            struct MHD_Connection *connection,
                            void **con_cls,
                            enum MHD_RequestTerminationCode toe)
{
    struct Request *request = (struct Request *)*con_cls;
    
    if (NULL == request)
        return;
    if (NULL != request->session)
        request->session->rc--;
    if (NULL != request->pp)
        MHD_destroy_post_processor (request->pp);
    free (request);
}


/**
 * Clean up handles of sessions that have been idle for
 * too long.
 */
void WebServer::expireSessions()
{
    /*
    struct Session *pos;
    struct Session *prev;
    struct Session *next;
    time_t now;
    
    now = time (NULL);
    prev = NULL;
    pos = sessions;
    while (NULL != pos)
    {
        next = pos->next;
        if (now - pos->start > 60 * 60)
        {
            // expire sessions after 1h
            if (NULL == prev)
                sessions = pos->next;
            else
                prev->next = next;
            free (pos);
        }
        else
            prev = pos;
        pos = next;
    }
     */
    std::cout << "Need to expire sessions" <<std::endl;
}


/**
 * Main MHD callback for handling requests.
 *
 * @param cls argument given together with the function
 *        pointer when the handler was registered with MHD
 * @param connection handle identifying the incoming connection
 * @param url the requested url
 * @param method the HTTP method used ("GET", "PUT", etc.)
 * @param version the HTTP version string (i.e. "HTTP/1.1")
 * @param upload_data the data being uploaded (excluding HEADERS,
 *        for a POST that fits into memory and that is encoded
 *        with a supported encoding, the POST data will NOT be
 *        given in upload_data and is instead available as
 *        part of MHD_get_connection_values; very large POST
 *        data *will* be made available incrementally in
 *        upload_data)
 * @param upload_data_size set initially to the size of the
 *        upload_data provided; the method must update this
 *        value to the number of bytes NOT processed;
 * @param ptr pointer that the callback can set to some
 *        address and that will be preserved by MHD for future
 *        calls for this request; since the access handler may
 *        be called many times (i.e., for a PUT/POST operation
 *        with plenty of upload data) this allows the application
 *        to easily associate some request-specific state.
 *        If necessary, this state can be cleaned up in the
 *        global "MHD_RequestCompleted" callback (which
 *        can be set with the MHD_OPTION_NOTIFY_COMPLETED).
 *        Initially, <tt>*con_cls</tt> will be NULL.
 * @return MHS_YES if the connection was handled successfully,
 *         MHS_NO if the socket must be closed due to a serios
 *         error while handling the request
 */
static int
create_response (void *cls,
                 struct MHD_Connection *connection,
                 const char *url,
                 const char *method,
                 const char *version,
                 const char *upload_data,
                 size_t *upload_data_size,
                 void **ptr)
{
    if (cls != NULL)
    {
    WebServer *server = (WebServer *)cls;
    return server->createResponse(connection, url, method, version, upload_data, upload_data_size, ptr);
    }
    
    return MHD_NO;
}


WebServer::WebServer(int aPort)
{
    _port = aPort;
    _notFoundPage = new WebPage();
}

void WebServer::addPage(WebPage * aPage)
{
    _pages.push_back(aPage);
}

void WebServer::start()
{
    /* initialize PRNG */
    srand ((unsigned int) time (NULL));
    d = MHD_start_daemon (MHD_USE_DEBUG,
                          _port,
                          NULL, NULL,
                          &create_response, this,
                          MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int) 15,
                          MHD_OPTION_NOTIFY_COMPLETED, &request_completed_callback, NULL,
                          MHD_OPTION_END);
    
    if (d == NULL)
    {
        std::cout << "Unable to start daemon!" << std::endl;
    }
}

void WebServer::run()
{
    expireSessions ();
    max = 0;
    FD_ZERO (&rs);
    FD_ZERO (&ws);
    FD_ZERO (&es);
    if (MHD_YES != MHD_get_fdset (d, &rs, &ws, &es, &max))
        return; /* fatal internal error */
    if (MHD_get_timeout (d, &mhd_timeout) == MHD_YES)
    {
        tv.tv_sec = mhd_timeout / 1000;
        tv.tv_usec = (mhd_timeout - (tv.tv_sec * 1000)) * 1000;
        tvp = &tv;
    }
    else
        tvp = NULL;
    select (max + 1, &rs, &ws, &es, tvp);
    MHD_run (d);
}

int WebServer::createResponse (struct MHD_Connection *connection,
                               const char *url,
                               const char *method,
                               const char *version,
                               const char *upload_data,
                               size_t *upload_data_size,
                               void **ptr)
{
    struct MHD_Response *response;
    struct Request *request;
    WebSession *session;
    int ret = MHD_NO;
    unsigned int i;
    
    request = (struct Request *)*ptr;
    if (NULL == request)
    {
        request = (struct Request *)calloc (1, sizeof (struct Request));
        if (NULL == request)
        {
            fprintf (stderr, "calloc error: %s\n", strerror (errno));
            return MHD_NO;
        }
        *ptr = request;
        if (0 == strcmp (method, MHD_HTTP_METHOD_POST))
        {
            request->pp = MHD_create_post_processor (connection, 1024, &post_iterator, request);
            if (NULL == request->pp)
            {
                fprintf (stderr, "Failed to setup post processor for `%s'\n",
                         url);
                return MHD_NO; /* internal error */
            }
        }
        return MHD_YES;
    }
    if (NULL == request->session)
    {
        request->session = getSession(connection);
        if (NULL == request->session)
        {
            fprintf (stderr, "Failed to setup session for `%s'\n",
                     url);
            return MHD_NO; /* internal error */
        }
    }
    session = request->session;
    session->start = time (NULL);
    if (0 == strcmp (method, MHD_HTTP_METHOD_POST))
    {
        /* evaluate POST data */
        MHD_post_process (request->pp,
                          upload_data,
                          *upload_data_size);
        /*
        if (0 != *upload_data_size)
        {
            *upload_data_size = 0;
            return MHD_YES;
        }
        */
        /* done with POST data, serve response */
        MHD_destroy_post_processor (request->pp);
        request->pp = NULL;
        method = MHD_HTTP_METHOD_GET; /* fake 'GET' */
        if (NULL != request->post_url)
            url = request->post_url;
        
        
        //handle data
        /* find out which page to serve */
        WebPage * page = NULL;
        for (auto it = _pages.begin(); it != _pages.end(); it++)
        {
            WebPage *current = *it;
            if (strcmp (current->getURL().c_str(), url) == 0)
            {
                page = current;
                break;
            }
        }
        
        if (page != NULL)
        {
            page->handlePost(session, upload_data, *upload_data_size);
        }
    }
    
    if ( (0 == strcmp (method, MHD_HTTP_METHOD_GET)) ||
        (0 == strcmp (method, MHD_HTTP_METHOD_HEAD)) )
    {
        /* find out which page to serve */
        WebPage * page = NULL;
        for (auto it = _pages.begin(); it != _pages.end(); it++)
        {
            WebPage *current = *it;
            if (strcmp (current->getURL().c_str(), url) == 0)
            {
                page = current;
                break;
            }
        }
        
        if (page == NULL)
        {
            page = _notFoundPage;
        }
        
        std::cout << "Serving page" << std::endl;
        
        struct MHD_Response *response;
        
        std::string replyString = page->fillRequest(session);
        // return static form
        response = MHD_create_response_from_buffer (strlen (replyString.c_str()),
                                                    (void *) replyString.c_str(),
                                                    MHD_RESPMEM_MUST_COPY);
        
        if (NULL != response)
        {
            addSessionCookie(session, response);
            MHD_add_response_header (response,
                                     MHD_HTTP_HEADER_CONTENT_ENCODING,
                                     page->getMime().c_str());
            ret = MHD_queue_response (connection,
                                      MHD_HTTP_OK,
                                      response);
            MHD_destroy_response (response);
        }
        
        /*
         
         i=0;
         while ( (pages[i].url != NULL) &&
         (0 != strcmp (pages[i].url, url)) )
         i++;
         ret = pages[i].handler (pages[i].handler_cls,
         pages[i].mime,
         session, connection);
         */
        if (ret != MHD_YES)
            fprintf (stderr, "Failed to create page for `%s'\n", url);
        return ret;
    }
    /* unsupported HTTP method */
    response = MHD_create_response_from_buffer (strlen (METHOD_ERROR),
                                                (void *) METHOD_ERROR,
                                                MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response (connection,
                              MHD_HTTP_NOT_ACCEPTABLE,
                              response);
    MHD_destroy_response (response);
    return ret;
}

/**
 * Add header to response to set a session cookie.
 *
 * @param session session to use
 * @param response response to modify
 */
void WebServer::addSessionCookie (WebSession *session, struct MHD_Response *response)
{
    char cstr[256];
    snprintf (cstr,
              sizeof (cstr),
              "%s=%s",
              COOKIE_NAME,
              session->getSID().c_str());
    if (MHD_NO ==
        MHD_add_response_header (response,
                                 MHD_HTTP_HEADER_SET_COOKIE,
                                 cstr))
    {
        fprintf (stderr,
                 "Failed to set session cookie header!\n");
    }
}

void WebServer::stop()
{
    MHD_stop_daemon (d);
}


