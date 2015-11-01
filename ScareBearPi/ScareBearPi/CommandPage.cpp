//
//  CommandPage.cpp
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#include "CommandPage.h"

#include "ScareBear.h"
#include "MusicFileManager.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

CommandPage::CommandPage() : WebPage("/command")
{
    
}

void CommandPage::handlePost(WebSession * aSession, const char * aData, size_t aDataLength)
{
    if (aDataLength > 0)
    {
        Document d;
        d.Parse(aData);
        
        std::string cmd = d["command"].GetString();
        
        if (cmd == "change-state")
        {
            std::string state = d["state"].GetString();
            if (state == "scare")
            {
                ScareBear::getInstance().setScareMode(true);
            }
            else
            {
                ScareBear::getInstance().setScareMode(false);
            }
        }
        else if (cmd == "play")
        {
            std::string clipName = d["clip"].GetString();
            
            ScareBear::getInstance().getMusicManager()->playClip(clipName);
        }
    }
}