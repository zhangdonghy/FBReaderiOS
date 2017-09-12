//
//  ZLTextFixedHSpaceElement.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextFixedHSpaceElement_hpp
#define ZLTextFixedHSpaceElement_hpp

#include <stdio.h>
#include "ZLTextElement.hpp"


class ZLTextFixedHSpaceElement : public ZLTextElement
{
    private:
    static shared_ptr<ZLTextElement> ourCollection[20];
    ZLTextFixedHSpaceElement(short length) {
        Length = length;
    }
    public:
    int Length;
    static shared_ptr<ZLTextElement> getElement(short length) {
        if (length < 20) {
            shared_ptr<ZLTextElement> cached = ourCollection[length];
            if (cached == 0) {
                ourCollection[length] = new ZLTextFixedHSpaceElement(length);
                //ourCollection[length] = cached;
            }
            return ourCollection[length] ;
        }	else {
            return (new ZLTextFixedHSpaceElement(length));
        }
    }
    virtual Kind kind() const;
};
#endif /* ZLTextFixedHSpaceElement_hpp */
