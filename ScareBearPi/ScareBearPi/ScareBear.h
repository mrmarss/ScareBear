//
//  ScareBear.h
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#ifndef __ScareBearPi__ScareBear__
#define __ScareBearPi__ScareBear__

#include <stdio.h>
#include "rapidjson/document.h"
#include "WebServer.h"

class ModeHandler;
class MusicFileManager;

class ScareBear
{
public:
    static ScareBear& getInstance()
    {
        static ScareBear    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    
    enum Mode
    {
        PlayMusicMode
    };
    
    void setMode(rapidjson::Document & aConfig);
    
    MusicFileManager * getMusicManager();
    
    void run();
    
private:
    
    bool _isScareMode;
    ModeHandler * _modeHandler;
    MusicFileManager *_musicManager;
    WebServer *_webServer;
    
    ScareBear();
    ~ScareBear();
    
    // C++ 03
    // ========
    // Stop copying of Singleton
    ScareBear(ScareBear const&);              // Don't Implement
    void operator=(ScareBear const&); // Don't implement
};

#endif /* defined(__ScareBearPi__ScareBear__) */
