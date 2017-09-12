//
//  ZLiOSLibrary.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLiOSLibrary_hpp
#define ZLiOSLibrary_hpp

#include <stdio.h>

#include "ZDZLibrary.hpp"

class ZLiOSLibrary:public ZDZLibrary{
private:
protected:
public:
    ZLiOSLibrary();
    virtual int getDisplayDPI();
};
#endif /* ZLiOSLibrary_hpp */
