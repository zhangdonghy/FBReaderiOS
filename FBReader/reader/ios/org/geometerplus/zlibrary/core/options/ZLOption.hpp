//
//  ZLOption.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLOption_hpp
#define ZLOption_hpp

#include <stdio.h>
#include <string>
#include "StringPair.hpp"
#include "Config.hpp"
class ZLOption
{
    private:
    StringPair* myId;
    protected:
     std::string myDefaultStringValue;
     std::string mySpecialName;
     std::string getConfigValue() {
         Config* config = Config::Instance();
        return config != 0 ? config->getValue(myId, myDefaultStringValue) : myDefaultStringValue;
    }
    
    void setConfigValue(std::string value) ;
public:  
    ZLOption(std::string group, std::string optionName, std::string defaultStringValue) ;
};
#endif /* ZLOption_hpp */
