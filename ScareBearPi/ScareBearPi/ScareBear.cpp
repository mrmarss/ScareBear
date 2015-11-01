//
//  ScareBear.cpp
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#include "ScareBear.h"

#include "MusicFileManager.h"
#include "GetMusicFilesPage.h"

#include <cstdlib>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <dirent.h>

#define kDATA_FOLDER    "/Users/Doug/ScareBear"
#define kMUSIC_FOLDER   "music"

ScareBear::ScareBear()
{
    _isScareMode = false;
    _musicManager = new MusicFileManager();
    std::string musicFolder = kDATA_FOLDER;
    musicFolder += "/";
    musicFolder += kMUSIC_FOLDER;
    _musicManager->setDirectory(musicFolder);
    
    //make the folder if it doesn't exist
    //system() is hacky, but I didn't want to use boost
    system(("mkdir -p " + musicFolder).c_str());
    
    _webServer = new WebServer(8888);
    _webServer->addPage(new GetMusicFilesPage());
    _webServer->start();
}

ScareBear::~ScareBear()
{
    std::cout << "You should probably clean up" << std::endl;
    
    if (_webServer != NULL)
    {
        _webServer->stop();
    }
}


void ScareBear::run()
{
    bool done = false;
    while (!done)
    {
        
        _webServer->run();
    }
}

MusicFileManager * ScareBear::getMusicManager()
{
    return _musicManager;
}