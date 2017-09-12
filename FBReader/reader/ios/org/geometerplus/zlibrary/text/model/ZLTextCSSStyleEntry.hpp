//
//  ZLTextCSSStyleEntry.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/6.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextCSSStyleEntry_hpp
#define ZLTextCSSStyleEntry_hpp

#include <stdio.h>
#include "ZDZLTextStyleEntry.hpp"

class ZLTextCSSStyleEntry:public ZDZLTextStyleEntry
{
    public:
    virtual EntryKind entryKind()
    {
        return CSS_Entry;
    }
    ZLTextCSSStyleEntry(short depth)
        :ZDZLTextStyleEntry(depth)
    {
        
    }
};
#endif /* ZLTextCSSStyleEntry_hpp */
