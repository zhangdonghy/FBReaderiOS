//
//  ZDZLibrary.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDZLibrary_hpp
#define ZDZLibrary_hpp

#include <stdio.h>
class ZDZLibrary
{
private:
    static ZDZLibrary* ourImplementation;
protected:
    ZDZLibrary() ;
public:
    static ZDZLibrary* Instance() ;
    virtual int getDisplayDPI()=0;
    
};
#endif /* ZDZLibrary_hpp */
