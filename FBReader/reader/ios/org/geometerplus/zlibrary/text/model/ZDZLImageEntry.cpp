//
//  ZDZLImageEntry.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/24.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZDZLImageEntry.hpp"


ZDZLImageEntry::ZDZLImageEntry(std::map<std::string, shared_ptr<ZLFileImage>> imageMap, std::string id, short vOffset, bool isCover)
{
    myImageMap = imageMap;
    Id = id;
    VOffset = vOffset;
    IsCover = isCover;
}

 shared_ptr<ZLFileImage> ZDZLImageEntry::getImage()
{
    return myImageMap[Id];//.at(Id);
}