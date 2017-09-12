//
//  Config.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/14.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef Config_hpp
#define Config_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include "StringPair.hpp"

class Config
{
private:
    static Config* ourInstance;
    static std::string myNullString;
    std::map<StringPair*, std::string>  myCache;
    std::set<std::string> myCacheGroups;
public:
    Config(){
        ourInstance = this;
    }
    static Config* Instance();
    std::string getValue(StringPair* id,std::string defaultValue);
    void setValue(StringPair*id,std::string value);
    
    void requestAllValuesForGroup(std::string group);
    void unsetValue(StringPair* id);
    void setToCache(std::string group, std::string name, std::string value) ;
};

#endif /* Config_hpp */
