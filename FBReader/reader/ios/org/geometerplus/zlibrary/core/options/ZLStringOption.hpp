//
//  ZLStringOption.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/14.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLStringOption_hpp
#define ZLStringOption_hpp

#include <stdio.h>
#include <string>
#include "ZLOption.hpp"

class ZLStringOption : public ZLOption
{
public:
    ZLStringOption(std::string group,std::string optionName,std::string defaultValue)
    :ZLOption(group,optionName,defaultValue)
    {
        
    }
    
    std::string getValue() {
        return getConfigValue();
        /*if (mySpecialName != null && !Config.Instance().isInitialized()) {
            return Config.Instance().getSpecialStringValue(mySpecialName, myDefaultStringValue);
        } else {
            return getConfigValue();
        }*/
    }
    
    void setValue(std::string value) {
        /*if (value == null) {
            return;
        }
        if (mySpecialName != null) {
            Config.Instance().setSpecialStringValue(mySpecialName, value);
        }*/
        setConfigValue(value);
    }
    
    void saveSpecialValue() {
       /* if (mySpecialName != null && Config.Instance().isInitialized()) {
            Config.Instance().setSpecialStringValue(mySpecialName, getValue());
        }*/
    }
    
};
#endif /* ZLStringOption_hpp */
