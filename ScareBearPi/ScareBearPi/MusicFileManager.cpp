//
//  MusicFileManager.cpp
//  ScareBearPi
//
//  Created by Doug on 11/1/15.
//  Copyright (c) 2015 Doug Roeper. All rights reserved.
//

#include "MusicFileManager.h"

#include <dirent.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>

#define kCARE   "care"
#define kSCARE  "scare"

MusicFileManager::MusicFileManager()
{
    
}

void MusicFileManager::setDirectory(std::string aDirectory)
{
    _directory = aDirectory;
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

std::list<std::string> MusicFileManager::getCareFiles()
{
    std::list<std::string> results;
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir((_directory + "/" + kCARE).c_str())) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
            std::string fileName = ent->d_name;
            if (fileName != "." && fileName != ".." && hasEnding(fileName, ".mp3"))
            {
                results.push_back(fileName);
            }
        }
        closedir (dir);
    }
    else
    {
        /* could not open directory */
        std::cout << "MusicFileManager - couldn't open music folder" << std::endl;
    }
    
    
    return results;
}

std::list<std::string> MusicFileManager::getScareFiles()
{
    std::list<std::string> results;
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir((_directory + "/" + kSCARE).c_str())) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
            std::string fileName = ent->d_name;
            if (fileName != "." && fileName != ".." && hasEnding(fileName, ".mp3"))
            {
                results.push_back(fileName);
            }
        }
        closedir (dir);
    }
    else
    {
        /* could not open directory */
        std::cout << "MusicFileManager - couldn't open music folder" << std::endl;
    }
    
    
    return results;
}

void MusicFileManager::playClip(std::string aClip)
{
    std::string fullPath = _directory + "/" + aClip;
    //system(("omxplayer " + fullPath).c_str());
    system(("afplay " + fullPath).c_str());
}



