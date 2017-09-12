//
//  ZDZLVideoEntry.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/24.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZDZLVideoEntry.hpp"



ZDZLVideoEntry::ZDZLVideoEntry(){
   
   // mySources = new map<string,string>();
}

void ZDZLVideoEntry::addSource(std::string mime, std::string path)
{
    // add a item
   // mySources.inser
    mySources.insert(std::pair<std::string, std::string>(mime, path));
}
std::map<std::string,std::string> ZDZLVideoEntry::sources()
{
    return mySources;
}
