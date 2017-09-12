//
//  ZLBooleanOption.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLBooleanOption_hpp
#define ZLBooleanOption_hpp

#include <stdio.h>
#include "ZLOption.hpp"
#include <string>

class ZLBooleanOption:public ZLOption
{
    private:
     bool myDefaultValue;
    public:
    ZLBooleanOption(std::string group, std::string optionName, bool defaultValue)
    :ZLOption(group, optionName, defaultValue ? "true" : "false")
    {
        
        myDefaultValue = defaultValue;
    }
    bool getValue() {
        /*if (mySpecialName.size() !=0 && !Config.Instance().isInitialized()) {
            return Config.Instance().getSpecialBooleanValue(mySpecialName, myDefaultValue);
        } else {
            return "true".equals(getConfigValue());
        }*/
       return "true" == (getConfigValue());
    }
};
#endif /* ZLBooleanOption_hpp */
