//
//  main.cpp
//  ScareBearPi
//
//  Created by Doug on 10/31/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#include <iostream>

#include "ScareBear.h"

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


int main(int argc, const char * argv[])
{
    
    ScareBear::getInstance().run();

    return 0;
}
