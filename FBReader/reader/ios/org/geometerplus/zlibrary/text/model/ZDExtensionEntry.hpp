//
//  ZDExtensionEntry.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/24.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDExtensionEntry_hpp
#define ZDExtensionEntry_hpp

#include <stdio.h>
#include <map>
#include <string>


class ZDExtensionEntry {
public:
    std::string Type;
    std::map<std::string,std::string> Data;
public:
    ZDExtensionEntry(std::string type, std::map<std::string,std::string> data);
    virtual bool equals(ZDExtensionEntry* another);
    virtual int hashCode();
};
#endif /* ZDExtensionEntry_hpp */
