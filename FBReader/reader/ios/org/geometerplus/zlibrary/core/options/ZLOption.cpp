//
//  ZLOption.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLOption.hpp"

ZLOption::ZLOption(std::string group, std::string optionName, std::string defaultStringValue) {
    myId = new StringPair(group, optionName);
    myDefaultStringValue = (defaultStringValue.size()!=0) ? defaultStringValue : "";
}




void
ZLOption::setConfigValue(std::string value) {
    Config* config = Config::Instance();
    if (config != 0) {
//        printf("ZLOption setConfigValue: %s\n", value.c_str());
        if (!(myDefaultStringValue == value)) {
            config->setValue(myId, value);
        } else {
            config->unsetValue(myId);
        }
    }
    /*if (config != 0) {
     if (!myDefaultStringValue.equals(value)) {
     config.setValue(myId, value);
     } else {
     config.unsetValue(myId);
     }
     }*/
}
