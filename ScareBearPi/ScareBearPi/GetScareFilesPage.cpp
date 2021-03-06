//
//  GetMusicFilesPage.cpp
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#include "GetScareFilesPage.h"

#include <iostream>

#include "ScareBear.h"
#include "MusicFileManager.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

GetScareFilesPage::GetScareFilesPage() : WebPage("/getScareClips")
{
    
}

std::string GetScareFilesPage::fillRequest(WebSession * aSession)
{
    std::string result;
    
    auto list = ScareBear::getInstance().getMusicManager()->getCareFiles();
    
    Document d;
    d.Parse("{}");
    
    Value clips;
    clips.SetArray();
    for (auto it = list.begin(); it != list.end(); it++)
    {
        std::string clipName = *it;
        Value name;
        name.SetString(clipName.c_str(), clipName.length(), d.GetAllocator());
        clips.PushBack(name, d.GetAllocator());
    }
    d.AddMember("clips", clips, d.GetAllocator());
    
    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    
    result = buffer.GetString();
    
    return result;
}