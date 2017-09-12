//
//  ZLTextWordRegionSoul.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextWordRegionSoul_hpp
#define ZLTextWordRegionSoul_hpp

#include <stdio.h>
#include "ZLTextRegion.hpp"
class ZLTextWord;

class ZLTextWordRegionSoul: public ZLTextRegion::Soul
{
    public:
    shared_ptr<ZLTextWord> Word;
    
    ZLTextWordRegionSoul(ZLTextPosition& position,ZLTextWord& word)
        :ZLTextRegion::Soul(position.getParagraphIndex(), position.getElementIndex(), position.getElementIndex())
    {
        Word = &word;
    }
    virtual SoulKind getSoulKind()
    {
        return  SoulKind_ZLTextWordRegionSoul;
    }
    
};
#endif /* ZLTextWordRegionSoul_hpp */
