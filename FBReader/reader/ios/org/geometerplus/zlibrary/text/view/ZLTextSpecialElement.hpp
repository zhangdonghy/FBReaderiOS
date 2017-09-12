//
//  ZLTextSpecialElement.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/2.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextSpecialElement_hpp
#define ZLTextSpecialElement_hpp

#include <stdio.h>
#include "ZLTextElement.hpp"


class ZLTextSpecialElement :public ZLTextElement
{
 private:
    Kind myKind;
public:
    virtual Kind kind() const{
        return  myKind;
    }
    ZLTextSpecialElement(Kind kind)
    {
        myKind = kind;
    }
    
};
#endif /* ZLTextSpecialElement_hpp */
