//
//  ModeHandler.cpp
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#include "ModeHandler.h"

#include <iostream>

#define kCONFIG_NAME_KEY    "mode"

ModeHandler::ModeHandler(rapidjson::Document & aConfig)
{
    //check if there is a mode element in the config
    if (aConfig.HasMember(kCONFIG_NAME_KEY))
    {
        //set is as the name
        rapidjson::Value & name = aConfig[kCONFIG_NAME_KEY];
        _name = name.GetString();
    }
    else
    {
        std::cout << "ModeHandler - No mode in config json!" << std::endl;
    }
}