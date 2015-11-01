//
//  GetMusicFilesPage.cpp
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#include "GetMusicFilesPage.h"

#include <iostream>

#include "ScareBear.h"
#include "MusicFileManager.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

GetMusicFilesPage::GetMusicFilesPage() : WebPage("/getClips")
{
    
}

std::string GetMusicFilesPage::fillRequest(WebSession * aSession)
{
    std::string result;
    
    auto list = ScareBear::getInstance().getMusicManager()->getFiles();
    
    Document d;
    d.Parse("{}");
    
    Value clips;
    clips.SetArray();
    for (auto it = list.begin(); it != list.end(); it++)
    {
        std::string clipName = *it;
        Value name;
        std::cout << clipName << std::endl;
        name.SetString(clipName.c_str(), clipName.length());
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