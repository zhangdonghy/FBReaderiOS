//
//  ZLColor.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLColor_hpp
#define ZLColor_hpp

#include <stdio.h>
#include <string>
#include "shared_ptr.h"

class ZLColor
{
public:
    short Red;
    short Green;
    short Blue;
    ZLColor(int r, int g, int b) ;
    ZLColor(int intValue) ;
    int intValue() ;
    
    virtual bool equals(shared_ptr<ZLColor> o) ;
    
    virtual int hashCode() ;
    virtual std::string toString();

};
#endif /* ZLColor_hpp */
