//
//  ModeHandler.h
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#ifndef __ScareBearPi__ModeHandler__
#define __ScareBearPi__ModeHandler__

#include <string>
#include "rapidjson/document.h"

// Defines a certian behavior for the bear and handles controlling the bear in that mode.
// Subclass for new mode.
class ModeHandler
{
private:
    std::string _name;

public:
    ModeHandler(rapidjson::Document & aConfig);
};

#endif /* defined(__ScareBearPi__ModeHandler__) */
