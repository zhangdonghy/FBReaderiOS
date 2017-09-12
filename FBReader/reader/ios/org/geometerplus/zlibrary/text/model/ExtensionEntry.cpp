//
//  ExtensionEntry.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ExtensionEntry.hpp"
#include "ZDHash.hpp"


ExtensionEntry::ExtensionEntry(std::string type,std::map<std::string,std::string> data)
{
    Type = type;
    Data = data;
}
bool
ExtensionEntry::equals(shared_ptr<ExtensionEntry> other)
{
    return  Type == other->Type && Data == other->Data;
}

int ExtensionEntry::hashCode()
{
    return GetZDHash(Type);//+23*GetZDHash();
    //return 0;// zhangdong
}
