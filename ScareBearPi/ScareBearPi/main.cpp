//
//  main.cpp
//  ScareBearPi
//
//  Created by Doug on 10/31/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#include <iostream>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

#include "microhttpd.h"
#define PORT 8888

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

std::string jsonTest()
{
    // 1. Parse a JSON string into DOM.
    //const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    Document d;
    d.Parse("{}");

    Value name;
    name.SetString("Doug R.");
    d.AddMember("Name", name, d.GetAllocator());
    
    Value age;
    age.SetInt(19);
    d.AddMember("Age", age, d.GetAllocator());
    
    Value email;
    email.SetString("email@test.com");
    d.AddMember("Email", email, d.GetAllocator());
    
    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    
    // Output {"project":"rapidjson","stars":11, etc...}
    return buffer.GetString();
}

int answer_to_connection (void *cls, struct MHD_Connection *connection,
                          const char *url,
                          const char *method, const char *version,
                          const char *upload_data,
                          size_t *upload_data_size, void **con_cls)
{
    const char *page  = jsonTest().c_str();
    
    struct MHD_Response *response;
    int ret;
    
    response = MHD_create_response_from_buffer (strlen (page),
                                                (void*) page, MHD_RESPMEM_PERSISTENT);
    
    ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
    MHD_destroy_response (response);
    
    return ret;
}


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Web server up!\n";
    
    
    struct MHD_Daemon *daemon;
    
    daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                               &answer_to_connection, NULL, MHD_OPTION_END);
    if (NULL == daemon) return 1;
    
    jsonTest();
    
    getchar ();
    
    MHD_stop_daemon (daemon);
    
    
    return 0;
}
