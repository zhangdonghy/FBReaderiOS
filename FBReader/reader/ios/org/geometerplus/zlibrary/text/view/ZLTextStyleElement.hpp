//
//  ZLTextStyleElement.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/5.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextStyleElement_hpp
#define ZLTextStyleElement_hpp

#include <stdio.h>
#include "ZLTextElement.hpp"

class ZDZLTextStyleEntry;
class ZLTextStyleElement: public ZLTextElement
{
    public:
    shared_ptr<ZDZLTextStyleEntry> Entry;
    
    ZLTextStyleElement(shared_ptr<ZDZLTextStyleEntry> entry) {
        Entry = entry;
    }
    virtual ZLTextElement::Kind kind() const
    {
        return ZLTextElement::TEXT_STYLE_ELEMENT;
    }
};
#endif /* ZLTextStyleElement_hpp */
