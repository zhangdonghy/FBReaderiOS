//
//  SystemInfo.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef SystemInfo_hpp
#define SystemInfo_hpp

#include <stdio.h>
#include <string>

class SystemInfo
{
    virtual std::string tempDirectory()=0;
    virtual std::string networkCacheDirectory()=0;
};

#endif /* SystemInfo_hpp */
