//
//  ZLTextOtherStyleEntry.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/6.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextOtherStyleEntry_hpp
#define ZLTextOtherStyleEntry_hpp

#include <stdio.h>
#include "ZDZLTextStyleEntry.hpp"

class ZLTextOtherStyleEntry:public ZDZLTextStyleEntry
{
    public:
    ZLTextOtherStyleEntry()
    :ZDZLTextStyleEntry(0)
    {
        
    }
    virtual EntryKind entryKind()
    {
        return  OTHER_Entry;
    }
};
#endif /* ZLTextOtherStyleEntry_hpp */
