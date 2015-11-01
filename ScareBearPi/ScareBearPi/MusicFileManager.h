//
//  MusicFileManager.h
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#ifndef __ScareBearPi__MusicFileManager__
#define __ScareBearPi__MusicFileManager__

#include <string>
#include <list>

class MusicFileManager
{
private:
    std::string _directory;
    
public:
    
    MusicFileManager();
    
    void setDirectory(std::string aDirectory);
    
    std::list<std::string> getFiles();
    
    
};

#endif /* defined(__ScareBearPi__MusicFileManager__) */
