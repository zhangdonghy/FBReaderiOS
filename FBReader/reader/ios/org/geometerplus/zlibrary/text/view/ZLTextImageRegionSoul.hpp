//
//  ZLTextImageRegionSoul.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextImageRegionSoul_hpp
#define ZLTextImageRegionSoul_hpp

#include <stdio.h>
#include "ZLTextImageElement.hpp"
#include "ZLTextPosition.hpp"
#include "ZLTextRegion.hpp"

class ZLTextImageRegionSoul: public ZLTextRegion::Soul
{
    public:
    
    shared_ptr<ZLTextImageElement> ImageElement;
    
    ZLTextImageRegionSoul(ZLTextPosition& position,ZLTextImageElement& imageElement)
    :ZLTextRegion::Soul(position.getParagraphIndex(), position.getElementIndex(), position.getElementIndex())
    {
        
        ImageElement = &imageElement;
    }
    virtual SoulKind getSoulKind()
    {
        return  SoulKind_ZLTextImageRegionSoul;
    }
};
#endif /* ZLTextImageRegionSoul_hpp */
