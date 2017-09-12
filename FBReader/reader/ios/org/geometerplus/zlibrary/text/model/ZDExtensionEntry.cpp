//
//  ZDExtensionEntry.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/24.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZDExtensionEntry.hpp"

ZDExtensionEntry::ZDExtensionEntry(std::string type, std::map<std::string,std::string> data)
{
    Type = type;
    Data = data;
}
 bool ZDExtensionEntry::equals(ZDExtensionEntry* another)
{
    if (this == another) {
        return true;
    }
    
    return Type == another->Type && Data == another->Data;
}
 int ZDExtensionEntry::hashCode()
{
    std::hash<std::string> d;
     std::hash<std::string> m;
    int c = d(Type);
    //int e = m(Data);
    return c+23;
    //return hash<string>(Type);
    //return Type.hashCode() + 23 * Data.hashCode();
}
